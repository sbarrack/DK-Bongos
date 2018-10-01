/* revolution.h is the interface for Wii Remote Attachments.
   
   Copyright (C) 2018  Stephen Barrack

   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*	Wii Remote attachments [1] use a 2-wire Interface (TWI/I2C) [2] which
	is supported by Teensy [3].

	[1] http://wiibrew.org/wiki/Wiimote/Extension_Controllers
	[2] https://en.wikipedia.org/wiki/I%C2%B2C
	[3] https://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
	
	Tested controllers: Nunchuck, Classic Controller (and Pro version), Guitar
	Tested Arduinos: Teensy 3.2, 3.5, 3.6
	
	The Wii Motion Plus is a useable device and does poll properly, but I
	removed support for it since it's useless.
*/

#pragma once
#include <i2c_t3.h>

#define MICROS(us)	delayMicroseconds(us)

#define CON	0x52

// wiimote attachment ids
const uint8_t nunchuck[6] = { 0, 0, 0xA4, 0x20, 0, 0 };
const uint8_t classic[6] = { 0, 0, 0xA4, 0x20, 1, 1 };		// no handles
const uint8_t classicPro[6] = { 1, 0, 0xA4, 0x20, 1, 1 };	// handles
const uint8_t guitar[6] = { 0, 0, 0xA4, 0x20, 1, 3 };

// TODO: check for different min timings for F_CPU, add an auto re-init feature
class WiiAttachment {
public:
	uint8_t raw[6];
	uint8_t id[6];
	WiiAttachment() : wire(i2c_t3(0)), pins(I2C_PINS_16_17) {}
	WiiAttachment(int bus, i2c_pins pins) : wire(i2c_t3(bus)), pins(pins) {}
	void identify(), poll(), init();
	
protected:
	i2c_t3 wire;
	i2c_pins pins;
	bool jally;
	virtual void updateReport() {}
	void jalhalla();
};

struct NunchuckReport {
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
	NunchuckReport report;
	Nunchuck() : WiiAttachment() {}
	Nunchuck(int bus, i2c_pins pins) : WiiAttachment(bus, pins) {}
protected:
	void updateReport();
};

struct ClassicControllerReport {
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
			uint8_t select : 1;
			uint8_t home : 1;
			uint8_t start : 1;
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
	ClassicControllerReport report;
	ClassicController() : WiiAttachment() {}
	ClassicController(int bus, i2c_pins pins) : WiiAttachment(bus, pins) {}
protected:
	void updateReport();
};

struct GuitarWiiReport {
	uint32_t sx : 6;
	uint32_t sy : 6;

	uint32_t cy : 5;	// touch bar
	uint32_t rt : 5;	// whammy bar

	union {
		uint8_t buttons[2];
		
		struct {
			uint8_t : 2;
			uint8_t select : 1;	// either +
			uint8_t : 1;
			uint8_t start : 1;	// Star Power
			uint8_t : 1;
			uint8_t dd : 1;	// strum down
			uint8_t : 1;
					  
			uint8_t du : 1;	// strum up
			uint8_t : 2;
			uint8_t x : 1;	// yellow (3)
			uint8_t a : 1;	// green (1)
			uint8_t y : 1;	// blue (4)
			uint8_t b : 1;	// red (2)
			uint8_t zl : 1;	// orange (5)
		};
	};
};

class GuitarWii : public WiiAttachment {
public:
	GuitarWiiReport report;
	GuitarWii() : WiiAttachment() {}
	GuitarWii(int bus, i2c_pins pins) : WiiAttachment(bus, pins) {}
	void sendJoyUSB();
protected:
	void updateReport();
};
