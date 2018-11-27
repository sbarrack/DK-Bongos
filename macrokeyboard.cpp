#include "macrokeyboard.h"

void MKB::init() {
	// init most useless digital pins
	pinMode(KEY1, INPUT);
#if MACRO_KEYS > 1
	pinMode(KEY2, INPUT);
#if MACRO_KEYS > 2
	pinMode(KEY3, INPUT); // PWM
#if MACRO_KEYS > 3
	pinMode(KEY4, INPUT); // A16
#if MACRO_KEYS > 4
	pinMode(KEY5, INPUT); // A17
#if MACRO_KEYS > 5
	pinMode(KEY6, INPUT); // Touch
#endif // MACRO_KEYS > 5
#endif // MACRO_KEYS > 4
#endif // MACRO_KEYS > 3
#endif // MACRO_KEYS > 2
#endif // MACRO_KEYS > 1

}

void MKB::scan() {
	buffer[1] = buffer[0];
	buffer[0].key1 = digitalReadFast(KEY1);
#if MACRO_KEYS > 1
	buffer[0].key2 = digitalReadFast(KEY2);
#if MACRO_KEYS > 2
	buffer[0].key3 = digitalReadFast(KEY3);
#if MACRO_KEYS > 3
	buffer[0].key4 = digitalReadFast(KEY4);
#if MACRO_KEYS > 4
	buffer[0].key5 = digitalReadFast(KEY5);
#if MACRO_KEYS > 5
	buffer[0].key6 = digitalReadFast(KEY6);
#endif // MACRO_KEYS > 5
#endif // MACRO_KEYS > 4
#endif // MACRO_KEYS > 3
#endif // MACRO_KEYS > 2
#endif // MACRO_KEYS > 1
	u8 exor = buffer[0].keys ^ buffer[1].keys;
	down.keys = exor & buffer[0].keys;
	held.keys = buffer[0].keys & buffer[1].keys;
	up.keys = exor & buffer[1].keys;

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
