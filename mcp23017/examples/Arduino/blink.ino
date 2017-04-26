#include "MCP23017.h"

MCP23017 io;

void setup() {

    Wire.begin();

    io.init();

    io.setPinMode(6, 0);
    io.setPinMode(7, 0);
}

int x = 1;

void loop() {

    io.setPin(7, x);
    delay(100);
    io.setPin(6, 1 - x);

    x = 1 - x;
    delay(100);
}
