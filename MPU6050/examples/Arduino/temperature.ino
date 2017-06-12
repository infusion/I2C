
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
    Serial.begin(9600);
    mpu.init();
}

void loop() {
    float temp = mpu.getTemperature();

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    delay(500);
}
