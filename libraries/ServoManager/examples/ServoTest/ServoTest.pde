#include <ServoManager.h>

#define THROTTLE 0
#define THROTTLE_SERVO_PIN 4
#define THROTTLE_COMMAND A0

void setup() {
    // Attach the THROTTLE servo to THROTTLE_SERVO_PIN
    ServoManager.attach(THROTTLE, THROTTLE_SERVO_PIN);
    // set it to 2000 microseconds
    ServoManager.set(THROTTLE, 2000);
}

void loop() {
    // run the servo manager: needs to be called at least once per 20ms (50Hz)
    ServoManager.run();
    // wait between command reads
    delay(5);
    // adjust the servo position according to the command
    ServoManager.mapSet(THROTTLE, analogRead(THROTTLE_COMMAND), 0, 1023);
}