//1-wire device ids
#define GCC				0x0900//03
#define BONGOS			0x0900//00?
#define N64C			0x0500//02
//TODO
/*#define GC_KEYBOARD		0x0820
#define GBA				0x0004
#define STEERING_WHEEL	0x0800
//#define CHAINSAW		*/

//commands, positioned at front of word to be shifted out, 
//	the MSB in each byte is a flag to stop (stop bit, 0x80)
#define RESET			0xFF800000
#define INITIALIZE		0x00800000
#define POLL_N64		0x01800000
#define READ_N64		0x02800000
#define WRITE_N64		0x03800000
#define READ_EEPROM		0x04800000
#define WRITE_EEPROM	0x05800000
/*#define INIT_EREADER	0x06800000
#define READ_EREADER	0x07800000
#define WRITE_EREADER	0x08800000
//VRU 0x09-D
#define SCAN_N64		0x13800000
#define READ_GBA		0x14800000
#define WRITE_GBA		0x15800000
#define FORCE_FEEDBACK	0x30800000	//gc steering wheel*/
#define ORIGIN_GC		0x41800000
#define SCAN_GC			0x54800000
//these ones have the optional extra byte parameters
#define SHORT_POLL_GC	0x40800000
//#define CALIBRATE_GC	0x42800000	//forces rumble mode 0x02
#define POLL_GC			0x43800000	//forces rumble mode 0x02
//other 2 command bytes ignore the 6 MSBs
//2nd command byte, poll size (XOR with byte 1)
/*#define SHORTEST	0x00000000
#define SHORTER		0x00010000
#define SHORT		0x00020000*/
#define NOT_SHORT	0x00838000
//3rd command byte, rumble mode (XOR with the other 2 bytes)
#define NORMAL_RUMBLE	0x00000880
#define RUMBLE_ON		0x00000980
#define WAS_RUMBLE		0x00000A80
//#define ?		0x00000300

/*#define CLEAR_BIT	"str %2, [%1, 8] \n"
#define SET_BIT		"str %2, [%1, 4] \n"
#define GET_BIT		"ldr r0, [%1, 16] \n" "str r0, [%0] \n" "add %0, %0, 4 \n"*/

union status {
	uint8_t raw[3];

	struct {
		uint16_t device;	//TODO: get flipped bytes unflipped

		union {
			uint8_t state;

			struct {
				uint8_t : 3;
						  uint8_t rumble : 1;	//???
						  uint8_t : 4;
			};
		};
	};
};

union gcReport {
	uint8_t raw[8];
	uint32_t raw32[2];

	struct {
		uint8_t a : 1;	//br
		uint8_t b : 1;	//bl
		uint8_t x : 1;	//tr
		uint8_t y : 1;	//tl
		uint8_t start : 1;
		uint8_t getOrigin : 1;	//x+y+start
		uint8_t errorLatch : 1;
		uint8_t errorStatus : 1;

		uint8_t dl : 1;
		uint8_t dr : 1;
		uint8_t dd : 1;
		uint8_t du : 1;
		uint8_t z : 1;
		uint8_t r : 1;
		uint8_t l : 1;
		uint8_t useOrigin : 1;

		uint8_t sx;
		uint8_t sy;
		uint8_t cx;	//key1
		uint8_t cy;	//key2
		uint8_t lt;	//key3
		uint8_t rt;	//mic
	};
};

union gcOrigin {
	uint8_t raw[10];

	struct {
		gcReport report;
		//unkown if that's what these 2 bytes are, perhaps it's the location of the center
		uint8_t deadzone0;
		uint8_t deadzone1;
	};
};

union n64report {
	uint8_t raw[4];
	uint32_t raw32;

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
		uint8_t : 1;
				  uint8_t reset : 1;	//l+r+start, start stays low

				  int8_t sx;
				  int8_t sy;
	};
};

struct n64data {
	status status;
	n64report report;
};

struct gcData {
	status status;
	gcOrigin origin;
	gcReport report;
};

static constexpr gcData gcDefault = { { GCC, 0x03 },
{ 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F, 0x00, 0x00 },
{ 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F } };

//TODO: forsake this until after cgcc competition
class Controller {
public:
	status status;
	Controller(int pin, volatile uint32_t *reg, uint32_t mask) {
		pinMode(pin, OUTPUT);
		genReg = reg;
		bitmask = mask;
	}
	Controller() : Controller(2, &CORE_PIN2_PORTREG, CORE_PIN2_BITMASK) {}
	void init() {
		int len = 3;
		transceive(INITIALIZE, len);
		for (int i = 0; i < len; i++) {
			status.raw[i] = raw[i];
		}
	}
protected:
	uint32_t bitmask;
	volatile uint32_t *genReg;
	uint8_t raw[10];
	//TODO: make it wait based on F_CPU
	//send-get
	inline void transceive(uint32_t command, uint32_t len) {
		uint32_t buffer[80] = { 0 };
		noInterrupts();

		//TODO: try brownan method

		interrupts();
		//parse buffer
		uint8_t data[10] = { 0 };
		int g = 7;
		for (int h = 0; h < (int)len; h++) {
			for (int i = h * 8; i < h * 8 + 8; i++) {
				if (buffer[i]) { //& bitmask) {
					data[h] |= _BV(g);
				}
				g--;
			}
			g = 7;
		}
		//copy
		for (int i = 0; i < (int)len; i++) {
			raw[i] = data[i];
		}
	}
};

class N64Controller : public Controller {
public:
	n64report report;
	N64Controller() : Controller() {}
	N64Controller(int pin, volatile uint32_t *reg, uint32_t mask) : Controller(pin, reg, mask) {}
	void poll() {
		int len = 4;
		transceive(POLL_N64, len);
		for (int i = 0; i < len; i++) {
			report.raw[i] = raw[i];
		}
	}
protected:

};

class GCController : public Controller {
public:
	gcOrigin origin;
	gcReport report;
	GCController() : Controller() {}
	GCController(int pin, volatile uint32_t *reg, uint32_t mask) : Controller(pin, reg, mask) {}
	void center() {
		int len = 10;
		transceive(ORIGIN_GC, len);
		for (int i = 0; i < len; i++) {
			origin.raw[i] = raw[i];
		}
	}
	void poll() {
		int len = 8;
		transceive(POLL_GC, len);
		for (int i = 0; i < len; i++) {
			report.raw[i] = raw[i];
		}
	}
	void scan() {
		int len = 8;
		transceive(SCAN_GC, len);
		for (int i = 0; i < len; i++) {
			report.raw[i] = raw[i];
		}
	}
protected:

};

//TODO: do this
class Console {
public:
	Console(int pin, volatile uint32_t *reg, uint32_t mask) {
		pinMode(pin, OUTPUT);
		genReg = reg;
		bitmask = mask;
	}
	Console() : Console(3, &CORE_PIN3_PORTREG, CORE_PIN3_BITMASK) {}
	virtual void init(gcData data) {
		transceive(data.status, 3);
		//same for origin?
	}
	virtual void update(gcData data) {	//leave empty and use child's subroutine
		transceive(data.report, 8);
	}
protected:
	uint32_t bitmask;
	volatile uint32_t *genReg;
	uint8_t raw[10];
	//TODO: make it wait based on F_CPU
	//get-send
	inline void transceive(volatile uint32_t *data, uint32_t len) {
		uint32_t buffer = 0;	//bigger?
		noInterrupts();

		//TODO: write inbound command data to buffer, use cmp and b to send the correct data back

		interrupts();
	}
};
