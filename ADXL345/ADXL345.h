
#ifndef _ADXL345_H
#define _ADXL345_H

#include <I2C.h>

// Device Address
#define ADXL345_ADDRESS      0x53  // ALT high=0x1D

#define ADXL345_DEVID               0x00    // R,   Device ID
#define ADXL345_THRESH_TAP          0x1D    // R/W, Tap threshold
#define ADXL345_OFSX                0x1E    // R/W, X-axis offset
#define ADXL345_OFSY                0x1F    // R/W, Y-axis offset
#define ADXL345_OFSZ                0x20    // R/W, Z-axis offset
#define ADXL345_DUR                 0x21    // R/W, Tap duration
#define ADXL345_LATENT              0x22    // R/W, Tap latency
#define ADXL345_WINDOW              0x23    // R/W, Tap window
#define ADXL345_THRESH_ACT          0x24    // R/W, Activity threshold
#define ADXL345_THRESH_INACT        0x25    // R/W, Inactivity threshold
#define ADXL345_TIME_INACT          0x26    // R/W, Inactivity time
#define ADXL345_ACT_INACT_CTL       0x27    // R/W, Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF           0x28    // R/W, Free-fall threshold
#define ADXL345_TIME_FF             0x29    // R/W, Free-fall time
#define ADXL345_TAP_AXES            0x2A    // R/W, Axis control for single/double tap
#define ADXL345_ACT_TAP_STATUS      0x2B    // R,   Source for single/double tap
#define ADXL345_BW_RATE             0x2C    // R/W, Data rate and power mode control
#define ADXL345_POWER_CTL           0x2D    // R/W, Power-saving features control
#define ADXL345_INT_ENABLE          0x2E    // R/W, Interrupt enable control
#define ADXL345_INT_MAP             0x2F    // R/W, Interrupt mapping control
#define ADXL345_INT_SOURCE          0x30    // R,   Source of interrupts
#define ADXL345_DATA_FORMAT         0x31    // R/W, Data format control
#define ADXL345_DATA_START          0x32    // R, 6 bytes of axis data
#define ADXL345_FIFO_CTL            0x38    // R/W, FIFO control
#define ADXL345_FIFO_STATUS         0x39    // R,   FIFO status

// TAP_AXES bit settings
#define ADXL345_TAP_AXES_Z_BIT       0 // Enables tap detection on Z axis
#define ADXL345_TAP_AXES_Y_BIT       1 // Enables tap detection on Y axis
#define ADXL345_TAP_AXES_X_BIT       2 // Enables tap detection on X axis
#define ADXL345_TAP_AXES_SUPPRESS_BIT     3 // Suppresses double tap detection




#define ADXL345_BW_RATE_LOW_POWER_BIT   4
#define ADXL345_BW_RATE_LOW_POWER      (1 << ADXL345_BW_RATE_LOW_POWER_BIT)

typedef enum {
    ADXL345_DATARATE_3200_HZ = 0x0F, // 1600Hz Bandwidth  
    ADXL345_DATARATE_1600_HZ = 0x0E, //  800Hz Bandwidth   
    ADXL345_DATARATE_800_HZ = 0x0D, //  400Hz Bandwidth   
    ADXL345_DATARATE_400_HZ = 0x0C, //  200Hz Bandwidth   
    ADXL345_DATARATE_200_HZ = 0x0B, //  100Hz Bandwidth  
    ADXL345_DATARATE_100_HZ = 0x0A, //   50Hz Bandwidth  (default)
    ADXL345_DATARATE_50_HZ = 0x09, //   25Hz Bandwidth    
    ADXL345_DATARATE_25_HZ = 0x08, // 12.5Hz Bandwidth   
    ADXL345_DATARATE_12_5_HZ = 0x07, // 6.25Hz Bandwidth    
    ADXL345_DATARATE_6_25HZ = 0x06, // 3.13Hz Bandwidth   
    ADXL345_DATARATE_3_13_HZ = 0x05, // 1.56Hz Bandwidth   
    ADXL345_DATARATE_1_56_HZ = 0x04, // 0.78Hz Bandwidth   
    ADXL345_DATARATE_0_78_HZ = 0x03, // 0.39Hz Bandwidth    
    ADXL345_DATARATE_0_39_HZ = 0x02, // 0.20Hz Bandwidth    
    ADXL345_DATARATE_0_20_HZ = 0x01, // 0.10Hz Bandwidth    
    ADXL345_DATARATE_0_10_HZ = 0x00 // 0.05Hz Bandwidth    
} adxl345_data_rate_t;

typedef enum {
    ADXL345_DATA_FORMAT_RANGE_2G = 0x00, // +/- 2g (default value)
    ADXL345_DATA_FORMAT_RANGE_4G = 0x01, // +/- 4g
    ADXL345_DATA_FORMAT_RANGE_8G = 0x02, // +/- 8g
    ADXL345_DATA_FORMAT_RANGE_16G = 0x03, // +/- 16g
    ADXL345_DATA_FORMAT_FULL_RESOLUTION = 0x08,
    ADXL345_DATA_FORMAT_INT_INVERT = 0x20, // Invert the interrupt pins
    ADXL345_DATA_FORMAT_SELF_TEST = 0x80, // Apply self-test force to sensor
} adxl345_data_format_t;

typedef enum {
    ADXL345_POWER_CTL_MEASURE = 0x08,
} adxl345_power_ctl_t;

class ADXL345 {
private:
    devid_t devId;
    uint8_t data[6];

    int16_t offsetX;
    int16_t offsetY;
    int16_t offsetZ;

public:

    ADXL345(devid_t id = ADXL345_ADDRESS) {
        devId = id;
    }
    void init();

#ifdef EIGEN
    void getAcceleration(vector3f &v);
#endif
    void getRawMeasure(int16_t *x, int16_t *y, int16_t *z);

    void getAcceleration(float *x, float *y, float *z);

    bool isAlive();
    // setRange(16)
    void setDataRate(adxl345_data_rate_t rate);
    void setDataRateLowPower(adxl345_data_rate_t rate);
    adxl345_data_rate_t getDataRate();
    bool isLowPowerEnabled();

};

#endif
