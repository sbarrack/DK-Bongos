#include <SPI.h>
#include <SD.h>

void setup()
{
    File hi;
    Serial.begin(9600);
    while (!Serial);

    Serial.print("Loading card... ");
    if (!SD.begin()) {
        Serial.println("FAILED");
        return;
    }
    Serial.println("DONE");

    Serial.print("Looking for hi.txt... ");
    if (SD.exists("hi.txt")) {
        Serial.print("FOUND\r\nDeleting hi.txt... ");
        if (SD.remove("hi.txt")) {
            Serial.println("DONE");
        } else {
            Serial.println("FAILED");
        }
    } else {
        Serial.print("MISSING\r\nCreating hi.txt... ");
        hi = SD.open("hi.txt", FILE_WRITE);
        hi.println("YAY!");
        hi.close();
        Serial.println("DONE");
    }

    Serial.print("Opening hi.txt... ");
    hi = SD.open("hi.txt");
    if (hi) {
        Serial.println("DONE\r\nPrinting contents...");
        while (hi.available()) {
            Serial.write(hi.read());
        }
        hi.close();
    } else {
        Serial.println("FAILED");
    }

    Serial.println("END");
}

void loop()
{

}
