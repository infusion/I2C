
#include <HMC5883L.h>

HMC5883L compass;

void displaySettings() {

    float gain = 0.0, rate = 0.0;

    uint8_t samples = 0;

    switch (compass.getGain()) {
        case HMC5883L_GAIN_1370:
            gain = 0.88;
            break;
        case HMC5883L_GAIN_1090:
            gain = 1.30;
            break;
        case HMC5883L_GAIN_820:
            gain = 1.90;
            break;
        case HMC5883L_GAIN_660:
            gain = 2.50;
            break;
        case HMC5883L_GAIN_440:
            gain = 4.00;
            break;
        case HMC5883L_GAIN_390:
            gain = 4.70;
            break;
        case HMC5883L_GAIN_330:
            gain = 5.60;
            break;
        case HMC5883L_GAIN_230:
            gain = 8.10;
            break;
    }

    switch (compass.getDataRate()) {
        case HMC5883L_DATARATE_0_75HZ:
            rate = 0.75;
            break;
        case HMC5883L_DATARATE_1_5HZ:
            rate = 1.50;
            break;
        case HMC5883L_DATARATE_3HZ:
            rate = 3.00;
            break;
        case HMC5883L_DATARATE_7_50HZ:
            rate = 7.50;
            break;
        case HMC5883L_DATARATE_15HZ:
            rate = 15.00;
            break;
        case HMC5883L_DATARATE_30HZ:
            rate = 30.00;
            break;
        case HMC5883L_DATARATE_75HZ:
            rate = 75.00;
            break;
    }

    switch (compass.getSampleAveraging()) {
        case HMC5883L_AVERAGING_1:
            samples = 1;
            break;
        case HMC5883L_AVERAGING_2:
            samples = 2;
            break;
        case HMC5883L_AVERAGING_4:
            samples = 4;
            break;
        case HMC5883L_AVERAGING_8:
            samples = 8;
            break;

    }

    Serial.print("HMC5883L settings (");

    switch (compass.getMeasureMode()) {
        case HMC5883L_MODE_IDLE:
            Serial.print("IDLE");
            break;
        case HMC5883L_MODE_SINGLE:
            Serial.print("SINGLE");
            break;
        case HMC5883L_MODE_CONTINUOUS:
            Serial.print("CONTINUOUS");
            break;
    }

    Serial.print(")\nGain: +/-");
    Serial.print(gain);
    Serial.print(" Ga, ");

    Serial.print("Data Rate: ");
    Serial.print(rate);
    Serial.print(" Hz, ");

    Serial.print("Sample Averaging: ");
    Serial.print(samples);
    Serial.print(" samples\n");
}

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
    compass.setGain(HMC5883L_GAIN_390);

    // Set data rate
    //  0.75Hz: HMC5883L_DATARATE_0_75HZ
    //  1.50Hz: HMC5883L_DATARATE_1_5HZ
    //  3.00Hz: HMC5883L_DATARATE_3HZ
    //  7.50Hz: HMC5883L_DATARATE_7_50HZ
    // 15.00Hz: HMC5883L_DATARATE_15HZ (default)
    // 30.00Hz: HMC5883L_DATARATE_30HZ
    // 75.00Hz: HMC5883L_DATARATE_75HZ
    compass.setDataRate(HMC5883L_DATARATE_30HZ);

    // Set number of samples averaged
    // 1 sample:  HMC5883L_AVERAGING_1
    // 2 samples: HMC5883L_AVERAGING_2
    // 4 samples: HMC5883L_AVERAGING_4
    // 8 samples: HMC5883L_AVERAGING_8 (default)
    compass.setSampleAveraging(HMC5883L_AVERAGING_4);

    // Set measurement mode
    // Idle mode:              HMC5883L_MODE_IDLE
    // Single-Measurement:     HMC5883L_MODE_SINGLE (default)
    // Continuous-Measurement: HMC5883L_MODE_CONTINUOUS
    compass.setMeasureMode(HMC5883L_MODE_SINGLE);


    // Now display the settings
    displaySettings();
}

void loop() {

    delay(1000);
}
