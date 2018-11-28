#include <Arduino.h>
#include "types.h"
#include "macrokeyboard.h"

MKB mkb;

void setup() {
	Serial.begin(115200);
	Serial1.begin(115200);

	Mouse.screenSize(1920, 1080);
	mkb.init();

	Serial.println("Ready");
}

void loop() {
	mkb.scan();
	/*while (Serial1.available() > 0) {
		Serial.write(Serial1.read());
	}
	while (Serial.available() > 0) {
		Serial1.write(Serial.read());
	}*/

	mkb.print();
	// open C:/User/Username/AppData/Roaming using win+r
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
	// mouse pointer to center screen
	else if (mkb.down.key2) {
		Mouse.moveTo(1920 >> 1, 1080 >> 1);
	}
	// comment a line of code out
	else if (mkb.down.key3) {
		Keyboard.set_key1(KEY_HOME);
		Keyboard.send_now();
		Keyboard.set_key1(0);
		Keyboard.send_now();
		Keyboard.print("// ");
	}
	// calculator
	else if (mkb.down.key4) {
		Keyboard.set_media(KEY_MEDIA_CALC);
		Keyboard.send_now();
		Keyboard.set_media(0);
		Keyboard.send_now();
	}
	// Happy Birthday! over bluetooth
	else if (mkb.down.key5) {
		Serial1.print("Happy Birthday!");
	}
	
	delay(7);
}
