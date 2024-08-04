
#include <MCP4251.h>

MCP4251 digipot;

// If calibrated well, the resistance can be set perfectly well

void setup() {

    pinMode(MCP4251_CS_PIN, OUTPUT);
    digitalWrite(MCP4251_CS_PIN, HIGH);

    SPI.begin();

    delay(100);

    digipot.setResistance(0, 3.2); // Set Pot0 to 3.2 kOhm
}

void loop() {

    delay(1000);
}
