#pragma once
#include <WS2812Serial.h>

#ifndef LEDS
#define LEDS	5
#endif // !LEDS

class LightString {
public:
	LightString(uint8_t pin = 1) : pin(pin),
		leds(WS2812Serial(LEDS, fb, db, pin, WS2812_GRB)) {}
	void init(), cycleOrange(), cycleBlue(), cycleYellow(),
		cycleRed(), cycleGreen(), cycleAll(), idle();
	uint32_t period = 25;
private:
	/*DMAMEM */uint8_t fb[LEDS * 12];
	uint8_t db[LEDS * 3];
	uint8_t pin;
	WS2812Serial leds;

	uint32_t orange = 0xFF8000, blue = 0x0000FF, yellow = 0xFFFF00,
		red = 0xFF0000, green = 0x00FF00;
	int dRedMode, dGreenMode, dBlueMode, dYellowMode, dOrangeMode;
};
