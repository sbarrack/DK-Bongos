// #include <SPI.h>
#include <SD.h>
#include <GamecubeAPI.h>

// gcc.read();
// buffer[1] = released = buffer[0];
// buffer[0] = pressed = held = gcc.getReport();
// gc.write(buffer[0]);
// uint16_t tmp = buffer[0].raw16[0] ^ buffer[1].raw16[0];
// pressed.raw16[0] &= tmp;
// held.raw16[0] = (buffer[0].raw16[0] & buffer[1].raw16[0]);
// released.raw16[0] &= tmp;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    while (!Serial);

    while (initSD());

}

unsigned long timeNow, timeMark, timeMarkOld;
enum Mode { normal, record, playback };
Mode mode = normal;

void loop()
{
    if (mode == playback) {
        doPlayback();
    } else if (mode == record) {
        doRecord();
    } else {
        doNormal();
    }
}

CGamecubeController gcc(7);
CGamecubeConsole gc(8);
Gamecube_Report_t buffer[2], pressed, held, released;

void doNormal() {
    while (mode == normal) {
        gcc.read();
        buffer[1] = buffer[0];
        buffer[0] = pressed = gcc.getReport();
        gc.write(buffer[0]);
        pressed.raw16[0] &= buffer[0].raw16[0] ^ buffer[1].raw16[0];

        if (pressed.ddown) {
            mode = record;
        } else if (false) { // TODO
            mode = playback;
        }
    }
}

union TASHeader {
    uint8_t raw[256];
    struct {
        uint32_t signature = "DTM\x1a";
        uint8_t gameID[6] = "GALE01";
        uint8_t isWiiGame; // bool

        uint8_t gcc1 : 1;
        uint8_t gcc2 : 1;
        uint8_t gcc3 : 1;
        uint8_t gcc4 : 1;
        uint8_t wiimote1 : 1;
        uint8_t wiimote2 : 1;
        uint8_t wiimote3 : 1;
        uint8_t wiimote4 : 1;

        uint8_t fromSaveState; // bool
        uint64_t VIcount; // int
        uint64_t inputCount; // int
        uint64_t lagCounter; // int
        uint64_t; // reserved
        uint32_t rerecordCount; // int
        uint8_t author[32] = "Stephen \"Bongo Boy\" Barrack";
        uint8_t videoBackend[16]; // string
        uint8_t audioEmulator[16]; // int
        uint8_t checksum[16]; // raw, MD5 of the game
        uint64_t startTimeUNIX; // int
        uint8_t validConfig; // bool
        uint8_t idleSkipping; // bool
        uint8_t dualCore; // bool
        uint8_t progressiveScan; // bool
        uint8_t HLEenabled; // bool
        uint8_t fastDisc; // bool
        uint8_t CPUcore; // int: 0 interpret, 1 JIT, 2 JITIL
        uint8_t EFBaccess; // bool
        uint8_t EFBCopy; // bool
        uint8_t EFBtoTexture; // bool
        uint8_t EFBCache; // bool
        uint8_t formatChanges; // bool
        uint8_t useXFB; // bool
        uint8_t realXFB; // bool
        
        uint8_t memCardA : 1;
        uint8_t memCardB : 1;
        uint8_t : 6;

        uint8_t memCardBlank; // bool

        uint8_t bongo1 : 1;
        uint8_t bongo2 : 1;
        uint8_t bongo3 : 1;
        uint8_t bongo4 : 1;
        uint8_t : 4;

        uint8_t syncGPU; // bool
        uint8_t netplay; // bool
        uint8_t PAL60; // bool
        uint8_t[12]; // reserved
        uint8_t discTwoISO[40]; // string
        uint8_t hash[20]; // raw, SHA-1 of git rev
        uint32_t IROMhash; // int
        uint32_t COEFhash; // int
        uint64_t tickCount; // int, 486MHz GC, 729MHz Wii
        uint8_t[11]; // reserved
    };
} header;

File tas;

union TASReport {
    uint8_t raw8[8];
    uint16_t raw16[0];
    uint32_t raw32[0];

    struct {
        uint8_t start : 1;
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t z : 1;
        uint8_t du : 1;
        uint8_t dd : 1;

        uint8_t dl : 1;
        uint8_t dr : 1;
        uint8_t l : 1;
        uint8_t r : 1;
        uint8_t disc : 1;
        uint8_t reset : 1;
        uint8_t connected : 1;
        uint8_t : 1;

        uint8_t lt, rt, sx, sy, cx, cy;
    };
};

uint8_t * toTAS(Gamecube_Report_t rep) {
    union TASReport tmp;
    
    tmp.start = rep.start;
    tmp.a = rep.a;
    tmp.b = rep.b;
    tmp.x = rep.x;
    tmp.y = rep.y;
    tmp.z = rep.z;
    tmp.du = rep.dup;
    tmp.dd = rep.ddown;

    tmp.dl = rep.dleft;
    tmp.dr = rep.dright;
    tmp.l = rep.l;
    tmp.r = rep.r;
    tmp.connected = 1;
    
    tmp.raw16[1] = rep.raw16[3]; // lt, rt
    tmp.raw16[2] = rep.raw16[1]; // sx, sy
    tmp.raw16[3] = rep.raw16[2]; // cx, cy

    return tmp.raw8;
}

void doRecord() {
    Serial.print("Recording... ");
    tas = SD.open("thing.dtm"/* TODO generate name */, FILE_WRITE);
    if (!tas) {
        Serial.println("FAILED");
        mode = normal;
        return;
    }
    // init header before the whole shabang
    tas.write(header.raw, 256);

    while (mode == record) {
        gcc.read();
        buffer[1] = buffer[0];
        buffer[0] = pressed = gcc.getReport();
        gc.write(buffer[0]);

        tas.write(toTAS(buffer[0]), 8);

        if (pressed.ddown) {
            mode = normal;
        }
    }

    Serial.println("DONE");
    tas.close();
}

void doPlayback() {
    // TODO
}

bool initSD() {
    Sd2Card card;
    SdVolume part;
    SdFile root;

    Serial.print("Checking card... ");
    if (card.init(SPI_HALF_SPEED)) {
        Serial.println("GOOD");
    } else {
        // SD card is not inserted or faulty wiring
        Serial.println("MISSING");
        return false;
    }

    Serial.print("Type: ");
    switch (card.type()) {
        case SD_CARD_TYPE_SD1:
        case SD_CARD_TYPE_SD2:
            Serial.println("SDSC");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            // No SDXC cards
            Serial.println("INVALID");
            return false;
    }

    Serial.print("Checking partition... ");
    if (!part.init(card)) {
        // Format to FAT16 or FAT32
        Serial.print("INVALID");
        return false;
    }

    Serial.print("Volume: FAT");
    Serial.print(part.fatType(), DEC);
    Serial.print(" ");
    Serial.print(part.blocksPerCluster() * part.clusterCount() / 2048);
    Serial.println("MB");

    // List files
    root.openRoot(part);
    root.ls(LS_R | LS_DATE | LS_SIZE);
    
    Serial.print("Loading card... ");
    if (!SD.begin()) {
        Serial.println("FAILED");
        return false;
    }
    Serial.println("DONE");

    return true;
}
