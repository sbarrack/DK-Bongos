/*	This is ProjectBongo; a program dedicated to the functionality of 
	Donkey Kong Bongos and other game controllers.

	Copyright (C) 2018  Stephen Barrack

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	For any issues, please contact stephen.barrack12@yahoo.com.
	*/

/*	This library is written to better suit the ARM architecture and make
	use of Teensy's features. Originally, my understanding of the N64/GCN
	controller protocol was that it was something proprietary, however,
	it is simply a faster version of the 1-wire Interface from MSB to LSB
	[1,2]. My implementation of this uses bit-banging to handle the data
	[3]. Additionally, Wii Remote attachment controllers use the 2-wire 
	Interface (TWI/I2C) which is supported by Teensy [4]. Thirdly, the 
	GB/GBC/GBA link cable uses SPI with both handhelds as the master/slave 
	with a 3-wire connection [5]. Timing for ARM instructions can be found 
	in the ARM Information Center [6] while usage can be found in the 
	reference manual section A7.7 [7]. Also, while the No-Operation 
	Instruction (NOP) consumes time on AVR, the ARM pipeline treats them as 
	padding only [8] so I adjusted accordingly to get exact timing. For more 
	information on the full details of the N64/GCN protocol see Squid64's 
	site [9]; it lists all the commands and devices used.

	[1] http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
	[2] https://en.wikipedia.org/wiki/1-wire
	[3] https://en.wikipedia.org/wiki/Bit_banging
	[4] http://wiibrew.org/wiki/Wiimote
	[5] https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
	[6] http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0439b/CHDDIGAC.html
	[7] https://www.pjrc.com/teensy/beta/DDI0403D_arm_architecture_v7m_reference_manual.pdf
	[8] http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/CHDJJGFB.html
	[9] https://sites.google.com/site/consoleprotocols/home/techinfo/nintendo-joy-bus-documentation?authuser=0

	Controllers: GC GCController, DK Bongos, GC Keyboard (not tested),
	Nunchuck, Classic Controller, N64 Controller, GBA (not tested)
	Arduinos: Teensy 3.5 120MHz
	*/

//TODO: Write my own darned documentation on all this garbage so that it's all in one place!

#pragma once
#include <i2c_t3.h>

//useful macros
#define delayMicros(us) delayMicroseconds(us)
//found somewhere on the internet, resets the Teensy
#define SOFT_RESET() (*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004
//copied from pins_teensy.c
#define GPIO_BITBAND_ADDR(reg, bitt) \
	(((uint32_t)&(reg) - 0x40000000) * 32 + (bitt) * 4 + 0x42000000)

//analog values
//NOTE: N64 sticks are signed, GC and Wii sticks are not
#define ANALOG_ERROR	0x00	//The controller disconnects if any analog sensor fails.
#define ANALOG_MIN		0x01
#define ANALOG_MAX		0xFF	
#define ANALOG_MID		0x80	//127 is not the center due to the error condition.
#define TRIGGER_LOW		0x1F
#define TRIGGER_FLOOR	0x49	//Light-shield is active above this value. 
#define TRIGGER_CEIL	0xFF	//TODO: need to find value just before the trigger button goes high
#define MIC_HIGH		0x6F
#define MIC_LOW			0x70
//TODO: double check these values and the adjusted ones
#define DEAD_LOW		0x68
#define DEAD_HIGH		0x98
#define STICK_LOW		0x4B
#define STICK_HIGH		0xB3
#define STICK_HIGHER	0xB5
#define STICK_MAX		0xE9
#define STICK_MIN		0x17
//adjusted analog values
/*#define DEAD_LOW_ADJ		-24
#define DEAD_HIGH_ADJ		24
#define STICK_LOW_ADJ		-53
#define STICK_HIGHISH_ADJ	51
#define STICK_HIGH_ADJ		53
#define STICK_MAX_ADJ		105
#define STICK_MIN_ADJ		-105*/

