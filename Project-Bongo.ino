#pragma region name
// #pragma comment(user, "comment") // comment in obj file
// #pragma include_alias("new.h", "old.h") // renames a header in code
// #pragma message("message") // sends message to stdout during compilation

// #pragma pack(push, n) // for specifying padding
// struct {};
// #pragma pack(pop) // used with push

// #define A 1 // A = 1
// #pragma push_macro("A") // A = 1
// #define A 2 // A = 2
// #pragma pop_macro("A") // A = 1
// pushing macros to stack allows you to change their definitions
#pragma endregion comment // allows you to fold code in VSCode

/**
 * Starting from scratch (sort'a) using what I know and then running
 * arm-none-eabi-objdump (arm-none-eabi-g++) to disassemble it and then figure
 * out the timing that way. Still could really use a logic analyzer XD
 */

// pullup for console only?

/** timings http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0344k/Cfacfihf.html
 * str = 2
 * ldr = 2
 * sub = 1
 * b = 1 if fail, 2 otherwise
 * nop = 1
 * add = 1
 */

#include "stuff.h"

union {
	u8 raw[4];
	// u16 raw16[2];
	u32 raw32;
	// TODO fix reversed bytes
	struct
	{
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
} n64data;

/**
 * Copied from pins_teensy.c; computes the address of the mode register for the
 * given pin.
 */
#define GPIO_BITBAND_ADDR(reg, bitt) \
	(((u32) & (reg)-0x40000000) * 32 + (bitt)*4 + 0x42000000) // + 160; // ?

// set the output port low
#define SET_LOW "str %4, [%2, 4]\n"
// set the output port high
#define SET_HIGH "str %4, [%2, 0]\n"

// @72mhz = 72 cycles
#define DELAY_1US "ldr r2,=22\n" \
				  "1:\n"         \
				  "subs r2,#1\n" \
				  "bne 1b\n"
// "nop\n nop\n"

#define BT2P5 "ldr r2,=49\n" \
			  "1:\n"         \
			  "subs r2,#1\n" \
			  "bne 1b\n"
#define BT2 "ldr r2,=42\n" \
			"1:\n"         \
			"subs r2,#1\n" \
			"bne 1b\n"     \
			"nop\n nop\n"
#define BT3 "ldr r2,=70\n" \
			"1:\n"         \
			"subs r2,#1\n" \
			"bne 1b\n"     \
			"nop\n nop\n"

// low 3us, high 1us
#define SEND_ZERO SET_LOW BT3 SET_HIGH DELAY_1US
// low 1us, high 3us
#define SEND_ONE SET_LOW DELAY_1US SET_HIGH BT3

/**
 * Rather than watch for the rising/falling edge we just poll in the middle of
 * the period. It's crude but it works. Note: timing is currently set to be in
 * the most optimal position between the falling and rising edges to get the
 * actual peak of the wave and the least error
 */
#define RECEIVE_BIT BT2P5 "ldr r1,[%3]\n"  /* read port value */     \
						  "str r1,[%0]\n"  /* store into array */    \
						  "add %0,%0,#4\n" /* increment array ptr */ \
	BT2

#define PIN 0

#define button(num, state) Joystick.button(num, state)
#define stick(x, y) \
	Joystick.X(x);  \
	Joystick.Y(y)
#define stickC(x, y)     \
	Joystick.Zrotate(x); \
	Joystick.Z(y)

#define USB_MAX 0x3FF
#define USB_MID 0x200
#define USB_MIN 0
enum Button
{
	x = 1,
	b,
	a,
	y,
	zl,
	zr,
	l,
	r,
	sel,
	start
};

static inline u32 poll()
{
	static u32 modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
	u32 buffer[81]; // max number of bits

	u8 oldSREG = SREG;
	cli();
	asm volatile(
		"ldr r0,=0\n"
		"ldr r1,=1\n"

		// 0x01
		"str r0,[%1]\n" // pin to output mode
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
		// RECEIVE_BIT // stop

		:
		: "r"(buffer), "r"(modeReg), "r"(&CORE_PIN0_PORTSET),
		  "r"(&CORE_PIN0_PINREG), "r"(CORE_PIN0_BITMASK)
		: "r0", "r1", "r2");
	SREG = oldSREG;

	// parse
	u32 data = 0;
	for (int i = 0; i < 32; i++)
	{
		if (buffer[i] & CORE_PIN0_BITMASK)
		{
			data |= 1 << (31 - i);
		}
	}

	return data;
}

const s16 angles[16] = {
	-1, 270, 90, -1, 180, 225, 135, 180, 0, 315, 45, 0, -1, 270, 90, -1};

inline void toUSB()
{
	button(a, n64data.a);
	button(b, n64data.b);
	button(zr, n64data.z);
	button(start, n64data.start);
	Joystick.hat(angles[n64data.raw[3] & 0x0F]);
	button(l, n64data.l);
	button(r, n64data.r);
	// c buttons using otherwise unused buttons
	button(x, n64data.cu);
	button(y, n64data.cd);
	button(zl, n64data.cl);
	button(sel, n64data.cr);
	// must safely convert s8 to u16
	stick(((u16)(((~((u8)n64data.sx)) & 0x80) | (((u8)n64data.sx) & 0x7F))) << 2,
		  ((u16)(((~((u8)n64data.sy)) & 0x80) | (((u8)n64data.sy) & 0x7F))) << 2);
	Joystick.send_now();
}

void setup()
{
	// Serial.begin(115200);
	// while (!Serial);
	Joystick.useManualSend(true);

	pinMode(PIN, OUTPUT);
}

void loop()
{
	n64data.raw32 = poll();

	// Serial.println(n64data.raw32, HEX);
	toUSB();

	delay(10); // more error the shorter time between polls
}
