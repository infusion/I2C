#include <Wire.h>
#include "MCP23008.h"

MCP23008 io;
  
void setup() {
  
  Wire.begin();

  io.init();

  io.setPinMode(6, INPUT);
  io.setPullUpMode(6, HIGH);  // turn on 100K pullup internally

  pinMode(13, OUTPUT);  // use the board LED for debugging
}

void loop() {
  digitalWrite(13, io.getPin(6));
  delay(10);
}