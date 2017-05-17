
#include <Nunchuk.h>


/**
 * Initializes the Nunchuk communication by sending a sequence of bytes
 */
void Nunchuk::init() {
    
    // Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
    I2C::setFastMode();

#ifdef NUNCHUK_DISABLE_ENCRYPTION
    I2C::writeByte(devId, 0xF0, 0x55);
    I2C::writeByte(devId, 0xFB, 0x00);
#else
    I2C::writeByte(devId, 0x40, 0x00);
#endif

#if NUNCHUK_DEBUG
    Serial.print("Ident: 0x"); // 0xA4200000 for Nunchuck, 0xA4200101 for Classic, 0xA4200402 for Balance

    I2C::readBytesStop(devId, 0xFA, data, 6);
    for (uint8_t i = 0; i < 6; i++) {
        Serial.print(data[i], HEX);
    }
    Serial.print("\n");

    delay(1000); // Wait for serial transfer, before loop()ing
#endif
}

/**
 * Central function to read a full chunk of data from Nunchuk
 *
 * @return A boolean if the data transfer was successful
 */
bool Nunchuk::read() {
    
#ifdef NUNCHUK_DISABLE_ENCRYPTION
    return I2C::readBytesStop(devId, 0x00, data, 6);
#else
    bool status = I2C::readBytesStop(devId, 0x00, data, 6);
    for (i = 0; i < 6; i++) {
        data[i] = (data[i] ^ 0x17) + 0x17;
    }
    return status;
#endif
}

/**
 * Checks the current state of button Z
 */
uint8_t Nunchuk::buttonZ() {
    return (~data[5] >> 0) & 1;
}

/**
 * Checks the current state of button C
 */
uint8_t Nunchuk::buttonC() {
    return (~data[5] >> 1) & 1;
}

/**
 * Retrieves the raw X-value of the joystick
 */
uint8_t Nunchuk::joystickX_raw() {
    return data[0];
}

/**
 * Retrieves the raw Y-value of the joystick
 */
uint8_t Nunchuk::joystickY_raw() {
    return data[1];
}

/**
 * Retrieves the calibrated X-value of the joystick
 */
int16_t Nunchuk::joystickX() {
    return (int16_t) joystickX_raw() - (int16_t) NUNCHUK_JOYSTICK_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the joystick
 */
int16_t Nunchuk::joystickY() {
    return (int16_t) joystickY_raw() - (int16_t) NUNCHUK_JOYSTICK_Y_ZERO;
}

/**
 * Calculates the angle of the joystick
 */
float Nunchuk::joystick_angle() {
    return atan2((float) joystickY(), (float) joystickX());
}

/**
 * Retrieves the raw X-value of the accelerometer
 */
uint16_t Nunchuk::accelX_raw() {
    return ((uint16_t) data[2] << 2) | ((data[5] >> 2) & 3);
}

/**
 * Retrieves the raw Y-value of the accelerometer
 */
uint16_t Nunchuk::accelY_raw() {
    return ((uint16_t) data[3] << 2) | ((data[5] >> 4) & 3);
}

/**
 * Retrieves the raw Z-value of the accelerometer
 */
uint16_t Nunchuk::accelZ_raw() {
    return ((uint16_t) data[4] << 2) | ((data[5] >> 6) & 3);
}

/**
 * Retrieves the calibrated X-value of the accelerometer
 */
int16_t Nunchuk::accelX() {
    return (int16_t) accelX_raw() - (int16_t) NUNCHUK_ACCEL_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the accelerometer
 */
int16_t Nunchuk::accelY() {
    return (int16_t) accelY_raw() - (int16_t) NUNCHUK_ACCEL_Y_ZERO;
}

/**
 * Retrieves the calibrated Z-value of the accelerometer
 */
int16_t Nunchuk::accelZ() {
    return (int16_t) accelZ_raw() - (int16_t) NUNCHUK_ACCEL_Z_ZERO;
}

/**
 * Calculates the pitch angle of the Nunchuk
 */
float Nunchuk::pitch() {
    return atan2((float) accelY(), (float) accelZ());
}

/**
 * Calculates the roll angle of the Nunchuk
 */
float Nunchuk::roll() {
    return atan2((float) accelX(), (float) accelZ());
}









