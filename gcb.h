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

//gcn64.h
#pragma once
#include <Arduino.h>

union gcStatus {
	uint8_t raw[3];

	struct {
		uint16_t device;

		union {
			uint8_t status;

			struct {
				uint8_t status0 : 3;
				uint8_t rumble : 1;
				uint8_t status1 : 4;
			};
		};
	};
};

#ifdef __cplusplus
extern "C" {
#endif
	void gcSend(const uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask) __attribute__((noinline));
	uint8_t gcGet(uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask)__attribute__((noinline));
	uint8_t gcSendGet(const uint8_t pin, uint8_t* command, const uint8_t commandLen, uint8_t* report, const uint8_t reportLen)__attribute__((noinline));
#ifdef __cplusplus
}
#endif
