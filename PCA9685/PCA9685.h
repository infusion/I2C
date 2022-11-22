
#ifndef _PCA9685_H
#define _PCA9685_H

#include <I2C.h>

// Device Address with all address pins low
#define PCA9685_ADDRESS            0x40

// Official osci frequency is 25MHz, but 26MHz is maybe closer to the right freq
// Use examples/calibration.ino to collect data and fit f(x) = round(a / (4096 * x)) - 1
#define PCA9685_OSCI_FREQUENCY     26000000

// Register
#define PCA9685_MODE1              0x00
#define PCA9685_MODE2              0x01
#define PCA9685_SUBADR1            0x02
#define PCA9685_SUBADR2            0x03
#define PCA9685_SUBADR3            0x04
#define PCA9685_ALLCALLADR         0x05
#define PCA9685_LED_OFFSET         0x06
#define PCA9685_ALL_OFFSET         0xFA
#define PCA9685_PRESCALE           0xFE

// Mode 1
#define PCA9685_MODE1_ALLCALL_BIT   0
#define PCA9685_MODE1_SUB3_BIT      1
#define PCA9685_MODE1_SUB2_BIT      2
#define PCA9685_MODE1_SUB1_BIT      3
#define PCA9685_MODE1_SLEEP_BIT     4
#define PCA9685_MODE1_AI_BIT        5
#define PCA9685_MODE1_EXTCLK_BIT    6
#define PCA9685_MODE1_RESTART_BIT   7

// Mode 2
#define PCA9685_MODE2_OUTDRV_BIT    2   // Bit set=open-drain, unset=totem pole
#define PCA9685_MODE2_INVRT_BIT     4

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
    void setOutputMode(bool totemPole);
};

#endif
