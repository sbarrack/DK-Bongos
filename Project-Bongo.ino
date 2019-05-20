#include <Wire.h>
#include <GamecubeAPI.h>

#define I2C_ADDR 8

CGamecubeConsole gc(2);
CGamecubeController gcc(3); // have more controllers
Gamecube_Report_t out, in = defaultGamecubeData.report;

void setup()
{
    Wire.setClock(400000); // fast mode
    Wire.begin(); // master
}

void loop()
{
    gcc.read();
    out = gcc.getReport();

    // send
    Wire.beginTransmission(I2C_ADDR);
    // bongos only need 2 bytes tops
    Wire.write(out.raw8[0]);
    Wire.write(out.raw8[7]);
    Wire.endTransmission();

    // get
    Wire.requestFrom(I2C_ADDR, sizeof(in.raw8));
    for (int i = 0; Wire.available() && i < sizeof(in.raw8); i++) {
        in.raw8[i] = Wire.read();
    }

    gc.write(in);
}
