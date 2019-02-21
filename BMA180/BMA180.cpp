
#include <BMA180.h>

/**
 * Set up accelerometer for general usage with default settings
 *
 */
void BMA180::init() {

  devId = I2C::init(devId);

  // Do a soft reset
  softReset();

  // Enable ee_w
  setWriteEnabled(true);

  // Disable wakeup
  // ...

  // Set filter level (10Hz?)
  // ...

  // Set sensibility to +/- 2G
  setRange(BMA180_RANGE_2G);

  // Set SMP skip mode
  // ...

  // Set interupt
  // ...

  // Finally disable ee_w
  setWriteEnabled(true);
}

void BMA180::setWriteEnabled(bool enable) {

  // Set x in 111x1111

  /* 7.10.3 ee_w This bit must be 1 to be able to write anything into image registers
     from 20h to 3Bh. Resetting ee_w to 0 will prevent any register updates.
     Default is =0 on reset */
  I2C::writeBit(devId, BMA180_CTRL_REG0, 4, enable);
}

void BMA180::setRange(bma180_range_t rng) {

  // Requires ee_w = 1
  // Set xxx in 1111xxx1

  /*
   * Value | Full scale range (+/-g)
   * ------+-----------------
   * 0     | 1
   * 1     | 1.5
   * 2     | 2
   * 3     | 3
   * 4     | 4
   * 5     | 8
   * 6     | 16
   * 7     | Not used
   */

  if (rng != range) {
    range = rng;
    I2C::readByte(devId, BMA180_OFFSET_LSB1, data);
    I2C::writeByte(devId, BMA180_OFFSET_LSB1, SET_BITS_IN_BYTE(data[0], 1, 3, rng));
  }
}

void BMA180::softReset() {
  I2C::writeByte(devId, BMA180_RESET, 0xB6);
  I2C::sleep(1); // No serial communication for 10us after soft_reset
}

void BMA180::getRawMeasure(int16_t *x, int16_t *y, int16_t *z) {

  int16_t *wrt = (int16_t *) data;

  I2C::readBytes(devId, BMA180_ACC_X_LSB, data, 6);

  // Fix silly format from Bosch:
  // MSB        LSB
  // data[1]    data[0]
  // ZYYYYYYY   XXXXXX**
  // =>
  // ZZZYYYYY   YYXXXXXX

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  *x = wrt[0] >> 2;
  *y = wrt[1] >> 2;
  *z = wrt[2] >> 2;
#else
#error "getRawMeasure is not implemented for big endian"
#endif
}

float BMA180::getGSense() {

  switch (range) {
    case BMA180_RANGE_1G: return 1.0;
    case BMA180_RANGE_1_5G: return 1.5;
    case BMA180_RANGE_2G: return 2.0;
    case BMA180_RANGE_3G: return 3.0;
    case BMA180_RANGE_4G: return 4.0;
    case BMA180_RANGE_8G: return 8.0;
    case BMA180_RANGE_16G: return 16.0;
  }
  return 0.0;
}

void BMA180::getAcceleration(float *x, float *y, float *z) {

  int16_t _x, _y, _z;
  float GSense = getGSense();

  getRawMeasure(&_x, &_y, &_z);

  *x = (_x - 0) * (1.0 / 8192.0) * GSense;
  *y = (_y - 0) * (1.0 / 8192.0) * GSense;
  *z = (_z - 0) * (1.0 / 8192.0) * GSense;
}

void BMA180::calibrate(uint16_t samples, uint16_t delayMS) {

  // ex: 200 samples + 10ms delay = 2sec calibration time

  int16_t x, y, z;
  int16_t minX = 32767, minY = 32767, minZ = 32767;
  int16_t maxX = -32768, maxY = -32768, maxZ = -32768;

  getRawMeasure(&x, &y, &z);
  for (uint16_t i = 0; i < samples; i++) {
    delay(delayMS);
    getRawMeasure(&x, &y, &z);

    minX = x < minX ? x : minX;
    minY = y < minY ? x : minY;
    minZ = z < minZ ? x : minZ;

    maxX = x > maxX ? x : maxX;
    maxY = y > maxY ? x : maxY;
    maxZ = z > maxZ ? x : maxZ;
  }
  offsetX = (minX + maxX) / 2;
  offsetY = (minY + maxY) / 2;
  offsetZ = (minZ + maxZ) / 2;

  scaleXinv = maxX - offsetX;
  scaleYinv = maxY - offsetY;
  scaleZinv = maxZ - offsetZ;
}

bool BMA180::isAlive() {

  // Check device ID for being 03h
  if (I2C::readByte(devId, BMA180_DEVID, data)) {
    return *data == 0x03;
  }
  return false;
}
