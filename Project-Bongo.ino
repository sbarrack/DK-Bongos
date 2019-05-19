
uint8_t buffer[8];

void setup() {
    Serial1.begin(115200, SERIAL_8N1);
    // Serial1.setTimeout(1000);
    while (!Serial1);
}

void loop() {
    if (Serial1.available() >= (int)sizeof(buffer)) {
        Serial1.readBytes(buffer, sizeof(buffer));
    }
    if (Serial1.availableForWrite() >= (int)sizeof(buffer)) {
        Serial1.write(buffer, sizeof(buffer));
        Serial1.flush();
    }
}
