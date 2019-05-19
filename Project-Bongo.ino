#include <GamecubeAPI.h>

#define BUFF_SIZE 8

// 2-9,23-26
CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers

Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    Serial.begin(115200); // SERIAL_8N1
    // Serial.setTimeout(1000);
    // while (!Serial);
}

void loop()
{
    if (Serial.availableForWrite() >= BUFF_SIZE) {
        gcc.read();
        out = gcc.getReport();
        Serial.write(out.raw8, BUFF_SIZE);
        Serial.flush();
    }
    if (Serial.available() >= BUFF_SIZE) {
        Serial.readBytes(in.raw8, BUFF_SIZE);
    }
    gc.write(in);
}
