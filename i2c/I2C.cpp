
#include <I2C.h>

#ifdef RASPBERRY

int I2C::open(uint8_t num) {

    int fd;

    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        // Todo handle errors
        return -1;
    }
    return fd;
}

void I2C::close(fd) {
    close(fd);
}

// When FD is open:
// ioctl(devId, I2C_SLAVE, i2cAddress);
//
// Read:
// write(devId, {reg}, 1);
// if (read(devId, data, 6) == 6) {} // 6 bytes
//
// Write:
// write(devId, {reg, data}, 1+datalen);
#endif

void I2C::sleep(uint16_t d) {
#ifdef ARDUINO
    delay(d);
#endif
#ifdef RASPBERRY
    bcm2835_delay(d);
#endif
}

void I2C::usleep(unsigned int d) {
#ifdef ARDUINO
    delayMicroseconds(d);
#endif
#ifdef RASPBERRY

#endif
}

void I2C::setFastMode() {

#ifdef ARDUINO
    Wire.setClock(400000);
#endif
}

bool I2C::readBit(uint8_t dev, uint8_t reg, uint8_t bit, uint8_t *data) {

    uint8_t prev;
    bool status = readBytes(dev, reg, &prev, 1);
    *data = prev & (1 << bit);
    return status;
}

bool I2C::readByte(uint8_t dev, uint8_t reg, uint8_t *data) {
    return readBytes(dev, reg, data, 1);
}

bool I2C::readBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length) {

    uint8_t *start = data;

    //if (length > BUFFER_LENGTH)
    //    return 0;

    Wire.beginTransmission(dev); // Initialize Tx buffer
    Wire.write(reg); // Put slave address in Tx buffer
    Wire.endTransmission(false); // Send Tx buffer and send restart

    Wire.requestFrom(dev, length);

    while (Wire.available()) {
        *data++ = Wire.read();
    }
    return length == (data - start);
}

bool I2C::readBytesStop(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length) {

    uint8_t *start = data;

    //if (length > BUFFER_LENGTH)
    //    return 0;

    Wire.beginTransmission(dev);
    Wire.write(reg);
    Wire.endTransmission(true);

    Wire.requestFrom(dev, length);
    // delayMicroseconds(10)
    while (Wire.available()) {
        *data++ = Wire.read();
    }
    return length == (data - start);
}

bool I2C::readWord(uint8_t dev, uint8_t reg, uint16_t *data) {

    return readWords(dev, reg, data, 1);
}

bool I2C::readWords(uint8_t dev, uint8_t reg, uint16_t *data, uint8_t length) {

    uint8_t i = 0;
    uint8_t *ptr = (uint8_t *) data;

    length *= 2;

    //if (length > BUFFER_LENGTH)
    //    return 0;

    Wire.beginTransmission(dev);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(dev, length);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    while (Wire.available()) {
        ptr[i++ ^ 1] = Wire.read();
    }
#else
#  error "readWords is not implemented for big endian"
#endif
    return length == i;
}

bool I2C::writeBit(uint8_t dev, uint8_t reg, uint8_t bit, bool data) {
    uint8_t prev;
    readBytes(dev, reg, &prev, 1);

    // Conditional bit set
    uint8_t val = prev ^ ((-data ^ prev) & (1 << bit));

    if (val == prev) {
      return true;
    }
    return writeByte(dev, reg, prev);
}

bool I2C::writeByte(uint8_t dev, uint8_t reg, uint8_t data) {

    Wire.beginTransmission(dev); // Initialize Tx buffer
    Wire.write(reg); // Put slave address in Tx buffer

    Wire.write(data); // Put data in Tx buffer

    return 0 == Wire.endTransmission(); // Send Tx buffer
}

bool I2C::writeBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint8_t length) {

    Wire.beginTransmission(dev); // Initialize Tx buffer
    Wire.write(reg); // Put slave address in Tx buffer

    uint8_t *p = data;
    uint8_t *e = data + length;

    while (p < e) {
        Wire.write(*p++); // Put data in Tx buffer
    }
    /*
     Return values of Wire.endTransmission()
     0 - Success
     1 - Data too long to fit in transmision buffer
     2 - Received NACK on transmit of address
     3 - Received NACK on transmit of data
     4 - Other
     */
    return 0 == Wire.endTransmission(); // Send Tx buffer
}

bool I2C::writeWord(uint8_t dev, uint8_t reg, uint16_t data) {
    return writeWords(dev, reg, &data, 1);
}

bool I2C::writeWords(uint8_t dev, uint8_t reg, uint16_t* data, uint8_t length) {

    Wire.beginTransmission(dev);
    Wire.write(reg); // send address

    uint8_t *p = (uint8_t *) data;
    uint8_t *e = (uint8_t *) data + 2 * length;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    for (; p < e; p += 2) {
        Wire.write(p[1]); // send MSB
        Wire.write(p[0]); // send LSB
    }
#else
#  error "writeWords is not implemented for big endian"
#endif
    return 0 == Wire.endTransmission();
}
