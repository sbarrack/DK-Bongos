#include <GamecubeAPI.h>

CGamecubeConsole gc(2);
CGamecubeController gcc(3); // bongos; have more controllers?

Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    Serial.begin(115200);
    // while(!Serial);
}

void loop()
{
    for (;;) {
        gcc.read();
        in = gcc.getReport();

        gc.write(out);
    }
}
