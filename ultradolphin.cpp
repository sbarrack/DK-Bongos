#include "ultradolphin.h"

void Controller::poll() {
	for (int i = 0; i < sizeof(in); i++) {
		in[i] = serial.readStringUntil(' ');
	}
	n64.sy = in[2].toInt();
	n64.sx = in[1].toInt();
	char buttons[16];
	in[0].toCharArray(buttons, sizeof(buttons));
	int max = sizeof(buttons) - 1;
	for (int i = 0; i < sizeof(buttons); i++) {
		n64.raw16[0] = buttons[i] == '4' ? n64.raw16[0] | (1 << (max - i)) : buttons[i] == '0' ? n64.raw16[0] & ~(1 << (max - i)) : n64.raw16[0];
	}

}

void Controller::print() {
	pad16(n64.raw16[0]);
	Serial.print(n64.raw16[0], HEX);
	Serial.print(" ");
	pad16(n64.raw16[2]);
	Serial.println(n64.raw16[1], HEX);

}

void Controller::pad16(uint16_t w) {
	Serial.print(w > 0xFFF ? "" : w > 0xFF ? "0" : w > 0xF ? "00" : "000");

}
