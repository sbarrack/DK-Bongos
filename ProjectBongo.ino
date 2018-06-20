/*	This is ProjectBongo; a program dedicated to the functionality of Donkey Kong Bongos and beyond.
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

/*	Special thanks to the Custom GameCube Controller modding cummunity.
	I couldn't have done this without your help and encouragement!
		<3 Steve
	*/

/*TODO: On bongos remap the special attacks, snapback/turnarounds. 
	Put in Wiimote stuff. Test out keyboard. Make a logic analyzer 
	out of Teensy to figure out how Maracas work. Get N64 controllers 
	working too.
	*/
#include <Arduino.h>
#include "gc.h"

#define buffr 6
#define buffSize 200

//gcConsole gc(3);
gcController bongos(2);

inline gcReport bongoTest(gcReport r);
inline gcReport bongofy(gcReport r);
inline gcReport jalhalla(gcReport r);
inline gcReport noTapJump(gcReport r);
inline gcReport tiltStick(gcReport r);

union bongoReport {
	uint8_t raw[2];

	struct {
		uint8_t tl : 1;
		uint8_t tr : 1;
		uint8_t bl : 1;
		uint8_t br : 1;
		uint8_t start : 1;
		uint8_t empty : 3;

		uint8_t mic;
	};
};

gcReport gcr;
bongoReport bongo;
bongoReport buff[buffSize];
int cBuff[buffSize];
int c, x, y;
double angle, ang1 = atan2(114, -127), ang2 = atan2(114, 127);
bool lightShield, shieldOn, nothing, wasPressed, inv, cr, cl, cd, cu, nn, wasn;
uint8_t xLast, yLast;

static constexpr bongoReport bongoDefRep = { 0, tLow };

void setup()
{
	Serial.begin(9600);
	delay(5000);
	Serial.println("ProjectBongo  Copyright (C) 2018  Stephen Barrack");
	Serial.println("This program comes with ABSOLUTELY NO WARRANTY.");
	Serial.println("This is free software, and you are welcome to redistribute it under certain conditions.");
	Serial.println("View README and LICENSE included with this project for full details.");

	pinMode(13, OUTPUT);
	for (int i = 5; i > 1; i--) {
		digitalWrite(13, HIGH);
		delay(25 << i);
		digitalWrite(13, LOW);
		delay(100);
	}
 
	bongo = bongoDefRep;
	wasn = true;
	lightShield = shieldOn = wasPressed = inv = cr = cl = cd = cu = nn = false;
	xLast = yLast = 0;
}

void loop()
{  
	gcr = gcDefault.report;
	bongos.read();
	gcr = bongos.getReport();

	gcr = bongoTest(gcr);
	Serial.println(gcr.a);

	//gc.write(gcr);
}

inline gcReport bongoTest(gcReport r) {
	r = gcDefault.report;
	r.a = buff[0].tl;
	r.b = buff[0].tr;
	r.x = buff[0].bl;
	r.y = buff[0].br;
	r.start = buff[0].start;
	r.right = buff[0].mic;

	return r;
}

