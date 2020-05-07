# GameBoy

Spec | Details
--- | ---
Voltage | 3.43V
Pullup | 1kOhm
Protocol | [Joybus](./Joybus), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)

## Devices

- Game Boy
  - Game Boy Color
- Game Boy Advanced
  - Game Boy Advanced SP
  - GameCube Link Cable

## Pinout

Soon&trade;

## Joybus

Command | Signal | Resonse
--- | --- | ---
GBA read | 0x14 | JOYTRANS (4), JOYSTAT (1)
GBA write | 0x15, JOYRECV (4) | JOYSTAT (1)

JOYTRANS, JOYRECV, and JOYSTAT are all 32-bit, long-word registers. JOYTRANS is the GBA's output. JOYRECV is the GBA's input. JOYSTAT indicates the transmission status of the GBA, but only sends the least significant byte. Bytes in those registers are sent little-endian.

### Polling

Signal | Game | Frequency
--- | --- | ---
0xFF | Pokemon XD | ~6.7 Hz
0x00 | Pokemon XD | 4.5 kHz, 3.75 kHz

## SPI

Soon&trade;

## Structures

```C++
// Joybus
typedef union {
    uint8_t raw[5];
    uint16_t raw16[2];

    struct {
        uint32_t data;

        union {
            uint8_t state;

            struct {
                uint8_t : 1;
                uint8_t receiving : 1;
                uint8_t : 1;
                uint8_t sending : 1;
                uint8_t general : 2;
                uint8_t : 2;
            };
        };
    };
} GBA_Report_t;
```

## References

1. <http://problemkaputt.de/gbatek.htm#siojoybusmode>
2. <https://mathewjhall.wordpress.com/2012/04/26/reading-roms-and-saving-pokemon/>
3. <https://www.insidegadgets.com/2011/03/19/gbcartread-arduino-based-gameboy-cart-reader-%e2%80%93-part-1-read-the-rom/>
