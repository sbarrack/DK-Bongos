/*	This is ProjectBongo; a program dedicated to the functionality of Donkey Kong Bongos and beyond.
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
	it is simply a faster version of the 1-Wire protocol from MSB to LSB
	(https://en.wikipedia.org/wiki/1-Wire). 

	Controllers: GC Controller, DK Bongos, GC Keyboard, N64 Controller
	Arduinos: Teensy 3.5 120MHz, all Teensyduinos (planned)
	*/

#pragma once

#define ANALOG_ERROR	0x00	//The controller disconnects if any analog sensor fails.
#define ANALOG_MIN		0x01
#define ANALOG_MAX		0xFF	
#define ANALOG_MID		0x80	//127 is not the center due to the error condition.
#define TRIGGER_LOW		0x20
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

//devices
#define GCC		0x0900
#define BONGOS	0x0900
//TODO: double check
#define N64C			0x0500
#define GC_KEYBOARD		0x0820
#define GBA				0x0004
#define STEERING_WHEEL	0x0800
#define CHAINSAW		0x0900

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

union Report {
	uint8_t raw[8];
	uint8_t raw[4];
	struct {	//GCC
		uint8_t errorStatus : 1;
		uint8_t errorLatch : 1;
		uint8_t getOrigin : 1;	//x+y+start
		uint8_t start : 1;
		uint8_t y : 1;
		uint8_t x : 1;
		uint8_t b : 1;
		uint8_t a : 1;

		//uint8_t useOrigin : 1;
		uint8_t l : 1;
		uint8_t r : 1;
		uint8_t z : 1;
		uint8_t du : 1;
		uint8_t dd : 1;
		uint8_t dr : 1;
		uint8_t dl : 1;

		uint8_t xAxis;
		uint8_t yAxis;
		uint8_t cxAxis;
		uint8_t cyAxis;
		uint8_t lt;
		uint8_t rt;
	};
	struct {	//Bongos
		uint8_t errorStatus : 1;
		uint8_t errorLatch : 1;
		//uint8_t unused : 1;
		uint8_t start : 1;
		uint8_t tl : 1;
		uint8_t tr : 1;
		uint8_t bl : 1;
		uint8_t br : 1;

		//uint8_t unused[6];
		uint8_t mic;	//~16bit unsigned???
	};
	struct {	//GC Keyboard
		uint8_t errorStatus : 1;
		uint8_t errorLatch : 1;
		//uint8_t unknown : 6;

		//uint8_t unknown[3];
		uint8_t key1;
		uint8_t key2;
		uint8_t key3;
		//uint8_t unknown;
	};
	struct {	//N64 Controller
		uint8_t a : 1;
		uint8_t b : 1;
		uint8_t z : 1;
		uint8_t start : 1;
		uint8_t du : 1;
		uint8_t dd : 1;
		uint8_t dl : 1;
		uint8_t dr : 1;

		uint8_t reset : 1;	//l+r+start
		//uint8_t unknown : 1;
		uint8_t l : 1;
		uint8_t r : 1;
		uint8_t cu : 1;
		uint8_t cd : 1;
		uint8_t cl : 1;
		uint8_t cr : 1;

		uint8_t xAxis;
		uint8_t yAxis;
	};
};

//GC Controller Only
union Origin {
	uint8_t raw[10];
	uint8_t raw[4];

	struct {
		Report report;
		uint8_t deadzone0;
		uint8_t deadzone1;
	};
};

union Status {
	uint8_t raw[3];

	struct {
		uint16_t device;

		union {
			uint8_t status;

			struct {
				//uint8_t unknown : 3;
				uint8_t rumble : 1;
				//uint8_t unknown : 4;
			};
		};
	};
};

struct Data {
	Report report;
	Origin origin;
	Status status;
};

static constexpr Report defaultGCReport = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW - 1, TRIGGER_LOW - 1 };
static constexpr Origin defaultOrigin = { 0, 0x80, ANALOG_MID, ANALOG_MID, ANALOG_MID, ANALOG_MID, TRIGGER_LOW - 1, TRIGGER_LOW - 1, ANALOG_MID, ANALOG_MID };
static constexpr Status defaultGCControllerStatus = { GCC, 0x03 };
static constexpr Data defaultGCControllerData = { defaultGCReport, defaultOrigin, defaultGCControllerStatus };

class Controller : protected Data {
public:
	Controller(const int pin) : pin(pin) {}
	Report getReport() { return report; }
	Origin getOrigin() { return origin; }
	Status getStatus() { return status; }
	Data getData() {
		Data* pointer = this;
		Data data;
		memcpy(&data, pointer, sizeof(data));
		return data;
	}
	//other functions

protected:
	const int pin;
	friend class Console;
};

class Console {
public:
	Console(const int pin) : pin(pin) {}
	//other functions

protected:
	const int pin;
};
