
#include <MPU6050.h>

/**
 * Set up compass for general usage with default settings
 * 
 */
void MPU6050::init() {

#ifdef RASPBERRY
    devId = i2cOpen(1, devId, 0);
#endif

    I2C::writeByte(devId, MPU6050_GYRO_CONFIG, 0x18); // Full scale range = 2000 dps

    I2C::writeByte(devId, MPU6050_ACCEL_CONFIG, 0x18); // Full scale range = +/-16g

    I2C::writeByte(devId, MPU6050_PWR_MGMT_1, 0x01); // PLL with xGyro reference


    /*
     // Use the most accurate clock source
         setClockSource(MPU6050_CLOCK_PLL_XGYRO);
     
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
void MPU6050::getRawGyro(int16_t *x, int16_t *y, int16_t *z) {

    int16_t *wrt = (int16_t *) data;

    // MSB FIRST!
    I2C::readWords(devId, MPU6050_GYRO_START, wrt, 3);
    *x = wrt[0];
    *y = wrt[1];
    *z = wrt[2];
}

void MPU6050::getRotation(float *_x, float *_y, float *_z) {

    int16_t x, y, z;

    getRawGyro(&x, &y, &z);

    *_x = x / gyroScale * M_PI / 180.0; // in rad/s?
    *_y = y / gyroScale * M_PI / 180.0;
    *_z = z / gyroScale * M_PI / 180.0;
}

/**
 * Get three axis accelerometer readings at the Sample Rate
 * Each 16-bit measurement has a scale defined in ACCEL_FS
 * 
 * @param x
 * @param y
 * @param z
 */
void MPU6050::getRawAccel(int16_t *x, int16_t *y, int16_t *z) {

    int16_t *wrt = (int16_t *) data;

    // MSB FIRST!
    I2C::readWords(devId, MPU6050_ACCEL_START, wrt, 3);
    *x = wrt[0];
    *y = wrt[1];
    *z = wrt[2];
}

void MPU6050::getAcceleration(float *_x, float *_y, float *_z) {

    int16_t x, y, z;

    getRawAccel(&x, &y, &z);

    *_x = x / accelScale * STANDARD_GRAVITY; // m/s^2?
    *_y = y / accelScale * STANDARD_GRAVITY;
    *_z = z / accelScale * STANDARD_GRAVITY;
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
void MPU6050::getRawMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {

    int16_t *wrt = (uint16_t *) data;

    // MSB FIRST!
    I2C::readWords(devId, MPU6050_ACCEL_START, wrt, 7);

    *ax = wrt[0];
    *ay = wrt[1];
    *az = wrt[2];
    
    *gx = wrt[4];
    *gy = wrt[5];
    *gz = wrt[6];
}

void MPU6050::getRawMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz) {

    int16_t *wrt = (uint16_t *) data;

    // MSB FIRST!
    I2C::readWords(devId, MPU6050_ACCEL_START, wrt, 11);

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
 * Get current temperature
 * 
 * @return 
 */
float MPU6050::getTemperature() {

    int16_t *wrt = (uint16_t *) data;

    I2C::readWords(devId, MPU6050_TEMP_START, wrt, 1);
    return *wrt; // TODO: temp / 340. + 36.53
}

/**
 * Check if device is connected and ID register is readable
 * 
 * @return 
 */
bool MPU6050::isAlive() {

    if (I2C::readByte(devId, MPU6050_WHO_AM_I, data)) {

        return (*data == MPU6050_DEVICE_ID)
    }
    return false;
}
