#pragma once
#include <Arduino.h>
#include "types.h"

#ifndef MACRO_KEYS
#define MACRO_KEYS 1 // any more than 6 keys should use a matrix instead
#endif // !MACRO_KEYS
#ifndef KEY1
#define KEY1 2
#endif // !KEY1
#ifndef KEY2
#define KEY2 24
#endif // !KEY2
#ifndef KEY3
#define KEY3 6
#endif // !KEY3
#ifndef KEY4
#define KEY4 27
#endif // !KEY4
#ifndef KEY5
#define KEY5 28
#endif // !KEY5
#ifndef KEY6
#define KEY6 33
#endif // !KEY6

union Row {
	u8 keys;
	struct {
		// LSB order
		u8 key1 : 1;
		u8 key2 : 1;
		u8 key3 : 1;
		u8 key4 : 1;
		u8 key5 : 1;
		u8 key6 : 1;
		//u8 : 2;
	};
};

class MKB {
public:
	void init();
	void scan();
	void print();

private:
	Row buffer[2];
	Row down, held, up; // will tell you if a key is pressed, held, or released

};
