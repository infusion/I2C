
#ifndef _NUNCHUK_H
#define _NUNCHUK_H

#include <I2C.h>

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS    0x52

#define NUNCHUK_IDENT      0xFA

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_USE_ENCRYPTION 0

class Nunchuk {
private:

    devid_t devId;
    uint8_t data[6];

public:

    Nunchuk(devid_t id = NUNCHUK_ADDRESS) {
        devId = id;
    }

    void init();

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) // Only Arduino UNO
    void initPower();
#endif
    bool read();

    uint8_t getButtonZ(); /* 1 if pressed, 0 if not */
    uint8_t getButtonC(); /* 1 if pressed, 0 if not */

    uint8_t getRawJoystickX();
    uint8_t getRawJoystickY();

    int16_t getJoystickX(); /* -127 to 128, in practice -100 to 100 */
    int16_t getJoystickY(); /* -127 to 128, in practice -100 to 100 */
    float getJoystickAngle();

    uint16_t getRawAccelX();
    uint16_t getRawAccelY();
    uint16_t getRawAccelZ();

    int16_t getAccelX(); /* side-side  acceleration, -511 to 512 */
    int16_t getAccelY(); /* front-back acceleration, -511 to 512 */
    int16_t getAccelZ(); /* top-bottom acceleration, -511 to 512 */

    float getPitch(); /* y-axis: +/- pi/2 */
    float getRoll(); /* x-axis: +/- pi/2 */
};

#endif
