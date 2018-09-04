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

#include <NinTeensy.h>
#include "wii.h"

GuitarWii gh;
gcConsole gc(2);
gcReport gcr;
const double ang1 = atan2(114, -127);
const double ang2 = atan2(114, 127);
int wasPressed, inv, wasn;

inline gcReport jalhalla(gcReport r);
inline gcReport noTapJump(gcReport r);
inline gcReport tiltStick(gcReport r);

void setup()
{
	gh.init();
	gcr = gcDefault.report;
	
}

void loop()
{
	gh.poll();

	gc.write(gcr);
}

inline gcReport jalhalla(gcReport r) {
	inv = !wasPressed && r.dright ? !inv : inv;
	if (inv) {
		r.xAxis = ~r.xAxis;
		r.yAxis = ~r.yAxis;
	}
	wasPressed = r.dright;
	return r;
}

inline gcReport noTapJump(gcReport r) {
	int y = r.yAxis - 127;
	if (!(r.x || r.y) && y > 53) {
		int x = r.xAxis - 127;
		double angle = atan2(y, x);
		if (angle > ang2 && angle < ang1) {
			r.xAxis = 53 * x / y + 127;
			r.yAxis = 180;
		}
	}
	return r;
}

inline gcReport tiltStick(gcReport r) {
	int cr = r.cxAxis > 170;
	int cl = r.cxAxis < 84;
	int cd = r.cyAxis > 164;
	int cu = r.cyAxis < 90;
	int nn = cr || cl || cd || cu;
	r.a |= nn;
	if (nn && wasn) {
		if ((cr && cu) || (cr && cd) || (cl && cu) || (cl && cd)) {
			r.xAxis = 128;
			r.yAxis = 128;
		}
		else if (cr) {
			r.xAxis = 170;
			r.yAxis = 128;
		}
		else if (cl) {
			r.xAxis = 84;
			r.yAxis = 128;
		}
		else if (cd) {
			r.xAxis = 128;
			r.yAxis = 164;
		}
		else if (cu) {
			r.xAxis = 128;
			r.yAxis = 90;
		}
		wasn = false;
	}
	if (!nn) wasn = true;
	r.cxAxis = 128;
	r.cyAxis = 128;
	return r;
}
