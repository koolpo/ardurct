package com.google.code.ardurct.libraries.touscruino.examples;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class BasicExample extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private touchPanelCB handleTouchPanel = new touchPanelCB();
	private switchesCB handleSwitches = new switchesCB();
	private tickCB handleTick = new tickCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();
	
	ArduRCT_EventHandler switchesEventHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleSwitches);
	ArduRCT_EventHandler touchPanelEventHandler = new ArduRCT_EventHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, handleTouchPanel);
	ArduRCT_EventHandler tickEventHandler = new ArduRCT_EventHandler(EVENT_SYSTEM_TICK, 20, handleTick);
	
	public void setup() {
		Serial.begin(9600);
		touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touscruino.setMargin(2);
		touscruino.setCursor(0, 0);
		touscruino.setRotation(GRAPHICS_ROTATION_90);
		touscruino.print("Select an example from the Examples menu");
		
		touscruino.registerEventHandler(tickEventHandler);
		touscruino.registerEventHandler(switchesEventHandler);
		touscruino.registerEventHandler(touchPanelEventHandler);
	}

	public void loop() {
		delay(5);
		touscruino.update();
	}

	class tickCB implements IEventCallback {
		public boolean handle(int type, int value) {
			//System.out.print(millis() + " ");
			return true;
		}
		public boolean handle(int type, int value, int x, int y) { return false; }
		public boolean handle(int type) { return false; }
	}

	class switchesCB implements IEventCallback {
		public boolean handle(int type, int value) {
			if (type == EVENT_SWITCH_PRESSED) Serial.print("B" + value + "P.");
			else if (type == EVENT_SWITCH_REPEATING) Serial.print(".");
			else if (type == EVENT_SWITCH_RELEASED) Serial.println("R");
			return true;
		}
		public boolean handle(int type, int value, int x, int y) { return false; }
		public boolean handle(int type) { return false; }
	}

	class touchPanelCB implements IEventCallback {
		public boolean handle(int type, int value, int x, int y) {
			if (type == EVENT_TOUCHPANEL_PRESSED) touscruino.fillRectangle(x-1, y-1, 3, 3, BLUE);
			else if (type == EVENT_TOUCHPANEL_DRAGGED) touscruino.fillRectangle(x-1, y-1, 3, 3, RED);
			return true;
		}
		public boolean handle(int type, int value) { return false; }
		public boolean handle(int type) { return false; }
	}
}
