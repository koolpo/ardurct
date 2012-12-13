package com.google.code.ardurct.libraries.touscruino.examples;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventManager.event_t;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class BasicExample extends TouscruinoFirmware {
	
	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();
	
	public void setup() {
		Serial.begin(9600);
		touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touscruino.setMargin(2);
		touscruino.setCursor(0, 0);
		touscruino.print("Select an example from the Examples menu");
	}

	public void loop() {
		delay(5);
		touscruino.update();
		while (touscruino.hasEvent()) {
			event_t evt = touscruino.getEvent();
			if ((evt.type & EVENT_BUTTON) != 0) {
				if (evt.type == EVENT_BUTTON_PRESSED) Serial.print("B" + evt.value + "P ");
				else if (evt.type == EVENT_BUTTON_REPEATING) Serial.print(".");
				else if (evt.type == EVENT_BUTTON_RELEASED) Serial.println(" R");
			}
		}
	}

}
