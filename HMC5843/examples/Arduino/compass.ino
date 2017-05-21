
#include <HMC5843.h>

HMC5843 compass;

void setup() {

    Serial.begin(9600);

    // Inititialize Wire - this isn't done automatically!
    Wire.begin();

    // Initialize compass with default settings
    compass.init();

    // Set measurement mode
    // Idle mode:              HMC5843_MODE_IDLE
    // Single-Measurement:     HMC5843_MODE_SINGLE (default)
    // Continuous-Measurement: HMC5843_MODE_CONTINUOUS
    compass.setMeasureMode(HMC58843_MODE_CONTINUOUS);
}

void loop() {

    int16_t x, y, z;

    // Read raw measure, we can use the raw measure and don't need the
    // scaled magnetic field since the direction is the same
    compass.getRawMeasure(&x, &y, &z);

    // Calculate the heading (azimith) when magnetometer is level (Z-axis is pointing up).
    float headingRad = atan2(y, x);
    float headingDeg;

    // In order to improve accuracy, we need to correct the declination (error of magnetic field at a certain point on earth)
    // Get your value from http://www.magnetic-declination.com/
    // For Stuttgart, Germany it is +2° 26'

    float declination = (2.0 + (26.0 / 60.0)) / (180 / PI);
    headingRad+= declination;

    // Bind angle to [0, 2PI)
    // headingRad is in (-PI, PI] because of atan2. Adding small declination doesn't change things much.
    // That means we must check for negative numbers only
    if (headingRad < 0)
      headingRad+= 2 * PI;

    headingDeg = headingRad * 180.0 / PI;

    Serial.print(headingDeg);
    Serial.print("°\n");

    delay(67); // 1000 / 15Hz
}
