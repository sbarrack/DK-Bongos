/*	This is ProjectBongo; a program dedicated to the functionality of Donkey 
	Kong Bongos and other game controllers.

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
	it is simply a faster version of the 1-Wire Interface from MSB to LSB
	(https://en.wikipedia.org/wiki/1-Wire). Additionally, Wii Remote
	attachment controllers use the 2-Wire Interface (TWI/I2C) which is
	supported by Teensy. Timing for ARM instructions can be found here,
	http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0439b/CHDDIGAC.html,
	while usage can be found in the reference manual section A7.7
	(https://www.pjrc.com/teensy/beta/DDI0403D_arm_architecture_v7m_reference_manual.pdf).
	Also, while the No-Operation Instruction (NOP) consumes time on AVR,
	the ARM pipeline treats them as padding only
	(http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/CHDJJGFB.html)
	so I adjusted accordingly to get exact timing.

	Controllers: GC GCController, DK Bongos, GC Keyboard (not tested),
	Nunchuck, Classic GCController
	Arduinos: Teensy 3.5 120MHz
	*/

#pragma once
#include <i2c_t3.h>

//useful macros
#define delayMicros(us) delayMicroseconds(us)
#define SOFT_RESET() (*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004
#define GPIO_BITBAND_ADDR(reg, bit) \
	(((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)

//analog values
#define ANALOG_ERROR	0x00	//The controller disconnects if any analog sensor fails.
#define ANALOG_MIN		0x01
#define ANALOG_MAX		0xFF	
#define ANALOG_MID		0x80	//127 is not the center due to the error condition.
#define TRIGGER_LOW		0x1F
#define TRIGGER_FLOOR	0x49	//Light-shield is active above this value. 
#define TRIGGER_CEIL	0xFF	//TODO: need to find value just before the trigger button goes high
#define MIC_HIGH		0x6F
#define MIC_LOW			0x70
//TODO: double check these values
#define DEAD_LOW		0x68
#define DEAD_HIGH		0x98
#define STICK_LOW		0x4B
#define STICK_HIGH		0xB3
#define STICK_HIGHER	0xB5
#define STICK_MAX		0xE9
#define STICK_MIN		0x17
//adjusted analog values
#define DEAD_LOW_ADJ		-24
#define DEAD_HIGH_ADJ		24
#define STICK_LOW_ADJ		-53
#define STICK_HIGHISH_ADJ	51
#define STICK_HIGH_ADJ		53
#define STICK_MAX_ADJ		105
#define STICK_MIN_ADJ		-105

//1-wire device ids
#define GCC		0x0900
#define BONGOS	0x0900
//TODO: double check
#define N64C			0x0500
#define GC_KEYBOARD		0x0820
#define GBA				0x0004
#define STEERING_WHEEL	0x0800
#define CHAINSAW		0x0900

//twi slave device numbers
#define CON 0x52
#define PLUS 0x53

//wiimote attachment ids
#define NUNCHUCK 0x0000A4200000
#define CLASSIC 0x0000A4200101 //no handles
#define CLASSIC_PRO 0x0100A4200101 //handles
#define MOTION_PLUS 0x0000A4200405
#define NUNCHUCK_PASSTHROUGH 0x0000A4200505
#define CLASSIC_PASSTHROUGH 0x0000A4200705
//TODO: double check
#define GRAPHIC_TABLET 0xFF00A4200013
#define GUITAR 0x0000A4200103
#define DRUMS 0x0100A4200103
#define TURNTABLE 0x0300A4200103
#define TATACON 0x0000A4200111

//scancodes
#define GCKB_HOME			0x06
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
#define GCKB_NUMLOCK		0x6A

/*struct N64 {
	uint8_t a : 1;
	uint8_t b : 1;
	uint8_t z : 1;
	uint8_t start : 1;
	uint8_t du : 1;
	uint8_t dd : 1;
	uint8_t dl : 1;
	uint8_t dr : 1;

	uint8_t reset : 1;	//l+r+start
	uint8_t : 1;
	uint8_t l : 1;
	uint8_t r : 1;
	uint8_t cu : 1;
	uint8_t cd : 1;
	uint8_t cl : 1;
	uint8_t cr : 1;

	uint8_t xAxis;
	uint8_t yAxis;
};*/

union GCReport {
	uint8_t raw[8];

	struct {
		uint8_t errorStatus : 1;
		uint8_t errorLatch : 1;
		uint8_t getOrigin : 1;	//x+y+start
		uint8_t start : 1;
		uint8_t y : 1;	//tl
		uint8_t x : 1;	//tr
		uint8_t b : 1;	//bl
		uint8_t a : 1;	//br

		uint8_t useOrigin : 1;
		uint8_t l : 1;
		uint8_t r : 1;
		uint8_t z : 1;
		uint8_t du : 1;
		uint8_t dd : 1;
		uint8_t dr : 1;
		uint8_t dl : 1;

		uint8_t xAxis;
		uint8_t yAxis;
		uint8_t cxAxis;	//key1
		uint8_t cyAxis;	//key2
		uint8_t left;	//key3
		uint8_t right;	//mic
	};
};

union GCOrigin {
	uint8_t raw[10];

	struct {
		GCReport report;
		uint8_t deadzone0;
		uint8_t deadzone1;
	};
};

union GCStatus {
	uint8_t raw[3];

	struct {
		uint16_t device;

		union {
			uint8_t status;

			struct {
				uint8_t : 3;
				uint8_t rumble : 1;
				uint8_t : 4;
			};
		};
	};
};

struct GCData {
	GCReport report;
	GCOrigin origin;
	GCStatus status;
};

static constexpr GCReport defaultGCReport = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW, TRIGGER_LOW };
static constexpr GCOrigin defaultOrigin = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW, TRIGGER_LOW, ANALOG_MID, ANALOG_MID };
static constexpr GCStatus defaultGCControllerStatus = { 9, 0, 3 };
static constexpr GCData defaultGCControllerData = { defaultGCReport, defaultOrigin, defaultGCControllerStatus };

