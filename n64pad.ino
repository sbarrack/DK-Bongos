//copied from pins_teensy.c
#define GPIO_BITBAND_ADDR(reg, bitt) (((uint32_t)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

#define CLEAR_BIT "str %4, [%2, 4] \n"
#define SET_BIT "str %4, [%2, 0] \n"
//1us @ 120MHz = 120 = 2 + (1 + 2) * n + 1 + 1
#define WAIT "ldr r2, =39 \n" "1: \n" "subs r2, #1 \n" "bne 1b \n"
//low 3us, high 1us
#define SEND_ZERO CLEAR_BIT WAIT WAIT WAIT SET_BIT WAIT
//low 1us, high 3us
#define SEND_ONE CLEAR_BIT WAIT SET_BIT WAIT WAIT WAIT
//read in the center of the period vs anticipating edge, take advantage of the 2 cycles (one for data, one for parity check)
#define GET_BIT WAIT WAIT "ldr r1, [%3] \n" /* read port value */ "str r1, [%0] \n" /* store into array */ "add %0, %0, #4 \n" /* increment array ptr */ WAIT WAIT

union {
	uint8_t raw[4];
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
		uint8_t : 1;
		uint8_t reset : 1;	//l+r+start, start stays low

		int8_t sx;
		int8_t sy;
	};
} rep;

inline uint32_t poll() {
	static uint32_t modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
	uint32_t bitBucket[33];
	//disable interrupts on Cortex-M4, better than emulated SREG
	__disable_irq();
	__asm__ volatile(
		"ldr r0, =0 \n"
		"ldr r1, =1 \n"
		"str r0, [%1] \n" //pin 12 to output mode
						  //0x01, poll
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ONE
		SEND_ONE
		//parsing the port data later is faster
		"str r1, [%1] \n" //pin 12 to input mode
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT GET_BIT GET_BIT GET_BIT
		GET_BIT //stop bit
		".pool \n" //asm directive to store constants?
		:: "r" (bitBucket), "r" (modeReg),
		"r" (&CORE_PIN12_PORTSET), "r" (&CORE_PIN12_PINREG),
		"r" (CORE_PIN12_BITMASK)
		: "r0", "r1", "r2"
	);
	__enable_irq();
	//extract bits
	uint32_t data = 0;
	int h = 7;
	for (int i = 0; i < 8; i++) {
		if (bitBucket[i] & CORE_PIN12_BITMASK) {
			data |= _BV(h);
		}
		h--;
	}
	h = 15;
	for (int i = 8; i < 16; i++) {
		if (bitBucket[i] & CORE_PIN12_BITMASK) {
			data |= _BV(h);
		}
		h--;
	}
	h = 23;
	for (int i = 16; i < 24; i++) {
		if (bitBucket[i] & CORE_PIN12_BITMASK) {
			data |= _BV(h);
		}
		h--;
	}
	h = 31;
	for (int i = 24; i < 32; i++) {
		if (bitBucket[i] & CORE_PIN12_BITMASK) {
			data |= _BV(h);
		}
		h--;
	}
	return data;
}

void setup() {
	Serial.begin(115200);
	while (!Serial);
	pinMode(12, OUTPUT);
}

void loop() {
	rep.raw32 = poll();
	Serial.print(rep.a ? "A" : "_");
	Serial.print(rep.b ? "B " : "_ ");
	Serial.print(rep.z ? "Z" : "_");
	Serial.print(rep.start ? "S " : "_ ");
	Serial.print(rep.du ? "D:U" : "D:_");
	Serial.print(rep.dd ? "D" : "_");
	Serial.print(rep.dl ? "L" : "_");
	Serial.print(rep.dr ? "R " : "_ ");

	Serial.print(rep.reset ? "!-" : "_-");
	Serial.print(rep.l ? "L" : "_");
	Serial.print(rep.r ? "R " : "_ ");
	Serial.print(rep.cu ? "C:U" : "C:_");
	Serial.print(rep.cd ? "D" : "_");
	Serial.print(rep.cl ? "L" : "_");
	Serial.print(rep.cr ? "R " : "_ ");

	Serial.print(rep.sx);
	Serial.print(",");
	Serial.println(rep.sy);
	delay(17);
}

