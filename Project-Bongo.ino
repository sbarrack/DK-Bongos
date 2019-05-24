#include <Wire.h>
#include <GamecubeAPI.h>
#include <N64API.h>

#define I2C_ADDR 8

CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers?
CN64Controller n64c(4);
Gamecube_Report_t out, in = defaultGamecubeData.report;
N64_Report_t out2;

void setup()
{
    Serial.begin(115200);
    // while(!Serial);

    Wire.setClock(400000); // fast mode
    Wire.begin(); // master
}

void loop()
{
    for (;;) {
        // gc
        gcc.read();
        out = gcc.getReport();

        // n64
        n64c.read();
        out2 = n64c.getReport();
        Serial.println(out2.raw32[0], HEX);

        // send
        Wire.beginTransmission(I2C_ADDR);
        // gc
        Wire.write(out.raw8[0]); // buttons
        Wire.write(out.raw8[7]); // mic
        // n64
        Wire.write(out2.raw8, 4);
        Wire.endTransmission(); // never fails ;)

        // get
        int n = Wire.requestFrom(I2C_ADDR, sizeof(in.raw8));
        for (int i = 0; i < n; i++) {
            in.raw8[i] = Wire.read();
        }

        gc.write(in);
    }
}
