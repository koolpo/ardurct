#include <TouchScreen.h>

TouchScreen touchscreen;

void setup() {
    Serial.begin(57600);
    touchscreen.begin(WHITE, BLUE, FONT_MEDIUM, FONT_BOLD);
}

void loop() {
    if (Serial.available() > 0) touchscreen.write(Serial.read()); 
}