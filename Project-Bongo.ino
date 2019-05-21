#include <Wire.h>
#include <GamecubeAPI.h>

#define I2C_ADDR 8

CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers
Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    // Serial.begin(115200);
    // while(!Serial);

    Wire.setClock(400000); // fast mode
    Wire.begin(); // master
}

void loop()
{
    for (;;) {
        gcc.read();
        out = gcc.getReport();

        // send
        Wire.beginTransmission(I2C_ADDR);
        Wire.write(out.raw8[0]); // buttons
        Wire.write(out.raw8[7]); // mic
        Wire.endTransmission(); // never fails ;)
        
        // get
        int n = Wire.requestFrom(I2C_ADDR, sizeof(in.raw8));
        for (int i = 0; i < n; i++) {
            in.raw8[i] = Wire.read();
        }

        gc.write(in);
    }
}
