#include "ArduRCT_Local.h"

void percentageSetup() {
    analogReference(EXTERNAL);
}

uint8_t percentageGet(uint8_t channel) {
    uint32_t value;
    switch(channel) {
        case THROTTLE:
            value = analogRead(JOYSTICK_THROTTLE_PIN);
            break;
        case YAW:
            value = analogRead(JOYSTICK_YAW_PIN);
            break;
        case PITCH:
            value = 1024-analogRead(JOYSTICK_PITCH_PIN);
            break;
        case ROLL:
            value = 1024-analogRead(JOYSTICK_ROLL_PIN);
            break;
        case ADJUST:
            value = analogRead(ADJUST_PIN);
            break;
        case BATTERY:
            value = analogRead(BATTERY_PIN);
            break;
    }
    return 100 * value / 1024;
}
