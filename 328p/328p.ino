#include <GamecubeAPI.h>

CGamecubeController controller(2);

Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    digitalWrite(LED_BUILTIN, controller.read() ? HIGH : LOW);
    report = controller.getReport();
    if (report.a) {
        controller.setRumble(true);
    } else {
        controller.setRumble(false);
    }
    if (Serial.availableForWrite() >= sizeof(report)) {
        // Serial.write(report.raw8, sizeof(report));
        for (int i = 0; i < sizeof(report); i++) {
            Serial.write(report.raw8[i]);
            Serial.flush();
        }
    }
    delay(100);
}
