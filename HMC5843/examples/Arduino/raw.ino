
#include <HMC5883L.h>

HMC5843 compass;

void setup() {

    Serial.begin(9600);

    // Inititialize wiring - this isn't done automatically!
    Wire.begin();

    // Initialize compass with default settings
    compass.init();

    compass.setMeasureMode(HMC5843_MODE_CONTINUOUS);
}

void loop() {

    int16_t x, y, z;

    compass.getRawMeasure(&x, &y, &z);

    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.println(z);

    delay(100); // 1000 / 10Hz
}
