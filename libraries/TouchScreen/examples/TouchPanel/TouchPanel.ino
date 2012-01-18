#include <TouchScreen.h>

TouchScreen touchscreen;

void setup() {
    Serial.begin(57600);
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD);
    touchscreen.setupTouchPanel();
    touchscreen.setBacklight(180);
}

void loop() {
    int16_t x, y, z;
    
    if (touchscreen.getTouchXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) {
        Serial.print("x=");
        Serial.print(x);
        Serial.print(", y=");
        Serial.print(y);
        Serial.print(", z=");
        Serial.println(z);
        touchscreen.fillRectangle(x-10, y-10, 20, 20, BLUE);
    }
    delay(100);
}