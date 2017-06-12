
#ifndef _MPU6050_H
#define _MPU6050_H

#include <I2C.h>

#define MPU6050_NOMINAL_GRAVITY    9.80665

// Device Address
#define MPU6050_ADDRESS_LOW         0x68 // when AD0 pin is low
#define MPU6050_ADDRESS_HIGH        0x69 // when AD0 pin is high
#define MPU6050_ADDRESS             MPU6050_ADDRESS_LOW

#define MPU6050_ACCEL_XOFFS_H     0x06
#define MPU6050_ACCEL_XOFFS_L     0x07
#define MPU6050_ACCEL_YOFFS_H     0x08
#define MPU6050_ACCEL_YOFFS_L     0x09
#define MPU6050_ACCEL_ZOFFS_H     0x0A
#define MPU6050_ACCEL_ZOFFS_L     0x0B

#define MPU6050_GYRO_XOFFS_H      0x13
#define MPU6050_GYRO_XOFFS_L      0x14
#define MPU6050_GYRO_YOFFS_H      0x15
#define MPU6050_GYRO_YOFFS_L      0x16
#define MPU6050_GYRO_ZOFFS_H      0x17
#define MPU6050_GYRO_ZOFFS_L      0x18

#define MPU6050_CONFIG            0x1A

#define MPU6050_GYRO_CONFIG       0x1B // Gyroscope Configuration
#define MPU6050_ACCEL_CONFIG      0x1C // Accelerometer Configuration

#define MPU6050_FF_THRESHOLD      0x1D
#define MPU6050_FF_DURATION       0x1E
#define MPU6050_MOT_THRESHOLD     0x1F
#define MPU6050_MOT_DURATION      0x20
#define MPU6050_ZMOT_THRESHOLD    0x21
#define MPU6050_ZMOT_DURATION     0x22

#define MPU6050_INT_PIN_CFG       0x37 // INT Pin
#define MPU6050_INT_ENABLE        0x38 // INT Enable

#define MPU6050_INT_STATUS        0x3A

#define MPU6050_ACCEL_START      0x3B // 0x3B - 0x40
#define MPU6050_TEMP_START        0x41 // 0x41 - 0x42
#define MPU6050_GYRO_START       0x43   // 0x43 - 0x48

#define MPU6050_MOT_DETECT_STATUS 0x61
#define MPU6050_MOT_DETECT_CTRL   0x69

#define MPU6050_USER_CTRL         0x6A // User Control
#define MPU6050_PWR_MGMT_1        0x6B // Power Management 1
#define MPU6050_WHO_AM_I          0x75 // Who Am I

#define MPU6050_DEVICE_ID           0x34

typedef enum {
    MPU6050_CLOCK_KEEP_RESET = 7,
    MPU6050_CLOCK_EXTERNAL_19MHZ = 5,
    MPU6050_CLOCK_EXTERNAL_32KHZ = 4,
    MPU6050_CLOCK_PLL_ZGYRO = 3,
    MPU6050_CLOCK_PLL_YGYRO = 2,
    MPU6050_CLOCK_PLL_XGYRO = 1,
    MPU6050_CLOCK_INTERNAL_8MHZ = 0
} mpu6050_clockSource_t;

typedef enum {
    MPU6050_SCALE_2000DPS = 3,
    MPU6050_SCALE_1000DPS = 2,
    MPU6050_SCALE_500DPS = 1,
    MPU6050_SCALE_250DPS = 0
} mpu6050_dps_t;

typedef enum {
    MPU6050_RANGE_16G = 3,
    MPU6050_RANGE_8G = 2,
    MPU6050_RANGE_4G = 1,
    MPU6050_RANGE_2G = 0,
} mpu6050_range_t;

typedef enum {
    MPU6050_DELAY_3MS = 3,
    MPU6050_DELAY_2MS = 2,
    MPU6050_DELAY_1MS = 1,
    MPU6050_NO_DELAY = 0,
} mpu6050_onDelay_t;

typedef enum {
    MPU6050_DHPF_HOLD = 7,
    MPU6050_DHPF_0_63HZ = 4,
    MPU6050_DHPF_1_25HZ = 3,
    MPU6050_DHPF_2_5HZ = 2,
    MPU6050_DHPF_5HZ = 1,
    MPU6050_DHPF_RESET = 0,
} mpu6050_dhpf_t;

typedef enum {
    MPU6050_DLPF_6 = 6,
    MPU6050_DLPF_5 = 5,
    MPU6050_DLPF_4 = 4,
    MPU6050_DLPF_3 = 3,
    MPU6050_DLPF_2 = 2,
    MPU6050_DLPF_1 = 1,
    MPU6050_DLPF_0 = 0,
} mpu6050_dlpf_t;

class MPU6050 {
private:

    uint8_t devId;
    uint8_t data[22];

public:

    MPU6050(uint8_t id = MPU6050_ADDRESS) {
        devId = id;
    }

    void init();

    void getRawGyro(int16_t *x, int16_t *y, int16_t *z);
    void getRawAccel(int16_t *x, int16_t *y, int16_t *z);

    void getAcceleration(float *_x, float *_y, float *_z);
    void getRotation(float *_x, float *_y, float *_z);

    float getTemperature();

    void getRawMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz);
    void getRawMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

    bool isAlive();

};

#endif
