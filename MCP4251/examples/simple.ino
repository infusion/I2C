
#include <MCP4251.h>

MCP4251 digipot;

// What pot and what value
uint8_t channel = 0; // Pot 0
uint8_t level = 144;

void setup() {

    pinMode(MCP4251_CS_PIN, OUTPUT);
    digitalWrite(MCP4251_CS_PIN, HIGH);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16); // 1 MHz

    delay(100);

    MCP4251.setValue(channel, level);
}

void loop() {

    delay(1000);
}
