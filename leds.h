#pragma once
#include <WS2812Serial.h>

#ifndef LEDS
#define LEDS	5
#endif // !LEDS

class LightString {
public:
	LightString(uint8_t pin = 1) : pin(pin),
		leds(WS2812Serial(LEDS, fb, db, pin, WS2812_GRB)) {}
	WS2812Serial leds;
	void init(), cycleOrange(), cycleBlue(), cycleYellow(),
		cycleRed(), cycleGreen(), cycleAll();
	uint32_t orange = 0xFF8000, blue = 0x0000FF, yellow = 0xFFFF00,
		red = 0xFF0000, green = 0x00FF00;
private:
	uint8_t fb[LEDS * 12];
	uint8_t db[LEDS * 3];
	uint8_t pin;
	int dRedMode, dGreenMode, dBlueMode, dYellowMode, dOrangeMode;
};
