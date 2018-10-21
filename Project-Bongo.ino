/* Borrowed this from some post I found online that was really helpful and gave me a lot of ideas.
 * Link: https://forum.pjrc.com/threads/26753-Reading-an-N64-controller-getting-no-data
 */

/* timings http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Cfacfihf.html
 * ldr = 2
 * str = 2
 * b = 1 no branch, 2-4 otherwise
 * add = 1
 * sub = 1
 * nop = 1, none if not volatile
 */

union N64Report {
	uint8_t raw[4];
	//uint16_t raw16[2];
	uint32_t raw32;
	// TODO fix reversed bytes?
	struct {
		int8_t sy;
		int8_t sx;

		uint8_t cr : 1;
		uint8_t cl : 1;
		uint8_t cd : 1;
		uint8_t cu : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t reset : 1;
		uint8_t : 1;
		
		uint8_t dr : 1;
		uint8_t dl : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t start : 1;
		uint8_t z : 1;
		uint8_t b : 1;
		uint8_t a : 1;
	};
} n64data;

// Copied from avr_emulation.h; computes the address of the mode register for the given pin.
#define GPIO_BITBAND_ADDR(reg, bit) (((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)

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

static inline uint32_t poll() {
	static uint32_t modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
	uint32_t buffer[33]; // 32 bits; 1 stop bit?

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
		//RECEIVE_BIT // stop
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
	//Serial.begin(115200);
	//while (!Serial);
	pinMode(PIN, OUTPUT); // init pin

}

uint8_t then[8];
void loop() {
	n64data.raw32 = poll();

	uint8_t now = n64data.a | n64data.cd << 1 | n64data.cr << 2;
	if (then[sizeof(then) - 1] == now) {
		switch (now) {
		case 1: // C4
			usbMIDI.sendNoteOn(60, 63, 1);
			break;
		case 2:
			usbMIDI.sendNoteOn(62, 63, 1);
			break;
		case 3:
			usbMIDI.sendNoteOn(64, 63, 1);
			break;
		case 4:
			usbMIDI.sendNoteOn(65, 63, 1);
			break;
		case 5:
			usbMIDI.sendNoteOn(67, 63, 1);
			break;
		case 6:
			usbMIDI.sendNoteOn(69, 63, 1);
			break;
		case 7:
			usbMIDI.sendNoteOn(71, 63, 1);
			break;
		}
		usbMIDI.send_now();
	} else {
		switch (then[sizeof(then) - 1]) {
		case 1:
			usbMIDI.sendNoteOff(60, 63, 1);
			break;
		case 2:
			usbMIDI.sendNoteOff(62, 63, 1);
			break;
		case 3:
			usbMIDI.sendNoteOff(64, 63, 1);
			break;
		case 4:
			usbMIDI.sendNoteOff(65, 63, 1);
			break;
		case 5:
			usbMIDI.sendNoteOff(67, 63, 1);
			break;
		case 6:
			usbMIDI.sendNoteOff(69, 63, 1);
			break;
		case 7:
			usbMIDI.sendNoteOff(71, 63, 1);
			break;
		}
		usbMIDI.send_now();
	}
	for (int i = sizeof(then) - 1; i > -1; i--) {
		then[i] = then[i - 1];
	}
	then[0] = now;

	delay(5); // more error the shorter the time between polls
}
