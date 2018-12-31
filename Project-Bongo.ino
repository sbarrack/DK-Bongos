/* timings http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Cfacfihf.html
 * ldr = 2
 * str = 2
 * b = 1 no branch, 2-4 otherwise
 * add = 1
 * sub = 1
 * nop = 1, 0 if not volatile
 */

#include <Arduino.h>
#include "types.h"

// address for GPIO n64mode register of given pin
#define GPIO_BITBAND_ADDR(reg, bitt) (((u32)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

#define SET_LOW "str %4, [%2, 4]\n" // set the outport low
#define SET_HIGH "str %4, [%2, 0]\n" // set the outport high
#define LOOP "1:\n" "subs r2,#1\n" "bne 1b\n"
// 1us @ 72MHz = 72
#define NOP1 "nop\n"
#define NOP2 NOP1 NOP1
#define NOP4 NOP2 NOP2
#define NOP31 "ldr r2,=9\n" LOOP
#define NOP36 "ldr r2,=10\n" LOOP
#define NOP70 "ldr r2,=22\n" LOOP
#define NOP216 "ldr r2,=70\n" LOOP NOP2

#define SEND_ZERO SET_LOW NOP216 SET_HIGH NOP70 // low 3us, high 1us
#define SEND_ONE SET_LOW NOP70 SET_HIGH NOP216 // low 1us, high 3us

#define RECEIVE_BIT "ldr r1,[%3]\nstr r1,[%0]\nadd %0,%0,#4\n"
#define RECEIVE_8_32ndBIT NOP4 "ldr r3,=8\n2:\n" RECEIVE_BIT "subs r3,#1\nbne 2b\n"

#define PIN 0
#define LEN 200

union GCReport {
	u8 raw[8];
	u32 raw32[2];
	u64 raw64;
	struct {
		u8 rt;
		u8 lt;
		u8 cy;
		u8 cx;
		u8 sy;
		u8 sx;

		u8 dl : 1;
		u8 dr : 1;
		u8 dd : 1;
		u8 du : 1;
		u8 z : 1;
		u8 r : 1;
		u8 l : 1;
		u8 : 1;

		u8 a : 1;
		u8 b : 1;
		u8 x : 1;
		u8 y : 1;
		u8 start : 1;
		u8 : 3;
	};
} gcdata;

static inline void n64poll() {
	static u32 n64modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
	u32 buffer[LEN];

	u8 oldSREG = SREG;
	cli();
	__asm__ volatile (
		"ldr r0,=0\n"
		"ldr r1,=1\n"
		"str r0,[%1]\n" // pin to output n64mode

		// 0x40 gcc poll
		SEND_ZERO SEND_ONE SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		// 0x03 full length
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ONE SEND_ONE
		// 0x00 rumble disabled
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ONE SEND_ZERO
		SEND_ONE // stop

		// 1 nib, last superbit should be start button
		RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT
		RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT
		RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT
		RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT RECEIVE_8_32ndBIT
		
		:
		: "r" (buffer), "r" (n64modeReg), "r" (&CORE_PIN0_PORTSET), "r" (&CORE_PIN0_PINREG), "r" (CORE_PIN0_BITMASK)
		: "r0", "r1", "r2", "r3"
	);
	SREG = oldSREG;

	for (int i = 0; i < LEN; i++) {
		Serial.print(((buffer[i] & CORE_PIN0_BITMASK) >> CORE_PIN0_BIT) & 1, BIN);
	}
}

void setup() {
	Serial.begin(115200);
	while (!Serial);
	pinMode(PIN, OUTPUT);
}

void loop() {
	delay(2000);
	n64poll();
	Serial.println("\n~~~~~~( New poll )~~~~~~");
}
