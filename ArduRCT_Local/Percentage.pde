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
            value = 1024-analogRead(JOYSTICK_YAW_PIN);
            break;
        case PITCH:
            value = analogRead(JOYSTICK_PITCH_PIN);
            break;
        case ROLL:
            value = analogRead(JOYSTICK_ROLL_PIN);
            break;
        case ADJUST:
            value = analogRead(ADJUST_PIN);
            break;
        case BATTERY:
            value = analogRead(BATTERY_PIN);
            break;
        default:
            value = 0;
            break;
    }
    return 100 * value / 1023;
}
