#include <TouchScreen.h>

TouchScreen touchscreen(TOUSCRUINO_PORT, TOUSCRUINO_CD, TOUSCRUINO_WR, TOUSCRUINO_RD, TOUSCRUINO_CS, TOUSCRUINO_RESET);

void setup() {
    Serial.begin(57600);
    touchscreen.setupTouchPanel(TOUSCRUINO_XM, TOUSCRUINO_XP, TOUSCRUINO_YM, TOUSCRUINO_YP, TOUSCRUINO_X_PLANE_RESISTANCE, TOUSCRUINO_PRESSURE_THRESHOLD);
    touchscreen.begin(FONT_SMALL, BLUE, WHITE);
}

void loop() {
    uint16_t x, y, z;
    
    if (touchscreen.getTouchedXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) {
        Serial.print("x=");
        Serial.print(x);
        Serial.print(", y=");
        Serial.print(y);
        Serial.print(", z=");
        Serial.println(z);
        uint16_t size = z/2;
        touchscreen.fillRectangle(x-size, y-size, x+size, y+size, WHITE);
    }
    delay(50);
}