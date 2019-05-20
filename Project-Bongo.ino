#include <Wire.h>
#include <GamecubeAPI.h>

CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers

Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    Wire.setClock(400000); // fast mode
    Wire.begin(8); // slave

    Wire.onRequest(send);
    Wire.onReceive(get);
}

void loop()
{
    gcc.read();
    out = gcc.getReport();
    gc.write(in);
}

void send() {
    Wire.write(out.raw8, sizeof(out.raw8));
}

void get() {
    if (Wire.available() >= (long long)sizeof(in.raw8)) {
        for (int i = 0; i < sizeof(in.raw8); i++) {
            in.raw8[i] = Wire.read();
        }
    }
}
