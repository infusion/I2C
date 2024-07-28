
#ifndef _I2C_H
#define _I2C_H

#ifdef ARDUINO
#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif
#endif

#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

#ifdef RASPBERRY
#define I2CADDR "/dev/i2c-1"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define GET_BITS_FROM_BYTE(x, a, b) (((x) >> (a)) & ((1 << ((b) - (a) + 1)) - 1))
#define SET_BITS_IN_BYTE(x, a, b, v) (((x) & ~(((1 << ((b) - (a) + 1)) - 1) << (a))) | (v << (a)))
#define SET_BIT_IN_BYTE(prev, pos, value) ((prev) ^ ((-(value) ^ (prev)) & (1 << (pos))))
#define SET_BIT_IN_BYTE_CONST(prev, pos, value) (((prev) & ~(1 << (pos))) | ((value) << (pos)))
#define SET_BIT_IN_BYTE_MUTABLE(dest, pos, value) dest ^= (-(value) ^ dest) & (1 << (pos))

typedef uint8_t devid_t;

class I2C
{
public:
#ifdef RASPBERRY
    static int fd;
#endif

    I2C()
    {
    }

    static void init();
    static void close();

    static void setFastMode();
    static void delay(uint16_t delay);
    static void udelay(unsigned int d);

    static bool readBit(devid_t dev, uint8_t reg, uint8_t bit, uint8_t *data);
    static bool readByte(devid_t dev, uint8_t reg, uint8_t *data);
    static bool readBytes(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool readBytesStop(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool readWord(devid_t dev, uint8_t reg, uint16_t *data);
    static bool readWords(devid_t dev, uint8_t reg, uint16_t *data, uint8_t length);

    static bool writeBit(devid_t dev, uint8_t reg, uint8_t bit, bool data);
    static bool writeByte(devid_t dev, uint8_t reg, uint8_t data);
    static bool writeBytes(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool writeWord(devid_t dev, uint8_t reg, uint16_t data);
    static bool writeWords(devid_t dev, uint8_t reg, uint16_t *data, uint8_t length);
};

int I2C::fd = -1;

#endif
