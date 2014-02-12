package com.google.code.ardurct.libraries.touscruino.examplesV2;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruino;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class BasicExample extends TouscruinoFirmware {

	// ------------------ Java specific ------------------------- //
	private touchPanelCB handleTouchPanel = new touchPanelCB();
	private tickCB handleTick = new tickCB();

	ArduRCT_TouScruino Touscruino = new ArduRCT_TouScruino();
	// ------------------ Java specific ------------------------- //
	
	ArduRCT_EventHandler touchPanelEventHandler = new ArduRCT_EventHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, handleTouchPanel);
	ArduRCT_EventHandler tickEventHandler = new ArduRCT_EventHandler(EVENT_SYSTEM_TICK, 20, handleTick);
	
	public void setup() {
		Serial.begin(9600);
		Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		Touscruino.setMargin(2);
		Touscruino.setCursor(0, 0);
		//touscruino.setRotation(GRAPHICS_ROTATION_90);
		Touscruino.print("Select an item from the Examples menu");
		
		Touscruino.registerEventHandler(tickEventHandler);
		Touscruino.registerEventHandler(touchPanelEventHandler);
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

	class touchPanelCB implements IEventCallback {
		public int handle(int type, int value, int x, int y) {
			if (type == EVENT_TOUCHPANEL_PRESSED) Touscruino.fillRectangle(x-1, y-1, 3, 3, BLUE);
			else if (type == EVENT_TOUCHPANEL_DRAGGED) Touscruino.fillRectangle(x-1, y-1, 3, 3, RED);
			return EVENT_HANDLING_DONE;
		}
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
		public int handle(int type) { return EVENT_HANDLING_VOID; }
	}
}
