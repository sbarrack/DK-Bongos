#pragma once
#include <Arduino.h>

union GCReport {
	uint8_t raw[8];
	//uint16_t raw16[4];
	//uint32_t raw32[2];
	uint64_t raw64 = 0x0080808080801F1F;
	struct {
		// LSB
		uint8_t a : 1; // br
		uint8_t b : 1; // bl
		uint8_t x : 1; // tr
		uint8_t y : 1; // tl
		uint8_t start : 1;
		uint8_t getOrigin : 1; // x + y + start
		uint8_t errorLatch : 1;
		uint8_t errorStatus : 1;
		// MSB

		uint8_t dl : 1;
		uint8_t dr : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t z : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t useOrigin : 1; // always high

		uint8_t sx;
		uint8_t sy;
		uint8_t cx; // key1
		uint8_t cy; // key2
		uint8_t lt; // key3
		uint8_t rt; // mic
	};
};

union GCOrigin {
	uint8_t raw[10];
	//uint16_t raw16[5];
	struct {
		GCReport rep;
		union {
			uint8_t raw[2];
			uint16_t raw16 = 0x0202;
		} dz; // unkown, maybe deadzones
	} data;
};

union Status {
	uint8_t raw[3] = { 9, 0, 3 };
	struct {
		uint16_t device; // TODO get flipped bytes unflipped?
		union {
			uint8_t state;
			struct {
				uint8_t : 3;
				uint8_t rumble : 1;	// ???
				//uint8_t : 4;
			};
		};
	};
};

union Request {
	uint8_t raw[3] = { 0 };
	struct {
		uint8_t command;

		uint8_t mode : 2;
		uint8_t : 6;

		uint8_t rumble : 2;
		//uint8_t : 6;
	};
};

union N64Report {
	uint8_t raw[4];
	//uint16_t raw16[2];
	//uint32_t raw32;
	struct {
		uint8_t dr : 1;
		uint8_t dl : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t start : 1;
		uint8_t z : 1;
		uint8_t b : 1;
		uint8_t a : 1;

		uint8_t cr : 1;
		uint8_t cl : 1;
		uint8_t cd : 1;
		uint8_t cu : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t reset : 1;
		//uint8_t : 1;

		int8_t sx;
		int8_t sy;
	};
};

// bongo code
/*#define buffr 6
#define buffSize 200
//bongo = bongoDefRep;
bool lightShield, shieldOn, cr, cl, cd, cu, nn;
uint8_t xLast, yLast;
void bongofy() {
	//bongo.tl = r.a;
	//bongo.tr = r.b;
	//bongo.bl = r.x;
	//bongo.br = r.y;
	//bongo.start = r.start;
	//bongo.mic = r.right;
	//r = defaultGCReport;
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
}*/
