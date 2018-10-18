#define MACRO_KEYS 5
#include "macrokeyboard.h"

#define SOFT_RESET() (*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004

MKB mkb;

// guitar code
/*void sendGuitarUSB() {
	Joystick.button(3, gh.report.a);
	Joystick.button(2, gh.report.b);
	Joystick.button(4, gh.report.y);
	Joystick.button(1, gh.report.x);
	Joystick.button(6, gh.report.zl);
	Joystick.button(10, gh.report.start);
	Joystick.button(9, gh.report.select);
	Joystick.hat(gh.report.du ? 0 : gh.report.dd ? 180 : -1);
	Joystick.X(gh.report.sx << 4);
	Joystick.Y(gh.report.sy << 4);
	Joystick.sliderRight(gh.report.rt << 5);
	Joystick.sliderLeft(gh.report.cy << 5);
	Joystick.send_now();
}*/

void setup() {
	Serial.begin(115200);
	while (!Serial);
	mkb.init();

}

void loop() {
	mkb.scan();
	mkb.print();

}
