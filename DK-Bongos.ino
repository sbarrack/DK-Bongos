#include <GamecubeAPI.h>
#include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

#define STOP_BYTE 0xff
Gamecube_Report_t report = defaultGamecubeData.report;
CGamecubeController controller(2);
CGamecubeConsole console(3);

void setup()
{
    // Serial.begin(2000000);

    // use "continue;" instead of break, use break to exit
    // eventually, switch to a makefile instead of IDE
    for (;;)
    {
        // TODO minify nintendo library in this project
	// bongo stuff here
    }
}

void loop() {}