//1-wire device ids
#define GCC				0x0900//03
#define BONGOS			0x0900//00?
#define N64C			0x0500//02
//TODO
/*#define GC_KEYBOARD		0x0820
#define GBA				0x0004
#define STEERING_WHEEL	0x0800
//#define CHAINSAW		*/

//twi slave device numbers
#define CON		0x52
//#define PLUS	0x53

//wiimote attachment ids
const uint8_t nunchuck[6] = { 0, 0, 0xA4, 0x20, 0, 0 };
const uint8_t classic[6] = { 0, 0, 0xA4, 0x20, 1, 1 };		//no handles
const uint8_t classicPro[6] = { 1, 0, 0xA4, 0x20, 1, 1 };	//handles
//TODO
/*const uint8_t guitar[6] = { 0, 0, 0xA4, 0x20, 1, 3 };
const uint8_t drums[6] = { 1, 0, 0xA4, 0x20, 1, 3 };
const uint8_t turntable[6] = { 3, 0, 0xA4, 0x20, 1, 3 };
const uint8_t tatacon[6] = { 0, 0, 0xA4, 0x20, 1, 0x11 };
const uint8_t graphicTablet[6] = { 0xFF, 0, 0xA4, 0x20, 0, 0x13 };*/

//scancodes
/*#define GCKB_HOME			0x06
#define GCKB_END			0x07
#define GCKB_PAGE_UP		0x08
#define GCKB_PAGE_DOWN		0x09
#define GCKB_SCROLL_LOCK	0x0A
#define GCKB_A				0x10
#define GCKB_B				0x11
#define GCKB_C				0x12
#define GCKB_D				0x13
#define GCKB_E				0x14
#define GCKB_F				0x15
#define GCKB_G				0x16
#define GCKB_H				0x17
#define GCKB_I				0x18
#define GCKB_J				0x19
#define GCKB_K				0x1A
#define GCKB_L				0x1B
#define GCKB_M				0x1C
#define GCKB_N				0x1D
#define GCKB_O				0x1E
#define GCKB_P				0x1F
#define GCKB_Q				0x20
#define GCKB_R				0x21
#define GCKB_S				0x22
#define GCKB_T				0x23
#define GCKB_U				0x24
#define GCKB_V				0x25
#define GCKB_W				0x26
#define GCKB_X				0x27
#define GCKB_Y				0x28
#define GCKB_Z				0x29
#define GCKB_1				0x2A
#define GCKB_2				0x2B
#define GCKB_3				0x2C
#define GCKB_4				0x2D
#define GCKB_5				0x2E
#define GCKB_6				0x2F
#define GCKB_7				0x30
#define GCKB_8				0x31
#define GCKB_9				0x32
#define GCKB_0				0x33
#define GCKB_MINUS			0x34
#define GCKB_PARENTHESIS	0x34
#define GCKB_NP_MINUS		0x34
#define GCKB_PLUS			0x35
#define GCKB_EQUALS			0x35
#define GCKB_PRINT_SCREEN	0x36
#define GCKB_NP_SLASH		0x36
#define GCKB_PAUSE			0x37
#define GCKB_SYSTEM_REQ		0x37
#define GCKB_NP_ASTERISK	0x37
#define GCKB_OPEN_BRACKET	0x38
#define GCKB_OPEN_BRACE		0x38
#define GCKB_SEMICOLON		0x39
#define GCKB_COLON			0x39
#define GCKB_NP_PLUS		0x39
#define GCKB_QUOTE			0x3A
#define GCKB_APOSTROPHE		0x3A
#define GCKB_CLOSE_BRACKET	0x3B
#define GCKB_CLOSE_BRACE	0x3B
#define GCKB_COMMA			0x3C
#define GCKB_LESS_THAN		0x3C
#define GCKB_PERIOD			0x3D
#define GCKB_GREATER_THAN	0x3D
#define GCKB_NP_POINT		0x3D
#define GCKB_FORWARD_SLASH	0x3E
#define GCKB_QUESTION_MARK	0x3E
#define GCKB_HYPHEN			0x3F
#define GCKB_DASH			0x3F
#define GCKB_F1				0x40
#define GCKB_F2				0x41
#define GCKB_F3				0x42
#define GCKB_F4				0x43
#define GCKB_F5				0x44
#define GCKB_F6				0x45
#define GCKB_F7				0x46
#define GCKB_F8				0x47
#define GCKB_F9				0x48
#define GCKB_F10			0x49
#define GCKB_F11			0x4A
#define GCKB_F12			0x4B
#define GCKB_ESCAPE			0x4C
#define GCKB_INSERT			0x4D
#define GCKB_DELETE			0x4E
#define GCKB_GRAVE			0x4F
#define GCKB_TILDE			0x4F
#define GCKB_BACKSPACE		0x50
#define GCKB_TAB			0x51
#define GCKB_CAPSLOCK		0x53
#define GCKB_L_SHIFT		0x54
#define GCKB_L_CONTROL		0x56
#define GCKB_L_ALT			0x57
#define GCKB_SHIFT			0x54
#define GCKB_CONTROL		0x56
#define GCKB_ALT			0x57
#define GCKB_L_OS			0x58
#define GCKB_SPACE			0x59
#define GCKB_R_OS			0x5A
#define GCKB_MENU			0x5B
#define GCKB_LEFT			0x5C
#define GCKB_DOWN			0x5D
#define GCKB_UP				0x5E
#define GCKB_RIGHT			0x5F
#define GCKB_ENTER			0x61
#define GCKB_RETURN			0x61
#define GCKB_NUMLOCK		0x6A*/

