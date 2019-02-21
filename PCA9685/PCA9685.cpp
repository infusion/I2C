// todo: configure totem pole output (default) vs open collector


#include <PCA9685.h>

/**
 * Initializes the module
 */
void PCA9685::init(float freq = PCA9685_OSCI_FREQUENCY) {

  devId = I2C::init(devId);

  osciFreq = freq;

  reset();

  setFreq(50); // Set 50Hz per default
}

void PCA9685::setFreq(float f) { // freq in Hz
  uint8_t mode;

  // The PRE_SCALE register can only be set when SLEEP bit of MODE1 is set
  I2C::readByte(devId, PCA9685_MODE1, &mode);
  I2C::writeByte(devId, PCA9685_MODE1, (mode & ~PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP);

  // prescale value = round(25Mhz / (4096 * f)) - 1, but 25MHz is not correct in practice and must be calibrated
  uint8_t prescale = (uint8_t) ((osciFreq / (4096.0 * f) + 0.5)) - 1;
  I2C::writeByte(devId, PCA9685_PRESCALE, prescale);

  // Write back old mode
  I2C::writeByte(devId, PCA9685_MODE1, mode);

  // It takes 500us for the osciallator to be up and running
  I2C::sleep(5);

  // Enable auto increment to save writes, all call and restart
  I2C::writeByte(devId, PCA9685_MODE1, mode | PCA9685_MODE1_AI | PCA9685_MODE1_ALLCALL | PCA9685_MODE1_RESTART);
}

void PCA9685::setPin(uint8_t pin, uint16_t val) {

  const uint8_t ON = 0;
  const uint8_t OFF = 1;

  uint16_t state[2] = {0}; // Set ON and OFF to zero

  if (val >= 4095) {
    state[ON] = 4096; // LED ON -> FULL
    // state[OFF] = 0; 
  } else if (val == 0) {
    // state[ON] = 0;
    state[OFF] = 4096; // LED OFF -> FULL
  } else {
    // state[ON] = 0;
    state[OFF] = val; // 0 < val < 4096
  }
  I2C::writeBytes(devId, PCA9685_LED_START + 4 * pin, (uint8_t*) state, 4);
}

void PCA9685::reset() {
  I2C::writeByte(devId, PCA9685_MODE1, PCA9685_MODE1_RESTART);
  I2C::sleep(10);
}
