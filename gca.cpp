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

//gc.c
#include "gca.h"

bool gcInit(const uint8_t pin, gcStatus* status) {
	uint8_t command[] = { 0x00 };
	uint8_t receivedBytes = gcSendGet(pin, command, sizeof(command), (uint8_t*)status, sizeof(gcStatus));
	return (receivedBytes == sizeof(gcStatus));
}

bool gcReset(const uint8_t pin, gcStatus* status) {
	uint8_t command[] = { 0xFF };
	uint8_t receivedBytes = gcSendGet(pin, command, sizeof(command), (uint8_t*)status, sizeof(gcStatus));
	return (receivedBytes == sizeof(gcStatus));
}

bool gcCenter(const uint8_t pin, gcOrigin* origin) {
	uint8_t command[] = { 0x41 };
	uint8_t receivedBytes = gcSendGet(pin, command, sizeof(command), (uint8_t*)origin, sizeof(gcStatus));
	return (receivedBytes == sizeof(gcOrigin));
}

//0x42 is calibrate, 0x40 is shortpoll or rumble?

bool gcRead(const uint8_t pin, gcReport* report, const bool rumble) {
	uint8_t command[] = { 0x40, 0x03, rumble };
	uint8_t receivedBytes = gcSendGet(pin, command, sizeof(command), (uint8_t*)report, sizeof(gcReport));
	return (receivedBytes == sizeof(gcReport));
}

bool gckbRead(const uint8_t pin, gcReport* report) {
	uint8_t command[] = { 0x54 };
	uint8_t receivedBytes = gcSendGet(pin, command, sizeof(command), (uint8_t*)report, sizeof(gcReport));
	return (receivedBytes == sizeof(gcReport));
}

//gba read is 0x14, write is 0x15?

uint8_t gcWrite(const uint8_t pin, gcStatus* status, gcOrigin* origin, gcReport* report) {
	uint8_t ret = 0;

	uint8_t bitMask = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	volatile uint8_t* modePort = portModeRegister(port);
	volatile uint8_t* outPort = portOutputRegister(port);
	volatile uint8_t* inPort = portInputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	uint8_t command[3] = { 0,0,0 }; // TODO do not init
	uint8_t receivedBytes = gcGet(command, sizeof(command), modePort, outPort, inPort, bitMask);

	// Init
	if (receivedBytes == 1 && command[0] == 0x00) {
		gcSend(status->raw, sizeof(gcStatus), modePort, outPort, bitMask);
		ret = 1;
	}
	// Get origin
	else if (receivedBytes == 1 && command[0] == 0x41) {
		gcSend(origin->raw, sizeof(gcOrigin), modePort, outPort, bitMask);
		ret = 2;
	}
	else if (receivedBytes == 3 && command[0] == 0x40 && command[1] == 0x03) {
		gcSend(report->raw, sizeof(gcReport), modePort, outPort, bitMask);
		ret = 3;

		if ((command[2] % 4) == 0x01)
			ret = 4;
		else if ((command[2] % 4) == 0x02)
			ret = 5;
		else if ((command[2] % 4) == 0x03)
			ret = 6;
	}

	SREG = oldSREG;

	return ret;
}
