#include "leds.h"

#define RED_INC		0x010000
#define GREEN_INC	0x000100
#define ORANGE_1	0xFFA000
#define ORANGE_2	0xFF6000
#define BLUE_0	0x0000FF
#define BLUE_1	0x0020FF
#define BLUE_2	0x2020FF
#define BLUE_3	0x2000FF
#define YELLOW_0	0xFFFF00
#define YELLOW_1	0xFFDF00
#define YELLOW_2	0xDFDF00
#define YELLOW_3	0xDFFF00
#define RED_0	0xFF0000
#define RED_1	0xFF0020
#define RED_2	0xFF2020
#define RED_3	0xFF2000
#define GREEN_0	0x00FF00
#define GREEN_1	0x00FF20
#define GREEN_2	0x20FF20
#define GREEN_3	0x20FF00

void LightString::init() {
	leds.begin();
	leds.setPixel(0, orange);
	leds.setPixel(1, blue);
	leds.setPixel(2, yellow);
	leds.setPixel(3, red);
	leds.setPixel(4, green);
	leds.show();
}

void LightString::cycleAll() {
	cycleGreen();
	cycleRed();
	cycleYellow();
	cycleBlue();
	cycleOrange();
}

void LightString::cycleOrange() {
	orange = dOrangeMode ? orange - GREEN_INC : orange + GREEN_INC;
	dOrangeMode = orange == ORANGE_1 ? 1 : orange == ORANGE_2 ? 0 : dOrangeMode;
	leds.setPixel(0, orange);
}

void LightString::cycleYellow() {
	switch (dYellowMode) {
	case 0:
		yellow -= GREEN_INC;
		break;
	case 1:
		yellow -= RED_INC;
		break;
	case 2:
		yellow += GREEN_INC;
		break;
	case 3:
		yellow += RED_INC;
	}
	switch (yellow)
	{
	case YELLOW_1:
		dYellowMode = 1;
		break;
	case YELLOW_2:
		dYellowMode = 2;
		break;
	case YELLOW_3:
		dYellowMode = 3;
		break;
	case YELLOW_0:
		dYellowMode = 0;
	}
	leds.setPixel(2, yellow);
}

void LightString::cycleBlue() {
	switch (dBlueMode) {
	case 0:
		blue += GREEN_INC;
		break;
	case 1:
		blue += RED_INC;
		break;
	case 2:
		blue -= GREEN_INC;
		break;
	case 3:
		blue -= RED_INC;
	}
	switch (blue)
	{
	case BLUE_1:
		dBlueMode = 1;
		break;
	case BLUE_2:
		dBlueMode = 2;
		break;
	case BLUE_3:
		dBlueMode = 3;
		break;
	case BLUE_0:
		dBlueMode = 0;
	}
	leds.setPixel(1, blue);
}

void LightString::cycleRed() {
	switch (dRedMode) {
	case 0:
		red++;
		break;
	case 1:
		red += GREEN_INC;
		break;
	case 2:
		red--;
		break;
	case 3:
		red -= GREEN_INC;
	}
	switch (red)
	{
	case RED_1:
		dRedMode = 1;
		break;
	case RED_2:
		dRedMode = 2;
		break;
	case RED_3:
		dRedMode = 3;
		break;
	case RED_0:
		dRedMode = 0;
	}
	leds.setPixel(3, red);
}

void LightString::cycleGreen() {
	switch (dGreenMode) {
	case 0:
		green++;
		break;
	case 1:
		green += RED_INC;
		break;
	case 2:
		green--;
		break;
	case 3:
		green -= RED_INC;
	}
	switch (green)
	{
	case GREEN_1:
		dGreenMode = 1;
		break;
	case GREEN_2:
		dGreenMode = 2;
		break;
	case GREEN_3:
		dGreenMode = 3;
		break;
	case GREEN_0:
		dGreenMode = 0;
	}
	leds.setPixel(4, green);
}
