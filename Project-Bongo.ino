/* Shout out to Rena, whoever you are, your code was really helpful!
 * https://forum.pjrc.com/threads/26753-Reading-an-N64-controller-getting-no-data
 */

/* timings http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Cfacfihf.html
 * ldr = 2
 * str = 2
 * b = 1 no branch, 2-4 otherwise
 * add = 1
 * sub = 1
 * nop = 1, none if not volatile
 */

#include <Arduino.h>
#include "types.h"
#include "revolution.h"

// address for GPIO n64mode register of given pin
#define GPIO_BITBAND_ADDR(reg, bitt) (((u32)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

#define SET_LOW "str %4, [%2, 4]\n" // set the outport low
#define SET_HIGH "str %4, [%2, 0]\n" // set the outport high

#define LOOP "1:\n" "subs r2,#1\n" "bne 1b\n"
// 1us @ 72MHz = 72
#define NOP2 "nop\n nop\n"
#define NOP70 "ldr r2,=22\n" LOOP
#define NOP151 "ldr r2,=49\n" LOOP
#define NOP132 "ldr r2,=42\n" LOOP NOP2
#define NOP216 "ldr r2,=70\n" LOOP NOP2

#define SEND_ZERO SET_LOW NOP216 SET_HIGH NOP70 // low 3us, high 1us
#define SEND_ONE SET_LOW NOP70 SET_HIGH NOP216 // low 1us, high 3us

// hard code timing to read at the expected peak rather than basing it on the falling edge; sensitive, don't change timing
#define RECEIVE_BIT NOP151 "ldr r1,[%3]\n" /* read port value */ "str r1,[%0]\n" /* store into array */ "add %0,%0,#4\n" /* increment array ptr */ NOP132

#define PIN 0
#define VOL 63
#define CHAN 1

union N64Report {
	u8 raw[4];
	//uint16_t raw16[2];
	u32 raw32;
	// TODO fix reversed bytes?
	struct {
		s8 sy;
		s8 sx;

		u8 cr : 1;
		u8 cl : 1;
		u8 cd : 1;
		u8 cu : 1;
		u8 r : 1;
		u8 l : 1;
		u8 reset : 1;
		u8 : 1;

		u8 dr : 1;
		u8 dl : 1;
		u8 dd : 1;
		u8 du : 1;
		u8 start : 1;
		u8 z : 1;
		u8 b : 1;
		u8 a : 1;
	};
} n64data, olddata;

static inline u32 n64poll() {
	static u32 n64modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
	u32 buffer[33]; // 32 bits; 1 stop bit?

	u8 oldSREG = SREG;
	cli();
	__asm__ volatile (
		"ldr r0,=0\n"
		"ldr r1,=1\n"
		"str r0,[%1]\n" // pin to output n64mode

		// 0x01
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ONE
		SEND_ONE // stop

		// 4 bytes
		"str r1,[%1]\n" // pin to input n64mode
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		//RECEIVE_BIT // stop
		:
		: "r" (buffer), "r" (n64modeReg), "r" (&CORE_PIN0_PORTSET), "r" (&CORE_PIN0_PINREG), "r" (CORE_PIN0_BITMASK)
		: "r0", "r1", "r2"
	);
	SREG = oldSREG;

	// parse
	u32 data = 0;
	for (int i = 0; i < 32; i++) {
		if (buffer[i] & CORE_PIN0_BITMASK) {
			data |= 1 << (31 - i);
		}
	}

	return data;
}

void setup() {
	//Serial.begin(115200);
	//while (!Serial);
	pinMode(PIN, OUTPUT); // init pin
}

int n64umod;
void loop() {
	delay(10); // more error the shorter the time between n64polls, less time to buffer inputs too
	n64data.raw32 = n64poll();

	// TODO input buffer for n64mod buttons?;
	// L, R = apply n64mod to 2nd, 3rd note pressed only? fixed harmony, toggle major/minor/interval?
	int n64mod = n64data.du - n64data.dd + (n64data.dr ? 12 : 0) - (n64data.dl ? 12 : 0);
	if (n64umod != n64mod) {
		usbMIDI.sendNoteOff(62 + n64umod, VOL, CHAN);
		usbMIDI.sendNoteOff(65 + n64umod, VOL, CHAN);
		usbMIDI.sendNoteOff(67 + n64umod, VOL, CHAN);
		usbMIDI.sendNoteOff(69 + n64umod, VOL, CHAN);
		usbMIDI.sendNoteOff(71 + n64umod, VOL, CHAN);
		usbMIDI.sendNoteOff(74 + n64umod, VOL, CHAN);
	}
	else if (olddata.raw32 != n64data.raw32) {
		if (n64data.a) { // D4
			usbMIDI.sendNoteOn(62 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(62 + n64mod, VOL, CHAN);
		}
		if (n64data.cd) { // F4
			usbMIDI.sendNoteOn(65 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(65 + n64mod, VOL, CHAN);
		}
		if (n64data.cr) { // G4
			usbMIDI.sendNoteOn(67 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(67 + n64mod, VOL, CHAN);
		}
		if (n64data.b) { // A4
			usbMIDI.sendNoteOn(69 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(69 + n64mod, VOL, CHAN);
		}
		if (n64data.cl) { // B4
			usbMIDI.sendNoteOn(71 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(71 + n64mod, VOL, CHAN);
		}
		if (n64data.cu) { // D5
			usbMIDI.sendNoteOn(74 + n64mod, VOL, CHAN);
		}
		else {
			usbMIDI.sendNoteOff(74 + n64mod, VOL, CHAN);
		}
	}
	usbMIDI.send_now();
	n64umod = n64mod;
	olddata = n64data;
	
}
