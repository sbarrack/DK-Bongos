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

// Copied from pins_teensy.c; computes the address of the mode register for the given pin.
#define GPIO_BITBAND_ADDR(reg, bitt) (((uint32_t)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

// set the output port low
#define SET_LOW "str %4, [%2, 4]\n"

// set the output port high
#define SET_HIGH "str %4, [%2, 0]\n"

// @72mhz = 72 cycles
#define DELAY_1US "ldr r2,=22\n" "1:\n" "subs r2,#1\n" "bne 1b\n"// "nop\n nop\n"

#define BT2P5 "ldr r2,=45\n" "1:\n" "subs r2,#1\n" "bne 1b\n"
#define BT2 "ldr r2,=46\n" "1:\n" "subs r2,#1\n" "bne 1b\n" "nop\n nop\n"
#define BT3 "ldr r2,=70\n" "1:\n" "subs r2,#1\n" "bne 1b\n" "nop\n nop\n"

// low 3us, high 1us
//#define SEND_ZERO SET_LOW DELAY_1US DELAY_1US DELAY_1US SET_HIGH DELAY_1US
#define SEND_ZERO SET_LOW BT3 SET_HIGH DELAY_1US

// low 1us, high 3us
#define SEND_ONE SET_LOW DELAY_1US SET_HIGH BT3

// Rather than watch for the rising/falling edge we just poll in the middle of the period. It's crude but it works.
#define RECEIVE_BIT BT2 "ldr r1,[%3]\n" /* read port value */ "str r1,[%0]\n" /* store into array */ "add %0,%0,#4\n" /* increment array ptr */ BT2P5

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
	uint32_t data = poll();
	Serial.print(data < 0x10000000 ? data < 0x1000000 ? data < 0x100000 ? data < 0x10000 ? data < 0x1000 ? data < 0x100 ? data < 0x10 ? "0000000" : "000000" : "00000" : "0000" : "000" : "00" : "0" : ""); // pad zeros
	Serial.println(data, HEX);
	delay(10);
}
