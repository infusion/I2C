
#ifndef _NUNCHUK_H
#define _NUNCHUK_H

#include <I2C.h>

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS 0x52

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_DISABLE_ENCRYPTION 1

// Print debug information instead of a CSV stream to the serial port
#define NUNCHUK_DEBUG 1


class Nunchuk {
private:

    uint8_t devId;
    uint8_t data[6];

public:

    Nunchuk(uint8_t id) {
        devId = id;
    }

    Nunchuk() {
        devId = NUNCHUK_ADDRESS;
    }

    void init();

    #ifdef ARDUINO
    void initPower();
    #endif
    bool read();

    uint8_t buttonZ();
    uint8_t buttonC();
    uint8_t joystickX_raw();
    uint8_t joystickY_raw();
    int16_t joystickX();
    int16_t joystickY();
    float joystick_angle();
    uint16_t accelX_raw();
    uint16_t accelY_raw();
    uint16_t accelZ_raw();
    int16_t accelX() ;
    int16_t accelY();
    int16_t accelZ();
    float pitch();
    float roll();
};

#endif

































