#include <GamecubeAPI.h>
#include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

#define STOP_BYTE 0xff
Gamecube_Report_t report = defaultGamecubeData.report;
CGamecubeController controller(2);

void setup()
{
    Serial.begin(2000000); // 16u2

    // use "continue;" instead of break, use break to exit
    for (;;)
    {
        if (controller.read())
        {
            gccToSerial();
        }
    }
}

void loop() {}

bool gccToSerial() {
    int temp = 0;
    report = controller.getReport();
    if (Serial.availableForWrite() > sizeof(report))
    {
        temp = Serial.write(report.raw8, sizeof(report));
        Serial.write(STOP_BYTE);
        Serial.flush();
    }
    return temp == sizeof(report);
}
