#pragma once
#include <Arduino.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

union InitAck {
    u8 raw8[3];
    u16 raw16[0];
    u32 raw32[0];
    u64 raw64[0];
    struct
    {
        u16 device; // bytes reversed
        u8 info;    // ?
    };
};

union PollAck {
    u8 raw8[8];
    u16 raw16[4];
    u32 raw32[2];
    u64 raw64[1];
    struct
    {
        union {
            u16 buttons; // bytes reversed
            struct
            {
                u8 errStat : 1;
                u8 errLatch : 1;
                u8 getOrigin : 1;
                u8 start : 1;
                u8 y : 1;
                u8 x : 1;
                u8 b : 1;
                u8 a : 1;

                u8 useOrigin : 1;
                u8 l : 1;
                u8 r : 1;
                u8 z : 1;
                u8 du : 1;
                u8 dd : 1;
                u8 dr : 1;
                u8 dl : 1;
            };
        };
        u8 sx;
        u8 sy;
        u8 cx;
        u8 cy;
        u8 lt;
        u8 rt;
    };
};

union OriginAck {
    u8 raw8[10];
    u16 raw16[5];
    u32 raw32[0];
    u64 raw64[0];
    struct
    {
        PollAck base;
        u8 clampx; // ?
        u8 clampy; // ?
    };
};

class Controller
{
  public:
    InitAck initAck;
    Controller(int pin);
    virtual int init(int timeout = 300);

  protected:
    int pin;
    u32 bitmask;
    volatile u8 *portSetReg, *portClearReg, *portToggleReg, *portInputReg, *portModeReg;
    void directWrite(u8 val);
    u8 directRead();
    void directMode(u8 mode);
    int synAck(u8 *cmd, int outlen, u8 *buff, int inlen);
    int plainInit(int timeout);
    int handleError(int error);
};

class GCC : public Controller
{
  public:
    PollAck pollAck;
    OriginAck originAck;
    GCC(int pin) : Controller(pin) {}
    int origin();
    int poll();
    int init(int timeout = 300);
};
