
#include <MCP4251.h>

MCP4251 digipot;

void setup() {

    pinMode(MCP4251_CS_PIN, OUTPUT);
    digitalWrite(MCP4251_CS_PIN, HIGH);

    SPI.begin();

    delay(100);

    digipot.setValue(0, 144); // Set pot 0 to 144/511
    digipot.setValue(1, 0);   // Set pot 1 to 0/511
}

void loop() {

    delay(1000);
}