//TODO: allow for seperate wire initialization based on different 
//arduinos and check timings for different frequencies
class Attachment {
public:
	Attachment() { id = 0; }

	void identify() {
		Wire.beginTransmission(CON);
		Wire.write(0xFA);
		Wire.write(0);
		while (Wire.endTransmission());
		delayMicros(36);
		Wire.requestFrom(CON, 6);
		Wire.readBytes(raw, 6);
		checkReset();
		//Serial.print("Using ID ");
		//ext.printRaw();
		for (int i = 0; i < 6; i++) {
			id |= (uint64_t)raw[i] << ((5 - i) << 3);
		}
	}

	void poll() {
		Wire.beginTransmission(CON);
		Wire.write(0);
		while (Wire.endTransmission());
		delayMicros(157);
		Wire.requestFrom(CON, 6);
		Wire.readBytes(raw, 6);
		checkReset();
	}

	void init() {
		//unencrypted init
		Wire.begin(I2C_MASTER, 0, 16, 17, I2C_PULLUP_EXT, 400000);
		Wire.beginTransmission(CON);
		Wire.write(0xF0);
		Wire.write(0x55);
		if (Wire.endTransmission())
			SOFT_RESET();
		Wire.beginTransmission(CON);
		Wire.write(0xFB);
		Wire.write(0);
		while (Wire.endTransmission());
		identify();
		poll();
	}

	//axes
	uint8_t xAxis() { return raw[0]; }
	uint8_t yAxis() { return raw[1]; }
	uint16_t ax() { return (((uint16_t)raw[2] & 0x00FF) << 2) | (raw[5] >> 2 & 3); }
	uint16_t ay() { return (((uint16_t)raw[3] & 0x00FF) << 2) | (raw[5] >> 4 & 3); }
	uint16_t az() { return (((uint16_t)raw[4] & 0x00FF) << 2) | (raw[5] >> 6 & 3); }

	//adjusted to be full byte prescision
	//TODO: check for bugs with remaps/macros
	uint8_t sxAxis() { return raw[0] << 2 & 0xFC; }
	uint8_t syAxis() { return raw[1] << 2 & 0xFC; }
	uint8_t cxAxis() { return (raw[0] | raw[1] >> 2 | raw[2] >> 4) & 0xF8; }
	uint8_t cyAxis() { return raw[2] << 3 & 0xF8; }
	uint8_t left() { return (raw[2] << 1 | raw[3] >> 2) & 0xF8; }
	uint8_t right() { return raw[3] << 3 & 0xF8; }

