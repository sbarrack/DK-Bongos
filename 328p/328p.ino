#include <GamecubeAPI.h>

CGamecubeController controller(2);
CGamecubeConsole console(13);

Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    controller.read();
    report = controller.getReport();
    if (Serial.availableForWrite() >= sizeof(report)) {
        Serial.write(report.raw8, sizeof(report));
    }
}
