#include "gc.hpp"
#ifdef DOGE
const float maxDelta = F_CPU * 0.0000045;
const float minDelta = F_CPU * 0.0000035;
const float midDelta = F_CPU * 0.000002125;
#endif

Controller::Controller(int pin)
{
    this->pin = pin;
    bitmask = digitalPinToBitMask(pin);
    portSetReg = portSetRegister(pin);
    portClearReg = portClearRegister(pin);
    portToggleReg = portToggleRegister(pin);
    portInputReg = portInputRegister(pin);
    portModeReg = portModeRegister(pin);
}

void Controller::directWrite(u8 val)
{
    if (val)
    {
        *portSetReg |= bitmask;
    }
    else
    {
        *portClearReg |= bitmask;
    }
}

u8 Controller::directRead()
{
    return *portInputReg & bitmask ? 1 : 0;
}

void Controller::directMode(u8 mode)
{
    if (mode == INPUT)
    {
        *portModeReg &= ~bitmask;
    }
    else if (mode == OUTPUT)
    {
        *portModeReg |= bitmask;
    }
}

int Controller::synAck(u8 *cmd, int outlen, u8 *buff, int inlen)
{
    directMode(OUTPUT);
    int error = 1;
    for (int i = 0; i < inlen; i++)
    {
        buff[i] = 0;
    }
#ifdef DOGE
    const int marks = inlen * 8 + 1;
    u32 times[marks];
    u8 bits[marks - 1];
#endif

    cli();
    for (int i = 0; i < outlen; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            directWrite(LOW);
            delayMicroseconds(1);
            directWrite((cmd[i] >> j) & 0x1);
            delayMicroseconds(2);
            directWrite(HIGH);
            delayMicroseconds(1);
        }
    }
    directWrite(LOW);
    delayMicroseconds(1);
    directWrite(HIGH);
#ifdef RENA
    delayMicroseconds(4);
#elif !defined(DOGE)
    delayMicroseconds(2);
#endif

    directMode(INPUT);
#ifdef RENA
    for (int i = 0; i < inlen; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            buff[i] |= directRead() << j;
            delayMicroseconds(4);
        }
    }
#elif defined(DOGE)
    while (directRead());
    times[0] = (ARM_DWT_CYCCNT);
    for (int i = 1; i < marks; i++)
    {
        while (!directRead());
        times[i] = (ARM_DWT_CYCCNT);
        while (directRead());
    }
#endif
    sei();

#ifdef DOGE
    int delta = times[1] - times[0];
    bits[0] = delta < midDelta;
    for (int i = 1; i < marks - 1; i++)
    {
        delta = times[i + 1] - times[i];
        if (times[i + 1] < times[i])
        {
            delta = times[i + 1] - (s32)times[i];
        }
        bits[i] = delta < maxDelta && delta < minDelta ? bits[i - 1] : !bits[i - 1];
    }
    int k = 0;
    for (int i = 0; i < inlen; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            buff[i] |= bits[k++] << j;
        }
    }
#endif
    return error;
}

int Controller::plainInit(int timeout)
{
#ifdef DOGE
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
#endif
    int error;
    u8 cmd[1] = {0x00};
    while ((error = synAck(cmd, 1, initAck.raw8, 3)) && timeout-- > 0)
    {
        delay(16);
    }
    return error;
}

int Controller::init(int timeout)
{
    return plainInit(timeout);
}

int Controller::handleError(int error)
{
    int temp = error;
    if (error)
    {
        temp = init();
    }
    return temp;
}

int GCC::origin()
{
    u8 cmd[1] = {0x41};
    return handleError(synAck(cmd, 1, originAck.raw8, 10));
}

int GCC::init(int timeout)
{
    int error = plainInit(timeout);
    if (!error)
    {
        delayMicroseconds(21);
        error = origin();
    }
    return error;
}

int GCC::poll()
{
    u8 cmd[3] = {0x40, 0x03, 0x00};
    int error = handleError(synAck(cmd, 3, pollAck.raw8, 8));
    if (!error && pollAck.getOrigin)
    {
        delayMicroseconds(21);
        error = origin();
    }
    return error;
}
