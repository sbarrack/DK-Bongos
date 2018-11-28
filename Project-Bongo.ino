#include <Arduino.h>
#include "types.h"
#include "macrokeyboard.h"

MKB mkb;

void setup() {
	Serial.begin(115200);

	Mouse.screenSize(1920, 1080);
	mkb.init();

	Serial.println("Ready");
}

void loop() {
	mkb.scan();

	//mkb.print();
	if (mkb.down.key1) {
		Keyboard.set_modifier(MODIFIERKEY_GUI);
		Keyboard.send_now();
		Keyboard.set_key1(KEY_R);
		Keyboard.send_now();
		Keyboard.set_modifier(0);
		Keyboard.set_key1(0);
		Keyboard.send_now();
		delay(100);
		Keyboard.println("%APPDATA%");
	}
	else if (mkb.down.key2) {
		Mouse.moveTo(1920 >> 1, 1080 >> 1);
	}

	delay(10);
}
