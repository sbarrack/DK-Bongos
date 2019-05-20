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
    for (int i = 0; Wire.available() && i < sizeof(buffer); i++) {
        buffer[i] = Wire.read();
    }
}

void send() {
    Wire.write(buffer, sizeof(buffer));
}
