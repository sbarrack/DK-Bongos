#define RENA
// #define DOGE
#include "gc.hpp"

Controller gcc(2);

void setup() {
    // while (!Serial);
    Serial.begin(115200);
}

void loop() {
    gcc.init();
    Serial.print(gcc.getDevice(), HEX);
    Serial.print(" ");
    Serial.println(gcc.getOriginalButtons(), HEX);
    delay(8);
    gcc.poll();
    Serial.println(gcc.getButtons(), HEX);
    delay(8);
    Serial.println();
}
