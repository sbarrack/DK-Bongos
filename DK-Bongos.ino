#include "bongos.h" // TODO modify assembly code
// #include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

GCController gcc(2);
GameCube gc(3);

void setup()
{
    // Serial.begin(2000000);

    // use "continue;" to step, use "break;" to exit
    for (;;)
    {
        passthrough();
    }
}

void loop() {}

void passthrough(void) {
    gcc.read();
    gc.write(gcc);
}

