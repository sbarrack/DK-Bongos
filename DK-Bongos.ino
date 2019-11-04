#include <Nintendo.h>
// #include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

CGamecubeController gcc(2);
CGamecubeConsole gc(3);

void setup()
{
    // Serial.begin(2000000);

    // use "continue;" to step, use "break;" to exit
    for (;;)
    {
        gcc.read();
        gc.write(gcc);
    }
}

void loop() {}
