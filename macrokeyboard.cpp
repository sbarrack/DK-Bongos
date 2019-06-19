#include "macrokeyboard.h"

void MKB::init() {
	// init most useless digital pins
	pinMode(KEY1, INPUT_PULLUP);
	pinMode(KEY2, INPUT_PULLUP);
	pinMode(KEY3, INPUT_PULLUP);
	pinMode(KEY4, INPUT_PULLUP);
	pinMode(KEY5, INPUT_PULLUP);
}

void MKB::scan() {
	buffer[1] = buffer[0];
	buffer[0].key1 = digitalReadFast(KEY1);
	buffer[0].key2 = digitalReadFast(KEY2);
	buffer[0].key3 = digitalReadFast(KEY3);
	buffer[0].key4 = digitalReadFast(KEY4);
	buffer[0].key5 = digitalReadFast(KEY5);
	buffer[0].keys = ~buffer[0].keys & 0x1F;

	u8 exor = buffer[0].keys ^ buffer[1].keys;
	down.keys = exor & buffer[0].keys;
	held.keys = buffer[0].keys & buffer[1].keys;
	up.keys = exor & buffer[1].keys;
}

void MKB::print() {
	Serial.print(buffer[0].keys, HEX);
	Serial.print("|");
	Serial.print(buffer[1].keys, HEX);
	Serial.print(" ");
	Serial.print(down.keys, HEX);
	Serial.print("|");
	Serial.print(held.keys, HEX);
	Serial.print("|");
	Serial.println(up.keys, HEX);
}
