#include <TouchScreen.h>

TouchScreen touchscreen;

uint8_t seven[] = "ld 0 0 22 0 ld 0 9 arnww 38";
uint8_t buffer[256];
uint8_t bufferPtr = 0;

void setup() {
    Serial.begin(57600);
    
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touchscreen.setBacklight(180);

    touchscreen.executeMacro(seven, 20, 20);
    touchscreen.executeMacro(seven, 10, 100, 2);
    
    delay(3000);
    
    touchscreen.setBacklight(0);
    touchscreen.fillScreen(WHITE);
    touchscreen.setBacklight(180);
}

void loop() {
    while (Serial.available()) buffer[bufferPtr++] = Serial.read();
    if (bufferPtr == 0) return;
    
    // wait for end of sentence
    if (buffer[bufferPtr-1] == '.') {
        // mark end of macro
        buffer[bufferPtr-1] = 0;
        bufferPtr = 0;
        // execute the macro
        touchscreen.executeMacro(buffer, 10, 10);
    }
    delay(10);
}


