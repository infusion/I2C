
#ifndef _MCP23008_H
#define _MCP23008_H

#include "I2C.h"

// Device Address with all address pins low
#define MCP23008_ADDRESS            0x20

// IO Direction Register
#define MCP23008_IODIR              0x00
#define MCP23008_IPOL               0x01
#define MCP23008_GPINTEN            0x02
#define MCP23008_DEFVAL             0x03
#define MCP23008_INTCON             0x04
#define MCP23008_IOCON              0x05

// MCP23008 IOCON settings
#define MCP23008_IOCON_UNUSED       0x01
#define MCP23008_IOCON_INTPOL       0x02
#define MCP23008_IOCON_ODR          0x04
#define MCP23008_IOCON_HAEN         0x08
#define MCP23008_IOCON_DISSLW       0x10
#define MCP23008_IOCON_SEQOP        0x20
#define MCP23008_IOCON_MIRROR       0x40
#define MCP23008_IOCON_BANK_MODE    0x80

#define MCP23008_IOCON_DEFAULT      MCP23008_IOCON_SEQOP

// Pull up Register - if bit is set, pin is pulled up with 100kOhm
#define MCP23008_GPPU               0x06

#define MCP23008_INTF               0x07
#define MCP23008_INTCAP             0x08

// General purpose IO - actual r/w register for pin
#define MCP23008_GPIO               0x09

// Output latch Register
#define MCP23008_OLAT               0x0A

// Number of I/O pins
#define MCP23008_PINS               8

class MCP23008 {
private:

    uint8_t devId;
    uint8_t direction = 0;
    uint8_t values = 0;

public:

    MCP23008(uint8_t id) {
        devId = id;
    }

    MCP23008() {
        devId = MCP23008_ADDRESS;
    }

    void init();

    void setPinMode(uint8_t pin, uint8_t mode);

    void setPullUpMode(uint8_t pin, uint8_t mode);

    void setPin(uint8_t pin, uint8_t value);
    
    void setPins(uint8_t map);

    uint8_t getPin(uint8_t pin);
};

#endif
