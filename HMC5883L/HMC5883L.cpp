
#include <HMC5883L.h>

/**
 * Set up compass for general usage with default settings
 * 
 */
void HMC5883L::init() {

  devId = I2C::init(devId);

  I2C::writeByte(devId, HMC5883L_CONFIG_A,
          // Set number of samples averaged per measurement to 8
          (HMC5883L_AVERAGING_8 << 5) |
          // Set rate to 30 Hz
          (HMC5883L_DATARATE_30HZ << 2) |
          // Set normal measure bias
          (HMC5883L_BIAS_NORMAL << 0));

  // Set gain to a known level of 1.3 Ga
  setGain(HMC5883L_GAIN_1090);

  // Set single mode (which has very low power requirements)
  setMeasureMode(HMC5883L_MODE_SINGLE);
}

// Config A Register

/**
 * Set the number of samples that get averaged
 * 
 * @param samples
 */
void HMC5883L::setSampleAveraging(hmc5883l_averaging_t samples) {

  // Set xx in 0xx11111

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);
  I2C::writeByte(devId, HMC5883L_CONFIG_A, SET_BITS_IN_BYTE(*data, 5, 6, samples));
}

/**
 * Get the current number of samples averaged
 * 
 * @return 
 */
hmc5883l_averaging_t HMC5883L::getSampleAveraging() {

  // Get xx in 0xx11111

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);

  return (hmc5883l_averaging_t) GET_BITS_FROM_BYTE(*data, 5, 6);
}

/**
 * Sets data rate for continues mode
 * 
 * @param dataRate
 */
void HMC5883L::setDataRate(hmc5883l_datarate_t rate) {

  // Set xxx in 011xxx11

  /*
   * Value | Output Rate (Hz)
   * ------+-----------------
   * 0     | 0.75
   * 1     | 1.5
   * 2     | 3
   * 3     | 7.5
   * 4     | 15 (Default)
   * 5     | 30
   * 6     | 75
   * 7     | Not used
   */

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);
  I2C::writeByte(devId, HMC5883L_CONFIG_A, SET_BITS_IN_BYTE(*data, 2, 4, rate));
}

/**
 * Get the current data rate
 * 
 * @return 
 */
hmc5883l_datarate_t HMC5883L::getDataRate() {

  // Get xxx in 011xxx11

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);

  return (hmc5883l_datarate_t) GET_BITS_FROM_BYTE(*data, 2, 4);
}

/**
 * Set the measurement bias
 * 
 * @param bias
 */
void HMC5883L::setMeasureBias(hmc5883l_bias_t bias) {

  // Set xx in 011111xx

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);
  I2C::writeByte(devId, HMC5883L_CONFIG_A, SET_BITS_IN_BYTE(*data, 0, 1, bias));
}

/**
 * Get current measurement bias
 * 
 * @return 
 */
hmc5883l_bias_t HMC5883L::getMeasureBias() {

  // Get xx in 011111xx

  I2C::readByte(devId, HMC5883L_CONFIG_A, data);

  return (hmc5883l_bias_t) GET_BITS_FROM_BYTE(*data, 0, 1);
}


// Config B Register

/**
 * Set magnetic field gain value
 * 
 * @param gain
 */
void HMC5883L::setGain(hmc5883l_gain_t gain) {

  // Set xxx in xxx00000 (gain is already aligned)

  I2C::writeByte(devId, HMC5883L_CONFIG_B, gain);

  switch (gain) {
    case HMC5883L_GAIN_1370:
      _scaleXY = 1.0 / 1370.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 1250.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_1090:
      _scaleXY = 1.0 / 1090.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 980.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_820:
      _scaleXY = 1.0 / 820.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 700.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_660:
      _scaleXY = 1.0 / 660.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 600.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_440:
      _scaleXY = 1.0 / 440.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 400.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_390:
      _scaleXY = 1.0 / 390.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 250.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_330:
      _scaleXY = 1.0 / 330.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 300.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
    case HMC5883L_GAIN_230:
      _scaleXY = 1.0 / 230.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      _scaleZ = 1.0 / 200.0 * HMC5883L_GAUSS_TO_MICROTESLA;
      break;
  }
}

/**
 * Get current gain setting
 * 
 * @return 
 */
hmc5883l_gain_t HMC5883L::getGain() {

  // Get xxx in xxx00000

  I2C::readByte(devId, HMC5883L_CONFIG_B, data);

  return (hmc5883l_gain_t) *data;
}


// MODE Register

/**
 * Sets the current meassurement mode
 * 
 * @param newMode
 */
void HMC5883L::setMeasureMode(hmc5883l_mode_t newMode) {

  // Set xx in 000000xx

  mode = (uint8_t) newMode;

  I2C::writeByte(devId, HMC5883L_MODE, mode);

  if (newMode == HMC5883L_MODE_CONTINUOUS) {
    I2C::sleep(6);
  }
}

/**
 * Gets the current meassurement mode
 * 
 * @return 
 */
hmc5883l_mode_t HMC5883L::getMeasureMode() {

  // Get xx in 000000xx

  I2C::readByte(devId, HMC5883L_MODE, data);

  return (hmc5883l_mode_t) GET_BITS_FROM_BYTE(*data, 0, 3);
}


// DATA Register

/**
 * Get the three axis heading measurement
 * 
 * @param x
 * @param y
 * @param z
 */
void HMC5883L::getRawMeasure(int16_t *x, int16_t *y, int16_t *z) {

  // Every read requires to read all registers
  // That's why having separate x,y,z methods doesn't make much sense

  if (mode == HMC5883L_MODE_SINGLE) {
    I2C::writeByte(devId, HMC5883L_MODE, HMC5883L_MODE_SINGLE);
    I2C::sleep(6);
  }

  uint16_t *wrt = (uint16_t *) data;

  // MSB FIRST!
  I2C::readWords(devId, HMC5883L_DATA_START, wrt, 3);
  *x = wrt[0];
  *y = wrt[2];
  *z = wrt[1];
}

/**
 * Gets normalized measure in microtesla (μT)
 * 
 * @param x
 * @param y
 * @param z
 */
void HMC5883L::getMagneticField(float *x, float *y, float *z) {

  int16_t _x, _y, _z;

  getRawMeasure(&_x, &_y, &_z);

  *x = _x * _scaleXY;
  *y = _y * _scaleXY;
  *z = _z * _scaleZ;
}

/**
 * Gets the azimuth in degree
 * 
 * @return 
 */
float HMC5883L::getAzimuth() {
  return 0;
}

// Status Register    

/**
 * Determines if the data registers are locked
 * 
 * @return 
 */
bool HMC5883L::isStatusLock() {
  I2C::readBit(devId, HMC5883L_STATUS, HMC5883L_STATUS_LOCK_BIT, data);
  return *data;
}

/**
 * Determines if data in registers is ready to be read
 * @return 
 */
bool HMC5883L::isStatusReady() {
  I2C::readBit(devId, HMC5883L_STATUS, HMC5883L_STATUS_READY_BIT, data);
  return *data;
}

// Ident Register

/**
 * Check if device is connected and ID register is readable
 * Both HMC5843 and HMC5883L have "H43" the same value
 * 
 * @return 
 */
bool HMC5883L::isAlive() {

  if (I2C::readBytes(devId, HMC5883L_IDENT_A, data, 3)) {
    return data[0] == 0x48 && data[1] == 0x34 && data[2] == 0x33;
  }
  return false;
}
