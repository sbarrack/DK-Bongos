#pragma once

struct Inputs {
	// LSB
	uint8_t a : 1;	// br
	uint8_t b : 1;	// bl
	uint8_t x : 1;	// tr
	uint8_t y : 1;	// tl
	uint8_t start : 1;
	uint8_t getOrigin : 1;	// x + y + start
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
	uint8_t useOrigin : 1;	// always high

	uint8_t sx;
	uint8_t sy;
	uint8_t cx;	// key1
	uint8_t cy;	// key2
	uint8_t lt;	// key3
	uint8_t rt;	// mic
};

union Report {
	uint8_t raw[8];
	uint32_t raw32[2] = { 0x00808080, 0x80801F1F };
	Inputs ins;
};

union Origin {
	uint8_t raw[10] = { 0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F, 2, 2 };

	struct {
		Inputs ins;
		// unkown, maybe deadzones
		uint8_t dz0;
		uint8_t dz1;
	};
};

union Status {
	uint8_t raw[3] = { 9, 0, 3 };

	struct {
		uint16_t device;	// TODO: get flipped bytes unflipped?

		union {
			uint8_t state;

			struct {
				uint8_t : 3;
				uint8_t rumble : 1;	// ???
				uint8_t : 4;
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
		uint8_t : 6;
	};
};
