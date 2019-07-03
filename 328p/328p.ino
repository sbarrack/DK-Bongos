#include <GamecubeAPI.h>
#include "SdFat.h" // SdFat-beta

// #define SD_CONFIG SdioConfig(DEDICATED_SPI)
// store data in machine-readable files (binary)
// use O_ flags and not FILE_ flags
// use sdformatter.exe

#define START 0xff
const int maxlen = sizeof(Gamecube_Report_t) + 1;
CGamecubeController controller(2);
Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(2000000);

    // use "continue;" instead of break
    for (;;)
    {
        if (controller.read())
        {
            digitalWrite(LED_BUILTIN, HIGH);

            report = controller.getReport();
            if (Serial.availableForWrite() >= maxlen)
            {
                Serial.write(START);
                Serial.write(report.raw8, sizeof(report));
                Serial.flush();
            }
        }
        else
        {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

void loop() {}
