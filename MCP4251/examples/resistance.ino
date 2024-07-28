
#include <MCP4251.h>

MCP4251 digipot;

// If calibrated well, the resistance can be set perfectly well
uint8_t channel = 0; // Pot 0
float resistance = 3.2; // kOhm

void setup() {

    pinMode(MCP4251_CS_PIN, OUTPUT);
    digitalWrite(MCP4251_CS_PIN, HIGH);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16); // 1 MHz

    delay(100);

    MCP4251.setResistance(channel, resistance);
}

void loop() {

    delay(1000);
}
