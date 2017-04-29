
#include <HMC5883L.h>

HMC5883L compass;

void setup() {

    Serial.begin(9600);

    // Inititialize wiring - this isn't done automatically!
    Wire.begin();

    // Initialize compass with default settings
    compass.init();

    // Set measurement gain
    // +/- 0.88 Ga: HMC5883L_GAIN_1370
    // +/- 1.30 Ga: HMC5883L_GAIN_1090 (default)
    // +/- 1.90 Ga: HMC5883L_GAIN_820
    // +/- 2.50 Ga: HMC5883L_GAIN_660
    // +/- 4.00 Ga: HMC5883L_GAIN_440
    // +/- 4.70 Ga: HMC5883L_GAIN_390
    // +/- 5.60 Ga: HMC5883L_GAIN_330
    // +/- 8.10 Ga: HMC5883L_GAIN_230
    compass.setGain(HMC5883L_GAIN_1090);

    // Set data rate
    //  0.75Hz: HMC5883L_DATARATE_0_75HZ
    //  1.50Hz: HMC5883L_DATARATE_1_5HZ
    //  3.00Hz: HMC5883L_DATARATE_3HZ
    //  7.50Hz: HMC5883L_DATARATE_7_50HZ
    // 15.00Hz: HMC5883L_DATARATE_15HZ (default)
    // 30.00Hz: HMC5883L_DATARATE_30HZ
    // 75.00Hz: HMC5883L_DATARATE_75HZ
    compass.setDataRate(HMC5883L_DATARATE_15HZ);

    // Set number of samples averaged
    // 1 sample:  HMC5883L_AVERAGING_1
    // 2 samples: HMC5883L_AVERAGING_2
    // 4 samples: HMC5883L_AVERAGING_4
    // 8 samples: HMC5883L_AVERAGING_8 (default)
    compass.setSampleAveraging(HMC5883L_AVERAGING_1);

    // Set measurement mode
    // Idle mode:              HMC5883L_MODE_IDLE
    // Single-Measurement:     HMC5883L_MODE_SINGLE (default)
    // Continuous-Measurement: HMC5883L_MODE_CONTINUOUS
    compass.setMeasureMode(HMC5883L_MODE_CONTINUOUS);
}

void loop() {

    int16_t x, y, z;

    compass.getMeasureRaw(&x, &y, &z);

    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.println(z);

    delay(67); // 1000 / 15Hz
}
