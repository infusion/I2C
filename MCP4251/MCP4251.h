#ifndef MCP4251_h
#define MCP4251_h

#include <SPI.h>

#if ARDUINO < 100
#include "Wprogram.h"
#else
#include "Arduino.h"
#endif

// GND      -> 4
// 5v       -> 14

// SCK   13 -> 2
// MISO  12 -> 13
// MOSI  11 -> 3
#define MCP4251_CS_PIN 10 // -> 1

// Use calibrate.py
#define MCP4251_CALIBRATE_A 260.9472191971956
#define MCP4251_CALIBRATE_B 52.54619782538408


class MCP4251
{
public:
    void setValue(uint8_t wiper, uint16_t value);
    void setResistance(uint8_t wiper, float resistance);
};

#endif
