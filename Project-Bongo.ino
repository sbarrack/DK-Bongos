/* Borrowed this from some post I found online that was really helpful and gave me a lot of ideas.
 * Link: https://forum.pjrc.com/threads/26753-Reading-an-N64-controller-getting-no-data
 */

// timings http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Cfacfihf.html
// str = 2
// ldr = 2
// sub = 1
// b = 1 if fail, 2 otherwise
// nop = 1
// add = 1

union N64Report {
	uint8_t raw[4];
	//uint16_t raw16[2];
	uint32_t raw32;
	struct {
		uint8_t dr : 1;
		uint8_t dl : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t start : 1;
		uint8_t z : 1;
		uint8_t b : 1;
		uint8_t a : 1;

		uint8_t cr : 1;
		uint8_t cl : 1;
		uint8_t cd : 1;
		uint8_t cu : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t reset : 1;
		uint8_t : 1;

		int8_t sx;
		int8_t sy;
	};
} n64data;

// Copied from pins_teensy.c; computes the address of the mode register for the given pin.
#define GPIO_BITBAND_ADDR(reg, bitt) (((uint32_t)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

// set the output port low
#define SET_LOW "str %4, [%2, 4]\n"

// set the output port high
#define SET_HIGH "str %4, [%2, 0]\n"

// @72mhz = 72 cycles
#define DELAY_1US "ldr r2,=22\n" "1:\n" "subs r2,#1\n" "bne 1b\n"// "nop\n nop\n"

#define BT2P5 "ldr r2,=49\n" "1:\n" "subs r2,#1\n" "bne 1b\n"
#define BT2 "ldr r2,=42\n" "1:\n" "subs r2,#1\n" "bne 1b\n" "nop\n nop\n"
#define BT3 "ldr r2,=70\n" "1:\n" "subs r2,#1\n" "bne 1b\n" "nop\n nop\n"

// low 3us, high 1us
#define SEND_ZERO SET_LOW BT3 SET_HIGH DELAY_1US

// low 1us, high 3us
#define SEND_ONE SET_LOW DELAY_1US SET_HIGH BT3

// Rather than watch for the rising/falling edge we just poll in the middle of the period. It's crude but it works. Note: timing is currently set to be in the most optimal position between the falling and rising edges to get the actual peak of the wave and the least error
#define RECEIVE_BIT BT2P5 "ldr r1,[%3]\n" /* read port value */ "str r1,[%0]\n" /* store into array */ "add %0,%0,#4\n" /* increment array ptr */ BT2

#define PIN 0

static inline uint32_t poll() {
	static uint32_t modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT); // + 160; // ?
	uint32_t buffer[33];

	uint8_t oldSREG = SREG;
	cli();
	__asm__ volatile (
		"ldr r0,=0\n"
		"ldr r1,=1\n"
		"str r0,[%1]\n" // pin to output mode

		// 0x01
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ONE
		SEND_ONE // stop

		// 4 bytes
		"str r1,[%1]\n" // pin to input mode
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT // stop
		:
		: "r" (buffer), "r" (modeReg), "r" (&CORE_PIN0_PORTSET), "r" (&CORE_PIN0_PINREG), "r" (CORE_PIN0_BITMASK)
		: "r0", "r1", "r2"
	);
	SREG = oldSREG;

	// parse
	uint32_t data = 0;
	for (int i = 0; i < 32; i++) {
		if (buffer[i] & CORE_PIN0_BITMASK) {
			data |= 1 << (31 - i);
		}
	}

	return data;
}

void setup() {
	Serial.begin(115200);
	while (!Serial);
	pinMode(PIN, OUTPUT);
}

void loop() {
	n64data.raw32 = poll();
	Serial.println(n64data.raw32, HEX);
	delay(10); // more error the shorter the time between polls
}
