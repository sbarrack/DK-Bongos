#include "GamecubeAPI.h"

CGamecubeController gcc(7);
CGamecubeConsole gc(8);

void setup()
{

}

void loop()
{
    for (;;) {
        gcc.read();
        auto rep = gcc.getReport();
        rep.xAxis = rep.yAxis = rep.cxAxis = rep.cyAxis = 0x80;
        rep.left = 0x49;
        gc.write(rep);
    }
}