inline gcReport bongofy(gcReport r) {
	bongo.tl = r.a;
	bongo.tr = r.b;
	bongo.bl = r.x;
	bongo.br = r.y;
	bongo.start = r.start;
	bongo.mic = r.right;
	r = gcDefault.report;
	for (int i = buffSize - 1; i > 0; i--)
		buff[i] = buff[i - 1];
	buff[0] = bongo;

	//start
	r.start = bongo.start;

	//taunt
	nothing = true;
	for (int i = 0; i < buffSize; i++) {
		if (buff[i].tl || buff[i].tr || buff[i].bl || buff[i].br || buff[i].start || buff[i].mic > micHigh) {
			nothing = false;
			break;
		}
	}
	r.dup = nothing;

	//light lightShield, sheild
	c = bongo.tl + bongo.tr + bongo.bl + bongo.br;
	if (c >= 4 && !shieldOn) {
		shieldOn = true;
		lightShield = bongo.mic > sLowNotch;
		r.right = shieldOn * tHigh;
		r.r = !lightShield;
	}
	else if (c > 1) {
		r.right = shieldOn * tHigh;
		r.r = shieldOn && !lightShield;
	}
	else {
		lightShield = shieldOn = false;
	}

	//A
	r.a = buff[buffr].mic > micHigh;

	//stick
	c = bongo.br | bongo.bl << 1 | bongo.tr << 2 | bongo.tl << 3;
	for (int i = buffSize - 1; i > 0; i--)
		cBuff[i] = cBuff[i - 1];
	cBuff[0] = c;
	c = 0;
	for (int i = 0; i <= buffr; i++) {
		if (cBuff[0] == cBuff[i])
			c++;
	}
	if (c >= buffr) {
		if (lightShield || shieldOn) {//lightShield movement
			switch (cBuff[0]) {
			case 3://spot dodge
				r.cyAxis = sMin;
				break;
			case 5://right
				r.xAxis = sHighNotch;
				break;
			case 6://roll right
				r.xAxis = sMax;
				break;
			case 7://down right
				r.xAxis = sHighNotch;
				r.yAxis = sLowNotch;
				break;
			case 9://roll left
				r.xAxis = sMin;
				break;
			case 10://left
				r.xAxis = sLowNotch;
				break;
			case 11://down left
				r.xAxis = sLowNotch;
				r.yAxis = sLowNotch;
				break;
			case 12://jump
				r.cyAxis = sMax;
				break;
			case 13://up right
				r.xAxis = sHighNotch;
				r.yAxis = syUpperNotch;
				break;
			case 14://up left
				r.xAxis = sLowNotch;
				r.yAxis = syUpperNotch;
				break;
			default:
				break;
			}
		}
		else {//normal movement (may need a buffer, also make moonwalking easier later)
			switch (cBuff[0]) {
			case 1://dash right
				r.xAxis = sMax;
				break;
			case 2://dash left
				r.xAxis = sMin;
				break;
			case 3://crouch
				r.yAxis = sMin;
				break;
			case 4://up tilt
				r.yAxis = syUpperNotch;
				break;
			case 5://tilt right
				r.xAxis = sDeadRight;
				break;
			case 7://down right
				r.xAxis = sHighNotch;
				r.yAxis = sLowNotch;
				break;
			case 8://tap jump
				if (r.a)
					r.y = 1;
				else
					r.yAxis = sMax;
				break;
			case 10://tilt left
				r.xAxis = sDeadLeft;
				break;
			case 11://down left
				r.xAxis = sLowNotch;
				r.yAxis = sLowNotch;
				break;
			case 12://up b
				r.b = 1;
				r.yAxis = sMax;
				break;
			case 13://up right
				r.xAxis = sHighNotch;
				r.yAxis = syUpperNotch;
				break;
			case 14://up left
				r.xAxis = sLowNotch;
				r.yAxis = syUpperNotch;
				break;
			default:
				break;
			}
		}
		xLast = r.xAxis;
		yLast = r.yAxis;
	}
	else {
		r.xAxis = xLast;
		r.yAxis = yLast;
		if (cBuff[0] == 6) {//side b right, neutral b tap
			r.b = 1;
			if (!(bongo.mic > micHigh))
				r.xAxis = sMax;
		}
		if (cBuff[0] == 9) {//side b left, down b tap
			r.b = 1;
			if (bongo.mic > micHigh)
				r.yAxis = sMin;
			else
				r.xAxis = sMin;
		}
	}

	//quit
	c = 0;
	for (int i = 0; i < buffSize; i++) {
		if (buff[i].start)
			c++;
	}
	c = c >= buffSize;
	r.l = r.l || c;
	r.r = r.r || c;
	r.a = r.a || c;

	return r;
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
	y = r.yAxis - 127;
	if (!(r.x || r.y) && y > 53) {
		x = r.xAxis - 127;
		angle = atan2(y, x);
		if (angle > ang2 && angle < ang1) {
			r.xAxis = 53 * x / y + 127;
			r.yAxis = 180;
		}
	}

	return r;
}

inline gcReport tiltStick(gcReport r) {
	cr = r.cxAxis > 170;
	cl = r.cxAxis < 84;
	cd = r.cyAxis > 164;
	cu = r.cyAxis < 90;
	nn = cr || cl || cd || cu;
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
