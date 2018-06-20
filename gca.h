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

//gc.h
#pragma once
#include <Arduino.h>
#include "gcb.h"

#define tHigh			0x4A	//74
#define aMin			0x01	//1
#define aMax			0xFF	//255
#define aMid			0x80	//128, 127 is not true center!
#define sLowNotch		0x49	//73
#define sHighNotch		0xB5	//181
#define syUpperNotch	0xB3	//179
#define sMax			0xE9	//233
#define sMin			0x18	//24
#define sDeadLeft		0x69	//105
#define sDeadRight		0x98	//152
#define tLow			0x1F	//31
#define micHigh			0x6F	//111

#define gcc		0x0900
#define dkb		0x0900
#define gckb	0x0820
#define gba		0x0004
#define gcsw	0x0800

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

union gcReport {
	uint8_t raw[8];
	//gcc
	struct {
		uint8_t a : 1;
		uint8_t b : 1;
		uint8_t x : 1;
		uint8_t y : 1;
		uint8_t start : 1;
		uint8_t origin : 1;
		uint8_t errlatch : 1;
		uint8_t errstat : 1;

		uint8_t dleft : 1;
		uint8_t dright : 1;
		uint8_t ddown : 1;
		uint8_t dup : 1;
		uint8_t z : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t high1 : 1;

		uint8_t xAxis;
		uint8_t yAxis;
		uint8_t cxAxis;
		uint8_t cyAxis;
		uint8_t left;
		uint8_t right;
	};
	//dkb
	struct {
		uint8_t br : 1;
		uint8_t bl : 1;
		uint8_t tr : 1;
		uint8_t tl : 1;
		uint8_t start0 : 1;
		uint8_t origin0 : 1;
		uint8_t errlatch0 : 1;
		uint8_t errstat0 : 1;

		uint8_t zero0 : 7;
		uint8_t one : 1;

		uint8_t zero[5];
		uint8_t mic;
	};
	//gckb
	struct {
		uint8_t empty0 : 6;
		uint8_t errlatch1 : 1;
		uint8_t errstat1 : 1;

		uint8_t empty[3];
		uint8_t key1;
		uint8_t key2;
		uint8_t key3;
		uint8_t empty1;
	};
};

union gcOrigin {
	uint8_t raw[10];

	struct {
		gcReport inititalData;
		uint8_t deadzone0;
		uint8_t deadzone1;
	};
};

struct gcData {
	gcStatus status;
	gcOrigin origin;
	gcReport report;
};

#ifdef __cplusplus
extern "C" {
#endif
	bool gcInit(const uint8_t pin, gcStatus* status);
	bool gcReset(const uint8_t pin, gcStatus* status);
	bool gcCenter(const uint8_t pin, gcOrigin* origin);
	bool gcRead(const uint8_t pin, gcReport* report, const bool rumble);
	bool gckbRead(const uint8_t pin, gcReport* report);
	uint8_t gcWrite(const uint8_t pin, gcStatus* status, gcOrigin* origin, gcReport* report);
#ifdef __cplusplus
}
#endif
