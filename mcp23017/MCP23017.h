
#ifndef _MCP23017_H_
#define _MCP23017_H_

#include "I2C.h"

// Device Address with all address pins low
#define MCP23017_ADDRESS            0x20

// IO Direction Registers
#define MCP23017_IODIRA             0x00
#define MCP23017_IODIRB             0x01

// Pull up Registers - if bit is set, pin is pulled up with 100kOhm
#define MCP23017_GPPUA              0x0C
#define MCP23017_GPPUB              0x0D

// General purpose IO - actual r/w register for pin
#define MCP23017_GPIOA              0x12
#define MCP23017_GPIOB              0x13

// Output latch Registers
#define MCP23017_OLATA              0x14
#define MCP23017_OLATB              0x15

// Number of I/O pins
#define MCP23017_PINS               16

class MCP23017 {
private:

    uint8_t devId;
    uint16_t direction = 0;
    uint16_t values = 0;

public:

    MCP23017(uint8_t id) {
        devId = id;
    }

    MCP23017() {
        devId = MCP23017_ADDRESS;
    }

    void init();

    void setPinMode(uint8_t pin, uint8_t mode);

    void setPullUpMode(uint8_t pin, uint8_t mode);

    void setPin(uint8_t pin, uint8_t value);

    uint8_t getPin(uint8_t pin);
};

#endif
