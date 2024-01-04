
#include <ADXL345.h>

ADXL345 accel;

void setup() {

    Serial.begin(38400);

    accel.init();
}

void loop() {

    int16_t x, y, z;

    accel.getAccelerationRaw(&x, &y, &z);

    Serial.print("x: ");
    Serial.print(x);
    Serial.print("\ty: ");
    Serial.print(y);
    Serial.print("\tz: ");
    Serial.println(z);

    delay(10); // 1000 / 100Hz
}
