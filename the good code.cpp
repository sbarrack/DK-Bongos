#define BIT(n) (1 << (n))
#define GPIO_BITBAND_ADDR(reg, bit) \
	(((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)

//port 12
#define SET_LOW  "str %4, [%2, 4]  \n"
#define SET_HIGH "str %4, [%2, 0]  \n"
/* precise timing to delay 1 microsecond at 72mhz */
#define DELAY_1US \
	"ldr r2, =22   \n" \
	"1:            \n" \
	"subs r2, #1   \n" \
	"bne 1b        \n"
#define SEND_ZERO \
	SET_LOW   \
	DELAY_1US \
	DELAY_1US \
	DELAY_1US \
	SET_HIGH  \
	DELAY_1US
#define SEND_ONE \
	SET_LOW   \
	DELAY_1US \
	SET_HIGH  \
	DELAY_1US \
	DELAY_1US \
	DELAY_1US
#define RECEIVE_BIT \
	DELAY_1US \
	DELAY_1US \
	"ldr r1, [%3]     \n" /* read port value     */ \
	"str r1, [%0]     \n" /* store into array    */ \
	"add %0, %0, #4   \n" /* increment array ptr */ \
	DELAY_1US \
	DELAY_1US

const int ledPin = 13; //LED on the Teensy board
const int padPin = 12; //data line of N64 pad.

static inline uint32_t poll() {
	static uint32_t modeReg =
		GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT) + 160;
	uint32_t bitBucket[33];

	__disable_irq();
	__asm__ volatile(
		"ldr r0, =0         \n"
		"ldr r1, =1         \n"
		"str r0, [%1]       \n" //pin 12 to output mode

		//send bits: 0000 0001 1 (poll command followed by stop bit)
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ZERO
		SEND_ZERO SEND_ZERO SEND_ZERO SEND_ONE SEND_ONE

		//receive 32 bits, followed by a stop bit.
		//for speed we just dump the whole port value into an array
		//and then parse it afterward.
		"str r1, [%1]       \n" //pin 12 to input mode
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT RECEIVE_BIT
		RECEIVE_BIT //stop bit

		".pool\n" //the assembler needs a place to store constants.
		: /* outputs (none) */
		: "r" (bitBucket), "r" (modeReg), /* inputs */
		  "r" (&CORE_PIN12_PORTSET), "r" (&CORE_PIN12_PINREG),
		  "r" (CORE_PIN12_BITMASK)
		: "r0", "r1", "r2" /* clobbers */
	);

	/* re-enabling IRQ right here freezes for some reason, so do it later. */

	//read the extracted bits from the buffer.
	uint32_t data = 0;
	for(int i=0; i<32; i++) {
		if(bitBucket[i] & CORE_PIN12_BITMASK) data |= BIT(i);
	};

	__enable_irq();
	return data;
}


void setup() {
	Serial.begin(115200);
	while (!Serial);

	CORE_PIN12_CONFIG = PORT_PCR_MUX(1);
}


void loop() {
	uint32_t data = poll();

	char str[40];
	char *s = str;
	for(int i=0; i<32; i++) {
		*s++ = ((data & BIT(i)) ? '1' : '0');
		if((i & 7) == 7) *s++ = ' ';
	}
	*s = '\0';

	Serial.printf("%s\n", str);
	delay(17);
}
