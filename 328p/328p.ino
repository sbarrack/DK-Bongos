#include <GamecubeAPI.h>
#include "SdFat.h" // SdFat-beta

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
}

void loop()
{
    for (;;)
    {
        digitalWrite(LED_BUILTIN, controller.read() ? HIGH : LOW);
        report = controller.getReport();
        // controller.setRumble(report.a);
        if (Serial.availableForWrite() >= maxlen)
        {
            Serial.write(START);
            Serial.write(report.raw8, sizeof(report));
            Serial.flush();
        }
    }
}
