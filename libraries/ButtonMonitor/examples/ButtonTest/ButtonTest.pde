#include <ButtonMonitor.h>

#define BUTTON1 0
#define BUTTON1_PIN 2
#define BUTTON2 1
#define BUTTON2_PIN 3

#define LED 13

void setup() {
    // Monitor the buttons
    ButtonMonitor.add(BUTTON1, BUTTON1_PIN);
    ButtonMonitor.add(BUTTON2, BUTTON2_PIN);
    // set the led as an output
	pinMode(LED, OUTPUT);
}

void loop() {
    // monitor the buttons: needs to be called at least once per BUTTON_MONITOR_DEBOUNCE_DELAY_MS (50ms)
    ButtonMonitor.update();
    // wait between command reads
    delay(5);
    // adjust the led according to the command
	if (ButtonMonitor.isDepressed(BUTTON1)) digitalWrite(LED, HIGH);
	if (ButtonMonitor.isDepressed(BUTTON2)) digitalWrite(LED, LOW);
}