# Joybus

* Serial
* Bi-directional
* Half-duplex
* Asynchronous
* Idle-high
* Frequency
  * 1 MHz GC/N64
  * 800 kHz Wii

## Protocol

Signal | Duty cycle
---: | ---
Zero | ¯\\\_\_\_\_<u>3 us</u>\_\_\_\_/¯¯¯¯¯
One | ¯\\<u>1 us</u>/¯¯¯¯¯¯¯¯¯¯¯¯¯
Console Stop | ¯\\<u>1 us</u>/¯¯¯¯¯¯¯¯¯
Controller Stop | ¯\\\_\_<u>2 us</u>\_\_/¯¯¯¯¯¯¯¯¯
**Period** | ¯\\\_\_\_\_\_\_<u>4 us</u>\_\_\_\_\_\_/¯

Note that this protocol is Big-Endian.

## Devices

Device | ID
--- | ---
GBA | 0x0004
N64 Controller | 0x0500
GC Keyboard | 0x0820
DK Bongos | 0x0900
GC Controller | 0x0900

## Commands

Command | Signal | Resonse
--- | --- | ---
"Reset" | 0xFF | Status
"Init" | 0x00 | Status

## Structures

Note, fields may be Little-Endian on certain devices.

```C++
typedef union {
    uint8_t raw[3];

    struct {
        union {
            uint16_t device;

            struct {
                uint8_t isDolphin : 1;
                uint8_t : 2;
                uint8_t notUltra64 : 2;
                uint8_t noRumble : 1;
                uint8_t : 2;
            };

            struct {
                uint8_t : 1;
                uint8_t fixedState : 1;
                uint8_t radioFreq : 1;
                uint8_t : 3;
                uint8_t receive : 1;
                uint8_t : 1;

                uint8_t type : 4; // 0x0=normal, 0x4=lite, 0x8=non-controller
                uint8_t notBrokenID : 1;
                uint8_t origin : 1;
                uint8_t : 2;
            } wireless;
        };

        union {
            uint8_t state;

            struct {
                uint8_t : 3;
                uint8_t rumble : 1;
                uint8_t : 4;
            };

            struct {
                uint8_t : 1;
                uint8_t receiving : 1;
                uint8_t : 1;
                uint8_t sending : 1;
                uint8_t general : 2;
                uint8_t : 2;
            } gba;
        };
    };
} Joybus_Status_t;
```

## References

1. <http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html>
2. <https://sites.google.com/site/consoleprotocols/home/nintendo-joy-bus-documentation>
3. <http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9>
4. <https://github.com/NicoHood/Nintendo/blob/master/src/Gamecube_N64.h>
