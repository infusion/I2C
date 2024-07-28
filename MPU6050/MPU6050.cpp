
#include <MPU6050.h>

/**
 * Set up device for general usage with default settings
 *
 */
void MPU6050::init()
{

  caliCounter = 0;
  offGX = 0;
  offGY = 0;
  offGZ = 0;

  I2C::init();

  // PLL with xGyro reference and disabled sleep mode - or 0x00 wakes up MPU?
  I2C::writeByte(devId, MPU6050_PWR_MGMT_1, 0x01);

  // Full scale range = 250°/sec
  MPU6050::setGyrRange(MPU6050_SCALE_250DPS);

  // Full scale range = +/-2g
  MPU6050::setAccRange(MPU6050_RANGE_2G);

  // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  I2C::writeByte(devId, 0x19, 7);

  // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  I2C::writeByte(devId, 0x1a, 0x00);

  /*
   // Use the most accurate clock source
       setClockSource(MPU6050_CLOCK_PLL_XGYRO);

       MPU6050_CONFIG = 0x03 // Disable FSYNC and set 44 Hz Acc filtering, 42 Hz Gyro filtering, 1 KHz sampling

   // most sensitive settings: +/- 2g and 250deg/sec
      setFullScaleGyroRange(MPU6050_GYRO_FS_250);
      setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

   // wake up the device
      setSleepEnabled(false);
   *
   * MPU6050_setDLPFMode(5);
   */

  /*
           setClockSource(MPU6050_CLOCK_PLL);

      // Set the sensitivity to max on gyro and accel
      setFullScaleGyroRange(MPU6050_GYRO_FS_250);
      setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

      // Allow direct I2C access to devices connected to the MPU6050 aux bus
      setI2cBypassEnabled(true);

      // Calculate the scale factors from the configured ranges
      accelScale = getAccelScale(getFullScaleAccelRange());
      gyroScale = getGyroScale(getFullScaleGyroRange());
   */
}

/**
 * Get three axis gyroscope readings at the Sample Rate
 * Each 16-bit measurement has a scale defined in FS_SEL
 *
 * @param x
 * @param y
 * @param z
 */
void MPU6050::getRawGyro(int16_t *x, int16_t *y, int16_t *z)
{

  int16_t *wrt = (int16_t *)data;

  // MSB FIRST!
  I2C::readWords(devId, MPU6050_GYRO_START, (uint16_t *)wrt, 3);
  *x = wrt[0];
  *y = wrt[1];
  *z = wrt[2];
}

/**
 * Get three axis gyroscope readings in °/s
 *
 * @param x
 * @param y
 * @param z
 */
void MPU6050::getRotation(float *_x, float *_y, float *_z)
{

  int16_t x, y, z;

  getRawGyro(&x, &y, &z);
  const double gyroScale = 131; // +-250°/s
  // double gyroScale = 65.5; // +-500°/s
  // double gyroScale = 32.8; // +-1000°/s
  // double gyroScale = 16.4; // +-2000°/s

  *_x = x * 1.0 / gyroScale; // deg/s
  *_y = y * 1.0 / gyroScale; // deg/s
  *_z = z * 1.0 / gyroScale; // deg/s

  // Angular Rate (deg / s) = Sensor Reading (LSB) / (LSB / deg/s)
  // angle deg += reading deg/s * Δt s
}

void MPU6050::getMotion6(float *_ax, float *_ay, float *_az, float *_gx, float *_gy, float *_gz)
{

#define ACCEL_SCALE (1.0 / 16384.0)
#define GYRO_SCALE (1.0 / 131.0) * M_PI / 180.0

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  getRawMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  *_gx = (gx - offGX) * GYRO_SCALE;
  *_gy = (gy - offGY) * GYRO_SCALE;
  *_gz = (gz - offGZ) * GYRO_SCALE;

  *_ax = ax * ACCEL_SCALE;
  *_ay = ay * ACCEL_SCALE;
  *_az = az * ACCEL_SCALE;
}

void MPU6050::calibrate()
{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  getRawMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  caliCounter++;

  float alpha = (caliCounter - 1) / (float)caliCounter;

  offGX = alpha * offGX + (1 - alpha) * gx;
  offGY = alpha * offGY + (1 - alpha) * gy;
  offGZ = alpha * offGZ + (1 - alpha) * gz;
}

/**
 * Get three axis accelerometer readings at the Sample Rate
 * Each 16-bit measurement has a scale defined in ACCEL_FS
 *
 * @param x
 * @param y
 * @param z
 */
void MPU6050::getRawAccel(int16_t *x, int16_t *y, int16_t *z)
{

  int16_t *wrt = (int16_t *)data;

  // MSB FIRST!
  I2C::readWords(devId, MPU6050_ACCEL_START, (uint16_t *)wrt, 3);
  *x = wrt[0];
  *y = wrt[1];
  *z = wrt[2];
}

/**
 * Get three axis accelerometer readings in g
 *
 * @param x
 * @param y
 * @param z
 */
void MPU6050::getAcceleration(float *_x, float *_y, float *_z)
{

  int16_t x, y, z;

  getRawAccel(&x, &y, &z);
  const float accelScale = 16384; // +-2G
  // float accelScale = 8192; // +-4G
  // float accelScale = 4096; // +-8G
  // float accelScale = 2048; // +-16G

  *_x = x * (1.0 / accelScale);
  *_y = y * (1.0 / accelScale);
  *_z = z * (1.0 / accelScale);

  /*
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
  */
}

/**
 * Gets gyro and accel measure at once
 *
 * @param ax
 * @param ay
 * @param az
 * @param gx
 * @param gy
 * @param gz
 */
void MPU6050::getRawMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz)
{

  int16_t *wrt = (int16_t *)data;

  // MSB FIRST!
  I2C::readWords(devId, MPU6050_ACCEL_START, (uint16_t *)wrt, 7);

  *ax = wrt[0];
  *ay = wrt[1];
  *az = wrt[2];

  *gx = wrt[4];
  *gy = wrt[5];
  *gz = wrt[6];
}

void MPU6050::getRawMotion9(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz, int16_t *mx, int16_t *my, int16_t *mz)
{

  int16_t *wrt = (int16_t *)data;

  // MSB FIRST!
  I2C::readWords(devId, MPU6050_ACCEL_START, (uint16_t *)wrt, 11);

  *ax = wrt[0];
  *ay = wrt[1];
  *az = wrt[2];

  *gx = wrt[4];
  *gy = wrt[5];
  *gz = wrt[6];

  *mx = wrt[8];
  *my = wrt[9];
  *mz = wrt[10];
}

/**
 * Get current temperature in Celsius
 *
 * @return
 */
float MPU6050::getTemperature()
{

  int16_t *wrt = (int16_t *)data;

  I2C::readWords(devId, MPU6050_TEMP_START, (uint16_t *)wrt, 1);
  return *wrt * (1.0 / 340.0) + 36.53;
}

/**
 * Check if device is connected and ID register is readable
 *
 * @return
 */
bool MPU6050::isAlive()
{

  if (I2C::readByte(devId, MPU6050_WHO_AM_I, data))
  {
    return *data == MPU6050_DEVICE_ID;
  }
  return false;
}

void MPU6050::setAccRange(mpu6050_range_t range)
{
  I2C::writeByte(devId, MPU6050_ACCEL_CONFIG, range << 3);
}

void MPU6050::setGyrRange(mpu6050_dps_t range)
{
  I2C::writeByte(devId, MPU6050_GYRO_CONFIG, range << 3);
}