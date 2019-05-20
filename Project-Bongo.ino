#include <Wire.h>

#define I2C_ADDR 8

union {
    uint8_t raw[8];
    uint16_t raw16[4];
    uint32_t raw32[2];

    struct
    {
        union {
            uint16_t buttons;
            struct
            {
                uint8_t a : 1;
                uint8_t b : 1;
                uint8_t x : 1;
                uint8_t y : 1;
                uint8_t start : 1;
                uint8_t : 3;

                union {
                    uint8_t dpad : 4;
                    struct
                    {
                        uint8_t dl : 1;
                        uint8_t dr : 1;
                        uint8_t dd : 1;
                        uint8_t du : 1;
                    };
                };
                uint8_t z : 1;
                uint8_t r : 1;
                uint8_t l : 1;
                uint8_t : 1;
            };
        };

        uint8_t sx, sy, cx, cy, lt, rt;
    };
} rep;

union {
    uint8_t raw[2];
    uint16_t raw16;

    struct
    {
        union {
            uint8_t buttons;
            struct
            {
                // bongo w/ start facing in
                uint8_t br : 1;
                uint8_t bl : 1;
                uint8_t tr : 1;
                uint8_t tl : 1;
                uint8_t start : 1;
                uint8_t : 3;
            };
        };

        uint8_t mic;
    };
} bongo;

void setup()
{
    Wire.setClock(400000); // fast mode
    Wire.begin(I2C_ADDR);  // slave

    Wire.onReceive(get);
    Wire.onRequest(send);
}

void loop()
{
    // normal bongos
    rep.raw[0] = bongo.buttons;
    rep.raw[7] = bongo.mic;
    rep.raw16[1] = rep.raw16[2] = 0x8080;
}

void get(int c)
{
    for (int i = 0; i < c; i++)
    {
        bongo.raw[i] = Wire.read();
    }
}

void send()
{
    Wire.write(rep.raw, sizeof(rep.raw));
}
