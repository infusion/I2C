
#ifndef _BMA180_H
#define _BMA180_H

#include <I2C.h>

// Device Address
#define BMA180_ADDRESS          0x40 // or 0x41 when SDO is high


#define BMA180_DEVID            0x00
#define BMA180_VERSION          0x01
#define BMA180_ACC_X_LSB        0x02  /* Data Start */
#define BMA180_ACC_X_MSB        0x03
#define BMA180_ACC_Y_LSB        0x04
#define BMA180_ACC_Y_MSB        0x05
#define BMA180_ACC_Z_LSB        0x06
#define BMA180_ACC_Z_MSB        0x07
#define BMA180_TEMP             0x08

#define BMA180_STATUS_REG1      0x09  // 1st tap, alert, slope signs, offset, ee_write
#define BMA180_STATUS_REG2      0x0A  // tap and slope sense bits
#define BMA180_STATUS_REG3      0x0B  // interrupt statuses from the tap sensing
#define BMA180_STATUS_REG4      0x0C  // high signs, x&y&z tapsense

#define BMA180_RESET            0x10

#define BMA180_CTRL_REG0        0x0D // EE_W enable write control is bit 4 of this register address!
#define BMA180_CTRL_REG1        0x0E // contains the offsets for x, y,z
#define BMA180_CTRL_REG2        0x0F // unlocking eeprom register
#define BMA180_CTRL_REG3        0x21 // configure different interrupt trigger bits in this register
#define BMA180_CTRL_REG4        0x22 // low_hy, mot_cd_r, ff_cd_tr, offset_finetuning

#define BMA180_BW_RATE          0x20
#define BMA180_OFFSET_LSB1      0x35

/*
 #define BMA180_CMD_CHIP_ID          0x00
#define BMA180_CMD_VERSION          0x01
#define BMA180_CMD_ACC_X_LSB        0x02
#define BMA180_CMD_ACC_X_MSB        0x03
#define BMA180_CMD_ACC_Y_LSB        0x04
#define BMA180_CMD_ACC_Y_MSB        0x05
#define BMA180_CMD_ACC_Z_LSB        0x06
#define BMA180_CMD_ACC_Z_MSB        0x07
#define BMA180_CMD_TEMP             0x08
#define BMA180_CMD_STATUS_REG1      0x09
#define BMA180_CMD_STATUS_REG2      0x0A
#define BMA180_CMD_STATUS_REG3      0x0B
#define BMA180_CMD_STATUS_REG4      0x0C
#define BMA180_CMD_CTRL_REG0        0x0D
#define BMA180_CMD_CTRL_REG1        0x0E
#define BMA180_CMD_CTRL_REG2        0x0F
#define BMA180_CMD_RESET            0x10
#define BMA180_CMD_BW_TCS           0x20
#define BMA180_CMD_CTRL_REG3        0x21
#define BMA180_CMD_CTRL_REG3        0x22*/



typedef enum {
    BMA180_RANGE_1G = 0x00,
    BMA180_RANGE_1_5G = 0x01,
    BMA180_RANGE_2G = 0x02, // Default
    BMA180_RANGE_3G = 0x03,
    BMA180_RANGE_4G = 0x04,
    BMA180_RANGE_8G = 0x05,
    BMA180_RANGE_16G = 0x06
} bma180_range_t;

class BMA180 {
private:
    uint8_t devId;
    uint8_t data[6];
    bma180_range_t range;

public:

    BMA180(uint8_t id = BMA180_ADDRESS) {
        devId = id;
    }
    void init();
    void softReset();

    void getRawMeasure(int16_t *x, int16_t *y, int16_t *z);
    void getAcceleration(float *x, float *y, float *z);
    void setRange(bma180_range_t range);
    float getGSense();
    void setWriteEnabled(bool enable);

    bool isAlive();

};

#endif
