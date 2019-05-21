#include <Wire.h>
#include "SdFat.h"

#define I2C_ADDR 8

SdFs sd;
FsFile file;

struct {
    uint8_t init : 1;
} SDerrors; // error if nonzero

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
    // Wire.setClock(400000); // fast mode
    // Wire.begin(I2C_ADDR);  // slave

    // Wire.onReceive(get);
    // Wire.onRequest(send);

    Serial.begin(115200);
    while (!Serial);

    SDerrors.init = sd.begin(SdioConfig(FIFO_SDIO))) ? 0 : 1;

    // // SD card I/O
    // if (sd.exists(FILE_NAME)) {
    //     file = sd.open(FILE_NAME, O_READ);
    //     file.read(buff, 15);
    //     Serial.write(buff, 15);
    //     file.close();
    //     sd.remove(FILE_NAME);
    // } else {
    //     Serial.print("touch ");
    //     Serial.println(FILE_NAME);
    //     file = sd.open(FILE_NAME, O_WRITE | O_CREAT);
    //     file.write(text, 15);
    //     file.close();
    // }
}

void loop()
{
    for (;;)
    { // faster when we don't yield()
        // normal bongos
        // rep.raw[0] = bongo.buttons;
        // rep.raw[7] = bongo.mic;
        // rep.raw16[1] = rep.raw16[2] = 0x8080;

        // if (!sd.card()->isBusy())
        // {
        //     // do SD stuff
        // }
    }
}

void yield() {} // get rid of the most useless function ever

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
