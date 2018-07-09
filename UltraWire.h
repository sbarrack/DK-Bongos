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
	supported by Teensy. Timing for ARM instructions can be found here
	(http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0439b/CHDDIGAC.html)
	while usage can be found in the reference manual section A7.7
	(https://www.pjrc.com/teensy/beta/DDI0403D_arm_architecture_v7m_reference_manual.pdf).

	Controllers: GC GCController, DK Bongos, GC Keyboard (not tested),
	Nunchuck, Classic GCController
	Arduinos: Teensy 3.5 120MHz
	*/

#pragma once
#include <i2c_t3.h>

//useful macros
#define delayMicros(us) delayMicroseconds(us)
#define SOFT_RESET() (*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004

//assembly macros
#define GPIO_BITBAND_ADDR(reg, bit) \
	(((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
#define LINE_CLEAR	"str %4, [%2, 4] \n"
#define LINE_SET	"str %4, [%2, 4] \n"
//1us at 120MHz is 120 cycles (screw minor details), 4 + (2 + p) * r2 + nops = 120
#define WAIT "ldr r2, =38 \n1: \nsubs r2, #1 \nbne 1b \nnop \nnop \n"
#define SEND_0 LINE_CLEAR WAIT WAIT WAIT LINE_SET WAIT
#define SEND_1 LINE_CLEAR WAIT LINE_SET WAIT WAIT WAIT
//just read in the middle of the period instead of waiting for the edge
#define GET_BIT WAIT WAIT "ldr r1, [%3] \nstr r1, [%0] \nadd %0, %0, #4 \n" WAIT WAIT
#define GET_BYTE GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT GET_BIT

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
	GCController(const int pin) : pin(pin) {}
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
	//friend class GCConsole;
	
	//all data is tranceived MSB first
	//TODO: make it actually send/get the data you want
	static inline void transceive(uint8_t *command, const int len, uint8_t *data, const int size) {
		static uint32_t modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
		data = new uint8_t[size];
		uint8_t oldSREG = SREG;
		cli();
		__disable_irq();
		__asm__ volatile(
			"ldr r0, =0 \n"	//r0 = 0
			"ldr r1, =1 \n"	//r1 = 1
			"str r0, [%1] \n"	//output mode
			//transmit
			//0x43, poll
			SEND_0 SEND_1 SEND_0 SEND_0	//4
			SEND_0 SEND_0 SEND_1 SEND_1	//3
			SEND_1	//stop
			//receive
			//report
			GET_BYTE GET_BYTE GET_BYTE GET_BYTE GET_BYTE GET_BYTE	//6
			//".pool \n"
			:
			: "r" (data), "r" (modeReg), "r" (&CORE_PIN12_PORTSET), 
				"r" (&CORE_PIN12_PINREG), "r" (CORE_PIN12_BITMASK)
			: "r0", "r1", "r2"
		);
		__enable_irq();
		SREG = oldSREG;
	}
};

/*class GCConsole {
public:
	GCConsole(const int pin) : pin(pin) {}

protected:
	const int pin;
};*/
