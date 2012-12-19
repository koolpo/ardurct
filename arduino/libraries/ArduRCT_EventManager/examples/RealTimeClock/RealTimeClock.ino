#include <ArduRCT_EventManager.h>

// create a soft RealTimeClock
ArduRCT_RealTimeClock rtc(2012, DECEMBER, 17, 12, 18, 0);

// create an eventManager with a RTC
ArduRCT_EventManager eventManager(&rtc);

// call outputTime every second
ArduRCT_EventHandler timeHandler(EVENT_TIME_SECOND, &outputTime); 

void setup() {
    Serial.begin(57600);
    // register the timeHandler with the eventManager
    eventManager.registerEventHandler(&timeHandler);
}

void loop() {
    eventManager.update();
}

bool outputTime(uint8_t eventType) {
    Serial.print((char *)(eventManager.getRTC()->getDateAsString(RTC_WITH_DAY_OF_WEEK)));
    Serial.println((char *)(eventManager.getRTC()->getTimeAsString(RTC_WITH_SECONDS)));
}