#include <Wire.h>
#include "SdFat.h"

#define I2C_ADDR 8
#define WII_EXT_ADDR 0x52

SdFs sd;
FsFile file;

struct
{
    uint8_t init : 1;
    uint8_t : 7;
} SDerrors; // error if nonzero

union{
    // 8 bytes of datareport that we get from the controller
    uint8_t raw[8];
    uint16_t raw16[4];
    uint32_t raw32[2];

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t : 3;

        // second data byte
        uint8_t dl : 1;
        uint8_t dr : 1;
        uint8_t dd : 1;
        uint8_t du : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t : 1;

        // 3rd-8th data byte
        uint8_t sx;
        uint8_t sy;
        uint8_t cx;
        uint8_t cy;
        uint8_t lt;
        uint8_t rt;
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

uint8_t wiiExtID[6];

union {
    uint8_t raw[6];

    struct {
        uint8_t sx;
        uint8_t sy;

        // missing least 2 sig bits
        uint8_t ax;
        uint8_t ay;
        uint8_t az;

        // buttons on wii ext are NOT'd, treat them as booleans
        uint8_t z : 1;
        uint8_t c : 1;
        // the extra bits
        uint8_t ax1 : 2;
        uint8_t ay1 : 2;
        uint8_t az1 : 2;
    } nunchuck;
} wiiExtRep;

void setup()
{    
    Serial.begin(115200);
    while (!Serial);

    // nano
    Wire.setClock(400000); // fast mode
    Wire.begin(I2C_ADDR);  // slave
    Wire.onReceive(get);
    Wire.onRequest(send);

    // wii ext
    Wire1.setClock(400000);
    Wire1.begin(); // master
    // init
    Wire1.beginTransmission(WII_EXT_ADDR);
    Wire1.write(0xf0); // register 0xA400F0
    Wire1.write(0x55); // data
    while (Wire1.endTransmission());
    delayMicroseconds(38); // min delay
    Wire1.beginTransmission(WII_EXT_ADDR);
    Wire1.write(0xfb);
    Wire1.write(0);
    while (Wire1.endTransmission());
    // idWiiExt();

    SDerrors.init = !sd.begin(SdioConfig(FIFO_SDIO));
    // testFileSD();
}

void loop()
{
    // poll wii ext
    delayMicroseconds(42);
    Wire1.beginTransmission(WII_EXT_ADDR);
    Wire1.write(0);
    while (Wire1.endTransmission());
    delayMicroseconds(149);
    int n = Wire1.requestFrom(WII_EXT_ADDR, sizeof(wiiExtRep.raw));
    for (int i = 0; i < n; i++) {
        wiiExtRep.raw[i] = Wire1.read();
    }
    
    // fill rep
    // wii ext
    rep.sx = wiiExtRep.nunchuck.sx;
    rep.sy = wiiExtRep.nunchuck.sy;
    rep.z = !wiiExtRep.nunchuck.z;
    rep.du = !wiiExtRep.nunchuck.c;
    // normal bongos
    rep.raw[0] = bongo.buttons;
    rep.raw[7] = bongo.mic;
    rep.raw16[2] = 0x8080;

    // if (!sd.card()->isBusy())
    // {
    //     // do SD stuff
    // }
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
    // Wire.write(rep.raw, sizeof(rep.raw));
    for (int i = 0; i < (int)sizeof(rep.raw); i++) {
        Wire.write(rep.raw[i]);
    }
}

void idWiiExt() {
    delayMicroseconds(43);
    Wire1.beginTransmission(WII_EXT_ADDR);
    Wire1.write(0xfa);
    Wire1.write(0);
    while (Wire1.endTransmission());
    delayMicroseconds(38);
    int n = Wire1.requestFrom(WII_EXT_ADDR, sizeof(wiiExtID));
    for (int i = 0; i < n; i++) {
        wiiExtID[i] = Wire1.read();
    }
}

void testFileSD() {
    if (sd.exists("hi.txt")) {
        file = sd.open("hi.txt", O_READ);
        uint8_t buff[15];
        file.read(buff, 15);
        Serial.write(buff, 15);
        file.close();
        sd.remove("hi.txt");
    } else {
        Serial.println("touch hi.txt");
        file = sd.open("hi.txt", O_WRITE | O_CREAT);
        file.write("YAY!\r\n\\(^u^)/\r\n", 15);
        file.close();
    }
}
