const uint8_t defstate[3] = { N64C, 0 };
const uint8_t defreport[4] = { 0, 0, ANALOG_MID, ANALOG_MID };
const status defstate = { GCC, 3 };
const gcreport defreport = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW, TRIGGER_LOW };
const gcorigin deforigin = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW, TRIGGER_LOW, 0, 0 };

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

static inline uint32_t poll() {
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
}

void setup() {
	//PORTC MUX, not sure what this does but maybe it's important
	CORE_PIN12_CONFIG = PORT_PCR_MUX(1);
}
