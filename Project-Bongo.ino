
#define BUFF_SIZE 8

uint8_t buffer[BUFF_SIZE];

void setup() {
    Serial1.begin(115200, SERIAL_8N1);
    // Serial1.setTimeout(1000);
    // while (!Serial1);
}

void loop() {
    if (Serial1.available() >= BUFF_SIZE) {
        Serial1.readBytes(buffer, BUFF_SIZE);
    }
    if (Serial1.availableForWrite() >= BUFF_SIZE) {
        Serial1.write(buffer, BUFF_SIZE);
        Serial1.flush();
    }
}
