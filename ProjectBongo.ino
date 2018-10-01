// Special thanks to KEMAN for working with me on the guitar controller!
#include "revolution.h"

/*//analog values
#define ANALOG_ERROR	0x00	//The controller disconnects if any analog sensor fails.
#define ANALOG_MIN		0x01
#define ANALOG_MAX		0xFF
#define ANALOG_MID		0x80	//127 is not the center due to the error condition.
#define TRIGGER_LOW		0x1F
#define TRIGGER_FLOOR	0x49	//Light-shield is active above this value.
#define TRIGGER_CEIL	0xFF	//TODO: need to find value just before the trigger BUTTON goes high
#define MIC_HIGH		0x6F
#define MIC_LOW			0x70
//TODO: double check these values and the adjusted ones
#define DEAD_LOW		0x68
#define DEAD_HIGH		0x98
#define STICK_LOW		0x4B
#define STICK_HIGH		0xB3
#define STICK_HIGHER	0xB5
#define STICK_MAX		0xE9
#define STICK_MIN		0x17
//adjusted analog values for N64 which is signed
/*#define ANALOG_ERROR_ADJ	-128
#define ANALOG_MIN_ADJ		-127
#define ANALOG_MAX_ADJ		127
#define ANALOG_MID_ADJ		0
#define DEAD_LOW_ADJ		-24
#define DEAD_HIGH_ADJ		24
#define STICK_LOW_ADJ		-53
#define STICK_HIGHISH_ADJ	51
#define STICK_HIGH_ADJ		53
#define STICK_MAX_ADJ		105
#define STICK_MIN_ADJ		-105*/

#define VOLUME	63
#define CHANNEL	1
#define DELAY	2000

#define SOFT_RESET()	(*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004

GuitarWii gh;

void setup() {
	//Serial.begin(115200);
	//while (!Serial);
	Joystick.useManualSend(true);
	gh.init();

}

void loop() {
	gh.poll();
	gh.sendJoyUSB();

}

/*void guitarMIDI() {
	if (gh.report.dd) {
		switch ((gh.report.buttons[1] & B11101000) >> 3)
		{
		case 0:	//A11/E, EADGBE
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			break;
		case B10000:	//D, XXDADF#
			usbMIDI.sendNoteOn(62, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			break;
		case B11000:	//F#m, F#C#F#AC#F#
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(73, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(85, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(90, VOLUME, CHANNEL);
			break;
		case B00100:	//G, GBDGBG
			usbMIDI.sendNoteOn(67, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(91, VOLUME, CHANNEL);
			break;
		case B00101:	//A, XAEAC#E
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(85, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			break;
		case B01000:	//Bm, XBF#BDF#
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(86, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(90, VOLUME, CHANNEL);
			break;
		case B10100:	//C, XCEGCE
			usbMIDI.sendNoteOn(60, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(67, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(72, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			break;
		case B00001:	//Am, XAEACE
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(84, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			break;
		case B01100:	//C#m, XDADFA
			usbMIDI.sendNoteOn(62, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(77, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			break;
		case B01101:	//E, EBEG#BE
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(80, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			break;
		case B01001:	//Em, EBEGBE
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			break;
		case B10001:	//Cm, XC#G#C#EG#
			usbMIDI.sendNoteOn(61, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(68, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(73, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(80, VOLUME, CHANNEL);
			break;
		default:
			break;
		}
		usbMIDI.send_now();
	}
	else if (gh.report.du) {
		switch ((gh.report.buttons[1] & B11101000) >> 3)
		{
		case 0:	//A11/E, EADGBE
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			break;
		case B10000:	//D, XXDADF#
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(62, VOLUME, CHANNEL);
			break;
		case B11000:	//F#m, F#C#F#AC#F#
			usbMIDI.sendNoteOn(90, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(85, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(73, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			break;
		case B00100:	//G, GBDGBG
			usbMIDI.sendNoteOn(91, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(67, VOLUME, CHANNEL);
			break;
		case B00101:	//A, XAEAC#E
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(85, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			break;
		case B01000:	//Bm, XBF#BDF#
			usbMIDI.sendNoteOn(90, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(86, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(78, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			break;
		case B10100:	//C, XCEGCE
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(72, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(67, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(60, VOLUME, CHANNEL);
			break;
		case B00001:	//Am, XAEACE
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(84, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			break;
		case B01100:	//C#m, XDADFA
			usbMIDI.sendNoteOn(81, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(77, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(74, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(69, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(62, VOLUME, CHANNEL);
			break;
		case B01101:	//E, EBEG#BE
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(80, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			break;
		case B01001:	//Em, EBEGBE
			usbMIDI.sendNoteOn(88, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(83, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(79, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(71, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(64, VOLUME, CHANNEL);
			break;
		case B10001:	//Cm, XC#G#C#EG#
			usbMIDI.sendNoteOn(80, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(76, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(73, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(68, VOLUME, CHANNEL);
			usbMIDI.send_now();
			delayMicros(DELAY);
			usbMIDI.sendNoteOn(61, VOLUME, CHANNEL);
			break;
		default:
			break;
		}
		usbMIDI.send_now();
	}
	else {
		for (int i = 60; i < 76; i++) {
			usbMIDI.sendNoteOn(i, 0, CHANNEL);
		}
		usbMIDI.send_now();
		for (int i = 76; i < 92; i++) {
			usbMIDI.sendNoteOn(i, 0, CHANNEL);
		}
		usbMIDI.send_now();
	}
	while (usbMIDI.read());
}*/
