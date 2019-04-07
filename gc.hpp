#pragma once
#include <Arduino.h>

#ifdef u8
#undef u8
#endif
#define u8 uint8_t
#ifdef u16
#undef u16
#endif
#define u16 uint16_t
#ifdef u32
#undef u32
#endif
#define u32 uint32_t
#ifdef u64
#undef u64
#endif
#define u64 uint64_t

#ifdef s8
#undef s8
#endif
#define s8 int8_t
#ifdef s16
#undef s16
#endif
#define s16 int16_t
#ifdef s32
#undef s32
#endif
#define s32 int32_t
#ifdef s64
#undef s64
#endif
#define s64 int64_t

union InitAck {
    u8 raw8[3];
    u16 raw16[0];
    u32 raw32[0];
    u64 raw64[0];
    struct {
        u16 device; // bytes reversed, use getDevice()
        u8 info; // ?
    };
};

union PollAck {
    u8 raw8[8];
    u16 raw16[4];
    u32 raw32[2];
    u64 raw64[1];
    struct {
        union {
            u16 buttons; // bytes reversed, use getButtons()
            struct {
                u8 errStat : 1;
                u8 errLatch : 1;
                u8 origin : 1;
                u8 start : 1;
                u8 y : 1;
                u8 x : 1;
                u8 b : 1;
                u8 a : 1;

                u8 : 1; // always high
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
    struct {
        PollAck base;
        u8 clampx; // ?
        u8 clampy; // ?
    };
};

class Controller {
public:
    InitAck initAck;
    PollAck pollAck;
    OriginAck originAck;
    Controller(int pin) : pin(pin) {
        bitmask = digitalPinToBitMask(pin);
        portSetReg = portSetRegister(pin);
        portClearReg = portClearRegister(pin);
        portToggleReg = portToggleRegister(pin);
        portInputReg = portInputRegister(pin);
        portModeReg = portModeRegister(pin);
    }
    int init(int timeout = 300);
    int origin();
    int poll();
    u16 getDevice();
    u16 getButtons();
    u16 getOriginalButtons();
private:
    int pin;
    u32 bitmask, *portSetReg, *portClearReg, *portToggleReg, *portInputReg, *portModeReg;
    void directWrite(u8 val);
    u8 directRead();
    void directMODE(u8 mode);
    int synAck(u8 *cmd, int outlen, u8 *buff, int inlen);
};
