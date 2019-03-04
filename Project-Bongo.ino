#include "stuff.h"

#define PIN 0

u32 times[66]; // 66 = (32 bits + 1 stop) * (1 fall + 1 rise)
int c;
u32 data;

inline void poll() { // 0x01
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK; // digitalWriteFast(PIN, LOW)
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK; // digitalWriteFast(PIN, HIGH)
	delayMicroseconds(1);

	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	// 0
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	// 1
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
	delayMicroseconds(3);
	// stop
	CORE_PIN0_PORTCLEAR |= CORE_PIN0_BITMASK;
	delayMicroseconds(1);
	CORE_PIN0_PORTSET |= CORE_PIN0_BITMASK;
}

void mark() {
	times[c++] = ARM_DWT_CYCCNT;
}

void setup() {
	// init cycle-counter
	ARM_DEMCR |= ARM_DEMCR_TRCENA;
	ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
	// max interrupt priority
	NVIC_SET_PRIORITY(IRQ_PORTA, 0);
	NVIC_SET_PRIORITY(IRQ_PORTB, 0);
	NVIC_SET_PRIORITY(IRQ_PORTC, 0);
	NVIC_SET_PRIORITY(IRQ_PORTD, 0);
	NVIC_SET_PRIORITY(IRQ_PORTE, 0);
	CORE_PIN0_DDRREG |= CORE_PIN0_BITMASK; // pinMode(PIN, OUTPUT)
	// CORE_PIN0_DDRREG &= ~CORE_PIN0_BITMASK; // pinMode(PIN, INPUT)

	Serial.begin(115200);
	while (!Serial);
}

void loop() {
	c = 0;
	cli();
	poll();
	ARM_DWT_CYCCNT = 0; // reset the counter
	attachInterrupt(digitalPinToInterrupt(0), mark, CHANGE);
	sei();
	delayMicroseconds(300); // wait to service interrupts
	detachInterrupt(digitalPinToInterrupt(PIN));

	data = 0;
	// fall on evens, rise on odds
	for (int i = 0; i < 64; i += 2) {
		times[i + 1] - times[i] < 240 ? data |= 1 : 0;
		data <<= 1;
	}

	Serial.println(data, HEX);
	delay(50);
}
