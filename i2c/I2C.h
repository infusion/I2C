

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
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#endif

#define GET_BITS_FROM_BYTE(x, a, b) (((x) >> (a)) & ((1 << ((b) - (a) + 1)) - 1))
#define SET_BITS_IN_BYTE(x, a, b, v) (((x) & ~(((1 << ((b) - (a) + 1)) - 1) << (a))) | (v << (a)))
#define SET_BIT_IN_BYTE(prev, pos, value) (prev) ^ ((-(value) ^ (prev)) & (1 << (pos))) 
#define SET_BIT_IN_BYTE_MUTABLE(dest, pos, value) dest^= (-(value) ^ dest) & (1 << (pos))


class I2C {
public:

    I2C() {
    }

#ifdef RASPBERRY
    int I2C::open(uint8_t num);
#endif

    static void setFastMode();
    static void sleep(uint16_t delay);
    static void usleep(unsigned int d);

    static bool readBit(uint8_t dev, uint8_t reg, uint8_t bit, uint8_t *data);
    static bool readByte(uint8_t dev, uint8_t reg, uint8_t *data);
    static bool readBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool readBytesStop(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool readWord(uint8_t dev, uint8_t reg, uint16_t *data);
    static bool readWords(uint8_t dev, uint8_t reg, uint16_t *data, uint8_t length);

    static bool writeBit(uint8_t dev, uint8_t reg, uint8_t bit, bool data);
    static bool writeByte(uint8_t dev, uint8_t reg, uint8_t data);
    static bool writeBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length);
    static bool writeWord(uint8_t dev, uint8_t reg, uint16_t data);
    static bool writeWords(uint8_t dev, uint8_t reg, uint16_t *data, uint8_t length);
};

#endif
