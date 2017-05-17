
#include <Nunchuk.h>

Nunchuk chuk;

void setup() {

    Serial.begin(9600);
    Wire.begin();
    // chuk.initPower(); // A1 and A2 is power supply
    chuk.init();
}

void loop() {

    if (chuk.read()) {

#ifdef DEBUG
    Serial.print("joy: ");
    Serial.print(chuk.joystickX());
    Serial.print(", ");
    Serial.print(chuk.joystickY());

    Serial.print("  acc:");
    Serial.print(chuk.accelX());
    Serial.print(", ");
    Serial.print(chuk.accelY());
    Serial.print(", ");
    Serial.print(chuk.accelZ());

    Serial.print("  but:");
    Serial.print(chuk.buttonZ());
    Serial.print(", ");
    Serial.print(chuk.buttonC());
    Serial.print("\n");
#else
    Serial.print(chuk.joystickX());
    Serial.print(",");
    Serial.print(chuk.joystickY());
    Serial.print(",");
    Serial.print(chuk.accelX());
    Serial.print(",");
    Serial.print(chuk.accelY());
    Serial.print(",");
    Serial.print(chuk.accelZ());
    Serial.print(",");
    Serial.print(chuk.buttonZ());
    Serial.print(",");
    Serial.print(chuk.buttonC());
    Serial.print("\n");
#endif
    }
    delay(10);
}
