
#ifndef _MCP23017_H
#define _MCP23017_H

#include <I2C.h>

// Device Address with all address pins low
#define MCP23017_ADDRESS            0x20

// IO Direction Registers
#define MCP23017_IODIRA             0x00
#define MCP23017_IODIRB             0x01

#define MCP23017_IPOLA              0x02
#define MCP23017_IPOLB              0x03
#define MCP23017_GPINTENA           0x04
#define MCP23017_GPINTENB           0x05
#define MCP23017_DEFVALA            0x06
#define MCP23017_DEFVALB            0x07
#define MCP23017_INTCONA            0x08
#define MCP23017_INTCONB            0x09
#define MCP23017_IOCONA             0x0A
#define MCP23017_IOCONB             0x0B

// Pull up Registers - if bit is set, pin is pulled up with 100kOhm
#define MCP23017_GPPUA              0x0C
#define MCP23017_GPPUB              0x0D

#define MCP23017_INTFA              0x0E
#define MCP23017_INTFB              0x0F
#define MCP23017_INTCAPA            0x10
#define MCP23017_INTCAPB            0x11

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

    void setPin(uint8_t pin, bool value);
    
    void setPins(uint16_t map);

    bool getPin(uint8_t pin);
};

#endif
