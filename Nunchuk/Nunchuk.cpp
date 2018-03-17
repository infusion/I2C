
#include <Nunchuk.h>

/**
 * Initializes the Nunchuk communication by sending a sequence of bytes
 */
void Nunchuk::init() {

    // Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
    I2C::setFastMode();

    // I2C::sleep(1)

#if NUNCHUK_USE_ENCRYPTION
    I2C::writeByte(devId, 0x40, 0x00); // I2C::sleep(1)
#else
    I2C::writeByte(devId, 0xF0, 0x55); // I2C::sleep(1)
    I2C::writeByte(devId, 0xFB, 0x00); // I2C::sleep(1)
#endif

#ifdef DEBUG
    Serial.print("Ident: 0x"); // 0xA4200000 for Nunchuck, 0xA4200101 for Classic, 0xA4200402 for Balance

    I2C::readBytesStop(devId, NUNCHUK_IDENT, data, 6);
    for (uint8_t i = 0; i < 6; i++) {
        Serial.print(data[i], HEX);
    }
    Serial.print("\n");

    I2C::sleep(100); // Wait for serial transfer, before loop()ing
#endif
}

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) // Only on Arduino UNO

/**
 * Use normal analog ports as power supply, which is useful if you want to have all pins in a row
 * Like for the famous WiiChuck adapter
 * @see https://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 */
void Nunchuk::initPower() {
    // Add power supply for port C2 (GND) and C3 (PWR)
    PORTC &= ~_BV(PORTC2);
    PORTC |= _BV(PORTC3);
    DDRC |= _BV(PORTC2) | _BV(PORTC3);
    I2C::sleep(100);
}
#endif

/**
 * Central function to read a full chunk of data from Nunchuk
 *
 * @return A boolean if the data transfer was successful
 */
bool Nunchuk::read() {
    bool status = I2C::readBytesStop(devId, 0x00, data, 6); // maybe add delayMicroseconds(10); after requestFrom()
#if NUNCHUK_USE_ENCRYPTION
    for (uint8_t i = 0; i < 6; i++) {
        data[i] = (data[i] ^ 0x17) + 0x17;
    }
#endif
    return status;
}

/**
 * Checks the current state of button Z
 */
uint8_t Nunchuk::getButtonZ() {
    return (~data[5] >> 0) & 1;
}

/**
 * Checks the current state of button C
 */
uint8_t Nunchuk::getButtonC() {
    return (~data[5] >> 1) & 1;
}

/**
 * Retrieves the raw X-value of the joystick
 */
uint8_t Nunchuk::getRawJoystickX() {
    return data[0];
}

/**
 * Retrieves the raw Y-value of the joystick
 */
uint8_t Nunchuk::getRawJoystickY() {
    return data[1];
}

/**
 * Retrieves the calibrated X-value of the joystick
 */
int16_t Nunchuk::getJoystickX() {
    return (int16_t) getRawJoystickX() - (int16_t) NUNCHUK_JOYSTICK_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the joystick
 */
int16_t Nunchuk::getJoystickY() {
    return (int16_t) getRawJoystickY() - (int16_t) NUNCHUK_JOYSTICK_Y_ZERO;
}

/**
 * Calculates the angle of the joystick
 */
float Nunchuk::getJoystickAngle() {
    return atan2((float) getJoystickY(), (float) getJoystickX());
}

/**
 * Retrieves the raw X-value of the accelerometer
 */
uint16_t Nunchuk::getRawAccelX() {
    return ((uint16_t) data[2] << 2) | ((data[5] >> 2) & 3);
}

/**
 * Retrieves the raw Y-value of the accelerometer
 */
uint16_t Nunchuk::getRawAccelY() {
    return ((uint16_t) data[3] << 2) | ((data[5] >> 4) & 3);
}

/**
 * Retrieves the raw Z-value of the accelerometer
 */
uint16_t Nunchuk::getRawAccelZ() {
    return ((uint16_t) data[4] << 2) | ((data[5] >> 6) & 3);
}

/**
 * Retrieves the calibrated X-value of the accelerometer
 */
int16_t Nunchuk::getAccelX() {
    return (int16_t) getRawAccelX() - (int16_t) NUNCHUK_ACCEL_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the accelerometer
 */
int16_t Nunchuk::getAccelY() {
    return (int16_t) getRawAccelY() - (int16_t) NUNCHUK_ACCEL_Y_ZERO;
}

/**
 * Retrieves the calibrated Z-value of the accelerometer
 */
int16_t Nunchuk::getAccelZ() {
    return (int16_t) getRawAccelZ() - (int16_t) NUNCHUK_ACCEL_Z_ZERO;
}

/**
 * Calculates the pitch angle THETA around y-axis of the Nunchuk
 */
float Nunchuk::getPitch() { // tiltY
    return atan2((float) getAccelY(), (float) getAccelZ());
}

/**
 * Calculates the roll angle PHI around x-axis of the Nunchuk
 */
float Nunchuk::getRoll() { // tiltX
    return atan2((float) getAccelX(), (float) getAccelZ());
}