	//buttons
	uint8_t l() { return ~raw[4] >> 5 & 1; }
	uint8_t r() { return ~raw[4] >> 1 & 1; }
	uint8_t zl() { return ~raw[5] >> 7 & 1; }
	uint8_t zr() { return ~raw[5] >> 2 & 1; }

	uint8_t c() { return ~raw[5] >> 1 & 1; }
	uint8_t z() { return ~raw[5] & 1; }

	uint8_t dr() { return raw[4] >> 7 & 1; }
	uint8_t dd() { return raw[4] >> 6 & 1; }
	uint8_t dl() { return raw[5] >> 1 & 1; }
	uint8_t du() { return raw[5] & 1; }

	uint8_t select() { return raw[4] >> 4 & 1; }
	uint8_t home() { return raw[4] >> 3 & 1; }
	uint8_t start() { return raw[4] >> 2 & 1; }

	uint8_t b() { return raw[5] >> 6 & 1; }
	uint8_t y() { return raw[5] >> 5 & 1; }
	uint8_t a() { return raw[5] >> 4 & 1; }
	uint8_t x() { return raw[5] >> 3 & 1; }

protected:
	uint8_t raw[6];
	uint64_t id;
	void checkReset() {
		uint8_t c = 0xFF;
		for (int i = 0; i < 6; i++)
			c &= raw[i];
		if (c == 0xFF)
			SOFT_RESET();
	}
};

