/*	This is ProjectBongo; a program dedicated to the functionality of
	Donkey Kong Bongos and other game controllers.

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

//TODO:
//On bongos, remap the special attacks, snapback/turnarounds. 
//Make a logic analyzer out of Teensy to figure out how Dreamcast Maracas work.

#include "wii.h"

/*#define buffr 6
#define buffSize 200*/

GuitarWii gh;

/*inline GCReport bongoTest(GCReport r);
inline GCReport bongofy(GCReport r);
inline GCReport jalhalla(GCReport r);
inline GCReport noTapJump(GCReport r);
inline GCReport tiltStick(GCReport r);

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

GCReport gcr;
bongoReport bongo;
bongoReport buff[buffSize];
int cBuff[buffSize];
int c, x, y;
double angle, ang1 = atan2(114, -127), ang2 = atan2(114, 127);
bool lightShield, shieldOn, nothing, wasPressed, inv, cr, cl, cd, cu, nn, wasn;
uint8_t xLast, yLast;

static constexpr bongoReport bongoDefRep = { 0, TRIGGER_LOW };*/

void setup()
{
	//Serial.begin(115200);
	//while (!Serial);

	pinMode(13, OUTPUT);
	digitalWriteFast(13, HIGH);
	
	/*pinMode(13, OUTPUT);
	digitalWriteFast(13, HIGH);
	Serial.println("ProjectBongo | Copyright (C) 2018 | Stephen Barrack");
	Serial.println("This program comes with ABSOLUTELY NO WARRANTY.");
	Serial.println("This is free software, and you are welcome to redistribute it under certain conditions.");
	Serial.println("View README and LICENSE included with this project for full details.");
	delay(1000);
	digitalWriteFast(13, LOW);*/
	
	/*bongo = bongoDefRep;
	wasn = true;
	lightShield = shieldOn = wasPressed = inv = cr = cl = cd = cu = nn = false;
	xLast = yLast = 0;*/

	gh.init();
	/*Serial.print("ID = 0x");
	for (int i = 0; i < 6; i++) {
		if (gh.id[i] < 0x10)
			Serial.print("0");
		Serial.print(gh.id[i], HEX);
	}
	//Serial.println("\n\nData:");*/
}

void loop()
{
	gh.poll();

	if (gh.report.dd || gh.report.du) {
		switch ((gh.report.buttons[1] & B11101000) >> 3) 
		{
		case 0:	//A11/E, EADGBE
			//Serial.println("open");
			usbMIDI.sendNoteOn(64, 127, 1);
			usbMIDI.sendNoteOn(69, 127, 1);
			usbMIDI.sendNoteOn(74, 127, 1);
			usbMIDI.sendNoteOn(79, 127, 1);
			usbMIDI.sendNoteOn(83, 127, 1);
			usbMIDI.sendNoteOn(88, 127, 1);
			break;
		case B10000:	//D, XXDADF#
			//Serial.println("D");
			break;
		case B11000:	//F#m, F#C#F#AC#F#
			//Serial.println("F#m");
			break;
		case B00100:	//G, GBDGBG
			//Serial.println("G");
			break;
		case B00101:	//A, XAEAC#E
			//Serial.println("A");
			break;
		case B01000:	//Bm, XBF#BDF#
			//Serial.println("Bm");
			break;
		case B10100:	//C, XCEGCE
			//Serial.println("C");
			break;
		case B00001:	//Am, XAEACE
			//Serial.println("Am");
			break;
		case B01100:	//C#m, XDADFA
			//Serial.println("C#m");
			break;
		case B01101:	//E, EBEG#BE
			//Serial.println("E");
			break;
		case B01001:	//Em, EBEGBE
			//Serial.println("Em");
			break;
		case B10001:	//Cm, XDbAbDbEAb
			//Serial.println("Cm");
			break;
		default:
			break;
		}

		usbMIDI.send_now();
	}
	else {
		//decay all notes a bit here
		usbMIDI.sendNoteOff(64, 127, 1);
		usbMIDI.sendNoteOff(69, 127, 1);
		usbMIDI.sendNoteOff(74, 127, 1);
		usbMIDI.sendNoteOff(79, 127, 1);
		usbMIDI.sendNoteOff(83, 127, 1);
		usbMIDI.sendNoteOff(88, 127, 1);

		usbMIDI.send_now();
	}

	while (usbMIDI.read());

	/*Serial.print(gh.report.sx);
	Serial.print(",");
	Serial.print(gh.report.sy);
	Serial.print(" ");
	Serial.print(gh.report.rt - 0xE, HEX);
	Serial.print(gh.report.a ? " #" : " _");
	Serial.print(gh.report.b ? "#" : "_");
	Serial.print(gh.report.y ? "#" : "_");
	Serial.print(gh.report.z ? "#" : "_");
	Serial.print(gh.report.x ? "# " : "_ ");
	Serial.print(gh.report.cy > 0x03 && gh.report.cy < 0x08 ? "#" : "_");
	Serial.print(gh.report.cy > 0x06 && gh.report.cy < 0x0E && gh.report.cy != 0x0F ? "#" : "_");
	Serial.print(gh.report.cy > 0x0B && gh.report.cy < 0x16 && gh.report.cy != 0x0F ? "#" : "_");
	Serial.print(gh.report.cy > 0x13 && gh.report.cy < 0x1B ? "#" : "_");
	Serial.print(gh.report.cy > 0x19 && gh.report.cy < 0x21 ? "# " : "_ ");
	Serial.print(gh.report.select ? "-" : "_");
	Serial.print(gh.report.start ? "+ " : "_ ");
	Serial.println(gh.report.dd ? "v" : gh.report.du ? "^" : "-");
	delay(17);*/
}

