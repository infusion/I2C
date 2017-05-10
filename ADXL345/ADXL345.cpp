
#include <ADXL345.h>

/**
 * Set up accelerometer for general usage with default settings
 * 
 */
void ADXL345::init() {

#ifdef RASPBERRY
    devId = i2cOpen(1, devId, 0);
#endif

    setDataRate(ADXL345_DATARATE_100_HZ);

    I2C::writeByte(devId, ADXL345_DATA_FORMAT, ADXL345_DATA_FORMAT_RANGE_2G | ADXL345_DATA_FORMAT_FULL_RESOLUTION);

    I2C::writeByte(devId, ADXL345_POWER_CTL, ADXL345_POWER_CTL_MEASURE);
}

void ADXL345::getRawMeasure(int16_t *x, int16_t *y, int16_t *z) {

    // Read all data axis registers in accordance to the data sheet.
    // "It is recommended that a multiple-uint8_t read of all registers be performed to prevent a change in data between reads of sequential registers."
    // A read on a register also changes the fifo, so getting everything at once is the only way

    int16_t *wrt = (int16_t *) data;

    // LSB FIRST!
    if (I2C::readBytes(devId, ADXL345_DATA_START, data, 6)) {
        *x = wrt[0];
        *y = wrt[1];
        *z = wrt[2];
    } else {
        *x = 0;
        *y = 0;
        *z = 0;
    }
}

void ADXL345::getAcceleration(float *x, float *y, float *z) {

    int16_t _x, _y, _z;

    getRawMeasure(&_x, &_y, &_z);

    // Wenn nicht in full range??: raw * (x * 2) / 1024, mit x={2,4,8,16} wenn 2g, 4g, 8g oder 16g
    // We're in full resolution mode, so scale by 4mg/LSB


    // resulution:
    //   4mg / LSB * 9.81m/s^2
    // = 0.004g / LSB * 9.81m/s^2 / 1g
    // = 0.004 * 9.81m/s^2 / LSB

    // Valid range:
    // -16g <= x <= 16g
    // -156.9064m/s^2 <= x <= 156.9064m/s^2

    *x = _x * (0.004 * ADXL345_EARTH_GRAVITY_MS2);
    *y = _y * (0.004 * ADXL345_EARTH_GRAVITY_MS2);
    *z = _z * (0.004 * ADXL345_EARTH_GRAVITY_MS2);
}






// 2g = most sensitive
// 16g = größte range?




// BW_RATE Register

/**
 * Sets the data rate for the ADXL345 (controls power consumption)
 * 
 * @param rate
 */
void ADXL345::setDataRate(adxl345_data_rate_t rate) {

    I2C::writeByte(devId, ADXL345_BW_RATE, rate);
}

/**
 * Sets the data rate for the ADXL345 with power saving
 * This results in more noisy data, but can save up to 65%
 * 
 * @param rate
 */
void ADXL345::setDataRateLowPower(adxl345_data_rate_t rate) {

    I2C::writeByte(devId, ADXL345_BW_RATE, rate | ADXL345_BW_RATE_LOW_POWER);
}

/**
 * Get measurement data rate.
 * 
 * @return data rate
 */
adxl345_data_rate_t ADXL345::getDataRate() {

    I2C::readByte(devId, ADXL345_BW_RATE, data);
    return (adxl345_data_rate_t) (*data & 0x0F);
}

/**
 * Check if low power mode is enabled
 * 
 * @return low power enabled status.
 */
bool ADXL345::isLowPowerEnabled() {
    I2C::readBit(devId, ADXL345_BW_RATE, ADXL345_BW_RATE_LOW_POWER_BIT, data);
    return data[0];
}






// DEV_ID Register

bool ADXL345::isAlive() {

    // Check device ID for being 345 in base 8
    if (I2C::readByte(devId, ADXL345_DEVID, data)) {
        return data[0] == 0345;
    }
    return false;
}