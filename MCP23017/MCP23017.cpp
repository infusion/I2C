
#include <MCP23017.h>

/**
 * Initializes the module
 */
void MCP23017::init() {

  I2C::init();

  // Attention: In the following, writeWord is used, even if it has wrong byte order!
  // It's okay, since we set all bits to the same value
  I2C::writeWord(devId, MCP23017_IODIRA, 0x0000); // set all pins on bank A/B to OUTPUT
  I2C::writeWord(devId, MCP23017_GPPUA, 0x0000); // deactivate all pull up resistors on bank A/B
}

/**
 * Sets the mode of a pin to either input or output (OUTPUT = 1, INPUT = 0)
 *
 * @param pin
 * @param mode
 */
void MCP23017::setPinMode(uint8_t pin, uint8_t mode) {

  if (pin >= MCP23017_PINS) {
    return;
  }

  SET_BIT_IN_BYTE_MUTABLE(direction, pin, !mode);

  I2C::writeByte(devId, pin < 8 ? MCP23017_IODIRA : MCP23017_IODIRB, ((uint8_t *) & direction)[pin < 8 ? 0 : 1]);
}

/**
 * Enable or disable pull up resistors (ENABLE = true, DISABLE = false)
 *
 * @param pin
 * @param mode
 */
void MCP23017::setPullUpMode(uint8_t pin, bool mode) {

  if (pin >= MCP23017_PINS) {
    return;
  }
  I2C::writeBit(devId, pin < 8 ? MCP23017_GPPUA : MCP23017_GPPUB, pin % 8, mode);
}

/**
 * Set a pin high or low
 *
 * @param pin
 * @param value
 */
void MCP23017::setPin(uint8_t pin, bool value) {

  if (pin >= MCP23017_PINS) {
    return;
  }

  SET_BIT_IN_BYTE_MUTABLE(values, pin, value);

  I2C::writeByte(devId, pin < 8 ? MCP23017_OLATA : MCP23017_OLATA, ((uint8_t *) & values)[pin < 8 ? 0 : 1]);
}

/**
 * Set all pins in one rush using an 16 bit map
 *
 * @param map
 */
void MCP23017::setPins(uint16_t map) {
  values = map;
  I2C::writeBytes(devId, MCP23017_OLATA, (uint8_t *) & map, 2);
}

/**
 * Get the value of an input pin
 *
 * @param pin
 * @return
 */
bool MCP23017::isPinHigh(uint8_t pin) {

  if (pin >= MCP23017_PINS) {
    return 0;
  }

  uint8_t ret;
  I2C::readBit(devId, pin < 8 ? MCP23017_GPIOA : MCP23017_GPIOB, pin % 8, &ret);
  return (bool) ret;
}
