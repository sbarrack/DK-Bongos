/*	This is the Guitar branch of ProjectBongo; a program dedicated to the
	functionality of the Guitar Hero/Rock Band Guitars for Wii.

	Copyright (C) 2018  Stephen Barrack

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	For any issues, please contact stephen.barrack12@yahoo.com.
*/

/*	Special thanks to KEMAN for working with me on making
	one sick controller! <3
*/

//#include "ultradolphin.h"
#include "revolution.h"

#define SERIAL_9N1 0x84
#define SERIAL_9E1 0x8E
#define SERIAL_9O1 0x8F
#define SERIAL_9N1_RXINV 0x94
#define SERIAL_9E1_RXINV 0x9E
#define SERIAL_9O1_RXINV 0x9F
#define SERIAL_9N1_TXINV 0xA4
#define SERIAL_9E1_TXINV 0xAE
#define SERIAL_9O1_TXINV 0xAF
#define SERIAL_9N1_RXINV_TXINV 0xB4
#define SERIAL_9E1_RXINV_TXINV 0xBE
#define SERIAL_9O1_RXINV_TXINV 0xBF

/*//analog values
#define ANALOG_ERROR	0x00	//The controller disconnects if any analog sensor fails.
#define ANALOG_MIN		0x01
#define ANALOG_MAX		0xFF
#define ANALOG_MID		0x80	//127 is not the center due to the error condition.
#define TRIGGER_LOW		0x1F
#define TRIGGER_FLOOR	0x49	//Light-shield is active above this value.
#define TRIGGER_CEIL	0xFF	//TODO: need to find value just before the trigger button goes high
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

#define button(num, state)	Joystick.button(num, state)
#define stickC(x, y)	Joystick.Zrotate(x); Joystick.Z(y)

#define buttonX	1
#define buttonB	2
#define buttonA	3
#define buttonY	4
//#define buttonZL	5
#define buttonZ	6
#define buttonL	7
#define buttonR	8
//#define SELECT	9
#define START	10
#define USB_MIN 0
#define USB_MID	0x200
#define USB_MAX	0x3FF

GuitarWii gh;
/*Console gc;
gcData gcc;*/
const double ang1 = atan2(114, -127);
const double ang2 = atan2(114, 127);
int wasPressed, inv, wasn;

uint8_t test;

inline void guitarUSBupdate();
/*inline gcReport jalhalla(gcReport r);
inline gcReport noTapJump(gcReport r);
inline gcReport tiltStick(gcReport r);*/

void setup()
{
	//gh.init();
	//Joystick.useManualSend(true);
	/*gcc = gcDefault;
	//need to know what order the console sends commands
	//may be just init and simple poll or init, origin, poll with rumble
	gc.init(gcc);	//TODO: have different prototypes*/
	Serial.begin(115200);
	Serial1.begin(1000000, SERIAL_8N1);
}

void loop()
{
	//gh.poll();

	//guitarUSBupdate();
	//TODO: write to report, after testing with gcDefault for connectivity

	//gc.update(gcc);	//TODO: have different prototypes
	if (Serial1.available()) {
		test = Serial1.read();
		if (test < 0x10) {
			Serial.print("0");
		}
		Serial.print(test, HEX);
	}
}

inline void guitarUSBupdate() {
	button(buttonZ, gh.report.x);
	button(buttonA, gh.report.z);
	button(buttonX, gh.report.y);
	button(buttonB, gh.report.b);
	button(buttonR, gh.report.a);
	button(START, gh.report.start);
	Joystick.hat(gh.report.select ? 0 : -1);
	Joystick.sliderRight(gh.report.rt << 5);
	Joystick.X(gh.report.sx << 4);
	Joystick.Y(gh.report.sy << 4);
	switch (gh.report.cy) {
	case 0x4:
		stickC(USB_MID, USB_MAX);
		break;
	case 0x7:
		stickC(USB_MAX, USB_MAX);
		break;
	case 0xA:
		stickC(USB_MAX, USB_MID);
		break;
	case 0xC:
	case 0xD:
		stickC(USB_MAX, USB_MIN);
		break;
	case 0x12:
	case 0x13:
		stickC(USB_MID, USB_MIN);
		break;
	case 0x14:
	case 0x15:
		stickC(USB_MIN, USB_MIN);
		break;
	case 0x17:
	case 0x18:
		stickC(USB_MIN, USB_MID);
		break;
	case 0x1A:
		stickC(USB_MIN, USB_MAX);
		break;
	//case 0x1F:
	//case 0xF:
	default:
		stickC(USB_MID, USB_MID);
		break;
	}
	Joystick.send_now();
}

/*inline gcReport jalhalla(gcReport r) {
	inv = !wasPressed && r.dr ? !inv : inv;
	if (inv) {
		r.sx = ~r.sx;
		r.sy = ~r.sy;
	}
	wasPressed = r.dr;
	return r;
}

inline gcReport noTapJump(gcReport r) {
	int y = r.sy - 127;
	if (!(r.x || r.y) && y > 53) {
		int x = r.sx - 127;
		double angle = atan2(y, x);
		if (angle > ang2 && angle < ang1) {
			r.sx = 53 * x / y + 127;
			r.sy = 180;
		}
	}
	return r;
}

inline gcReport tiltStick(gcReport r) {
	int cr = r.cx > 170;
	int cl = r.cx < 84;
	int cd = r.cy > 164;
	int cu = r.cy < 90;
	int nn = cr || cl || cd || cu;
	r.a |= nn;
	if (nn && wasn) {
		if ((cr && cu) || (cr && cd) || (cl && cu) || (cl && cd)) {
			r.sx = 128;
			r.sy = 128;
		}
		else if (cr) {
			r.sx = 170;
			r.sy = 128;
		}
		else if (cl) {
			r.sx = 84;
			r.sy = 128;
		}
		else if (cd) {
			r.sx = 128;
			r.sy = 164;
		}
		else if (cu) {
			r.sx = 128;
			r.sy = 90;
		}
		wasn = false;
	}
	if (!nn) wasn = true;
	r.cx = 128;
	r.cy = 128;
	return r;
}*/