class GCController : protected GCData {
public:
	GCController(const int pin) : pin(pin) {
		//there's 64 pins XD
		switch (pin) {
		case 0:
			corePinShifted = 31 - CORE_PIN0_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
			outputSetReg = &CORE_PIN0_PORTSET;
			inputReg = &CORE_PIN0_PINREG;
			bitmask = CORE_PIN0_BITMASK;
			break;
		case 1:
			corePinShifted = 31 - CORE_PIN1_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN1_PORTREG, CORE_PIN1_BIT);
			outputSetReg = &CORE_PIN1_PORTSET;
			inputReg = &CORE_PIN1_PINREG;
			bitmask = CORE_PIN1_BITMASK;
			break;
		case 2:
			corePinShifted = 31 - CORE_PIN2_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN2_PORTREG, CORE_PIN2_BIT);
			outputSetReg = &CORE_PIN2_PORTSET;
			inputReg = &CORE_PIN2_PINREG;
			bitmask = CORE_PIN2_BITMASK;
			break;
		case 3:
			corePinShifted = 31 - CORE_PIN3_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN3_PORTREG, CORE_PIN3_BIT);
			outputSetReg = &CORE_PIN3_PORTSET;
			inputReg = &CORE_PIN3_PINREG;
			bitmask = CORE_PIN3_BITMASK;
			break;
		case 4:
			corePinShifted = 31 - CORE_PIN4_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN4_PORTREG, CORE_PIN4_BIT);
			outputSetReg = &CORE_PIN4_PORTSET;
			inputReg = &CORE_PIN4_PINREG;
			bitmask = CORE_PIN4_BITMASK;
			break;
		case 5:
			corePinShifted = 31 - CORE_PIN5_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN5_PORTREG, CORE_PIN5_BIT);
			outputSetReg = &CORE_PIN5_PORTSET;
			inputReg = &CORE_PIN5_PINREG;
			bitmask = CORE_PIN5_BITMASK;
			break;
		case 6:
			corePinShifted = 31 - CORE_PIN6_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN6_PORTREG, CORE_PIN6_BIT);
			outputSetReg = &CORE_PIN6_PORTSET;
			inputReg = &CORE_PIN6_PINREG;
			bitmask = CORE_PIN6_BITMASK;
			break;
		case 7:
			corePinShifted = 31 - CORE_PIN7_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN7_PORTREG, CORE_PIN7_BIT);
			outputSetReg = &CORE_PIN7_PORTSET;
			inputReg = &CORE_PIN7_PINREG;
			bitmask = CORE_PIN7_BITMASK;
			break;
		case 8:
			corePinShifted = 31 - CORE_PIN8_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN8_PORTREG, CORE_PIN8_BIT);
			outputSetReg = &CORE_PIN8_PORTSET;
			inputReg = &CORE_PIN8_PINREG;
			bitmask = CORE_PIN8_BITMASK;
			break;
		case 9:
			corePinShifted = 31 - CORE_PIN9_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN9_PORTREG, CORE_PIN9_BIT);
			outputSetReg = &CORE_PIN9_PORTSET;
			inputReg = &CORE_PIN9_PINREG;
			bitmask = CORE_PIN9_BITMASK;
			break;
		case 10:
			corePinShifted = 31 - CORE_PIN10_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN10_PORTREG, CORE_PIN10_BIT);
			outputSetReg = &CORE_PIN10_PORTSET;
			inputReg = &CORE_PIN10_PINREG;
			bitmask = CORE_PIN10_BITMASK;
			break;
		case 11:
			corePinShifted = 31 - CORE_PIN11_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN11_PORTREG, CORE_PIN11_BIT);
			outputSetReg = &CORE_PIN11_PORTSET;
			inputReg = &CORE_PIN11_PINREG;
			bitmask = CORE_PIN11_BITMASK;
			break;
		case 12:
			corePinShifted = 31 - CORE_PIN12_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
			outputSetReg = &CORE_PIN12_PORTSET;
			inputReg = &CORE_PIN12_PINREG;
			bitmask = CORE_PIN12_BITMASK;
			break;
		case 13:
			corePinShifted = 31 - CORE_PIN13_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN13_PORTREG, CORE_PIN13_BIT);
			outputSetReg = &CORE_PIN13_PORTSET;
			inputReg = &CORE_PIN13_PINREG;
			bitmask = CORE_PIN13_BITMASK;
			break;
		case 14:
			corePinShifted = 31 - CORE_PIN14_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN14_PORTREG, CORE_PIN14_BIT);
			outputSetReg = &CORE_PIN14_PORTSET;
			inputReg = &CORE_PIN14_PINREG;
			bitmask = CORE_PIN14_BITMASK;
			break;
		case 15:
			corePinShifted = 31 - CORE_PIN15_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN15_PORTREG, CORE_PIN15_BIT);
			outputSetReg = &CORE_PIN15_PORTSET;
			inputReg = &CORE_PIN15_PINREG;
			bitmask = CORE_PIN15_BITMASK;
			break;
		case 16:
			corePinShifted = 31 - CORE_PIN16_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN16_PORTREG, CORE_PIN16_BIT);
			outputSetReg = &CORE_PIN16_PORTSET;
			inputReg = &CORE_PIN16_PINREG;
			bitmask = CORE_PIN16_BITMASK;
			break;
		case 17:
			corePinShifted = 31 - CORE_PIN17_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN17_PORTREG, CORE_PIN17_BIT);
			outputSetReg = &CORE_PIN17_PORTSET;
			inputReg = &CORE_PIN17_PINREG;
			bitmask = CORE_PIN17_BITMASK;
			break;
		case 18:
			corePinShifted = 31 - CORE_PIN18_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN18_PORTREG, CORE_PIN18_BIT);
			outputSetReg = &CORE_PIN18_PORTSET;
			inputReg = &CORE_PIN18_PINREG;
			bitmask = CORE_PIN18_BITMASK;
			break;
		case 19:
			corePinShifted = 31 - CORE_PIN19_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN19_PORTREG, CORE_PIN19_BIT);
			outputSetReg = &CORE_PIN19_PORTSET;
			inputReg = &CORE_PIN19_PINREG;
			bitmask = CORE_PIN19_BITMASK;
			break;
		case 20:
			corePinShifted = 31 - CORE_PIN20_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN20_PORTREG, CORE_PIN20_BIT);
			outputSetReg = &CORE_PIN20_PORTSET;
			inputReg = &CORE_PIN20_PINREG;
			bitmask = CORE_PIN20_BITMASK;
			break;
		case 21:
			corePinShifted = 31 - CORE_PIN21_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN21_PORTREG, CORE_PIN21_BIT);
			outputSetReg = &CORE_PIN21_PORTSET;
			inputReg = &CORE_PIN21_PINREG;
			bitmask = CORE_PIN21_BITMASK;
			break;
		case 22:
			corePinShifted = 31 - CORE_PIN22_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN22_PORTREG, CORE_PIN22_BIT);
			outputSetReg = &CORE_PIN22_PORTSET;
			inputReg = &CORE_PIN22_PINREG;
			bitmask = CORE_PIN22_BITMASK;
			break;
		case 23:
			corePinShifted = 31 - CORE_PIN23_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN23_PORTREG, CORE_PIN23_BIT);
			outputSetReg = &CORE_PIN23_PORTSET;
			inputReg = &CORE_PIN23_PINREG;
			bitmask = CORE_PIN23_BITMASK;
			break;
		case 24:
			corePinShifted = 31 - CORE_PIN24_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN24_PORTREG, CORE_PIN24_BIT);
			outputSetReg = &CORE_PIN24_PORTSET;
			inputReg = &CORE_PIN24_PINREG;
			bitmask = CORE_PIN24_BITMASK;
			break;
		case 25:
			corePinShifted = 31 - CORE_PIN25_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN25_PORTREG, CORE_PIN25_BIT);
			outputSetReg = &CORE_PIN25_PORTSET;
			inputReg = &CORE_PIN25_PINREG;
			bitmask = CORE_PIN25_BITMASK;
			break;
		case 26:
			corePinShifted = 31 - CORE_PIN26_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN26_PORTREG, CORE_PIN26_BIT);
			outputSetReg = &CORE_PIN26_PORTSET;
			inputReg = &CORE_PIN26_PINREG;
			bitmask = CORE_PIN26_BITMASK;
			break;
		case 27:
			corePinShifted = 31 - CORE_PIN27_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN27_PORTREG, CORE_PIN27_BIT);
			outputSetReg = &CORE_PIN27_PORTSET;
			inputReg = &CORE_PIN27_PINREG;
			bitmask = CORE_PIN27_BITMASK;
			break;
		case 28:
			corePinShifted = 31 - CORE_PIN28_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN28_PORTREG, CORE_PIN28_BIT);
			outputSetReg = &CORE_PIN28_PORTSET;
			inputReg = &CORE_PIN28_PINREG;
			bitmask = CORE_PIN28_BITMASK;
			break;
		case 29:
			corePinShifted = 31 - CORE_PIN29_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN29_PORTREG, CORE_PIN29_BIT);
			outputSetReg = &CORE_PIN29_PORTSET;
			inputReg = &CORE_PIN29_PINREG;
			bitmask = CORE_PIN29_BITMASK;
			break;
		case 30:
			corePinShifted = 31 - CORE_PIN30_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN30_PORTREG, CORE_PIN30_BIT);
			outputSetReg = &CORE_PIN30_PORTSET;
			inputReg = &CORE_PIN30_PINREG;
			bitmask = CORE_PIN30_BITMASK;
			break;
		case 31:
			corePinShifted = 31 - CORE_PIN31_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN31_PORTREG, CORE_PIN31_BIT);
			outputSetReg = &CORE_PIN31_PORTSET;
			inputReg = &CORE_PIN31_PINREG;
			bitmask = CORE_PIN31_BITMASK;
			break;
		case 32:
			corePinShifted = 31 - CORE_PIN32_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN32_PORTREG, CORE_PIN32_BIT);
			outputSetReg = &CORE_PIN32_PORTSET;
			inputReg = &CORE_PIN32_PINREG;
			bitmask = CORE_PIN32_BITMASK;
			break;
		case 33:
			corePinShifted = 31 - CORE_PIN33_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN33_PORTREG, CORE_PIN33_BIT);
			outputSetReg = &CORE_PIN33_PORTSET;
			inputReg = &CORE_PIN33_PINREG;
			bitmask = CORE_PIN33_BITMASK;
			break;
		case 34:
			corePinShifted = 31 - CORE_PIN34_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN34_PORTREG, CORE_PIN34_BIT);
			outputSetReg = &CORE_PIN34_PORTSET;
			inputReg = &CORE_PIN34_PINREG;
			bitmask = CORE_PIN34_BITMASK;
			break;
		case 35:
			corePinShifted = 31 - CORE_PIN35_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN35_PORTREG, CORE_PIN35_BIT);
			outputSetReg = &CORE_PIN35_PORTSET;
			inputReg = &CORE_PIN35_PINREG;
			bitmask = CORE_PIN35_BITMASK;
			break;
		case 36:
			corePinShifted = 31 - CORE_PIN36_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN36_PORTREG, CORE_PIN36_BIT);
			outputSetReg = &CORE_PIN36_PORTSET;
			inputReg = &CORE_PIN36_PINREG;
			bitmask = CORE_PIN36_BITMASK;
			break;
		case 37:
			corePinShifted = 31 - CORE_PIN37_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN37_PORTREG, CORE_PIN37_BIT);
			outputSetReg = &CORE_PIN37_PORTSET;
			inputReg = &CORE_PIN37_PINREG;
			bitmask = CORE_PIN37_BITMASK;
			break;
		case 38:
			corePinShifted = 31 - CORE_PIN38_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN38_PORTREG, CORE_PIN38_BIT);
			outputSetReg = &CORE_PIN38_PORTSET;
			inputReg = &CORE_PIN38_PINREG;
			bitmask = CORE_PIN38_BITMASK;
			break;
		case 39:
			corePinShifted = 31 - CORE_PIN39_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN39_PORTREG, CORE_PIN39_BIT);
			outputSetReg = &CORE_PIN39_PORTSET;
			inputReg = &CORE_PIN39_PINREG;
			bitmask = CORE_PIN39_BITMASK;
			break;
		case 40:
			corePinShifted = 31 - CORE_PIN40_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN40_PORTREG, CORE_PIN40_BIT);
			outputSetReg = &CORE_PIN40_PORTSET;
			inputReg = &CORE_PIN40_PINREG;
			bitmask = CORE_PIN40_BITMASK;
			break;
		case 41:
			corePinShifted = 31 - CORE_PIN41_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN41_PORTREG, CORE_PIN41_BIT);
			outputSetReg = &CORE_PIN41_PORTSET;
			inputReg = &CORE_PIN41_PINREG;
			bitmask = CORE_PIN41_BITMASK;
			break;
		case 42:
			corePinShifted = 31 - CORE_PIN42_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN42_PORTREG, CORE_PIN42_BIT);
			outputSetReg = &CORE_PIN42_PORTSET;
			inputReg = &CORE_PIN42_PINREG;
			bitmask = CORE_PIN42_BITMASK;
			break;
		case 43:
			corePinShifted = 31 - CORE_PIN43_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN43_PORTREG, CORE_PIN43_BIT);
			outputSetReg = &CORE_PIN43_PORTSET;
			inputReg = &CORE_PIN43_PINREG;
			bitmask = CORE_PIN43_BITMASK;
			break;
		case 44:
			corePinShifted = 31 - CORE_PIN44_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN44_PORTREG, CORE_PIN44_BIT);
			outputSetReg = &CORE_PIN44_PORTSET;
			inputReg = &CORE_PIN44_PINREG;
			bitmask = CORE_PIN44_BITMASK;
			break;
		case 45:
			corePinShifted = 31 - CORE_PIN45_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN45_PORTREG, CORE_PIN45_BIT);
			outputSetReg = &CORE_PIN45_PORTSET;
			inputReg = &CORE_PIN45_PINREG;
			bitmask = CORE_PIN45_BITMASK;
			break;
		case 46:
			corePinShifted = 31 - CORE_PIN46_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN46_PORTREG, CORE_PIN46_BIT);
			outputSetReg = &CORE_PIN46_PORTSET;
			inputReg = &CORE_PIN46_PINREG;
			bitmask = CORE_PIN46_BITMASK;
			break;
		case 47:
			corePinShifted = 31 - CORE_PIN47_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN47_PORTREG, CORE_PIN47_BIT);
			outputSetReg = &CORE_PIN47_PORTSET;
			inputReg = &CORE_PIN47_PINREG;
			bitmask = CORE_PIN47_BITMASK;
			break;
		case 48:
			corePinShifted = 31 - CORE_PIN48_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN48_PORTREG, CORE_PIN48_BIT);
			outputSetReg = &CORE_PIN48_PORTSET;
			inputReg = &CORE_PIN48_PINREG;
			bitmask = CORE_PIN48_BITMASK;
			break;
		case 49:
			corePinShifted = 31 - CORE_PIN49_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN49_PORTREG, CORE_PIN49_BIT);
			outputSetReg = &CORE_PIN49_PORTSET;
			inputReg = &CORE_PIN49_PINREG;
			bitmask = CORE_PIN49_BITMASK;
			break;
		case 50:
			corePinShifted = 31 - CORE_PIN50_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN50_PORTREG, CORE_PIN50_BIT);
			outputSetReg = &CORE_PIN50_PORTSET;
			inputReg = &CORE_PIN50_PINREG;
			bitmask = CORE_PIN50_BITMASK;
			break;
		case 51:
			corePinShifted = 31 - CORE_PIN51_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN51_PORTREG, CORE_PIN51_BIT);
			outputSetReg = &CORE_PIN51_PORTSET;
			inputReg = &CORE_PIN51_PINREG;
			bitmask = CORE_PIN51_BITMASK;
			break;
		case 52:
			corePinShifted = 31 - CORE_PIN52_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN52_PORTREG, CORE_PIN52_BIT);
			outputSetReg = &CORE_PIN52_PORTSET;
			inputReg = &CORE_PIN52_PINREG;
			bitmask = CORE_PIN52_BITMASK;
			break;
		case 53:
			corePinShifted = 31 - CORE_PIN53_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN53_PORTREG, CORE_PIN53_BIT);
			outputSetReg = &CORE_PIN53_PORTSET;
			inputReg = &CORE_PIN53_PINREG;
			bitmask = CORE_PIN53_BITMASK;
			break;
		case 54:
			corePinShifted = 31 - CORE_PIN54_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN54_PORTREG, CORE_PIN54_BIT);
			outputSetReg = &CORE_PIN54_PORTSET;
			inputReg = &CORE_PIN54_PINREG;
			bitmask = CORE_PIN54_BITMASK;
			break;
		case 55:
			corePinShifted = 31 - CORE_PIN55_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN55_PORTREG, CORE_PIN55_BIT);
			outputSetReg = &CORE_PIN55_PORTSET;
			inputReg = &CORE_PIN55_PINREG;
			bitmask = CORE_PIN55_BITMASK;
			break;
		case 56:
			corePinShifted = 31 - CORE_PIN56_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN56_PORTREG, CORE_PIN56_BIT);
			outputSetReg = &CORE_PIN56_PORTSET;
			inputReg = &CORE_PIN56_PINREG;
			bitmask = CORE_PIN56_BITMASK;
			break;
		case 57:
			corePinShifted = 31 - CORE_PIN57_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN57_PORTREG, CORE_PIN57_BIT);
			outputSetReg = &CORE_PIN57_PORTSET;
			inputReg = &CORE_PIN57_PINREG;
			bitmask = CORE_PIN57_BITMASK;
			break;
		case 58:
			corePinShifted = 31 - CORE_PIN58_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN58_PORTREG, CORE_PIN58_BIT);
			outputSetReg = &CORE_PIN58_PORTSET;
			inputReg = &CORE_PIN58_PINREG;
			bitmask = CORE_PIN58_BITMASK;
			break;
		case 59:
			corePinShifted = 31 - CORE_PIN59_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN59_PORTREG, CORE_PIN59_BIT);
			outputSetReg = &CORE_PIN59_PORTSET;
			inputReg = &CORE_PIN59_PINREG;
			bitmask = CORE_PIN59_BITMASK;
			break;
		case 60:
			corePinShifted = 31 - CORE_PIN60_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN60_PORTREG, CORE_PIN60_BIT);
			outputSetReg = &CORE_PIN60_PORTSET;
			inputReg = &CORE_PIN60_PINREG;
			bitmask = CORE_PIN60_BITMASK;
			break;
		case 61:
			corePinShifted = 31 - CORE_PIN61_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN61_PORTREG, CORE_PIN61_BIT);
			outputSetReg = &CORE_PIN61_PORTSET;
			inputReg = &CORE_PIN61_PINREG;
			bitmask = CORE_PIN61_BITMASK;
			break;
		case 62:
			corePinShifted = 31 - CORE_PIN62_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN62_PORTREG, CORE_PIN62_BIT);
			outputSetReg = &CORE_PIN62_PORTSET;
			inputReg = &CORE_PIN62_PINREG;
			bitmask = CORE_PIN62_BITMASK;
			break;
		case 63:
			corePinShifted = 31 - CORE_PIN63_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN63_PORTREG, CORE_PIN63_BIT);
			outputSetReg = &CORE_PIN63_PORTSET;
			inputReg = &CORE_PIN63_PINREG;
			bitmask = CORE_PIN63_BITMASK;
		}
	}
	GCReport getReport() { return report; }
	GCOrigin getOrigin() { return origin; }
	GCStatus getStatus() { return status; }
	//TODO: inplement n64 data
	GCData getData() {
		GCData *pointer = this;
		GCData data;
		memcpy(&data, pointer, sizeof(data));
		return data;
	}
	void init() {
		uint8_t command[] = { 0x00 };
		transceive(command, sizeof(command), status.raw, sizeof(status.raw));
	}
	uint8_t* pollOld() {
		uint8_t command[] = { 0x01 };
		uint8_t temp[2];
		transceive(command, sizeof(command), temp, sizeof(temp));
		return temp;
	}
	void recenter() {
		uint8_t command[] = { 0x41 };
		transceive(command, sizeof(command), origin.raw, sizeof(origin.raw));
	}
	void poll() {
		uint8_t command[] = { 0x43 };
		transceive(command, sizeof(command), report.raw, sizeof(report.raw));
	}
	void scan() {
		uint8_t command[] = { 0x54 };
		transceive(command, sizeof(command), report.raw, sizeof(report.raw));
	}

