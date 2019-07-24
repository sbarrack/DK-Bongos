/*
Copyright (c) 2014-2016 NicoHood
See the readme for credit to other people.

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
THE SOFTWARE.
*/

#pragma once
#include <Arduino.h>

#if (!defined(__AVR__) && F_CPU != 16000000)
#error This library is only for AVRs set to 16 MHz (e.g. Uno, Nano)
#endif

// byte-flipped because C/C++ is little-endian
#define NINTENDO_DEVICE_GC_WIRED    0x0009

typedef union{
    uint8_t raw[3];
    struct {
        uint16_t device;
        union{
            uint8_t status;
            struct{
                uint8_t : 3;
                uint8_t rumble : 1;
                uint8_t : 4;
            };
        };
    };
} Gamecube_N64_Status_t;

typedef union{
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
        uint8_t : 3;

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
} Gamecube_Report_t;

typedef union{
    uint8_t raw[10];
    uint16_t raw16[5];
    uint32_t raw32[2];
    Gamecube_Report_t report;
} Gamecube_Origin_t;

struct Gamecube_Data_t{
    Gamecube_N64_Status_t status;
    Gamecube_Origin_t origin;
    Gamecube_Report_t report;
};

static constexpr Gamecube_Data_t defaultGamecubeData = {
    .status = { 0x09, 0x00, 0x03 },
    .origin = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F, 0x00, 0x00 },
    .report = { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x1F, 0x1F }
};

#ifdef __cplusplus
extern "C" {
#endif

uint8_t gc_n64_send_get(const uint8_t pin, uint8_t* command, const uint8_t commandLen,
    uint8_t* report, const uint8_t reportLen) __attribute__((noinline));

void gc_n64_send(const uint8_t* buff, uint8_t len,
    volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask)
    __attribute__((noinline));

uint8_t gc_n64_get(uint8_t* buff, uint8_t len,
    volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask)
    __attribute__((noinline));

bool gc_init(const uint8_t pin, Gamecube_N64_Status_t* status);
bool gc_origin(const uint8_t pin, Gamecube_Origin_t* origin);
bool gc_read(const uint8_t pin, Gamecube_Report_t* report, const bool rumble);
uint8_t gc_write(const uint8_t pin, Gamecube_N64_Status_t* status, Gamecube_Origin_t* origin, Gamecube_Report_t* report);

#ifdef __cplusplus
}
#endif

class CGamecubeController : protected Gamecube_Data_t {
public:
    inline CGamecubeController(const uint8_t p) : pin(p){}

    inline void reset(void){
        status = Gamecube_N64_Status_t();
    }

    inline bool begin(void){
        // Try to init the controller
        if (!gc_init(pin, &status))
        {
            // Reset in any case, as some bytes may have been written.
            reset();
            return false;
        }

        // If initialization was successful also get the original controller stats
        if (!gc_origin(pin, &origin))
        {
            return false;
        }

        // No error
        return true;
    }

    inline uint16_t getDevice(void){
        return status.device;
    }

    inline bool connected(void){
        return (status.device != NINTENDO_DEVICE_GC_NONE);
    }

    inline bool read(void){
        // Check if the controller was initialized
        if (!connected())
        {
            // Try to init the controller
            if (!begin())
            {
                return false;
            }
        }

        // Read the controller, abort if it fails.
        // Additional information: If you press X + Y + Start on the controller for 3 seconds
        // It will turn off unless you release the buttons. The recalibration is all done
        // on the Gamecube side. If the controller resets origin will have different values for sure.
        if (!gc_read(pin, &report, status.rumble))
        {
            reset();
            return false;
        }

        // Check if controller reported that we read the origin values (check if it disconnected).
        // The Gamecube would just request (instantly) the origin again, but we keep things simple.
        if (report.origin) {
            reset();
            return false;
        }

        // Return status information for optional use.
        // On error the report may have been modified!
        return true;
    }

    inline bool getRumble(void){
        return status.rumble;
    }

    inline bool setRumble(bool rumble){
        bool oldRumble = getRumble();
        status.rumble = rumble;
        return oldRumble;
    }

    inline bool end(void){
        // Try to init the controller
        if (connected() || getRumble())
        {
            // Reset in any case, as some bytes may have been written.
            Gamecube_Report_t tmp;
            return gc_read(pin, &tmp, false);
        }

        // Error: Already disconnected
        return false;
    }

    inline Gamecube_N64_Status_t getStatus(void){
        return status;
    }

    inline Gamecube_Origin_t getOrigin(void){
        return origin;
    }

    inline Gamecube_Report_t getReport(void){
        return report;
    }

    inline Gamecube_Data_t getData(void){
        Gamecube_Data_t* dataPtr = this;
        Gamecube_Data_t data;
        memcpy(&data, dataPtr, sizeof(data));
        return data;
    }

protected:
    const uint8_t pin;
    friend class CGamecubeConsole;
};

class CGamecubeConsole{
public:
    inline CGamecubeConsole(const uint8_t p) : pin(p){}

    inline bool write(Gamecube_Data_t &data){
        // Abort if controller was not initialized.
        // Gamecube will refuse and weird connect/disconnect errors will occur.
        if (data.report.origin) {
            return false;
        }

        // Don't want interrupts getting in the way
        uint8_t oldSREG = SREG;
        cli();

        // Write a respond to the gamecube, depending on what it requests
        uint8_t ret = gc_write(pin, &data.status, &data.origin, &data.report);

        // Init
        if(ret == 1)
        {
            // Try to answer a possible following origin request
            ret = gc_write(pin, &data.status, &data.origin, &data.report);
        }

        // Origin
        if(ret == 2){
            // Try to answer a possible following read request
            ret = gc_write(pin, &data.status, &data.origin, &data.report);
        }

        // End of time sensitive code
        SREG = oldSREG;

        // Set rumble depending on read return value
        if (ret == 3) {
            data.status.rumble = false;
            return true;
        }
        else if (ret == 4) {
            data.status.rumble = true;
            return true;
        }
        else if (ret == 5) {
            data.status.rumble = false;
            return true;
        }

        // Return error if no reading was possible
        return false;
    }

    inline bool write(CGamecubeController &controller){
        // Cast controller to its protected (friend) data
        Gamecube_Data_t& data = controller;
        return write(data);
    }

    inline bool write(Gamecube_Report_t &report){
        // Inititalize init and origin with default values
        Gamecube_Data_t data = defaultGamecubeData;

        // Copy report into the temporary struct and write to gamecube
        memcpy(&data.report, &report, sizeof(data.report));
        return write(data);
    }

protected:
    const uint8_t pin;
};

