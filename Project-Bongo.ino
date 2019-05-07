#include <Nintendo.h>

CGamecubeController gcc(7);
CGamecubeConsole gc(8);

void yield() {}

void setup()
{

}

void loop()
{
    gcc.read();
    auto rep = gcc.getReport();

    rep.xAxis = rep.yAxis = rep.cxAxis = rep.cyAxis = 0x80;
    rep.left = 0x20;

    gc.write(rep);
}