protected:
	const int pin;
	uint32_t modeReg, bitmask, corePinShifted;
	volatile uint32_t *outputSetReg, *inputReg;
	//friend class GCConsole;
	
	//all data is tranceived MSB first, nops are pipeline padding only
	//TODO: make it wait based on F_CPU
	inline void transceive(uint8_t *command, int len, uint8_t *data, int size) {
		data = new uint8_t[size];
		uint8_t oldSREG = SREG;
		cli();
		__asm__ volatile(
			"ldr r0, =0 \n"	//r0 = 0
			//begin transmission
			"str r0, [%1] \n"	//output mode
			"0: \n"				//byte loop
			"ldr r1, =8 \n"
				"1: \n"				//bit loop
					"str %4, [%2, 4] \n"	//line low
					"bl wait%= \n"			//call wait subroutine
					"lsls %7, #1 \n"		//shift next bit into carry (C)
					"bcc 2f \n"				//if C then line high
					"str %4, [%2, 0] \n"	//line high
					"2: \n"
					"bl wait2%= \n"
				"subs r1, #1 \n"
				"bne 1b \n"
			"add %7, %7, #1 \n"	//next command byte
			"subs %5, #1 \n"
			"bne 0b \n"
			//stop bit
			"str %4, [%2, 4] \n"
			"bl wait%= \n"
			"str %4, [%2, 0] \n"
			"bl wait2%= \n"
			//begin reception
			"ldr r0, =1 \n"
			"str r0, [%1] \n"	//input mode
			//this method doesn't wait for the edge to read, crude but effective
			"3: \n"				//byte loop
			"ldr r1, =0 \n"
				"4: \n"				//bit loop
					"bl wait2%= \n"
					"ldr r0, [%3] \n"	//read
					//get the correct bit into the correct place in data using str and lsl and orr and and
					"and r0, %4 \n"	//bitmask
					"lsl r0, %8 \n"	//position each bit to the msb
					"lsr r0, r1 \n"	//position each bit to orr with data
					"orr %0, r0 \n"	//set bit in data
					"bl wait2%= \n"
				"add r1, #1 \n"
				"cmp r1, #8 \n"
				"bne 4b \n"
			"add %0, %0, #1 \n"	//next data byte
			"subs %6, #1 \n"
			"bne 3b \n"
			"b end%= \n"

			"wait2%=: \n"			//waits 2us instead
			"ldr r2, =75 \n"	//2 + r2 * 3 + 13 = 240c
			"b 5f \n"
			"wait%=: \n"			//wait subroutine
			"ldr r2, =36 \n"	//r2 * 3 + 12 = 1us@120MHz (120 cycles)
				"5: \n"
					"subs r2, #1 \n"	//r2--
				"bne 5b \n"			//if !r2 goto 1
			"bx lr \n"			//return from subroutine

			"end%=: \n"
			//".pool \n"	//stores constants; known directive?
			:
			: "r" (data), "r" (modeReg), "r" (outputSetReg), 
				"r" (inputReg), "r" (bitmask), "r" (len), 
				"r" (size), "r" (command), "r" (corePinShifted)
			: "r0", "r1", "r2", "r3"
		);
		SREG = oldSREG;
	}
};

/*class GCConsole {
public:
	GCConsole(const int pin) : pin(pin) {}

protected:
	const int pin;
};*/
