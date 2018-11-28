#pragma once
#include <Arduino.h>
#include "types.h"

#define KEY1 2
#define KEY2 6
#define KEY3 24
#define KEY4 25
#define KEY5 28

#define KEY_MEDIA_BRIGHT_D 0xe46f
#define KEY_MEDIA_BRIGHT_U 0xe470
#define KEY_MEDIA_TOGGLE_CAM 0xe478
#define KEY_MEDIA_MUSIC 0xe583
#define KEY_MEDIA_MAIL 0xe58a
#define KEY_MEDIA_CALC 0xe592
#define KEY_MEDIA_THIS_PC 0xe594

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
