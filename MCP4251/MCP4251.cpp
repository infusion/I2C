
#include <MCP4251.h>

void MCP4251::setValue(uint8_t wiper, uint16_t value) { // {0, 1}, [0 - 511]

  // Command Byte + Data Byte
  // CCCC00-D + DDDDDDDD
  digitalWrite(MCP4251_CS_PIN, LOW);
  SPI.transfer((uint8_t)(((wiper & 0x01) << 4) | ((value >> 8) & 0x01))); // WIPER0 has address 0 and WIPER1 has address 1
  SPI.transfer((uint8_t)(value & 0xff));
  digitalWrite(MCP4251_CS_PIN, HIGH);
}

void MCP4251::setResistance(uint8_t wiper, float resistance) {

  return setValue(wiper, (uint8_t)(MCP4251_CALIBRATE_A - MCP4251_CALIBRATE_B * resistance + .5));
}
