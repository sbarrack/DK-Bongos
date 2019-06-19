#include <GamecubeAPI.h>
// #include <HID-Project.h>

CGamecubeController bongos(7);
// CGamecubeConsole console(8);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (!Serial);
    Serial.begin(115200);

    // Gamepad.begin();
}

void loop()
{
    if (bongos.read()) {
        auto status = bongos.getStatus();
        auto report = bongos.getReport();

        Serial.print(F("HIT "));
        Serial.print(status.device, HEX);
        Serial.print(F(" | "));
        Serial.print(report.buttons0, BIN);
        Serial.print(F(" "));
        Serial.print(report.right, HEX);

        // for Windows calibration comment out the l/r buttons below
        // (because l/r triggers the wizard)
        // Gamepad.rzAxis(report.right - 0x80);
        // Gamepad.buttons(0x00UL | (report.buttons0 & 0x1F) | ((report.buttons1 & 0x70) << 1));
        // Gamepad.write();

        // if (console.write(report)) {
        //     Serial.println(F(" HIT"));
        //     digitalWrite(LED_BUILTIN, LOW);
        // } else {
        //     Serial.println(F(" MISS"));
        //     digitalWrite(LED_BUILTIN, HIGH);
        // }
    } else {
        Serial.println(F("MISS"));
        digitalWrite(LED_BUILTIN, HIGH);
        
        // Gamepad.end();
    }
}
