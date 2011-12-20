#include <TouchScreen.h>

TouchScreen touchscreen(TOUSCRUINO_PORT, TOUSCRUINO_CD, TOUSCRUINO_WR, TOUSCRUINO_RD, TOUSCRUINO_CS, TOUSCRUINO_RESET);

void setup() {
    Serial.begin(57600);
    touchscreen.begin(FONT_SMALL, BLUE, WHITE);
}

void loop() {
    if (Serial.available() > 0) 
        touchscreen.write(Serial.read()); 
}