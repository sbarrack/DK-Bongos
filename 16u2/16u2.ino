#include <GamecubeAPI.h>
#include <HID-Project.h>

#define STOP_BYTE 0xff
const int maxlen = sizeof(Gamecube_Report_t) + 1;
Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    // Serial.begin(115200); // USB
    Serial1.begin(2000000); // 328p
    Gamepad.begin();

    // use "continue;" instead of break, use break to exit
    for (;;)
    {
        if (serialToGcc())
        {
            gccToHid();
        }
    }
}

void loop() {}

bool serialToGcc()
{
    return Serial1.available() >= maxlen ? sizeof(report) == Serial1.readBytesUntil(STOP_BYTE, report.raw8, sizeof(report)) : false;
}

void gccToHid()
{
    Gamepad.buttons(report.buttons0 & 0x1F | (report.buttons1 & 0x70) << 1);

    int8_t hat = GAMEPAD_DPAD_CENTERED;
    if (report.dpad)
    {
        if (report.dpad < 4)
        {
            hat = report.dleft ? GAMEPAD_DPAD_LEFT : GAMEPAD_DPAD_RIGHT;
        }
        else
        {
            if (report.dpad < 8)
            {
                hat = report.dleft ? GAMEPAD_DPAD_DOWN_LEFT : report.dright ? GAMEPAD_DPAD_DOWN_RIGHT : GAMEPAD_DPAD_DOWN;
            }
            else
            {
                hat = report.dleft ? GAMEPAD_DPAD_UP_LEFT : report.dright ? GAMEPAD_DPAD_UP_RIGHT : GAMEPAD_DPAD_UP;
            }
        }
    }
    Gamepad.dPad1(hat);

    Gamepad.xAxis(report.xAxis - 0x80 << 8);
    Gamepad.yAxis(~(report.yAxis - 0x80) << 8);
    Gamepad.rxAxis(report.cxAxis - 0x80 << 8);
    Gamepad.ryAxis(report.cyAxis - 0x80 << 8);
    Gamepad.zAxis(report.left - 0x80);
    Gamepad.rzAxis(report.right - 0x80);

    Gamepad.write();
}
