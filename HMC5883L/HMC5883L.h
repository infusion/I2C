
#ifndef _HMC5883L_H
#define _HMC5883L_H

#include <I2C.h>

#define HMC5883L_GAUSS_TO_MICROTESLA       100.0

// Device Address, there is only one
#define HMC5883L_ADDRESS             0x1E

// Config register

#define HMC5883L_CONFIG_A            0x00 // R/W
#define HMC5883L_CONFIG_B            0x01 // R/W

// Mode register
#define HMC5883L_MODE                0x02 // R/W

// Data registers
#define HMC5883L_DATA_MSB_X          0x03 // R
#define HMC5883L_DATA_LSB_X          0x04 // R
#define HMC5883L_DATA_MSB_Z          0x05 // R
#define HMC5883L_DATA_LSB_Z          0x06 // R
#define HMC5883L_DATA_MSB_Y          0x07 // R
#define HMC5883L_DATA_LSB_Y          0x08 // R
#define HMC5883L_DATA_START          HMC5883L_DATA_MSB_X

// Status register
#define HMC5883L_STATUS              0x09 // R

#define HMC5883L_STATUS_READY_BIT    0
#define HMC5883L_STATUS_LOCK_BIT     1

// Ident Registers
#define HMC5883L_IDENT_A             0x0A // R
#define HMC5883L_IDENT_B             0x0B // R
#define HMC5883L_IDENT_C             0x0C // R

#ifdef USE_HMC5883
#define HMC5883_TEMP_MSB            0x31 // R
#define HMC5883_TEMP_LSB            0x32 // R
#endif

typedef enum {
    HMC5883L_AVERAGING_1 = 0x00, // Default
    HMC5883L_AVERAGING_2 = 0x01,
    HMC5883L_AVERAGING_4 = 0x02,
    HMC5883L_AVERAGING_8 = 0x03
} hmc5883l_averaging_t;

typedef enum {
    HMC5883L_DATARATE_0_75HZ = 0x00,
    HMC5883L_DATARATE_1_5HZ = 0x01,
    HMC5883L_DATARATE_3HZ = 0x02,
    HMC5883L_DATARATE_7_50HZ = 0x03,
    HMC5883L_DATARATE_15HZ = 0x04, // Default
    HMC5883L_DATARATE_30HZ = 0x05,
    HMC5883L_DATARATE_75HZ = 0x06
} hmc5883l_datarate_t;

typedef enum {
    HMC5883L_BIAS_NORMAL = 0x00, // Default
    HMC5883L_BIAS_POSITIVE = 0x01,
    HMC5883L_BIAS_NEGATIVE = 0x02
} hmc5883l_bias_t;

typedef enum {
    HMC5883L_GAIN_1370 = 0x00 << 5, // ± 0.88 Ga
    HMC5883L_GAIN_1090 = 0x01 << 5, // ± 1.3 Ga, Default
    HMC5883L_GAIN_820 = 0x02 << 5, // ± 1.9 Ga
    HMC5883L_GAIN_660 = 0x03 << 5, // ± 2.5 Ga
    HMC5883L_GAIN_440 = 0x04 << 5, // ± 4.0 Ga
    HMC5883L_GAIN_390 = 0x05 << 5, // ± 4.7 Ga
    HMC5883L_GAIN_330 = 0x06 << 5, // ± 5.6 Ga
    HMC5883L_GAIN_230 = 0x07 << 5 // ± 8.1 Ga
} hmc5883l_gain_t;

typedef enum {
    HMC5883L_MODE_CONTINUOUS = 0x00,
    HMC5883L_MODE_SINGLE = 0x01, // Default
    HMC5883L_MODE_IDLE = 0x02
} hmc5883l_mode_t;

class HMC5883L {
private:

    devid_t devId;
    uint8_t data[6];
    uint8_t mode;

    float _scaleXY, _scaleZ;

public:

    HMC5883L(devid_t id = HMC5883L_ADDRESS) {
        devId = id;
    }

    void init();

    bool isAlive();

    // Config A Register

    hmc5883l_averaging_t getSampleAveraging();
    void setSampleAveraging(hmc5883l_averaging_t samples);

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

    float getAzimuth();

    // Status Register

    bool isStatusLock();

    bool isStatusReady();
};

#endif
