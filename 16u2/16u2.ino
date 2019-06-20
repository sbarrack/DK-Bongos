#include <GamecubeAPI.h>
#include <HID-Project.h>

#define START 0xff
const int maxlen = sizeof(Gamecube_Report_t) + 1;
Gamecube_Report_t report = defaultGamecubeData.report;

void setup()
{
    // Serial.begin(115200);  // USB
    Serial1.begin(2000000); // 328p
    Gamepad.begin();
}

void loop()
{
    for (;;)
    {
        int c = Serial1.available();
        if (c >= maxlen)
        {
            uint8_t bite = 0;
            while (bite != START && c-- > 1)
            {
                bite = Serial1.read();
            }
            if (bite == START)
            {
                for (int i = 0; i < c; i++)
                {
                    report.raw8[i] = Serial1.read();
                }
            }
        }

        Gamepad.xAxis((report.xAxis - 0x80) << 8);
        Gamepad.yAxis(~(report.yAxis - 0x80) << 8);
        Gamepad.rxAxis((report.cxAxis - 0x80) << 8);
        Gamepad.ryAxis((report.cyAxis - 0x80) << 8);
        Gamepad.zAxis(report.left - 0x80);
        Gamepad.rzAxis(report.right - 0x80);
        if (report.dup && report.dright)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_UP_RIGHT);
        }
        else if (report.dup && report.dleft)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_UP_LEFT);
        }
        else if (report.ddown && report.dright)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_DOWN_RIGHT);
        }
        else if (report.ddown && report.dleft)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_DOWN_LEFT);
        }
        else if (report.dup)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_UP);
        }
        else if (report.ddown)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_DOWN);
        }
        else if (report.dright)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_RIGHT);
        }
        else if (report.dleft)
        {
            Gamepad.dPad1(GAMEPAD_DPAD_LEFT);
        }
        else
        {
            Gamepad.dPad1(GAMEPAD_DPAD_CENTERED);
        }
        Gamepad.buttons(0x00UL | (report.buttons0 & 0x1F) | ((report.buttons1 & 0x70) << 1));
        Gamepad.write();
    }
}
