
#include <HMC5843.h>

/**
 * Set up compass for general usage with default settings
 *
 */
void HMC5843::init() {

  I2C::init();

  // The HMC5843 needs 5ms before it will be initialized
  I2C::sleep(5);

  I2C::writeByte(devId, HMC5843_CONFIG_A,
          // Set rate to 50 Hz
          (HMC5843_DATARATE_50HZ << 2) |
          // Set normal measure bias
          (HMC5843_BIAS_NORMAL << 0));

  // Set gain to a known level of 1.0 Ga
  setGain(HMC5843_GAIN_1300);

  // Set single mode (which has very low power requirements)
  setMeasureMode(HMC5843_MODE_SINGLE);
}

// Config A Register

/**
 * Sets data rate for continues mode
 *
 * @param dataRate
 */
void HMC5843::setDataRate(hmc5883_datarate_t rate) {

  // Set xxx in 011xxx11

  /*
   * Value | Output Rate (Hz)
   * ------+-----------------
   * 0     | 0.5
   * 1     | 1
   * 2     | 2
   * 3     | 5
   * 4     | 10 (Default)
   * 5     | 20
   * 6     | 50
   * 7     | Not used
   */

  I2C::readByte(devId, HMC5843_CONFIG_A, data);
  I2C::writeByte(devId, HMC5843_CONFIG_A, SET_BITS_IN_BYTE(*data, 2, 4, rate));
}

/**
 * Get the current data rate
 *
 * @return
 */
hmc5883_datarate_t HMC5843::getDataRate() {

  // Get xxx in 011xxx11

  I2C::readByte(devId, HMC5843_CONFIG_A, data);

  return (hmc5883_datarate_t) GET_BITS_FROM_BYTE(*data, 2, 4);
}

/**
 * Set the measurement bias
 *
 * @param bias
 */
void HMC5843::setMeasureBias(hmc5883_bias_t bias) {

  // Set xx in 011111xx

  I2C::readByte(devId, HMC5843_CONFIG_A, data);
  I2C::writeByte(devId, HMC5843_CONFIG_A, SET_BITS_IN_BYTE(*data, 0, 1, bias));
}

/**
 * Get current measurement bias
 *
 * @return
 */
hmc5883_bias_t HMC5843::getMeasureBias() {

  // Get xx in 011111xx

  I2C::readByte(devId, HMC5843_CONFIG_A, data);

  return (hmc5883_bias_t) GET_BITS_FROM_BYTE(*data, 0, 1);
}


// Config B Register

/**
 * Set magnetic field gain value
 *
 * @param gain
 */
void HMC5843::setGain(hmc5883_gain_t gain) {

  // Set xxx in xxx00000 (gain is already aligned)

  I2C::writeByte(devId, HMC5843_CONFIG_B, gain);

  switch (gain) {
    case HMC5843_GAIN_1370:
      _scaleXY = 1.0 / 1370.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 1250.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_1090:
      _scaleXY = 1.0 / 1090.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 980.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_820:
      _scaleXY = 1.0 / 820.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 700.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_660:
      _scaleXY = 1.0 / 660.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 600.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_440:
      _scaleXY = 1.0 / 440.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 400.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_390:
      _scaleXY = 1.0 / 390.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 250.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_330:
      _scaleXY = 1.0 / 330.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 300.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
    case HMC5843_GAIN_230:
      _scaleXY = 1.0 / 230.0 * HMC5843_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 200.0 * HMC5843_GAUSS_TO_MICROTESLA;
      break;
  }
}

/**
 * Get current gain setting
 *
 * @return
 */
hmc5883_gain_t HMC5843::getGain() {

  // Get xxx in xxx00000

  I2C::readByte(devId, HMC5843_CONFIG_B, data);

  return (hmc5883_gain_t) *data;
}


// MODE Register

/**
 * Sets the current meassurement mode
 *
 * @param newMode
 */
void HMC5843::setMeasureMode(hmc5883_mode_t newMode) {

  // Set xx in 000000xx

  mode = (uint8_t) newMode;

  I2C::writeByte(devId, HMC5843_MODE, mode);

  if (newMode == HMC5843_MODE_CONTINUOUS) {
    I2C::sleep(6);
  }
}

/**
 * Gets the current meassurement mode
 *
 * @return
 */
hmc5883_mode_t HMC5843::getMeasureMode() {

  // Get xx in 000000xx

  I2C::readByte(devId, HMC5843_MODE, data);

  return (hmc5883_mode_t) GET_BITS_FROM_BYTE(*data, 0, 3);
}


// DATA Register

/**
 * Get the three axis heading measurement
 *
 * @param x
 * @param y
 * @param z
 */
void HMC5843::getRawMeasure(int16_t *x, int16_t *y, int16_t *z) {

  // Every read requires to read all registers
  // That's why having separate x,y,z methods doesn't make much sense

  uint16_t *wrt = (uint16_t *) data;

  // MSB FIRST!
  I2C::readWords(devId, HMC5843_DATA_START, wrt, 3);
  *x = wrt[0];
  *y = wrt[1];
  *z = wrt[2];

  // After a read in single mode, the device turns to sleep mode
  if (mode == HMC5843_MODE_SINGLE) {
    I2C::writeByte(devId, HMC5843_MODE, HMC5843_MODE_SINGLE);
    I2C::sleep(6);
  }
}

/**
 * Gets normalized measure in microtesla (μT)
 *
 * @param x
 * @param y
 * @param z
 */
void HMC5843::getMagneticField(float *x, float *y, float *z) {

  int16_t _x, _y, _z;

  getRawMeasure(&_x, &_y, &_z);

  // Default scale = 0.92mG/digit -> scale = 0.00092 => x * scale in Gauss
  // Earth magetic field ranges from 0.25 to 0.65 Gauss
  *x = _x * _scaleXY;
  *y = _y * _scaleXY;
  *z = _z * _scaleZ;
}

/**
 * Gets the azimuth in degree
 *
 * @return
 */
float HMC5843::getAzimuth() {
  return 0;
}

// Status Register

/**
 * Determines if the data registers are locked
 *
 * @return
 */
bool HMC5843::isStatusLock() {
  I2C::readBit(devId, HMC5843_STATUS, HMC5843_STATUS_LOCK_BIT, data);
  return *data;
}

/**
 * Determines if data in registers is ready to be read
 * @return
 */
bool HMC5843::isStatusReady() {
  I2C::readBit(devId, HMC5843_STATUS, HMC5843_STATUS_READY_BIT, data);
  return *data;
}

/**
 * Determines if the internal voltage regulator is enabled
 * @return
 */
bool HMC5843::isRegulatorEnabled() {
  I2C::readBit(devId, HMC5843_STATUS, HMC5843_STATUS_REN_BIT, data);
  return *data;
}


// Ident Register

/**
 * Check if device is connected and ID register is readable
 * Both HMC5843 and HMC5843 have "H43" the same value
 *
 * @return
 */
bool HMC5843::isAlive() {

  if (I2C::readBytes(devId, HMC5843_IDENT_A, data, 3)) {
    return data[0] == 0x48 && data[1] == 0x34 && data[2] == 0x33;
  }
  return false;
}
