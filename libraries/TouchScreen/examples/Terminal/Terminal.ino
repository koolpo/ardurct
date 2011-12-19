#include <TouchScreen.h>

TouchScreen touchscreen();

void setup() {
	Serial.begin(57600);
	touchscreen.begin(SMALL_FONT, BLUE, WHITE);
}

void loop() {
	if (Serial.available() > 0) 
        lcd.write(Serial.read()); 
}