union status {
	uint8_t raw[3];

	struct {
		uint16_t device;

		union {
			uint8_t state;

			struct {
				uint8_t : 3;
				uint8_t rumble : 1;
				uint8_t : 4;
			};
		};
	};
};

union gcreport {
	uint8_t raw[8];
	uint32_t raw32[2];

	struct {
		uint8_t a : 1;	//br
		uint8_t b : 1;	//bl
		uint8_t x : 1;	//tr
		uint8_t y : 1;	//tl
		uint8_t start : 1;
		uint8_t getOrigin : 1;	//x+y+start
		uint8_t errorLatch : 1;
		uint8_t errorStatus : 1;

		uint8_t dl : 1;
		uint8_t dr : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t z : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t useOrigin : 1;

		uint8_t sx;
		uint8_t sy;
		uint8_t cx;	//key1
		uint8_t cy;	//key2
		uint8_t lt;	//key3
		uint8_t rt;	//mic
	};
};

union gcorigin {
	uint8_t raw[10];

	struct {
		gcreport report;
		//unkown if that's what these 2 bytes are
		uint8_t deadzone0;
		uint8_t deadzone1;
	};
};

union n64report {
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
		uint8_t reset : 1;	//l+r+start

		int8_t sx;
		int8_t sy;
	};
};

struct n64data {
	status state;
	n64report report;
};

struct gcdata {
	status state;
	gcorigin origin;
	gcreport report;
};

