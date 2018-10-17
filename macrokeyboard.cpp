#include "macrokeyboard.h"

void MKB::init() {
	// init most useless digital pins
	pinMode(KEY_1, INPUT);
#if MACRO_KEYS > 1
	pinMode(KEY_2, INPUT);
#if MACRO_KEYS > 2
	pinMode(KEY_3, INPUT); // PWM
#if MACRO_KEYS > 3
	pinMode(KEY_4, INPUT); // A16
#if MACRO_KEYS > 4
	pinMode(KEY_5, INPUT); // A17
#if MACRO_KEYS > 5
	pinMode(KEY_6, INPUT); // Touch
#endif // MACRO_KEYS > 5
#endif // MACRO_KEYS > 4
#endif // MACRO_KEYS > 3
#endif // MACRO_KEYS > 2
#endif // MACRO_KEYS > 1

}

void MKB::scan() {
	buffer[1] = buffer[0];
	buffer[0].key1 = digitalReadFast(KEY_1);
#if MACRO_KEYS > 1
	buffer[0].key2 = digitalReadFast(KEY_2);
#if MACRO_KEYS > 2
	buffer[0].key3 = digitalReadFast(KEY_3);
#if MACRO_KEYS > 3
	buffer[0].key4 = digitalReadFast(KEY_4);
#if MACRO_KEYS > 4
	buffer[0].key5 = digitalReadFast(KEY_5);
#if MACRO_KEYS > 5
	buffer[0].key6 = digitalReadFast(KEY_6);
#endif // MACRO_KEYS > 5
#endif // MACRO_KEYS > 4
#endif // MACRO_KEYS > 3
#endif // MACRO_KEYS > 2
#endif // MACRO_KEYS > 1
	uint8_t xor = buffer[0].keys ^ buffer[1].keys;
	down.keys = xor & buffer[0].keys;
	held.keys = buffer[0].keys & buffer[1].keys;
	up.keys = xor & buffer[1].keys;

}

void MKB::print() {
	Serial.print(buffer[0].keys, HEX);
	Serial.print("|");
	Serial.print(buffer[1].keys, HEX);
	Serial.print("|");
	Serial.print(down.keys, HEX);
	Serial.print("|");
	Serial.print(held.keys, HEX);
	Serial.print("|");
	Serial.println(up.keys, HEX);

}
