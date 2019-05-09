#include "GamecubeAPI.h"

CGamecubeController gcc(7);
CGamecubeConsole gc(8);
Gamecube_Report_t rep;
unsigned long timeNow, timeMark, timeMarkOld;

void setup()
{
    rep = defaultGamecubeData.report;
}

void loop()
{
    for (;;) {
        timeNow = micros();

        // if (dTime >= .5 frames || overflow < -.5 frames)
        if (timeNow >= timeMark || timeNow < timeMarkOld) {
            timeMarkOld = timeMark;
            timeMark += 8333;

            gcc.read();
            rep = gcc.getReport();
            rep.xAxis = rep.yAxis = rep.cxAxis = rep.cyAxis = 0x80;
        }

        gc.write(rep);
    }
}
