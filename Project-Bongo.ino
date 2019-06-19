#include <Wire.h>
#include "SdFat.h"

#define WII_EXT_ADDR 0x52
#define I2C_ADDR WII_EXT_ADDR

SdFs sd;
FsFile file;

struct
{
    uint8_t init : 1;
    uint8_t : 7;
} SDerrors; // error if nonzero

union {
    // 8 bytes of datareport that we get from the controller
    uint8_t raw[8];
    uint16_t raw16[4];
    uint32_t raw32[2];

    struct {
        uint8_t : 8;

        uint8_t dpad : 4;
        uint8_t : 4;
    };

    struct
    {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t : 3;

        // second data byte
        uint8_t dl : 1;
        uint8_t dr : 1;
        uint8_t dd : 1;
        uint8_t du : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t : 1;

        // 3rd-8th data byte
        uint8_t sx;
        uint8_t sy;
        uint8_t cx;
        uint8_t cy;
        uint8_t lt;
        uint8_t rt;
    };
} rep;

union {
    uint8_t raw[2];
    uint16_t raw16;

    struct
    {
        union {
            uint8_t buttons;

            struct
            {
                // bongo w/ start facing in
                uint8_t br : 1;
                uint8_t bl : 1;
                uint8_t tr : 1;
                uint8_t tl : 1;
                uint8_t start : 1;
                uint8_t : 3;
            };
        };

        uint8_t mic;
    };
} bongo;

union {
    uint8_t raw[4];
    uint16_t raw16[2];
    uint32_t raw32;

    struct {
        uint8_t dpad : 4;
        uint8_t : 4;
    };

    struct {
        uint8_t dr : 1;
        uint8_t dl : 1;
        uint8_t dd : 1;
        uint8_t du : 1;
        uint8_t start : 1;
        uint8_t z : 1;
        uint8_t b : 1;
        uint8_t a : 1;

        uint8_t cr : 1;
        uint8_t cl : 1;
        uint8_t cd : 1;
        uint8_t cu : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t : 1;
        uint8_t reset : 1; // l + r + start

        int8_t sx;
        int8_t sy;
    };
} n64c;

// bytes are backwards cause unions are little endian in c and we don't got time for that!
enum WiiExtFull : uint64_t {
    Nunchuck = 0x000020a40000,
    Guitar = 0x030120a40000,
    Drums = 0x030120a40001

    // don't care about classic, tablet, or tatacon
    // turntable soon(tm)
    // balance board is not accessible
    // wii motion+ is stupid so no
};

union {
    uint8_t raw[6];
    WiiExtFull type;
} wiiExtID;

// buttons/booleans are inverted, fyi
// alternative is to bitwise NOT all incoming data
union {
    uint8_t raw[6];

    // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
    // Don't expect me to explain these for ya
    struct
    {
        uint8_t sx;
        uint8_t sy;

        uint16_t ax : 8;
        uint16_t ay : 8;
        uint16_t az : 8;

        uint8_t z : 1;
        uint8_t c : 1;
        uint16_t ax1 : 2;
        uint16_t ay1 : 2;
        uint16_t az1 : 2;
    } nunchuck;

    // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Guitar_Hero_(Wii)_Guitars
    struct {
        uint8_t sx : 6;
        uint8_t : 1;
        uint8_t guitarType : 1;

        uint8_t sy : 6;
        uint8_t : 2;

        uint8_t fretbar : 5; // funky values
        uint8_t : 3;

        uint8_t whammybar : 5;
        uint8_t : 3;

        uint8_t : 2;
        uint8_t start : 1; // either/both +'s
        uint8_t : 1;
        uint8_t select : 1; // - or starpower
        uint8_t : 1;
        uint8_t dd; // strum down
        uint8_t : 1;

        uint8_t du; // strum up
        uint8_t : 2;
        // these have normal button labels, check mobo inside
        uint8_t yellow : 1;
        uint8_t green : 1;
        uint8_t blue : 1;
        uint8_t red : 1;
        uint8_t orange : 1;
    } guitar;

    // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Guitar_Hero_World_Tour_(Wii)_Drums
    struct {
        uint8_t sx : 6;
        uint8_t : 2;

        uint8_t sy : 6;
        uint8_t : 2;

        uint8_t : 1;
        uint8_t vSelect : 5; // which pad the velocity is for
        uint8_t vNone : 1; // if there's no velocity
        uint8_t l : 1; // high-hat pedal

        uint8_t : 5;
        uint8_t velocity : 3; // low values = more velocity, high-hat and bass share one

        uint8_t : 2;
        uint8_t start : 1; // +
        uint8_t : 1;
        uint8_t select : 1; // -
        uint8_t : 3;

        uint8_t : 2;
        uint8_t r : 1; // bass pedal
        uint8_t blue : 1;
        uint8_t green : 1;
        uint8_t yellow : 1;
        uint8_t red : 1;
        uint8_t orange : 1;
    } drums;
} wiiExtRep;

void setup()
{
    Serial.begin(115200);
    // while (!Serial);

    // // nano
    // Wire.setClock(400000); // fast mode
    // Wire.begin(I2C_ADDR);  // slave
    // Wire.onReceive(get);
    // Wire.onRequest(send);

    // // wii ext
    // Wire1.setClock(400000);
    // Wire1.begin(); // master
    // // init
    // Wire1.beginTransmission(WII_EXT_ADDR);
    // Wire1.write(0xf0); // @ 0x(4)a400f0
    // Wire1.write(0x55); // write 0x55
    // while (Wire1.endTransmission());
    // delayMicroseconds(38); // min delay
    // Wire1.beginTransmission(WII_EXT_ADDR);
    // Wire1.write(0xfb);
    // Wire1.write(0);
    // while (Wire1.endTransmission());
    // idWiiExt();

    // SDerrors.init = !sd.begin(SdioConfig(FIFO_SDIO));
    // // testFileSD();

    Serial1.begin(115200);

}

