/*	I don't know who Rena is and why she didn't make $1 million off
	this idea, but kudos to her 
	(https://forum.pjrc.com/threads/26753-Reading-an-N64-controller-getting-no-data).
*/

//copied from avr_emulation.h, address of mode reg for given pin
#define GPIO_BITBAND_ADDR(reg, bit) \
	(((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
#define LINE_CLEAR "str %4, [%2, 4] \n"
#define LINE_SET "str %4, [%2, 0] \n"
//1us @ 120MHz, r2 = (120 - 6) / 3
#define NOP2 "nop \nnop \n"
#define NOP4 NOP2 NOP2
#define WAIT \
	"ldr r2, =38 \n" \
	"1: \n" \
	"subs r2, #1 \n" \
	"bne 1b \n"
#define SEND_0 \
	LINE_CLEAR \
	WAIT WAIT WAIT NOP4 \
	LINE_SET \
	WAIT
#define STOP \
	LINE_CLEAR \
	WAIT \
	LINE_SET \
	WAIT WAIT NOP2
#define SEND_1 \
	STOP \
	WAIT NOP2
//read in the middle of the period instead of waiting for the edge
#define GET_BIT \
	WAIT \
	"ldr r1, [%3] \n" \
	"str r1, [%0] \n" \
	"add %0, %0, #4 \n" \
	WAIT WAIT
#define GET_BYTE \
	GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT

#define PIN_LED 13
#define PIN_PAD 12

#define BIT(n) (1 << n)

inline uint32_t poll() {
	static uint32_t modeReg =
		GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT) + 160;
	uint32_t bitBucket[33];
	__disable_irq();
  uint8_t oldSREG = SREG;
  cli();
	__asm__ volatile(
		"ldr r0, =0 \n"
		"ldr r1, =1 \n"
		"str r0, [%1] \n"	//output
		//transmit
		//0x01, poll n64
		SEND_0 SEND_0 SEND_0 SEND_0
		SEND_0 SEND_0 SEND_0 SEND_1
		STOP
		//receive
		"str r1, [%1] \n"	//input
		GET_BYTE GET_BYTE GET_BYTE GET_BYTE
		GET_BIT	//stop
		".pool\n" //the assembler needs a place to store constants
		:
		: "r" (bitBucket), "r" (modeReg),
		  "r" (&CORE_PIN12_PORTSET), "r" (&CORE_PIN12_PINREG),
		  "r" (CORE_PIN12_BITMASK)
		: "r0", "r1", "r2"
	);
	//don't re-enable irq here
	uint32_t data = 0;
	for(int i=0; i<32; i++) {
		if(bitBucket[i] & CORE_PIN12_BITMASK) data |= BIT(i);
	}
 SREG = oldSREG;
	__enable_irq();
	return data;
}


void setup() {
	Serial.begin(115200);
	while (!Serial);
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_PAD, OUTPUT);	//line needs to idle high
	digitalWriteFast(PIN_LED, 1);
	delay(250);
	digitalWriteFast(PIN_LED, 0);
	delay(250);
	digitalWriteFast(PIN_LED, 1);
	delay(250);
	digitalWriteFast(PIN_LED, 0);
	Serial.println("STARTING");
	CORE_PIN12_CONFIG = PORT_PCR_MUX(1);
	delay(2000);
	digitalWriteFast(PIN_LED, 1);
}

void loop() {
	uint32_t data = poll();
	//print the data to the console in binary
	char str[40];
	char *s = str;
	for(int i=0; i<32; i++) {
		*s++ = ((data & BIT(i)) ? '1' : '0');
		if((i & 7) == 7) *s++ = ' ';
	}
	*s = '\0';
	Serial.printf("%s\n", str);
	delay(16);
}
