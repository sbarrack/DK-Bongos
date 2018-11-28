#pragma once
#include <Arduino.h>
#include "types.h"

#define KEY1 2
#define KEY2 6
#define KEY3 24
#define KEY4 25
#define KEY5 28

union Row {
	u8 keys;
	struct {
		// LSB order
		u8 key1 : 1;
		u8 key2 : 1;
		u8 key3 : 1;
		u8 key4 : 1;
		u8 key5 : 1;
		//u8 : 3;
	};
};

class MKB {
public:
	void init();
	void scan();
	void print();
	Row down, held, up; // will tell you if a key is pressed, held, or released
private:
	Row buffer[2];
};
