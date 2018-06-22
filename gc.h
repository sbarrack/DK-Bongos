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

/*	This library is written from scratch to work similarly to the NicoHood 
	Nintendo library. The assembly has been rewritten from AVR to ARM 
	Cortex-M4 for Teensy 3.5/3.6 instead of Arduino Uno/Nano. All inline 
	assembly code is documented for easy reading. Also took inspiration 
	from Andrew Brownan's Gamecube-N64-Controller project and LogicalUnit's 
	N64_Interface. Plenty more to come in the future with support for more
	arduinos and game controllers!

	Controllers: GCC, DK Bongos, GC Keyboard
	Arduinos: Teensy 3.5 120MHz
	*/

//gcapi.h
#pragma once
#include "gca.h"

static constexpr gcReport emptyReport = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static constexpr gcReport dkbDefReport = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F };
static constexpr gcStatus gcDefStatus = { 0x09, 0x00, 0x03 };
static constexpr gcReport gcDefReport = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F };
static constexpr gcOrigin gcDefOrigin = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F, 0x00, 0x00 };
static constexpr gcData gcDefault = { gcDefStatus, gcDefOrigin, gcDefReport };

class gcController : protected gcData {
public:
	gcController(const uint8_t p);
	void reset(void);
	bool begin(void);
	uint16_t getDevice(void);
	bool connected(void);
	bool read(void);
	bool getRumble(void);
	bool setRumble(bool rumble);
	bool end(void);
	gcStatus getStatus(void);
	gcOrigin getOrigin(void);
	gcReport getReport(void);
	gcData getData(void);

protected:
	const uint8_t pin;
	friend class gcConsole;
};

class gcConsole {
public:
	gcConsole(const uint8_t p);
	bool write(gcData &data);
	bool write(gcController &controller);
	bool write(gcReport &report);

protected:
	const uint8_t pin;
};

#include "gc.hpp"
