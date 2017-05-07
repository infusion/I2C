
#include <MCP23008.h>

void MCP23008::init() {

#ifdef RASPBERRY
    devId = i2cOpen(1, devId, 0);
#endif

    I2C::writeByte(devId, MCP23008_IODIR, 0x00); // set all pins on bank A to OUTPUT
    I2C::writeByte(devId, MCP23008_GPPU, 0x00); // deactivate all pull up resistors on bank A
}

void MCP23008::setPinMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23008_PINS) {
        return;
    }

    if (mode == 1) { // OUTPUT = 1, INPUT = 0
        direction&= ~(1 << pin);
    } else {
        direction|= 1 << pin;
    }
    I2C::writeByte(devId, MCP23008_IODIR, direction);
}

void MCP23008::setPullUpMode(uint8_t pin, uint8_t mode) {

    if (pin >= MCP23008_PINS) {
        return;
    }
    I2C::writeBit(devId, MCP23008_GPPU, pin, mode);
}

void MCP23008::setPin(uint8_t pin, bool value) {

    if (pin >= MCP23008_PINS) {
        return;
    }

    // Conditional Set/Clear bit
    values ^= (-value ^ values) & (1 << pin);

    I2C::writeByte(devId, MCP23008_OLAT, values);
}

void MCP23008::setPins(uint8_t map) {
    values = map;
    I2C::writeByte(devId, MCP23008_OLAT, values);
}

bool MCP23008::getPin(uint8_t pin) {

    if (pin >= MCP23008_PINS) {
        return 0;
    }

    uint8_t ret;
    I2C::readByte(devId, MCP23008_GPIO, &ret);
    return (ret >> pin) & 1;
}
