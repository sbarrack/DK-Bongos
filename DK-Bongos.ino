#include "bongos.h" // smaller nintendo library
#include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

CGamecubeController controller(2);
CGamecubeConsole console(3);

void setup()
{
    // Serial.begin(2000000);

    // use "continue;" instead of break, use break to exit
    for (;;)
    {
	    // bongo stuff here
    }
}

void loop() {}

