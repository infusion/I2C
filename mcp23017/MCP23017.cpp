
#include "MCP23017.h"

void MCP23017::init() {

#ifdef RASPBERRY
    devId = i2cOpen(1, devId, 0);
#endif

    // Attention: In the following, writeWord is used, even if it has wrong byte order!
    // It's okay, since we set all bits to the same value
    I2C::writeWord(devId, MCP23017_IODIRA, 0x0000); // set all pins on bank A/B to OUTPUT
    I2C::writeWord(devId, MCP23017_GPPUA, 0x0000); // deactivate all pull up resistors on bank A/B
}

void MCP23017::setPinMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23017_PINS) {
        return;
    }

    if (mode == 1) { // OUTPUT = 1, INPUT = 0
        direction&= ~(1 << pin);
    } else {
        direction|= 1 << pin;
    }
    I2C::writeByte(devId, pin < 8 ? MCP23017_IODIRA : MCP23017_IODIRB, ((uint8_t *) &direction)[pin < 8 ? 0 : 1]);
}

void MCP23017::setPullUpMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23017_PINS) {
        return;
    }
    I2C::writeBit(devId, pin < 8 ? MCP23017_GPPUA : MCP23017_GPPUB, pin % 8, mode);
}

void MCP23017::setPin(uint8_t pin, uint8_t value) {

    if (pin >= MCP23017_PINS) {
        return;
    }

    if (value == 0) {
        values&= ~(1 << pin);
    } else {
        values|= 1 << pin;
    }
    I2C::writeByte(devId, pin < 8 ? MCP23017_OLATA : MCP23017_OLATA, ((uint8_t *) &values)[pin < 8 ? 0 : 1]);
}

void MCP23017::setPins(uint16_t map) { // Update all io pins at once
    values = map;
    I2C::writeBytes(devId, MCP23017_OLATA, (uint8_t *) &values, 2);
}

uint8_t MCP23017::getPin(uint8_t pin) {

    if (pin >= MCP23017_PINS) {
        return 0;
    }

    uint8_t tmp;
    I2C::readBytes(devId, pin < 8 ? MCP23017_GPIOA : MCP23017_GPIOB, &tmp, 1);

    // If pin >= 8, pin-= 8
    pin%= 8;

    return (tmp >> pin) & 1;
}
