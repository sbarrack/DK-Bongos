/*poll in terms of each port register and recommend to put all 1wire
controllers on a single port, C is good*/
class GCController : protected gcdata {
public:
	GCController(const int pin) : pin(pin) {
		//there's 64 pins XD
		switch (pin) {
		case 0:
			corePin = CORE_PIN0_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN0_PORTREG, CORE_PIN0_BIT);
			outputSetReg = &CORE_PIN0_PORTSET;
			inputReg = &CORE_PIN0_PINREG;
			bitmask = CORE_PIN0_BITMASK;
			break;
		case 1:
			corePin = CORE_PIN1_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN1_PORTREG, CORE_PIN1_BIT);
			outputSetReg = &CORE_PIN1_PORTSET;
			inputReg = &CORE_PIN1_PINREG;
			bitmask = CORE_PIN1_BITMASK;
			break;
		case 2:
			corePin = CORE_PIN2_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN2_PORTREG, CORE_PIN2_BIT);
			outputSetReg = &CORE_PIN2_PORTSET;
			inputReg = &CORE_PIN2_PINREG;
			bitmask = CORE_PIN2_BITMASK;
			break;
		case 3:
			corePin = CORE_PIN3_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN3_PORTREG, CORE_PIN3_BIT);
			outputSetReg = &CORE_PIN3_PORTSET;
			inputReg = &CORE_PIN3_PINREG;
			bitmask = CORE_PIN3_BITMASK;
			break;
		case 4:
			corePin = CORE_PIN4_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN4_PORTREG, CORE_PIN4_BIT);
			outputSetReg = &CORE_PIN4_PORTSET;
			inputReg = &CORE_PIN4_PINREG;
			bitmask = CORE_PIN4_BITMASK;
			break;
		case 5:
			corePin = CORE_PIN5_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN5_PORTREG, CORE_PIN5_BIT);
			outputSetReg = &CORE_PIN5_PORTSET;
			inputReg = &CORE_PIN5_PINREG;
			bitmask = CORE_PIN5_BITMASK;
			break;
		case 6:
			corePin = CORE_PIN6_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN6_PORTREG, CORE_PIN6_BIT);
			outputSetReg = &CORE_PIN6_PORTSET;
			inputReg = &CORE_PIN6_PINREG;
			bitmask = CORE_PIN6_BITMASK;
			break;
		case 7:
			corePin = CORE_PIN7_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN7_PORTREG, CORE_PIN7_BIT);
			outputSetReg = &CORE_PIN7_PORTSET;
			inputReg = &CORE_PIN7_PINREG;
			bitmask = CORE_PIN7_BITMASK;
			break;
		case 8:
			corePin = CORE_PIN8_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN8_PORTREG, CORE_PIN8_BIT);
			outputSetReg = &CORE_PIN8_PORTSET;
			inputReg = &CORE_PIN8_PINREG;
			bitmask = CORE_PIN8_BITMASK;
			break;
		case 9:
			corePin = CORE_PIN9_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN9_PORTREG, CORE_PIN9_BIT);
			outputSetReg = &CORE_PIN9_PORTSET;
			inputReg = &CORE_PIN9_PINREG;
			bitmask = CORE_PIN9_BITMASK;
			break;
		case 10:
			corePin = CORE_PIN10_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN10_PORTREG, CORE_PIN10_BIT);
			outputSetReg = &CORE_PIN10_PORTSET;
			inputReg = &CORE_PIN10_PINREG;
			bitmask = CORE_PIN10_BITMASK;
			break;
		case 11:
			corePin = CORE_PIN11_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN11_PORTREG, CORE_PIN11_BIT);
			outputSetReg = &CORE_PIN11_PORTSET;
			inputReg = &CORE_PIN11_PINREG;
			bitmask = CORE_PIN11_BITMASK;
			break;
		case 12:
			corePin = CORE_PIN12_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN12_PORTREG, CORE_PIN12_BIT);
			outputSetReg = &CORE_PIN12_PORTSET;
			inputReg = &CORE_PIN12_PINREG;
			bitmask = CORE_PIN12_BITMASK;
			break;
		case 13:
			corePin = CORE_PIN13_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN13_PORTREG, CORE_PIN13_BIT);
			outputSetReg = &CORE_PIN13_PORTSET;
			inputReg = &CORE_PIN13_PINREG;
			bitmask = CORE_PIN13_BITMASK;
			break;
		case 14:
			corePin = CORE_PIN14_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN14_PORTREG, CORE_PIN14_BIT);
			outputSetReg = &CORE_PIN14_PORTSET;
			inputReg = &CORE_PIN14_PINREG;
			bitmask = CORE_PIN14_BITMASK;
			break;
		case 15:
			corePin = CORE_PIN15_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN15_PORTREG, CORE_PIN15_BIT);
			outputSetReg = &CORE_PIN15_PORTSET;
			inputReg = &CORE_PIN15_PINREG;
			bitmask = CORE_PIN15_BITMASK;
			break;
		case 16:
			corePin = CORE_PIN16_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN16_PORTREG, CORE_PIN16_BIT);
			outputSetReg = &CORE_PIN16_PORTSET;
			inputReg = &CORE_PIN16_PINREG;
			bitmask = CORE_PIN16_BITMASK;
			break;
		case 17:
			corePin = CORE_PIN17_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN17_PORTREG, CORE_PIN17_BIT);
			outputSetReg = &CORE_PIN17_PORTSET;
			inputReg = &CORE_PIN17_PINREG;
			bitmask = CORE_PIN17_BITMASK;
			break;
		case 18:
			corePin = CORE_PIN18_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN18_PORTREG, CORE_PIN18_BIT);
			outputSetReg = &CORE_PIN18_PORTSET;
			inputReg = &CORE_PIN18_PINREG;
			bitmask = CORE_PIN18_BITMASK;
			break;
		case 19:
			corePin = CORE_PIN19_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN19_PORTREG, CORE_PIN19_BIT);
			outputSetReg = &CORE_PIN19_PORTSET;
			inputReg = &CORE_PIN19_PINREG;
			bitmask = CORE_PIN19_BITMASK;
			break;
		case 20:
			corePin = CORE_PIN20_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN20_PORTREG, CORE_PIN20_BIT);
			outputSetReg = &CORE_PIN20_PORTSET;
			inputReg = &CORE_PIN20_PINREG;
			bitmask = CORE_PIN20_BITMASK;
			break;
		case 21:
			corePin = CORE_PIN21_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN21_PORTREG, CORE_PIN21_BIT);
			outputSetReg = &CORE_PIN21_PORTSET;
			inputReg = &CORE_PIN21_PINREG;
			bitmask = CORE_PIN21_BITMASK;
			break;
		case 22:
			corePin = CORE_PIN22_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN22_PORTREG, CORE_PIN22_BIT);
			outputSetReg = &CORE_PIN22_PORTSET;
			inputReg = &CORE_PIN22_PINREG;
			bitmask = CORE_PIN22_BITMASK;
			break;
		case 23:
			corePin = CORE_PIN23_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN23_PORTREG, CORE_PIN23_BIT);
			outputSetReg = &CORE_PIN23_PORTSET;
			inputReg = &CORE_PIN23_PINREG;
			bitmask = CORE_PIN23_BITMASK;
			break;
		case 24:
			corePin = CORE_PIN24_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN24_PORTREG, CORE_PIN24_BIT);
			outputSetReg = &CORE_PIN24_PORTSET;
			inputReg = &CORE_PIN24_PINREG;
			bitmask = CORE_PIN24_BITMASK;
			break;
		case 25:
			corePin = CORE_PIN25_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN25_PORTREG, CORE_PIN25_BIT);
			outputSetReg = &CORE_PIN25_PORTSET;
			inputReg = &CORE_PIN25_PINREG;
			bitmask = CORE_PIN25_BITMASK;
			break;
		case 26:
			corePin = CORE_PIN26_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN26_PORTREG, CORE_PIN26_BIT);
			outputSetReg = &CORE_PIN26_PORTSET;
			inputReg = &CORE_PIN26_PINREG;
			bitmask = CORE_PIN26_BITMASK;
			break;
		case 27:
			corePin = CORE_PIN27_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN27_PORTREG, CORE_PIN27_BIT);
			outputSetReg = &CORE_PIN27_PORTSET;
			inputReg = &CORE_PIN27_PINREG;
			bitmask = CORE_PIN27_BITMASK;
			break;
		case 28:
			corePin = CORE_PIN28_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN28_PORTREG, CORE_PIN28_BIT);
			outputSetReg = &CORE_PIN28_PORTSET;
			inputReg = &CORE_PIN28_PINREG;
			bitmask = CORE_PIN28_BITMASK;
			break;
		case 29:
			corePin = CORE_PIN29_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN29_PORTREG, CORE_PIN29_BIT);
			outputSetReg = &CORE_PIN29_PORTSET;
			inputReg = &CORE_PIN29_PINREG;
			bitmask = CORE_PIN29_BITMASK;
			break;
		case 30:
			corePin = CORE_PIN30_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN30_PORTREG, CORE_PIN30_BIT);
			outputSetReg = &CORE_PIN30_PORTSET;
			inputReg = &CORE_PIN30_PINREG;
			bitmask = CORE_PIN30_BITMASK;
			break;
		case 31:
			corePin = CORE_PIN31_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN31_PORTREG, CORE_PIN31_BIT);
			outputSetReg = &CORE_PIN31_PORTSET;
			inputReg = &CORE_PIN31_PINREG;
			bitmask = CORE_PIN31_BITMASK;
			break;
		case 32:
			corePin = CORE_PIN32_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN32_PORTREG, CORE_PIN32_BIT);
			outputSetReg = &CORE_PIN32_PORTSET;
			inputReg = &CORE_PIN32_PINREG;
			bitmask = CORE_PIN32_BITMASK;
			break;
		case 33:
			corePin = CORE_PIN33_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN33_PORTREG, CORE_PIN33_BIT);
			outputSetReg = &CORE_PIN33_PORTSET;
			inputReg = &CORE_PIN33_PINREG;
			bitmask = CORE_PIN33_BITMASK;
			break;
		case 34:
			corePin = CORE_PIN34_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN34_PORTREG, CORE_PIN34_BIT);
			outputSetReg = &CORE_PIN34_PORTSET;
			inputReg = &CORE_PIN34_PINREG;
			bitmask = CORE_PIN34_BITMASK;
			break;
		case 35:
			corePin = CORE_PIN35_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN35_PORTREG, CORE_PIN35_BIT);
			outputSetReg = &CORE_PIN35_PORTSET;
			inputReg = &CORE_PIN35_PINREG;
			bitmask = CORE_PIN35_BITMASK;
			break;
		case 36:
			corePin = CORE_PIN36_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN36_PORTREG, CORE_PIN36_BIT);
			outputSetReg = &CORE_PIN36_PORTSET;
			inputReg = &CORE_PIN36_PINREG;
			bitmask = CORE_PIN36_BITMASK;
			break;
		case 37:
			corePin = CORE_PIN37_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN37_PORTREG, CORE_PIN37_BIT);
			outputSetReg = &CORE_PIN37_PORTSET;
			inputReg = &CORE_PIN37_PINREG;
			bitmask = CORE_PIN37_BITMASK;
			break;
		case 38:
			corePin = CORE_PIN38_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN38_PORTREG, CORE_PIN38_BIT);
			outputSetReg = &CORE_PIN38_PORTSET;
			inputReg = &CORE_PIN38_PINREG;
			bitmask = CORE_PIN38_BITMASK;
			break;
		case 39:
			corePin = CORE_PIN39_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN39_PORTREG, CORE_PIN39_BIT);
			outputSetReg = &CORE_PIN39_PORTSET;
			inputReg = &CORE_PIN39_PINREG;
			bitmask = CORE_PIN39_BITMASK;
			break;
		case 40:
			corePin = CORE_PIN40_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN40_PORTREG, CORE_PIN40_BIT);
			outputSetReg = &CORE_PIN40_PORTSET;
			inputReg = &CORE_PIN40_PINREG;
			bitmask = CORE_PIN40_BITMASK;
			break;
		case 41:
			corePin = CORE_PIN41_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN41_PORTREG, CORE_PIN41_BIT);
			outputSetReg = &CORE_PIN41_PORTSET;
			inputReg = &CORE_PIN41_PINREG;
			bitmask = CORE_PIN41_BITMASK;
			break;
		case 42:
			corePin = CORE_PIN42_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN42_PORTREG, CORE_PIN42_BIT);
			outputSetReg = &CORE_PIN42_PORTSET;
			inputReg = &CORE_PIN42_PINREG;
			bitmask = CORE_PIN42_BITMASK;
			break;
		case 43:
			corePin = CORE_PIN43_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN43_PORTREG, CORE_PIN43_BIT);
			outputSetReg = &CORE_PIN43_PORTSET;
			inputReg = &CORE_PIN43_PINREG;
			bitmask = CORE_PIN43_BITMASK;
			break;
		case 44:
			corePin = CORE_PIN44_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN44_PORTREG, CORE_PIN44_BIT);
			outputSetReg = &CORE_PIN44_PORTSET;
			inputReg = &CORE_PIN44_PINREG;
			bitmask = CORE_PIN44_BITMASK;
			break;
		case 45:
			corePin = CORE_PIN45_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN45_PORTREG, CORE_PIN45_BIT);
			outputSetReg = &CORE_PIN45_PORTSET;
			inputReg = &CORE_PIN45_PINREG;
			bitmask = CORE_PIN45_BITMASK;
			break;
		case 46:
			corePin = CORE_PIN46_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN46_PORTREG, CORE_PIN46_BIT);
			outputSetReg = &CORE_PIN46_PORTSET;
			inputReg = &CORE_PIN46_PINREG;
			bitmask = CORE_PIN46_BITMASK;
			break;
		case 47:
			corePin = CORE_PIN47_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN47_PORTREG, CORE_PIN47_BIT);
			outputSetReg = &CORE_PIN47_PORTSET;
			inputReg = &CORE_PIN47_PINREG;
			bitmask = CORE_PIN47_BITMASK;
			break;
		case 48:
			corePin = CORE_PIN48_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN48_PORTREG, CORE_PIN48_BIT);
			outputSetReg = &CORE_PIN48_PORTSET;
			inputReg = &CORE_PIN48_PINREG;
			bitmask = CORE_PIN48_BITMASK;
			break;
		case 49:
			corePin = CORE_PIN49_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN49_PORTREG, CORE_PIN49_BIT);
			outputSetReg = &CORE_PIN49_PORTSET;
			inputReg = &CORE_PIN49_PINREG;
			bitmask = CORE_PIN49_BITMASK;
			break;
		case 50:
			corePin = CORE_PIN50_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN50_PORTREG, CORE_PIN50_BIT);
			outputSetReg = &CORE_PIN50_PORTSET;
			inputReg = &CORE_PIN50_PINREG;
			bitmask = CORE_PIN50_BITMASK;
			break;
		case 51:
			corePin = CORE_PIN51_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN51_PORTREG, CORE_PIN51_BIT);
			outputSetReg = &CORE_PIN51_PORTSET;
			inputReg = &CORE_PIN51_PINREG;
			bitmask = CORE_PIN51_BITMASK;
			break;
		case 52:
			corePin = CORE_PIN52_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN52_PORTREG, CORE_PIN52_BIT);
			outputSetReg = &CORE_PIN52_PORTSET;
			inputReg = &CORE_PIN52_PINREG;
			bitmask = CORE_PIN52_BITMASK;
			break;
		case 53:
			corePin = CORE_PIN53_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN53_PORTREG, CORE_PIN53_BIT);
			outputSetReg = &CORE_PIN53_PORTSET;
			inputReg = &CORE_PIN53_PINREG;
			bitmask = CORE_PIN53_BITMASK;
			break;
		case 54:
			corePin = CORE_PIN54_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN54_PORTREG, CORE_PIN54_BIT);
			outputSetReg = &CORE_PIN54_PORTSET;
			inputReg = &CORE_PIN54_PINREG;
			bitmask = CORE_PIN54_BITMASK;
			break;
		case 55:
			corePin = CORE_PIN55_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN55_PORTREG, CORE_PIN55_BIT);
			outputSetReg = &CORE_PIN55_PORTSET;
			inputReg = &CORE_PIN55_PINREG;
			bitmask = CORE_PIN55_BITMASK;
			break;
		case 56:
			corePin = CORE_PIN56_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN56_PORTREG, CORE_PIN56_BIT);
			outputSetReg = &CORE_PIN56_PORTSET;
			inputReg = &CORE_PIN56_PINREG;
			bitmask = CORE_PIN56_BITMASK;
			break;
		case 57:
			corePin = CORE_PIN57_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN57_PORTREG, CORE_PIN57_BIT);
			outputSetReg = &CORE_PIN57_PORTSET;
			inputReg = &CORE_PIN57_PINREG;
			bitmask = CORE_PIN57_BITMASK;
			break;
		case 58:
			corePin = CORE_PIN58_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN58_PORTREG, CORE_PIN58_BIT);
			outputSetReg = &CORE_PIN58_PORTSET;
			inputReg = &CORE_PIN58_PINREG;
			bitmask = CORE_PIN58_BITMASK;
			break;
		case 59:
			corePin = CORE_PIN59_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN59_PORTREG, CORE_PIN59_BIT);
			outputSetReg = &CORE_PIN59_PORTSET;
			inputReg = &CORE_PIN59_PINREG;
			bitmask = CORE_PIN59_BITMASK;
			break;
		case 60:
			corePin = CORE_PIN60_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN60_PORTREG, CORE_PIN60_BIT);
			outputSetReg = &CORE_PIN60_PORTSET;
			inputReg = &CORE_PIN60_PINREG;
			bitmask = CORE_PIN60_BITMASK;
			break;
		case 61:
			corePin = CORE_PIN61_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN61_PORTREG, CORE_PIN61_BIT);
			outputSetReg = &CORE_PIN61_PORTSET;
			inputReg = &CORE_PIN61_PINREG;
			bitmask = CORE_PIN61_BITMASK;
			break;
		case 62:
			corePin = CORE_PIN62_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN62_PORTREG, CORE_PIN62_BIT);
			outputSetReg = &CORE_PIN62_PORTSET;
			inputReg = &CORE_PIN62_PINREG;
			bitmask = CORE_PIN62_BITMASK;
			break;
		case 63:
			corePin = CORE_PIN63_BIT;
			modeReg = GPIO_BITBAND_ADDR(CORE_PIN63_PORTREG, CORE_PIN63_BIT);
			outputSetReg = &CORE_PIN63_PORTSET;
			inputReg = &CORE_PIN63_PINREG;
			bitmask = CORE_PIN63_BITMASK;
		}
	}
	gcreport getReport() { return report; }
	gcorigin getOrigin() { return origin; }
	status getStatus() { return state; }
	//TODO: inplement n64 data
	gcdata getData() {
		gcdata *pointer = this;
		gcdata data;
		memcpy(&data, pointer, sizeof(data));
		return data;
	}
	void init() {
		uint8_t command[] = { 0x00 };
	}
	void pollN64() {
		uint8_t command[] = { 0x01 };
	}
	void recenter() {
		uint8_t command[] = { 0x41 };
	}
	void poll() {
		uint8_t command[] = { 0x43 };
	}
	void scan() {
		uint8_t command[] = { 0x54 };
	}

protected:
	const int pin;
	uint32_t modeReg, bitmask, corePin;
	volatile uint32_t *outputSetReg, *inputReg;
	//friend class GCConsole;

	//TODO: make it wait based on F_CPU
	//NOTE: everything on this arm core is 32-bit, do everything in words
	inline void transceive() {
		uint8_t oldSREG = SREG;
		cli();
		__asm__ volatile(
			""
			:
		:
			:
			);
		SREG = oldSREG;
	}
};
