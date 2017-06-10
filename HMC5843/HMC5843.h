
#ifndef _HMC5843_H
#define _HMC5843_H

#include <I2C.h>

#define HMC5843_GAUSS_TO_MICROTESLA       100.0

// Device Address, there is only one
#define HMC5843_ADDRESS             0x1E

// Config register

#define HMC5843_CONFIG_A            0x00 // R/W
#define HMC5843_CONFIG_B            0x01 // R/W

// Mode register
#define HMC5843_MODE                0x02 // R/W

// Data registers
#define HMC5843_DATA_MSB_X          0x03 // R
#define HMC5843_DATA_LSB_X          0x04 // R
#define HMC5843_DATA_MSB_Y          0x05 // R
#define HMC5843_DATA_LSB_Y          0x06 // R
#define HMC5843_DATA_MSB_Z          0x07 // R
#define HMC5843_DATA_LSB_Z          0x08 // R
#define HMC5843_DATA_START          HMC5843_DATA_MSB_X

// Status register
#define HMC5843_STATUS              0x09 // R

#define HMC5843_STATUS_READY_BIT    0
#define HMC5843_STATUS_LOCK_BIT     1
#define HMC5843_STATUS_REN_BIT      2

// Ident Registers
#define HMC5843_IDENT_A             0x0A // R
#define HMC5843_IDENT_B             0x0B // R
#define HMC5843_IDENT_C             0x0C // R


typedef enum {
    HMC5843_DATARATE_0_5HZ = 0x00,
    HMC5843_DATARATE_1HZ = 0x01,
    HMC5843_DATARATE_2HZ = 0x02,
    HMC5843_DATARATE_5HZ = 0x03,
    HMC5843_DATARATE_10HZ = 0x04, // Default
    HMC5843_DATARATE_20HZ = 0x05,
    HMC5843_DATARATE_50HZ = 0x06
} hmc5883l_datarate_t;

typedef enum {
    HMC5843_BIAS_NORMAL = 0x00, // Default
    HMC5843_BIAS_POSITIVE = 0x01,
    HMC5843_BIAS_NEGATIVE = 0x02
} hmc5883l_bias_t;

typedef enum {
    HMC5843_GAIN_1620 = 0x00 << 5, // ± 0.7 Ga
    HMC5843_GAIN_1300 = 0x01 << 5, // ± 1.0 Ga, Default
    HMC5843_GAIN_970 = 0x02 << 5, // ± 1.5 Ga
    HMC5843_GAIN_780 = 0x03 << 5, // ± 2.0 Ga
    HMC5843_GAIN_530 = 0x04 << 5, // ± 3.2 Ga
    HMC5843_GAIN_460 = 0x05 << 5, // ± 3.8 Ga
    HMC5843_GAIN_390 = 0x06 << 5, // ± 4.5 Ga
    HMC5843_GAIN_280 = 0x07 << 5 // ± 6.5 Ga (Not recommended)
} hmc5883l_gain_t;

typedef enum {
    HMC5843_MODE_CONTINUOUS = 0x00,
    HMC5843_MODE_SINGLE = 0x01, // Default
    HMC5843_MODE_IDLE = 0x02,
    HMC5843_MODE_SLEEP = 0x03  
} hmc5883l_mode_t;

class HMC5843 {
private:

    uint8_t devId;
    uint8_t data[6];
    uint8_t mode;

    float _scaleXY, _scaleZ;

public:

    HMC5843(uint8_t id = HMC5843_ADDRESS) {
        devId = id;
    }

    void init();

    bool isAlive();

    // Config A Register

    hmc5883l_datarate_t getDataRate();
    void setDataRate(hmc5883l_datarate_t rate);

    void setMeasureBias(hmc5883l_bias_t bias);
    hmc5883l_bias_t getMeasureBias();

    // Config B Register

    hmc5883l_gain_t getGain();
    void setGain(hmc5883l_gain_t gain);

    // Mode Register

    hmc5883l_mode_t getMeasureMode();
    void setMeasureMode(hmc5883l_mode_t mode);

    // Data Register

    void getRawMeasure(int16_t *x, int16_t *y, int16_t *z);
    void getMagneticField(float *x, float *y, float *z);

    // Status Register

    bool isRegulatorEnabled();
    bool isStatusLock();
    bool isStatusReady();
};

#endif
