// #define RENA
#define DOGE
#include "gc.hpp"

GCC gcc(2);

void setup()
{
    while (!Serial);
    Serial.begin(115200);
}

void loop()
{
    gcc.init();
    Serial.print(gcc.initAck.device, HEX);
    Serial.print(" ");
    Serial.println(gcc.originAck.base.buttons, HEX);
    delay(8);
    gcc.poll();
    Serial.println(gcc.pollAck.buttons, HEX);
    delay(8);
    Serial.println();
}
