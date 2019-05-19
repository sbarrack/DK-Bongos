#include <GamecubeAPI.h>

// 2-9,23-26
CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers

Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    Serial.begin(115200);
    // Serial.setTimeout(1000);
    while (!Serial);
}

void loop()
{
    gcc.read();
    out = gcc.getReport();
    if (Serial.availableForWrite() >= (int)sizeof(out.raw8)) {
        Serial.write(out.raw8, sizeof(out.raw8));
        Serial.flush();
    }
    if (Serial.available() >= (int)sizeof(in.raw8)) {
        Serial.readBytes(in.raw8, sizeof(in.raw8));
    }
    gc.write(in);
}