class Controller {
public:
	status state;
	Controller() {
		modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
		outSetReg = &CORE_PIN12_PORTSET;
		inReg = &CORE_PIN12_PINREG;
		bitmask = CORE_PIN12_BITMASK;
	}
	Controller(const int pin) {
		switch (pin) {
		case 0:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
			outSetReg = &CORE_PIN0_PORTSET;
			inReg = &CORE_PIN0_PINREG;
			bitmask = CORE_PIN0_BITMASK;
			break;
		case 1:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN1_PORTREG, CORE_PIN1_BIT);
			outSetReg = &CORE_PIN1_PORTSET;
			inReg = &CORE_PIN1_PINREG;
			bitmask = CORE_PIN1_BITMASK;
			break;
		case 2:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN2_PORTREG, CORE_PIN2_BIT);
			outSetReg = &CORE_PIN2_PORTSET;
			inReg = &CORE_PIN2_PINREG;
			bitmask = CORE_PIN2_BITMASK;
			break;
		case 3:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN3_PORTREG, CORE_PIN3_BIT);
			outSetReg = &CORE_PIN3_PORTSET;
			inReg = &CORE_PIN3_PINREG;
			bitmask = CORE_PIN3_BITMASK;
			break;
		case 4:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN4_PORTREG, CORE_PIN4_BIT);
			outSetReg = &CORE_PIN4_PORTSET;
			inReg = &CORE_PIN4_PINREG;
			bitmask = CORE_PIN4_BITMASK;
			break;
		case 5:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN5_PORTREG, CORE_PIN5_BIT);
			outSetReg = &CORE_PIN5_PORTSET;
			inReg = &CORE_PIN5_PINREG;
			bitmask = CORE_PIN5_BITMASK;
			break;
		case 6:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN6_PORTREG, CORE_PIN6_BIT);
			outSetReg = &CORE_PIN6_PORTSET;
			inReg = &CORE_PIN6_PINREG;
			bitmask = CORE_PIN6_BITMASK;
			break;
		case 7:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN7_PORTREG, CORE_PIN7_BIT);
			outSetReg = &CORE_PIN7_PORTSET;
			inReg = &CORE_PIN7_PINREG;
			bitmask = CORE_PIN7_BITMASK;
			break;
		case 8:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN8_PORTREG, CORE_PIN8_BIT);
			outSetReg = &CORE_PIN8_PORTSET;
			inReg = &CORE_PIN8_PINREG;
			bitmask = CORE_PIN8_BITMASK;
			break;
		case 9:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN9_PORTREG, CORE_PIN9_BIT);
			outSetReg = &CORE_PIN9_PORTSET;
			inReg = &CORE_PIN9_PINREG;
			bitmask = CORE_PIN9_BITMASK;
			break;
		case 10:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN10_PORTREG, CORE_PIN10_BIT);
			outSetReg = &CORE_PIN10_PORTSET;
			inReg = &CORE_PIN10_PINREG;
			bitmask = CORE_PIN10_BITMASK;
			break;
		case 11:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN11_PORTREG, CORE_PIN11_BIT);
			outSetReg = &CORE_PIN11_PORTSET;
			inReg = &CORE_PIN11_PINREG;
			bitmask = CORE_PIN11_BITMASK;
			break;
		case 12:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
			outSetReg = &CORE_PIN12_PORTSET;
			inReg = &CORE_PIN12_PINREG;
			bitmask = CORE_PIN12_BITMASK;
			break;
		case 13:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN13_PORTREG, CORE_PIN13_BIT);
			outSetReg = &CORE_PIN13_PORTSET;
			inReg = &CORE_PIN13_PINREG;
			bitmask = CORE_PIN13_BITMASK;
			break;
		case 14:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN14_PORTREG, CORE_PIN14_BIT);
			outSetReg = &CORE_PIN14_PORTSET;
			inReg = &CORE_PIN14_PINREG;
			bitmask = CORE_PIN14_BITMASK;
			break;
		case 15:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN15_PORTREG, CORE_PIN15_BIT);
			outSetReg = &CORE_PIN15_PORTSET;
			inReg = &CORE_PIN15_PINREG;
			bitmask = CORE_PIN15_BITMASK;
			break;
		case 16:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN16_PORTREG, CORE_PIN16_BIT);
			outSetReg = &CORE_PIN16_PORTSET;
			inReg = &CORE_PIN16_PINREG;
			bitmask = CORE_PIN16_BITMASK;
			break;
		case 17:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN17_PORTREG, CORE_PIN17_BIT);
			outSetReg = &CORE_PIN17_PORTSET;
			inReg = &CORE_PIN17_PINREG;
			bitmask = CORE_PIN17_BITMASK;
			break;
		case 18:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN18_PORTREG, CORE_PIN18_BIT);
			outSetReg = &CORE_PIN18_PORTSET;
			inReg = &CORE_PIN18_PINREG;
			bitmask = CORE_PIN18_BITMASK;
			break;
		case 19:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN19_PORTREG, CORE_PIN19_BIT);
			outSetReg = &CORE_PIN19_PORTSET;
			inReg = &CORE_PIN19_PINREG;
			bitmask = CORE_PIN19_BITMASK;
			break;
		case 20:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN20_PORTREG, CORE_PIN20_BIT);
			outSetReg = &CORE_PIN20_PORTSET;
			inReg = &CORE_PIN20_PINREG;
			bitmask = CORE_PIN20_BITMASK;
			break;
		case 21:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN21_PORTREG, CORE_PIN21_BIT);
			outSetReg = &CORE_PIN21_PORTSET;
			inReg = &CORE_PIN21_PINREG;
			bitmask = CORE_PIN21_BITMASK;
			break;
		case 22:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN22_PORTREG, CORE_PIN22_BIT);
			outSetReg = &CORE_PIN22_PORTSET;
			inReg = &CORE_PIN22_PINREG;
			bitmask = CORE_PIN22_BITMASK;
			break;
		case 23:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN23_PORTREG, CORE_PIN23_BIT);
			outSetReg = &CORE_PIN23_PORTSET;
			inReg = &CORE_PIN23_PINREG;
			bitmask = CORE_PIN23_BITMASK;
			break;
		case 24:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN24_PORTREG, CORE_PIN24_BIT);
			outSetReg = &CORE_PIN24_PORTSET;
			inReg = &CORE_PIN24_PINREG;
			bitmask = CORE_PIN24_BITMASK;
			break;
		case 25:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN25_PORTREG, CORE_PIN25_BIT);
			outSetReg = &CORE_PIN25_PORTSET;
			inReg = &CORE_PIN25_PINREG;
			bitmask = CORE_PIN25_BITMASK;
			break;
		case 26:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN26_PORTREG, CORE_PIN26_BIT);
			outSetReg = &CORE_PIN26_PORTSET;
			inReg = &CORE_PIN26_PINREG;
			bitmask = CORE_PIN26_BITMASK;
			break;
		case 27:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN27_PORTREG, CORE_PIN27_BIT);
			outSetReg = &CORE_PIN27_PORTSET;
			inReg = &CORE_PIN27_PINREG;
			bitmask = CORE_PIN27_BITMASK;
			break;
		case 28:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN28_PORTREG, CORE_PIN28_BIT);
			outSetReg = &CORE_PIN28_PORTSET;
			inReg = &CORE_PIN28_PINREG;
			bitmask = CORE_PIN28_BITMASK;
			break;
		case 29:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN29_PORTREG, CORE_PIN29_BIT);
			outSetReg = &CORE_PIN29_PORTSET;
			inReg = &CORE_PIN29_PINREG;
			bitmask = CORE_PIN29_BITMASK;
			break;
		case 30:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN30_PORTREG, CORE_PIN30_BIT);
			outSetReg = &CORE_PIN30_PORTSET;
			inReg = &CORE_PIN30_PINREG;
			bitmask = CORE_PIN30_BITMASK;
			break;
		case 31:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN31_PORTREG, CORE_PIN31_BIT);
			outSetReg = &CORE_PIN31_PORTSET;
			inReg = &CORE_PIN31_PINREG;
			bitmask = CORE_PIN31_BITMASK;
			break;
		case 32:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN32_PORTREG, CORE_PIN32_BIT);
			outSetReg = &CORE_PIN32_PORTSET;
			inReg = &CORE_PIN32_PINREG;
			bitmask = CORE_PIN32_BITMASK;
			break;
		case 33:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN33_PORTREG, CORE_PIN33_BIT);
			outSetReg = &CORE_PIN33_PORTSET;
			inReg = &CORE_PIN33_PINREG;
			bitmask = CORE_PIN33_BITMASK;
			break;
		case 34:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN34_PORTREG, CORE_PIN34_BIT);
			outSetReg = &CORE_PIN34_PORTSET;
			inReg = &CORE_PIN34_PINREG;
			bitmask = CORE_PIN34_BITMASK;
			break;
		case 35:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN35_PORTREG, CORE_PIN35_BIT);
			outSetReg = &CORE_PIN35_PORTSET;
			inReg = &CORE_PIN35_PINREG;
			bitmask = CORE_PIN35_BITMASK;
			break;
		case 36:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN36_PORTREG, CORE_PIN36_BIT);
			outSetReg = &CORE_PIN36_PORTSET;
			inReg = &CORE_PIN36_PINREG;
			bitmask = CORE_PIN36_BITMASK;
			break;
		case 37:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN37_PORTREG, CORE_PIN37_BIT);
			outSetReg = &CORE_PIN37_PORTSET;
			inReg = &CORE_PIN37_PINREG;
			bitmask = CORE_PIN37_BITMASK;
			break;
		case 38:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN38_PORTREG, CORE_PIN38_BIT);
			outSetReg = &CORE_PIN38_PORTSET;
			inReg = &CORE_PIN38_PINREG;
			bitmask = CORE_PIN38_BITMASK;
			break;
		case 39:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN39_PORTREG, CORE_PIN39_BIT);
			outSetReg = &CORE_PIN39_PORTSET;
			inReg = &CORE_PIN39_PINREG;
			bitmask = CORE_PIN39_BITMASK;
			break;
		case 40:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN40_PORTREG, CORE_PIN40_BIT);
			outSetReg = &CORE_PIN40_PORTSET;
			inReg = &CORE_PIN40_PINREG;
			bitmask = CORE_PIN40_BITMASK;
			break;
		case 41:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN41_PORTREG, CORE_PIN41_BIT);
			outSetReg = &CORE_PIN41_PORTSET;
			inReg = &CORE_PIN41_PINREG;
			bitmask = CORE_PIN41_BITMASK;
			break;
		case 42:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN42_PORTREG, CORE_PIN42_BIT);
			outSetReg = &CORE_PIN42_PORTSET;
			inReg = &CORE_PIN42_PINREG;
			bitmask = CORE_PIN42_BITMASK;
			break;
		case 43:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN43_PORTREG, CORE_PIN43_BIT);
			outSetReg = &CORE_PIN43_PORTSET;
			inReg = &CORE_PIN43_PINREG;
			bitmask = CORE_PIN43_BITMASK;
			break;
		case 44:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN44_PORTREG, CORE_PIN44_BIT);
			outSetReg = &CORE_PIN44_PORTSET;
			inReg = &CORE_PIN44_PINREG;
			bitmask = CORE_PIN44_BITMASK;
			break;
		case 45:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN45_PORTREG, CORE_PIN45_BIT);
			outSetReg = &CORE_PIN45_PORTSET;
			inReg = &CORE_PIN45_PINREG;
			bitmask = CORE_PIN45_BITMASK;
			break;
		case 46:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN46_PORTREG, CORE_PIN46_BIT);
			outSetReg = &CORE_PIN46_PORTSET;
			inReg = &CORE_PIN46_PINREG;
			bitmask = CORE_PIN46_BITMASK;
			break;
		case 47:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN47_PORTREG, CORE_PIN47_BIT);
			outSetReg = &CORE_PIN47_PORTSET;
			inReg = &CORE_PIN47_PINREG;
			bitmask = CORE_PIN47_BITMASK;
			break;
		case 48:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN48_PORTREG, CORE_PIN48_BIT);
			outSetReg = &CORE_PIN48_PORTSET;
			inReg = &CORE_PIN48_PINREG;
			bitmask = CORE_PIN48_BITMASK;
			break;
		case 49:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN49_PORTREG, CORE_PIN49_BIT);
			outSetReg = &CORE_PIN49_PORTSET;
			inReg = &CORE_PIN49_PINREG;
			bitmask = CORE_PIN49_BITMASK;
			break;
		case 50:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN50_PORTREG, CORE_PIN50_BIT);
			outSetReg = &CORE_PIN50_PORTSET;
			inReg = &CORE_PIN50_PINREG;
			bitmask = CORE_PIN50_BITMASK;
			break;
		case 51:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN51_PORTREG, CORE_PIN51_BIT);
			outSetReg = &CORE_PIN51_PORTSET;
			inReg = &CORE_PIN51_PINREG;
			bitmask = CORE_PIN51_BITMASK;
			break;
		case 52:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN52_PORTREG, CORE_PIN52_BIT);
			outSetReg = &CORE_PIN52_PORTSET;
			inReg = &CORE_PIN52_PINREG;
			bitmask = CORE_PIN52_BITMASK;
			break;
		case 53:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN53_PORTREG, CORE_PIN53_BIT);
			outSetReg = &CORE_PIN53_PORTSET;
			inReg = &CORE_PIN53_PINREG;
			bitmask = CORE_PIN53_BITMASK;
			break;
		case 54:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN54_PORTREG, CORE_PIN54_BIT);
			outSetReg = &CORE_PIN54_PORTSET;
			inReg = &CORE_PIN54_PINREG;
			bitmask = CORE_PIN54_BITMASK;
			break;
		case 55:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN55_PORTREG, CORE_PIN55_BIT);
			outSetReg = &CORE_PIN55_PORTSET;
			inReg = &CORE_PIN55_PINREG;
			bitmask = CORE_PIN55_BITMASK;
			break;
		case 56:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN56_PORTREG, CORE_PIN56_BIT);
			outSetReg = &CORE_PIN56_PORTSET;
			inReg = &CORE_PIN56_PINREG;
			bitmask = CORE_PIN56_BITMASK;
			break;
		case 57:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN57_PORTREG, CORE_PIN57_BIT);
			outSetReg = &CORE_PIN57_PORTSET;
			inReg = &CORE_PIN57_PINREG;
			bitmask = CORE_PIN57_BITMASK;
			break;
		case 58:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN58_PORTREG, CORE_PIN58_BIT);
			outSetReg = &CORE_PIN58_PORTSET;
			inReg = &CORE_PIN58_PINREG;
			bitmask = CORE_PIN58_BITMASK;
			break;
		case 59:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN59_PORTREG, CORE_PIN59_BIT);
			outSetReg = &CORE_PIN59_PORTSET;
			inReg = &CORE_PIN59_PINREG;
			bitmask = CORE_PIN59_BITMASK;
			break;
		case 60:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN60_PORTREG, CORE_PIN60_BIT);
			outSetReg = &CORE_PIN60_PORTSET;
			inReg = &CORE_PIN60_PINREG;
			bitmask = CORE_PIN60_BITMASK;
			break;
		case 61:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN61_PORTREG, CORE_PIN61_BIT);
			outSetReg = &CORE_PIN61_PORTSET;
			inReg = &CORE_PIN61_PINREG;
			bitmask = CORE_PIN61_BITMASK;
			break;
		case 62:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN62_PORTREG, CORE_PIN62_BIT);
			outSetReg = &CORE_PIN62_PORTSET;
			inReg = &CORE_PIN62_PINREG;
			bitmask = CORE_PIN62_BITMASK;
			break;
		case 63:
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN63_PORTREG, CORE_PIN63_BIT);
			outSetReg = &CORE_PIN63_PORTSET;
			inReg = &CORE_PIN63_PINREG;
			bitmask = CORE_PIN63_BITMASK;
		}
	}
	void init() {
		uint8_t command[] = { 0 };
		transceive(command, 3, state.raw);
	}
