#include <Wire.h>

#define I2C_ADDR 8

uint8_t buffer[8];

void setup() {
    Wire.setClock(400000); // fast mode
    Wire.begin(I2C_ADDR); // slave

    Wire.onReceive(get);
    Wire.onRequest(send);
}

void loop() {

}

void get() {
    
}

void send() {

}
