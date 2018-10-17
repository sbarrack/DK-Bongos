#pragma once

#ifndef MACRO_KEYS
#define MACRO_KEYS 1 // any more than 6 keys should use a matrix instead
#endif // !MACRO_KEYS
#ifndef KEY_1
#define KEY_1 2
#endif // !KEY_1
#ifndef KEY_2
#define KEY_2 24
#endif // !KEY_2
#ifndef KEY_3
#define KEY_3 6
#endif // !KEY_3
#ifndef KEY_4
#define KEY_4 27
#endif // !KEY_4
#ifndef KEY_5
#define KEY_5 28
#endif // !KEY_5
#ifndef KEY_6
#define KEY_6 33
#endif // !KEY_6

union Row {
	uint8_t keys;
	struct {
		// LSB order
		uint8_t key1 : 1;
		uint8_t key2 : 1;
		uint8_t key3 : 1;
		uint8_t key4 : 1;
		uint8_t key5 : 1;
		uint8_t key6 : 1;
		//uint8_t : 2;
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
