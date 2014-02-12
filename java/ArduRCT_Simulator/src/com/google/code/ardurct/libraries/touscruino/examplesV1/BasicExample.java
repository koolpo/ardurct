package com.google.code.ardurct.libraries.touscruino.examplesV1;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class BasicExample extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private switchesCB handleSwitches = new switchesCB();
	private tickCB handleTick = new tickCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruino Touscruino = new ArduRCT_TouScruino();
	
	ArduRCT_EventHandler switchesEventHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleSwitches);
	ArduRCT_EventHandler tickEventHandler = new ArduRCT_EventHandler(EVENT_SYSTEM_TICK, 20, handleTick);
	
	public void setup() {
		Serial.begin(9600);
		Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		Touscruino.setMargin(2);
		Touscruino.setCursor(0, 0);
		//touscruino.setRotation(GRAPHICS_ROTATION_90);
		Touscruino.print("Select an item from the Examples menu");
		
		Touscruino.registerEventHandler(tickEventHandler);
		Touscruino.registerEventHandler(switchesEventHandler);
	}

	public void loop() {
		delay(5);
		Touscruino.manageEvents();
	}

	class tickCB implements IEventCallback {
		public int handle(int type, int value) {
			//System.out.print(millis() + " ");
			return EVENT_HANDLING_DONE;
		}
		public int handle(int type, int value, int x, int y) { return EVENT_HANDLING_VOID; }
		public int handle(int type) { return EVENT_HANDLING_VOID; }
	}

	class switchesCB implements IEventCallback {
		public int handle(int type, int value) {
			if (type == EVENT_SWITCH_PRESSED) Serial.print("B" + value + "P.");
			else if (type == EVENT_SWITCH_REPEATING) Serial.print(".");
			else if (type == EVENT_SWITCH_RELEASED) Serial.println("R");
			return EVENT_HANDLING_DONE;
		}
		public int handle(int type, int value, int x, int y) { return EVENT_HANDLING_VOID; }
		public int handle(int type) { return EVENT_HANDLING_VOID; }
	}
}
