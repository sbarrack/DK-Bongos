# GameCube

Spec | Details
--- | ---
Voltage | 5V, 3.43V
Pullup | 1kOhm
Protocol | [Joybus](./Joybus)

## Devices

- GameCube Controller
- ASCII Keyboard Controller
- [Game Boy](./GameBoy)
- [Donkey Kong Bongos](./Bongos)

## Pinout

J1 | Label | OEM color | Description
--- | --- | --- | ---
1 | VCC2 | blue | 3.43V
2 | VCC1 | yellow | 5V
3 | Data | red | 3.43V
4 | GND | green | Common with J1-5
5 | GND | white
6 | GND | black | Common with shield
| Shield | bare

GameCube controller pinout guides by [Kadano](http://309980.my-oja.net/SSBM/GCC/GCC_PCB_documentation_v0.2a.png) and [Ashen](https://www.gc-forever.com/forums/viewtopic.php?t=1300).

### Alternate color codes

J1 | [Aiskaer](https://www.amazon.com/gp/product/B001AI7CF2/) | HK | Komodo | HDE
--- | --- | --- | --- | ---
1 | white | blue | white | blue
2 | green | yellow | brown | black
3 | red | white | yellow | yellow
4 | yellow | red | blue | red
5 | yellow | brown | red | white
6 | black | black
| black

## Commands

Command | Signal | Resonse
--- | --- | ---
Origin | 0x41 | GC_Origin_t
Poll | 0x400300 | GC_Report_t
Keyboard scan | 0x54 | GC_Report_t

## Polling

Signal | Game | Frequency
--- | --- | ---
0x00 || 15 Hz
0x400300 | Melee | 120 Hz
0x400300 | Donkey Konga | 1.125 kHz

## Structures

```C++
typedef union {
    uint8_t raw[8];
    uint16_t raw16[4];
    uint32_t raw32[2];

    struct{
        uint8_t : 8;

        uint8_t dpad : 4;
        uint8_t : 4;
    };

    struct {
        uint8_t tl : 1;
        uint8_t tr : 1;
        uint8_t bl : 1;
        uint8_t br : 1;
        uint8_t : 4;
    }; // bongos, start facing in

    struct {
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1;
        uint8_t errorLatch : 1;
        uint8_t errorStat : 1;

        uint8_t dl : 1;
        uint8_t dr : 1;
        uint8_t dd : 1;
        uint8_t du : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t : 1;

        uint8_t sx;
        uint8_t sy;

        union {
            struct {
                uint8_t cx;
                uint8_t cy;
                uint8_t lt;
                uint8_t rt;
            };

            struct {
                uint8_t key[3]; // keyboard
                uint8_t mic; // bongos
            };
        };
    };
} GC_Report_t;

typedef union {
    uint8_t raw[10];
    uint16_t raw16[5];
    uint32_t raw32[2];

    GC_Report_t report;
} GC_Origin_t;
```

## References

1. <http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html>
2. <https://sites.google.com/site/consoleprotocols/home/nintendo-joy-bus-documentation/joybus-command-list>
3. <https://github.com/NicoHood/Nintendo/blob/master/src/Gamecube.h>
4. <http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9>
5. <https://docs.google.com/spreadsheets/d/1X-F21F838uQRCuYsc7nQoAv6I2dchvXtBSNTf-ZOc9M/edit#gid=0>
