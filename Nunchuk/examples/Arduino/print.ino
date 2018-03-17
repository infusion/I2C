
#include <Wire.h>
#include <Nunchuk.h>

Nunchuk nc;

void setup() {

    Serial.begin(115200);
    Wire.begin();
    // chuk.initPower(); // A1 and A2 is power supply
    nc.init();
}

void loop() {

    if (nc.read()) {

#ifdef DEBUG
    Serial.print("joy: ");
    Serial.print(nc.getJoystickX());
    Serial.print(", ");
    Serial.print(nc.getJoystickY());

    Serial.print("  acc:");
    Serial.print(nc.getAccelX());
    Serial.print(", ");
    Serial.print(nc.getAccelY());
    Serial.print(", ");
    Serial.print(nc.getAccelZ());

    Serial.print("  but:");
    Serial.print(nc.getButtonZ());
    Serial.print(", ");
    Serial.print(nc.getButtonC());
    Serial.print("\n");
#else
    Serial.print(nc.getJoystickX());
    Serial.print(",");
    Serial.print(nc.getJoystickY());
    Serial.print(",");
    Serial.print(nc.getAccelX());
    Serial.print(",");
    Serial.print(nc.getAccelY());
    Serial.print(",");
    Serial.print(nc.getAccelZ());
    Serial.print(",");
    Serial.print(nc.getButtonZ());
    Serial.print(",");
    Serial.print(nc.getButtonC());
    Serial.print("\n");
#endif
    }
    delay(50);
}
