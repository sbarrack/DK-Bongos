#include "direct_manip.hpp"

Pin::Pin(uint32_t bitidx, uint32_t bitmask, volatile uint32_t *outputReg,
         volatile uint32_t *setReg, volatile uint32_t *clearReg, volatile uint32_t *toggleReg,
         volatile uint32_t *inputReg, volatile uint32_t *modeReg, volatile uint32_t *configReg)
{
    this->bitidx = bitidx;
    this->bitmask = bitmask;
    this->outputReg = outputReg;
    this->setReg = setReg;
    this->clearReg = clearReg;
    this->toggleReg = toggleReg;
    this->inputReg = inputReg;
    this->modeReg = modeReg;
    this->configReg = configReg;
}

// Recommend using directSet or directClear over this
void Pin::directWrite(uint8_t val)
{
    *outputReg = val ? *outputReg | bitmask : *outputReg & ~bitmask;
}

void Pin::directSet()
{
    *setReg |= bitmask;
}

void Pin::directClear()
{
    *clearReg |= bitmask;
}

void Pin::directToggle()
{
    *toggleReg |= bitmask;
}

uint8_t Pin::directRead()
{
    return (*inputReg & bitmask) >> bitidx;
}

// More modes soon(tm)
void Pin::directMode(uint8_t mode)
{
    switch (mode)
    {
    case OUTPUT:
        *modeReg |= bitmask;
        *configReg = (PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1)) & ~PORT_PCR_ODE;
        break;
    case INPUT_PULLUP:
        *configReg = PORT_PCR_PE | PORT_PCR_PS;
    case INPUT:
        *modeReg &= ~bitmask;
        *configReg |= PORT_PCR_MUX(1);
        break;
    }
}