protected:
	uint32_t modeReg, bitmask;
	volatile uint32_t *outSetReg, *inReg, *buffer;
	//TODO: make it wait based on F_CPU
	inline void transceive(uint8_t *command, int len, uint8_t *data) {
		//change to work similar to n64pad
	}
};

class N64Controller : public Controller {
public:
	n64report report;
	N64Controller() : Controller() {}
	N64Controller(const int pin) : Controller(pin) {}
	void poll() {
		uint8_t command[] = { 1 };
		transceive(command, 4, report.raw);
	}
protected:

};

class GCController : public Controller {
public:
	gcorigin origin;
	gcreport report;
	GCController() : Controller() {}
	GCController(const int pin) : Controller(pin) {}
	void center() {
		uint8_t command[] = { 0x41 };
		transceive(command, 10, origin.raw);
	}
	void poll() {
		uint8_t command[] = { 0x43 };
		transceive(command, 8, report.raw);
	}
	void scan() {
		uint8_t command[] = { 0x54 };
		transceive(command, 8, report.raw);
	}
protected:

};

//TODO: check for different min timings for F_CPU, add a re-init feature
class WiiAttachment {
public:
	uint8_t raw[6];
	uint8_t id[6];
	WiiAttachment() : wire(i2c_t3(0)), pins(I2C_PINS_16_17) {}
	WiiAttachment(int bus, i2c_pins pins) : wire(i2c_t3(bus)), pins(pins) {}
	void identify() {
		wire.beginTransmission(CON);
		wire.write(0xFA);
		wire.write(0);
		while (wire.endTransmission());
		delayMicros(36);
		wire.requestFrom(CON, 6);
		wire.readBytes(id, 6);
	}
	void poll() {
		wire.beginTransmission(CON);
		wire.write(0);
		while (wire.endTransmission());
		delayMicros(157);
		wire.requestFrom(CON, 6);
		wire.readBytes(raw, 6);
		updateReport();
	}
	//starts attachment unencrypted
	void init() {
		wire.begin(I2C_MASTER, 0, pins, I2C_PULLUP_EXT, 400000);
		wire.beginTransmission(CON);
		wire.write(0xF0);
		wire.write(0x55);
		if (wire.endTransmission()) {
			//SOFT_RESET();
			//init();
			//return;
		}
		wire.beginTransmission(CON);
		wire.write(0xFB);
		wire.write(0);
		while (wire.endTransmission());
		//identify();
		poll();
	}
protected:
	i2c_t3 wire;
	i2c_pins pins;
	virtual void updateReport() {}
};