/*inline GCReport bongoTest(GCReport r) {
	r = defaultGCReport;
	r.a = buff[0].tl;
	r.b = buff[0].tr;
	r.x = buff[0].bl;
	r.y = buff[0].br;
	r.start = buff[0].start;
	r.right = buff[0].mic;
	return r;
}

inline GCReport bongofy(GCReport r) {
	bongo.tl = r.a;
	bongo.tr = r.b;
	bongo.bl = r.x;
	bongo.br = r.y;
	bongo.start = r.start;
	bongo.mic = r.right;
	r = defaultGCReport;
	for (int i = buffSize - 1; i > 0; i--)
		buff[i] = buff[i - 1];
	buff[0] = bongo;

	//start
	r.start = bongo.start;

	//taunt
	nothing = true;
	for (int i = 0; i < buffSize; i++) {
		if (buff[i].tl || buff[i].tr || buff[i].bl || buff[i].br || buff[i].start || buff[i].mic > MIC_HIGH) {
			nothing = false;
			break;
		}
	}
	r.du = nothing;

	//light lightShield, sheild
	c = bongo.tl + bongo.tr + bongo.bl + bongo.br;
	if (c >= 4 && !shieldOn) {
		shieldOn = true;
		lightShield = bongo.mic > STICK_LOW;
		r.right = shieldOn ? TRIGGER_FLOOR : TRIGGER_LOW;
		r.r = !lightShield;
	}
	else if (c > 1) {
		r.right = shieldOn ? MIC_HIGH : TRIGGER_LOW;
		r.r = shieldOn && !lightShield;
	}
	else {
		lightShield = shieldOn = false;
	}

	//A
	r.a = buff[buffr].mic > MIC_HIGH;

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
				r.cyAxis = STICK_MIN;
				break;
			case 5://right
				r.xAxis = STICK_HIGH;
				break;
			case 6://roll right
				r.xAxis = STICK_MAX;
				break;
			case 7://down right
				r.xAxis = STICK_HIGH;
				r.yAxis = STICK_LOW;
				break;
			case 9://roll left
				r.xAxis = STICK_MIN;
				break;
			case 10://left
				r.xAxis = STICK_LOW;
				break;
			case 11://down left
				r.xAxis = STICK_LOW;
				r.yAxis = STICK_LOW;
				break;
			case 12://jump
				r.cyAxis = STICK_MAX;
				break;
			case 13://up right
				r.xAxis = STICK_HIGH;
				r.yAxis = STICK_HIGHER;
				break;
			case 14://up left
				r.xAxis = STICK_LOW;
				r.yAxis = STICK_HIGHER;
				break;
			default:
				break;
			}
		}
		else {//normal movement (may need a buffer, also make moonwalking easier later)
			switch (cBuff[0]) {
			case 1://dash right
				r.xAxis = STICK_MAX;
				break;
			case 2://dash left
				r.xAxis = STICK_MIN;
				break;
			case 3://crouch
				r.yAxis = STICK_MIN;
				break;
			case 4://up tilt
				r.yAxis = STICK_HIGHER;
				break;
			case 5://tilt right
				r.xAxis = DEAD_HIGH;
				break;
			case 7://down right
				r.xAxis = STICK_HIGH;
				r.yAxis = STICK_LOW;
				break;
			case 8://tap jump
				if (r.a)
					r.y = 1;
				else
					r.yAxis = STICK_MAX;
				break;
			case 10://tilt left
				r.xAxis = DEAD_LOW;
				break;
			case 11://down left
				r.xAxis = STICK_LOW;
				r.yAxis = STICK_LOW;
				break;
			case 12://up b
				r.b = 1;
				r.yAxis = STICK_MAX;
				break;
			case 13://up right
				r.xAxis = STICK_HIGH;
				r.yAxis = STICK_HIGHER;
				break;
			case 14://up left
				r.xAxis = STICK_LOW;
				r.yAxis = STICK_HIGHER;
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
			if (!(bongo.mic > MIC_HIGH))
				r.xAxis = STICK_MAX;
		}
		if (cBuff[0] == 9) {//side b left, down b tap
			r.b = 1;
			if (bongo.mic > MIC_HIGH)
				r.yAxis = STICK_MIN;
			else
				r.xAxis = STICK_MIN;
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

inline GCReport jalhalla(GCReport r) {
	inv = !wasPressed && r.dr ? !inv : inv;
	if (inv) {
		r.xAxis = ~r.xAxis;
		r.yAxis = ~r.yAxis;
	}
	wasPressed = r.dr;
	return r;
}

inline GCReport noTapJump(GCReport r) {
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

inline GCReport tiltStick(GCReport r) {
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
}*/
