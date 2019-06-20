// led logic is inverted, pins are different than 328p!
// https://github.com/NicoHood/HoodLoader2/wiki/4-Pin-Header
#include <HID-Project.h>
#include <GamecubeAPI.h>

Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    Serial.begin(115200); // USB
    Serial1.begin(115200); // 328p
    Gamepad.begin();
}

void loop()
{
    if (Serial1.available() >= sizeof(report)) {
        Serial1.readBytes(report.raw8, sizeof(report));
        for (int i = 0; i < sizeof(report); i++) {
            Serial.print(report.raw8[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    // The two control sticks
    Gamepad.xAxis((report.xAxis - 0x80) << 8);
    Gamepad.yAxis(~(report.yAxis - 0x80) << 8); // y stick needs to be inverted
    Gamepad.rxAxis((report.cxAxis - 0x80) << 8);
    Gamepad.ryAxis((report.cyAxis - 0x80) << 8);

    // you can merge L/R (PS3 controller uses this methode too)
    // or you can also seperate the triggers
    // for Windows calibration comment out the l/r buttons below
    // (because l/r triggers the wizard)
    //Gamepad.zAxis(abs(report.left - report.right) - 0x80);
    Gamepad.zAxis(report.left - 0x80);
    Gamepad.rzAxis(report.right - 0x80);

    // D-Pad:
    if (report.dup && report.dright) {
        Gamepad.dPad1(GAMEPAD_DPAD_UP_RIGHT);
    }
    else if (report.dup && report.dleft) {
        Gamepad.dPad1(GAMEPAD_DPAD_UP_LEFT);
    }
    else if (report.ddown && report.dright) {
        Gamepad.dPad1(GAMEPAD_DPAD_DOWN_RIGHT);
    }
    else if (report.ddown && report.dleft) {
        Gamepad.dPad1(GAMEPAD_DPAD_DOWN_LEFT);
    }
    else if (report.dup) {
        Gamepad.dPad1(GAMEPAD_DPAD_UP);
    }
    else if (report.ddown) {
        Gamepad.dPad1(GAMEPAD_DPAD_DOWN);
    }
    else if (report.dright) {
        Gamepad.dPad1(GAMEPAD_DPAD_RIGHT);
    }
    else if (report.dleft) {
        Gamepad.dPad1(GAMEPAD_DPAD_LEFT);
    }

    // 8 Gamecube buttons
    Gamepad.buttons(0x00UL | (report.buttons0 & 0x1F) | ((report.buttons1 & 0x70) << 1));

    // You could add a zero value correction here or a minimum
    // for the left/right shoulder triggers but the os can
    // (and should) calibrate controllers very well.

    // Write the information to the PC
    Gamepad.write();
}
