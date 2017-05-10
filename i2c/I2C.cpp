
#include <I2C.h>

void I2C::sleep(uint16_t d) {
#ifdef ARDUINO
    delay(d);
#endif
#ifdef RASPBERRY
    bcm2835_delay(d);
#endif
}

bool I2C::readBit(uint8_t dev, uint8_t reg, uint8_t bit, uint8_t *data) {

    uint8_t prev;
    bool status = readByte(dev, reg, &prev);
    *data = prev & (1 << bit);
    return status;
}

bool I2C::readByte(uint8_t dev, uint8_t reg, uint8_t *data) {

    return readBytes(dev, reg, data, 1);
}

bool I2C::readBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint8_t length) {

    int8_t count = 0;

    //if (length > BUFFER_LENGTH)
    //    return 0;

    Wire.beginTransmission(dev);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(dev, (uint8_t) length);

    for (; Wire.available(); count++) {
        data[count] = Wire.read();
    }
    return count == length;
}

bool I2C::readWord(uint8_t dev, uint8_t reg, uint16_t *data) {

    return readWords(dev, reg, data, 1);
}

bool I2C::readWords(uint8_t dev, uint8_t reg, uint16_t *data, uint8_t length) {

    uint8_t ndx = 1;
    uint8_t count = 0;
    uint8_t *ptr = (uint8_t *) data;

    //if (length > BUFFER_LENGTH / 2)
    //    return 0;

    Wire.beginTransmission(dev);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(dev, (uint8_t) (length * 2));

    while (Wire.available()) {

        ptr[ndx] = Wire.read();
        ndx = !ndx;

        if (ndx) {
            ptr += 2;
            count++;
        }
    }
    return count == length;
}

bool I2C::writeBit(uint8_t dev, uint8_t reg, uint8_t bit, bool data) {
    uint8_t prev;
    if (!readByte(dev, reg, &prev)) {
        return false;
    }

    // Conditional bit set
    prev ^= (-data ^ prev) & (1 << bit);

    return writeByte(dev, reg, prev);
}

bool I2C::writeByte(uint8_t dev, uint8_t reg, uint8_t data) {
    return writeBytes(dev, reg, &data, 1);
}

bool I2C::writeBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint8_t length) {

    Wire.beginTransmission(dev);
    Wire.write(reg); // send address

    uint8_t* p = data;
    uint8_t* e = data + length;

    for (; p < e; p++) {
        Wire.write(p[0]);
    }
    /*
     Return values of Wire.endTransmission()
     0 - Success
     1 - Data too long to fit in transmision buffer
     2 - Received NACK on transmit of address
     3 - Received NACK on transmit of data
     4 - Other
     */
    return 0 == Wire.endTransmission();
}

bool I2C::writeWord(uint8_t dev, uint8_t reg, uint16_t data) {
    return writeWords(dev, reg, &data, 1);
}

bool I2C::writeWords(uint8_t dev, uint8_t reg, uint16_t* data, uint8_t length) {

    Wire.beginTransmission(dev);
    Wire.write(reg); // send address

    uint8_t* p = (uint8_t *) data;
    uint8_t* e = (uint8_t *) data + 2 * length;

    for (; p < e; p += 2) {
        Wire.write(p[1]); // send MSB
        Wire.write(p[0]); // send LSB
    }
    return 0 == Wire.endTransmission();
}
