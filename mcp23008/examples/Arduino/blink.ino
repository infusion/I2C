#include "MCP23008.h"

MCP23008 io;

void setup() {

    Wire.begin();

    io.init();

    io.setPinMode(6, OUTPUT);
    io.setPinMode(7, OUTPUT);
}

int x = HIGH;

void loop() {

    io.setPin(7, x);
    delay(100);
    io.setPin(6, 1 - x);
    delay(100);

    x = 1 - x;
}
