#include <SD.h>
#include <GamecubeAPI.h>

#define TEST_FILE "test.tas"

void setup()
{
    Serial.begin(115200);
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
Gamecube_Report_t buffer[2], pressed, held;

void updateBuffers() {
    gcc.read();
    buffer[1] = buffer[0];
    buffer[0] = pressed = held = gcc.getReport();
    pressed.raw16[0] &= buffer[0].raw16[0] ^ buffer[1].raw16[0];
    held.raw16[0] = (buffer[0].raw16[0] & buffer[1].raw16[0]);
    gc.write(buffer[0]);
}

void doNormal() {
    while (mode == normal) {
        updateBuffers();

        if (pressed.dleft) {
            mode = record;
        } else if (pressed.dright) {
            mode = playback;
        }
    }
}

File tas;

void doRecord() {
    // TODO unique filenames
    if (SD.exists(TEST_FILE)) {
        // Serial.print("Removing old file... ");
        if (!SD.remove(TEST_FILE)) {
            // Serial.println("FAILED");
            mode = normal;
            return;
        }
        // Serial.println("DONE");
    }

    // Serial.print("Recording... ");
    tas = SD.open(TEST_FILE, FILE_WRITE); // TODO generate new name and save it
    if (!tas) {
        // Serial.println("FAILED");
        mode = normal;
        return;
    }
    
    while (mode == record) {
        updateBuffers();

        tas.write(buffer[0].raw8, 8);

        if (pressed.dleft) {
            mode = normal;
        }
    }

    // Serial.println("DONE");
    tas.close();
}

void doPlayback() {
    // Serial.print("Replaying... ");
    tas = SD.open(TEST_FILE); // TODO get last TAS file
    mode = normal; // either way
    if (!tas) {
        // Serial.println("FAILED");
        return;
    }

    while (tas.available()) {
        tas.readBytes(held.raw8, 8);
        gc.write(held);
    }

    // Serial.println("DONE");
    tas.close();
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
        return true;
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
            // No SDXC cards (>32GB)
            Serial.println("INVALID");
            return true;
    }

    Serial.print("Checking partition... ");
    if (!part.init(card)) {
        // Format to FAT16 or FAT32
        Serial.print("INVALID");
        return true;
    }
    Serial.println("GOOD");

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
        return true;
    }
    Serial.println("DONE");

    return false;
}
