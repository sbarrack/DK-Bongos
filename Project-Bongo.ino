#include <GamecubeAPI.h>

// 2-9,23-26
CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    gcc.read();
    auto in = gcc.getReport();
    Serial.write(in.raw8, sizeof(in.raw8));
    if (Serial.available() >= (int)sizeof(in.raw8)) {
        Serial.readBytes(in.raw8, sizeof(in.raw8));
    }
    gc.write(in);
}
