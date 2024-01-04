
#include <I2C.h>

/**
 *
 */
void I2C::init()
{
#ifdef ARDUINO
  Wire.begin();
#endif

#ifdef RASPBERRY

  if ((fd = open(I2CADDR, O_RDWR)) < 0)
  {
    printf("%s\n", strerror(errno));
    return;
  }
#endif
}

/**
 *
 */
void I2C::close()
{
#ifdef RASPBERRY
  if (fd > 0)
  {
    ::close(fd);
    fd = -1;
  }
#endif
}

/**
 * Sleep for d ms
 */
void I2C::delay(uint16_t d)
{
#ifdef ARDUINO
  delay(d);
#endif
#ifdef RASPBERRY
  usleep(d * 1000);
#endif
}

/**
 * Sleep for d us
 */
void I2C::udelay(unsigned int d)
{
#ifdef ARDUINO
  delayMicroseconds(d);
#endif
#ifdef RASPBERRY
  usleep(d);
#endif
}

/**
 *
 */
void I2C::setFastMode()
{
  // Must be called before Wire.begin()!
#ifdef ARDUINO
  Wire.setClock(400000);
#endif
}

/**
 *
 */
bool I2C::readBit(devid_t dev, uint8_t reg, uint8_t bit, uint8_t *data)
{
  uint8_t prev;
  bool status = readBytes(dev, reg, &prev, 1);
  *data = prev & (1 << bit);
  return status;
}

/**
 *
 */
bool I2C::readByte(devid_t dev, uint8_t reg, uint8_t *data)
{
  return readBytes(dev, reg, data, 1);
}

/**
 *
 */
bool I2C::readBytes(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length)
{
#ifdef ARDUINO
  uint8_t *start = data;

  // if (length > BUFFER_LENGTH)
  //     return 0;

  Wire.beginTransmission(dev); // Initialize Tx buffer
  Wire.write(reg);             // Put slave address in Tx buffer
  Wire.endTransmission(false); // Send Tx buffer and send restart

  Wire.requestFrom(dev, length, (uint8_t) true);

  while (Wire.available())
  {
    *data++ = Wire.read();
  }
  return length == (data - start);
#endif

#ifdef RASPBERRY

  // Bind channel to given address
  if (ioctl(fd, I2C_SLAVE, dev) < 0)
  {
    printf("%s\n", strerror(errno));
    return false;
  }

  write(fd, &reg, 1);

  if (read(fd, data, length) != length)
  {
    printf("%s\n", strerror(errno));
    return false;
  }
  return true;
#endif
}

/**
 *
 */
bool I2C::readBytesStop(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length)
{
#ifdef ARDUINO
  uint8_t *start = data;

  // if (length > BUFFER_LENGTH)
  //     return 0;

  Wire.beginTransmission(dev);
  Wire.write(reg);
  Wire.endTransmission(true);

  Wire.requestFrom(dev, length, (uint8_t) true);
  // delayMicroseconds(10)
  while (Wire.available())
  {
    *data++ = Wire.read();
  }
  return length == (data - start);
#endif

#ifdef RASPBERRY
  // TODO
  return readBytes(dev, reg, data, length);
#endif
}

/**
 *
 */
bool I2C::readWord(devid_t dev, uint8_t reg, uint16_t *data)
{
  return readWords(dev, reg, data, 1);
}

/**
 *
 */
bool I2C::readWords(devid_t dev, uint8_t reg, uint16_t *data, uint8_t length)
{
  length *= 2;

#ifdef ARDUINO
  uint8_t i = 0;
  uint8_t *ptr = (uint8_t *)data;

  // if (length > BUFFER_LENGTH)
  //     return 0;

  Wire.beginTransmission(dev);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(dev, length, (uint8_t) true);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  while (Wire.available())
  {
    ptr[i++ ^ 1] = Wire.read();
  }
#else
#error "readWords is not implemented for big endian"
#endif
  return length == i;
#endif

#ifdef RASPBERRY
  // Bind channel to given address
  if (ioctl(fd, I2C_SLAVE, dev) < 0)
  {
    printf("%s\n", strerror(errno));
    return false;
  }
  write(fd, &reg, 1);

  if (read(fd, data, length) != length)
  {
    printf("%s\n", strerror(errno));
    return false;
  }
  for (uint8_t i = 0; i < length; i += 2)
  {
    *((uint16_t *)(data + i)) = __builtin_bswap16(*((uint16_t *)(data + i)));
  }
  return true;
#endif
}

/**
 *
 */
bool I2C::writeBit(devid_t dev, uint8_t reg, uint8_t bit, bool value)
{
  uint8_t prev;
  readBytes(dev, reg, &prev, 1);

  uint8_t val = SET_BIT_IN_BYTE(prev, bit, value);

  if (val == prev)
  {
    return true;
  }
  return writeByte(dev, reg, val);
}

/**
 *
 */
bool I2C::writeByte(devid_t dev, uint8_t reg, uint8_t data)
{
#ifdef ARDUINO
  Wire.beginTransmission(dev); // Initialize Tx buffer
  Wire.write(reg);             // Put slave address in Tx buffer

  Wire.write(data); // Put data in Tx buffer

  return 0 == Wire.endTransmission(); // Send Tx buffer
#endif

#ifdef RASPBERRY
  // Bind channel to given address
  if (ioctl(fd, I2C_SLAVE, dev) < 0)
  {
    printf("%s\n", strerror(errno));
    return false;
  }
  write(fd, &reg, 1);
  if (write(fd, &data, 1) < 1)
  {
    return false;
  }
  return true;
#endif
}

/**
 *
 */
bool I2C::writeBytes(devid_t dev, uint8_t reg, uint8_t *data, uint8_t length)
{
#ifdef ARDUINO
  Wire.beginTransmission(dev); // Initialize Tx buffer
  Wire.write(reg);             // Put slave address in Tx buffer

  uint8_t *p = data;
  uint8_t *e = data + length;

  while (p < e)
  {
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
#endif

#ifdef RASPBERRY
  // Bind channel to given address
  if (ioctl(fd, I2C_SLAVE, dev) < 0)
  {
    printf("%s\n", strerror(errno));
    return false;
  }
  write(fd, &reg, 1);
  if (write(fd, data, length) < length)
  {
    return false;
  }
  return true;
#endif
}

/**
 *
 */
bool I2C::writeWord(devid_t dev, uint8_t reg, uint16_t data)
{
  return writeWords(dev, reg, &data, 1);
}

/**
 *
 */
bool I2C::writeWords(devid_t dev, uint8_t reg, uint16_t *data, uint8_t length)
{
#ifdef ARDUINO
  Wire.beginTransmission(dev);
  Wire.write(reg); // send address

  uint8_t *p = (uint8_t *)data;
  uint8_t *e = (uint8_t *)data + 2 * length;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (; p < e; p += 2)
  {
    Wire.write(p[1]); // send MSB
    Wire.write(p[0]); // send LSB
  }
#else
#error "writeWords is not implemented for big endian"
#endif
  return 0 == Wire.endTransmission();
#endif

#ifdef RASPBERRY
//#error "writeWords is not implemented for raspberry"
#endif
}
