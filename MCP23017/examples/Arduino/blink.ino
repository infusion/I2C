
#include <MCP23017.h>

MCP23017 io;

void setup() {

    io.init();

    io.setPinMode(6, OUTPUT);
    io.setPinMode(7, OUTPUT);
}

int x = HIGH;

void loop() {

    io.setPin(7, x);
    delay(100);
    io.setPin(6, 1 - x);

    x = 1 - x;
    delay(100);
}