void loop()
{
<<<<<<< HEAD
    while (Serial.available()) {
        Serial1.write(Serial.read());
    }
    while (Serial1.available()) {
        Serial.write(Serial1.read());
    }

    // // poll wii ext
    // delayMicroseconds(42);
    // Wire1.beginTransmission(WII_EXT_ADDR);
    // Wire1.write(0); // @ addr 0x(4)a40000
    //                 // read
    // while (Wire1.endTransmission());
    // delayMicroseconds(149);
    // int n = Wire1.requestFrom(WII_EXT_ADDR, sizeof(wiiExtRep.raw));
    // for (int i = 0; i < n; i++)
    // {
    //     wiiExtRep.raw[i] = Wire1.read();
    // }

    // // normal bongos
    // rep.raw[0] = bongo.buttons;
    // rep.raw[7] = bongo.mic;

    // // wii ext
    // if (wiiExtID.type == Nunchuck) {
    //     rep.cx = wiiExtRep.nunchuck.sx;
    //     rep.cy = wiiExtRep.nunchuck.sy;
    //     rep.lt = wiiExtRep.nunchuck.z ? 0x20 : 0x80;
    // } // TODO add other cases

    // // n64c
    // rep.sx = n64c.sx - 0x80; // convert the signed to unsigned
    // rep.sy = n64c.sy - 0x80;
    // rep.l = n64c.l;
    // rep.r = n64c.r;
    // rep.z = n64c.z;
    // rep.dpad = n64c.dpad;

    // // if (!sd.card()->isBusy())
    // // {
    // //     // do SD stuff
    // // }
}

void yield() {}

void get(int c)
{
    int n = c >= 2 ? sizeof(bongo.raw) : c;
    for (int i = 0; i < n; i++)
    {
        bongo.raw[i] = Wire.read();
    }
    n = c - n;
    for (int i = 0; i < n; i++)
    {
        n64c.raw[i] = Wire.read();
    }
}

void send()
{
    Wire.write(rep.raw, sizeof(rep.raw));
}

void idWiiExt()
{
    delayMicroseconds(43);
    Wire1.beginTransmission(WII_EXT_ADDR);
    Wire1.write(0xfa);
    Wire1.write(0);
    while (Wire1.endTransmission());
    delayMicroseconds(38);
    int n = Wire1.requestFrom(WII_EXT_ADDR, sizeof(wiiExtID.raw));
    for (int i = 0; i < n; i++)
    {
        wiiExtID.raw[i] = Wire1.read();
    }
}

void testFileSD()
{
    if (sd.exists("hi.txt"))
    {
        file = sd.open("hi.txt", O_READ);
        uint8_t buff[15];
        file.read(buff, 15);
        Serial.write(buff, 15);
        file.close();
        sd.remove("hi.txt");
    }
    else
    {
        Serial.println("touch hi.txt");
        file = sd.open("hi.txt", O_WRITE | O_CREAT);
        file.write("YAY!\r\n\\(^u^)/\r\n", 15);
        file.close();
    }
=======
  // Try to read the controller data
  if (N64Controller.read())
  {
    // Print Controller information
    auto status = N64Controller.getStatus();
    auto report = N64Controller.getReport();
    print_n64_report(report, status);
    delay(100);
  }
  else
  {
    // Add debounce if reading failed
    Serial.println(F("Error reading N64 controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);
}

void print_n64_report(N64_Report_t &n64_report, N64_Status_t &n64_status)
{
  // Print device information
  Serial.print(F("Device: "));
  switch (n64_status.device) {
    case NINTENDO_DEVICE_N64_NONE:
      Serial.println(F("No N64 Controller found!"));
      break;
    case NINTENDO_DEVICE_N64_WIRED:
      Serial.println(F("Original Nintendo N64 Controller"));
      break;

    default:
      Serial.print(F("Unknown "));
      Serial.println(n64_status.device, HEX);
      break;
  }

  // Prints the raw data from the controller
  Serial.println();
  Serial.println(F("Printing N64 controller report:"));

  Serial.print(F("A:\t"));
  Serial.println(n64_report.a);

  Serial.print(F("B:\t"));
  Serial.println(n64_report.b);

  Serial.print(F("Z:\t"));
  Serial.println(n64_report.z);

  Serial.print(F("Start:\t"));
  Serial.println(n64_report.start);

  Serial.print(F("Dup:\t"));
  Serial.println(n64_report.dup);
  Serial.print(F("Ddown:\t"));
  Serial.println(n64_report.ddown);
  Serial.print(F("Dleft:\t"));
  Serial.println(n64_report.dleft);
  Serial.print(F("Dright:\t"));
  Serial.println(n64_report.dright);

  Serial.print(F("L:\t"));
  Serial.println(n64_report.l);
  Serial.print(F("R:\t"));
  Serial.println(n64_report.r);

  Serial.print(F("Cup:\t"));
  Serial.println(n64_report.cup);
  Serial.print(F("Cdown:\t"));
  Serial.println(n64_report.cdown);
  Serial.print(F("Cleft:\t"));
  Serial.println(n64_report.cleft);
  Serial.print(F("Cright:\t"));
  Serial.println(n64_report.cright);

  Serial.print(F("xAxis:\t"));
  Serial.println(n64_report.xAxis, DEC);
  Serial.print(F("yAxis:\t"));
  Serial.println(n64_report.yAxis, DEC);

  Serial.println();
>>>>>>> teensy
}
