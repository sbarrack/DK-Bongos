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

#include "revolution.h"
#include "leds.h"

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

#define SOFT_RESET()	(*(volatile uint32_t*)0xE000ED0C) = 0x05FA0004

GuitarWii gh;
LightString leds;

/*const double ang1 = atan2(114, -127);
const double ang2 = atan2(114, 127);
int wasPressed, inv, wasn;*/

inline void guitarUSBupdate();
/*inline gcReport jalhalla(gcReport r);
inline gcReport noTapJump(gcReport r);
inline gcReport tiltStick(gcReport r);*/

void setup()
{
	gh.init();
	Joystick.useManualSend(true);
	leds.init();

}

void loop()
{
	gh.poll();
	guitarUSBupdate();
	leds.idle();

}

inline void guitarUSBupdate() {
	Joystick.button(3, gh.report.a);
	Joystick.button(2, gh.report.b);
	Joystick.button(4, gh.report.y);
	Joystick.button(1, gh.report.x);
	Joystick.button(6, gh.report.z);
	Joystick.button(10, gh.report.start);
	Joystick.button(9, gh.report.select);
	Joystick.X(gh.report.sx << 4);
	Joystick.Y(gh.report.sy << 4);
	Joystick.sliderRight(gh.report.rt << 5);
	Joystick.sliderLeft(gh.report.cy << 5);
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
