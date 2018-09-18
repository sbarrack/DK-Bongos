#pragma once
#include <WS2812Serial.h>

#ifndef NUM_LEDS
#define NUM_LEDS	5
#endif // !NUM_LEDS

class LightString {
public:
	LightString(uint8_t pin = 1) : pin(pin),
		leds(WS2812Serial(NUM_LEDS, fb, db, pin, WS2812_GRB)) {}
	void init();
	void cycleOrange();
	void cycleBlue();
	void cycleYellow();
	void cycleRed();
	void cycleGreen();
	void cycleAll();
	void idle();
	uint32_t period = 25;
private:
	uint8_t fb[NUM_LEDS * 12];
	uint8_t db[NUM_LEDS * 3];
	uint8_t pin;
	WS2812Serial leds;
	uint32_t orange = 0xFF8000;
	uint32_t blue = 0x0000FF;
	uint32_t yellow = 0xFFFF00;
	uint32_t red = 0xFF0000;
	uint32_t green = 0x00FF00;
	int dRedMode, dGreenMode, dBlueMode, dYellowMode, dOrangeMode;
};
