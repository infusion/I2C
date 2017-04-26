
#include "MCP23008.h"

void MCP23008::init() {

#ifdef RASPBERRY
    devId = i2cOpen(1, devId, 0);
#endif

    I2C::writeByte(devId, MCP23008_IODIR, 0x00); // set all pins on bank A to INPUT
    I2C::writeByte(devId, MCP23008_GPPU, 0x00); // deactivate all pull up resistors on bank A

    I2C::writeByte(devId, MCP23008_IOCON, MCP23008_IOCON_DEFAULT); // Init IOCON
}

void MCP23008::setPinMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23008_PINS) {
        return;
    }

    if (mode == 0) {
        direction&= ~(1 << pin);
    } else {
        direction|= 1 << pin;
    }
    I2C::writeBytes(devId, MCP23008_IODIR, &direction, 1);
}

void MCP23008::setPullUpMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23008_PINS) {
        return;
    }

    uint8_t old;
    I2C::readBytes(devId, MCP23008_GPPU, &old, 1);

    if (mode == 0) {
        old&= ~(1 << pin);
    } else {
        old|= 1 << pin;
    }
    I2C::writeBytes(devId, MCP23008_GPPU, &old, 1);

}

void MCP23008::setPin(uint8_t pin, uint8_t value) {

    if (pin >= MCP23008_PINS) {
        return;
    }

    if (value == 0) {
        values&= ~(1 << pin);
    } else {
        values|= 1 << pin;
    }
    I2C::writeBytes(devId, MCP23008_GPIO, &values, 1);
}

uint8_t MCP23008::getPin(uint8_t pin) {

    if (pin >= MCP23008_PINS) {
        return 0;
    }

    uint8_t ret;
    I2C::readBytes(devId, MCP23008_GPIO, &ret, 1);
    return (ret >> pin) & 1;
}
