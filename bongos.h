/* Copyright (c) 2014-2016 NicoHood

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */
#pragma once
#include <Arduino.h>

#if (!defined(__AVR__) && F_CPU != 16000000)
#error This library is only for AVRs set to 16 MHz (e.g. Uno, Nano)
#endif

#define GCC_AND_BONGOS 0x0009 // byte flipped because C/C++ is little-endian

typedef union {
    uint8_t raw[3];
    struct {
        uint16_t device;
        union {
            uint8_t status;
            struct {
                uint8_t : 3;
                uint8_t rumble : 1;
                uint8_t : 4;
            };
        };
    };
} GCStatus_t;

typedef union {
    uint8_t raw[8];
    uint16_t raw16[4];
    uint32_t raw32[2];
    struct{
        uint8_t : 8;
        uint8_t dpad : 4;
    };
    struct {
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // 1 if just plugged in or x+y+start
        uint8_t errLatch : 1;
        uint8_t errStat : 1;

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
        uint8_t cx;
        uint8_t cy;
        uint8_t lt;
        uint8_t rt;
    };
} GCReport_t;

typedef union {
    uint8_t raw[10];
    uint16_t raw16[5];
    uint32_t raw32[2];
    GCReport_t report;
} GCOrigin_t;

struct GCData_t {
    GCStatus_t status;
    GCOrigin_t origin;
    GCReport_t report;
};

static constexpr GCData_t defGCData = {
    .status = { 0x09, 0x00, 0x03 }, // think it's actually 0x090002
    .origin = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F, 0x00, 0x00 },
    .report = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F }
};


#ifdef __cplusplus
extern "C" {
#endif

uint8_t gcTransceive(const uint8_t pin, uint8_t* command, const uint8_t cmdLen, uint8_t* buffer, const uint8_t buffLen) __attribute__((noinline));
void gcTransmit(const uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask) __attribute__((noinline));
uint8_t gcReceive(uint8_t* buff, uint8_t len, volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask) __attribute__((noinline));


bool gccInitialize(const uint8_t pin, GCStatus_t* stat);
bool gccOriginate(const uint8_t pin, GCOrigin_t* orig);
bool gccPoll(const uint8_t pin, GCReport_t* rept, const bool rumble);
uint8_t gcWrite(const uint8_t pin, GCStatus_t* stat, GCOrigin_t* orig, GCReport_t* rept);

#ifdef __cplusplus
}
#endif


class GCController : protected GCData_t {
public:
    inline GCController(const uint8_t p) : pin(p) {}

    inline void reset(void) { // in C, ommitting void specifies unlimited parameters
        status = GCStatus_t();
    }

    inline bool begin(void) {
        if (!gccInitialize(pin, &status)) {
            reset();
            return false;
        }

        return !gccOriginate(pin, &origin);
    }

    inline uint16_t getDevice(void) {
        return status.device;
    }

    inline bool connected(void) {
        return status.device;
    }

    inline bool read(void) {
        if (!connected()) {
            if (!begin()) {
                return false;
            }
        }
        if (!gc_read(pin, &report, status.rumble)) {
            reset();
            return false;
        }

        return !report.origin;
    }

    inline bool getRumble(void) {
        return status.rumble;
    }

    inline bool setRumble(bool rumble) {
        bool temp = getRumble();
        status.rumble = rumble;
        return temp;
    }

    inline bool end(void) {
        if (connected() || getRumble()) {
            Gamecube_Report_t tmp;
            return gc_read(pin, &tmp, false);
        }

        return false;
    }

    inline GCStatus_t getStatus(void) {
        return status;
    }

    inline GCOrigin_t getOrigin(void) {
        return origin;
    }

    inline GCReport_t getReport(void) {
        return report;
    }

    inline GCData_t getData(void) {
        GCData_t* dataPtr = this; // cast "this" instead?
        GCData_t data;
        memcpy(&data, dataPtr, sizeof(data));
        return data;
    }

protected:
    const uint8_t pin;
    friend class GameCube;
};

class GameCube {
public:
    inline GameCube(const uint8_t p) : pin(p) {}

    inline bool write(GCData_t& data) { // GCData_t& data is a reference, not a copy
        if (data.report.origin) {
            return false;
        }

        uint8_t oldSREG = SREG;
        cli();

        uint8_t ret = gcWrite(pin, &data.status, &data.origin, &data.report);
        if (ret == 1) {
            ret = gcWrite(pin, &data.status, &data.origin, &data.report);
        }
        if (ret == 2) {
            ret = gcWrite(pin, &data.status, &data.origin, &data.report);
        }

        SREG = oldSREG;

        if (ret == 3 || ret == 5) {
            data.status.rumble = false;
        } else if (ret == 4) {
            data.status.rumble = true;
        } else {
            return false;
        }

        return true;
    }

    inline bool write(GCController& controller) {
        GCData_t& data = controller; // cast controller instad?
        return write(data);
    }

    inline bool write(GCReport_t& report) {
        GCData_t data = defGCData;
        memcpy(&data.report, &report, sizeof(data.report));
        return write(data);
    }

protected:
    const uint8_t pin;
};

