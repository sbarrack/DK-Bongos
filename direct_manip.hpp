#pragma once

#include "stuff.h"

class Pin
{
  public:
    Pin(uint32_t bitidx, uint32_t bitmask, volatile uint32_t *outputReg,
        volatile uint32_t *setReg, volatile uint32_t *clearReg, volatile uint32_t *toggleReg,
        volatile uint32_t *inputReg, volatile uint32_t *modeReg, volatile uint32_t *configReg);
    void directWrite(uint8_t val);
    void directSet();
    void directClear();
    void directToggle();
    uint8_t directRead();
    void directMode(uint8_t mode);

  private:
    uint32_t bitidx;              // CORE_PIN13_BIT
    uint32_t bitmask;             // CORE_PIN13_BITMASK
    volatile uint32_t *outputReg; // &CORE_PIN13_PORTREG
    volatile uint32_t *setReg;    // &CORE_PIN13_PORTSET
    volatile uint32_t *clearReg;  // &CORE_PIN13_PORTCLEAR
    volatile uint32_t *toggleReg; // &CORE_PIN13_PORTTOGGLE
    volatile uint32_t *inputReg;  // &CORE_PIN13_PINREG
    volatile uint32_t *modeReg;   // &CORE_PIN13_DDRREG
    volatile uint32_t *configReg; // &CORE_PIN13_CONFIG
};
