//#include "macrokeyboard.h"
#include "ultradolphin.h"

//#define MACRO_KEYS 5

#define SOFT_RESET() (*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004

//MKB mkb;

Controller con;

void setup() {
	Serial.begin(115200);
	while (!Serial);
	//mkb.init();

}

void loop() {
	//mkb.scan();
	//mkb.print();
	con.poll();
	con.print();

}
