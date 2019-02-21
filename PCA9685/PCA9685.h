
#ifndef _PCA9685_H
#define _PCA9685_H

#include <I2C.h>

// Device Address with all address pins low
#define PCA9685_ADDRESS            0x40

// Official osci frequency is 25MHz, but 26MHz is maybe closer to the right freq
// Use examples/calibration.ino to collect data and fit f(x) = round(a / (4096 * x)) - 1
#define PCA9685_OSCI_FREQUENCY     26000000

#define PCA9685_MODE1              0x00
#define PCA9685_MODE2              0x01
#define PCA9685_LED_START          0x06

#define PCA9685_PRESCALE           0xFE

#define PCA9685_MODE1_ALLCALL      0x01
#define PCA9685_MODE1_SLEEP        0x10
#define PCA9685_MODE1_AI           0x20
#define PCA9685_MODE1_EXTCLK       0x40
#define PCA9685_MODE1_RESTART      0x80

class PCA9685 {
private:

    devid_t devId;
    float osciFreq; // Osci frequency

public:

    PCA9685(devid_t id = PCA9685_ADDRESS) {
        devId = id;
    }

    void init(float freq);
    void setFreq(float f);
    void setPin(uint8_t pin, uint16_t val);
    void reset();
};

#endif
