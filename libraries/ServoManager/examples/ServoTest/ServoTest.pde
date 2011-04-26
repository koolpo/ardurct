#include <ServoManager.h>

#define THROTTLE 0
#define THROTTLE_SERVO_PIN 2

void setup() {
    // Attach the THROTTLE servo to THROTTLE_SERVO_PIN
    ServoManager.attach(THROTTLE, THROTTLE_SERVO_PIN);
    // set it to 2000 microseconds
    ServoManager.set(THROTTLE, 2000);
}

void loop() {
    // run the servo manager: needs to be called at least once per 20ms (50Hz)
    ServoManager.run();
}