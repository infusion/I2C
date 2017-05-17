
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
    Serial.print(chuk.getJoystickX());
    Serial.print(", ");
    Serial.print(chuk.getJoystickY());

    Serial.print("  acc:");
    Serial.print(chuk.getAccelX());
    Serial.print(", ");
    Serial.print(chuk.getAccelY());
    Serial.print(", ");
    Serial.print(chuk.getAccelZ());

    Serial.print("  but:");
    Serial.print(chuk.getButtonZ());
    Serial.print(", ");
    Serial.print(chuk.getButtonC());
    Serial.print("\n");
#else
    Serial.print(chuk.getJoystickX());
    Serial.print(",");
    Serial.print(chuk.getJoystickY());
    Serial.print(",");
    Serial.print(chuk.getAccelX());
    Serial.print(",");
    Serial.print(chuk.getAccelY());
    Serial.print(",");
    Serial.print(chuk.getAccelZ());
    Serial.print(",");
    Serial.print(chuk.getButtonZ());
    Serial.print(",");
    Serial.print(chuk.getButtonC());
    Serial.print("\n");
#endif
    }
    delay(10);
}
