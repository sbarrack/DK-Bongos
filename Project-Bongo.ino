
uint8_t buffer[8];

void setup() {
    Serial1.begin(115200);
}

void loop() {
    if (Serial1.available() >= (int)sizeof(buffer)) {
        Serial1.readBytes(buffer, sizeof(buffer));
        buffer[0] = (buffer[0] & 0xfe) | ((~buffer[0]) & 1); // ~a
        Serial1.write(buffer, sizeof(buffer));
    }
}
