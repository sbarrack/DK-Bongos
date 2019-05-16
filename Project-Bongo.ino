#include <SPI.h>
#include <SD.h>

void setup()
{
    Serial.begin(9600);
    while (!Serial);

    Serial.print("Loading card... ");
    if (!SD.begin()) {
        Serial.println("FAILED");
        return;
    }
    Serial.println("DONE");

    Serial.print("Opening hi.txt... ");
    File hi = SD.open("hi.txt");
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