struct ncreport {
	uint8_t sx;
	uint8_t sy;

	uint32_t ax : 10;
	uint32_t ay : 10;
	uint32_t az : 10;

	uint32_t c : 1;
	uint32_t z : 1;
};

class Nunchuck : public WiiAttachment {
public:
	ncreport report;
	Nunchuck() : WiiAttachment() {}
	Nunchuck(int bus, i2c_pins pins) : WiiAttachment(bus, pins) {}
protected:
	void updateReport() {
		report.sx = raw[0];
		report.sy = raw[1];

		report.ax = raw[2] << 2 | (raw[5] >> 2 & 3);
		report.ay = raw[3] << 2 | (raw[5] >> 4 & 3);
		report.az = raw[4] << 2 | raw[5] >> 6;

		report.c = ~raw[5] >> 1;
		report.z = ~raw[5];
 	}
};

struct ccreport {
	uint32_t sx : 6;
	uint32_t sy : 6;
	uint32_t cx : 5;
	uint32_t cy : 5;

	uint32_t lt : 5;
	uint32_t rt : 5;

	union {
		uint8_t buttons[2] = { 1, 0 };

		struct {
			uint8_t : 1;
			uint8_t r : 1;
			uint8_t start : 1;
			uint8_t home : 1;
			uint8_t select : 1;
			uint8_t l : 1;
			uint8_t dd : 1;
			uint8_t dr : 1;

			uint8_t du : 1;
			uint8_t dl : 1;
			uint8_t zr : 1;
			uint8_t x : 1;
			uint8_t a : 1;
			uint8_t y : 1;
			uint8_t b : 1;
			uint8_t zl : 1;
		};
	};
};

class ClassicController : public WiiAttachment {
public:
	ccreport report;
	ClassicController() : WiiAttachment() {}
	ClassicController(int bus, i2c_pins pins) : WiiAttachment(bus, pins) {}
protected:
	void updateReport() {
		report.sx = raw[0];
		report.sy = raw[1];
		report.cx = raw[0] >> 3 | raw[1] >> 5 | raw[2] >> 7;
		report.cy = raw[2];

		report.lt = raw[2] >> 2 | raw[3] >> 5;
		report.rt = raw[3];
		
		report.buttons[0] = raw[4];
		report.buttons[1] = raw[5];
	}
};
