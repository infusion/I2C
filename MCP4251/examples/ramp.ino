
#include <MCP4251.h>

MCP4251 digipot;

void setup() {

    Serial.begin(9600);

    pinMode(MCP4251_CS_PIN, OUTPUT);
    digitalWrite(MCP4251_CS_PIN, HIGH);

    SPI.begin();
}

uint8_t channel = 0; // Pot 0
uint8_t level = 0;
void loop() {

    for (; ; level++) {
        digipot.setValue(channel, level);
        delay(100);
        Serial.println(level);
        if (level == 255)
            break;
    }
    delay(1000);

    for (; ; level--) {
        digipot.setValue(channel, level);
        delay(100);
        Serial.println(level);
        if (level == 0)
            break;
    }
    delay(1000);
}
