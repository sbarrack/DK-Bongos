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

//gc stuff here

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
