
#include <MCP23008.h>

/**
 * Initializes the module
 */
void MCP23008::init() {

  I2C::init();

  I2C::writeByte(devId, MCP23008_IODIR, 0x00); // set all pins on bank A to OUTPUT
  I2C::writeByte(devId, MCP23008_GPPU, 0x00); // deactivate all pull up resistors on bank A
}

/**
 * Sets the mode of a pin to either input or output (OUTPUT = 1, INPUT = 0)
 *
 * @param pin
 * @param mode
 */
void MCP23008::setPinMode(uint8_t pin, uint8_t mode) {

  if (pin >= MCP23008_PINS) {
    return;
  }

  SET_BIT_IN_BYTE_MUTABLE(direction, pin, !mode);

  I2C::writeByte(devId, MCP23008_IODIR, direction);
}

/**
 * Enable or disable pull up resistors (ENABLE = true, DISABLE = false)
 *
 * @param pin
 * @param mode
 */
void MCP23008::setPullUpMode(uint8_t pin, bool mode) {

  if (pin >= MCP23008_PINS) {
    return;
  }
  I2C::writeBit(devId, MCP23008_GPPU, pin, mode);
}

/**
 * Set a pin high or low
 *
 * @param pin
 * @param value
 */
void MCP23008::setPin(uint8_t pin, bool value) {

  if (pin >= MCP23008_PINS) {
    return;
  }

  SET_BIT_IN_BYTE_MUTABLE(values, pin, value);

  I2C::writeByte(devId, MCP23008_OLAT, values);
}

/**
 * Set all pins in one rush using an 8 bit map
 *
 * @param map
 */
void MCP23008::setPins(uint8_t map) {
  values = map;
  I2C::writeByte(devId, MCP23008_OLAT, map);
}

/**
 * Get the value of an input pin
 *
 * @param pin
 * @return
 */
bool MCP23008::isPinHigh(uint8_t pin) {

  if (pin >= MCP23008_PINS) {
    return 0;
  }

  uint8_t ret;
  I2C::readBit(devId, MCP23008_GPIO, pin, &ret);
  return (bool) ret;
}
