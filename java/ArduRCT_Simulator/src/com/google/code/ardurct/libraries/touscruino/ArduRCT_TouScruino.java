package com.google.code.ardurct.libraries.touscruino;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_Analog;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Encoder;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventManager;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Switch;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_TouchPanel;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.eventManager.IEventDefines;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;

public class ArduRCT_TouScruino extends ArduRCT_JAVA
implements ITouscruinoDefines, IEventDefines {
			
	ArduRCT_EventManager eventManager = new ArduRCT_EventManager();
	ArduRCT_RealTimeClock rtc = new ArduRCT_RealTimeClock();
	
	ArduRCT_Switch menu = new ArduRCT_Switch(TOUSCRUINO_MENU);
	ArduRCT_Switch up = new ArduRCT_Switch(TOUSCRUINO_UP);
	ArduRCT_Switch down = new ArduRCT_Switch(TOUSCRUINO_DOWN);
	ArduRCT_Switch right = new ArduRCT_Switch(TOUSCRUINO_RIGHT);
	ArduRCT_Switch enter = new ArduRCT_Switch(TOUSCRUINO_ENTER);
	
	ArduRCT_TouchPanel touchPanel = new ArduRCT_TouchPanel();
		
	public ArduRCT_TouScruino() {
		eventManager.setRTC(rtc);
		eventManager.registerSwitch(up);
		eventManager.registerSwitch(down);
		eventManager.registerSwitch(menu);
		eventManager.registerSwitch(right);
		eventManager.registerSwitch(enter);
		eventManager.registerEventHandler(new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, uiManager));
		eventManager.registerTouchPanel(touchPanel);
		eventManager.registerEventHandler(new ArduRCT_EventHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, uiManager));

	}
		
	public void manageEvents() {
		eventManager.manageEvents();
	}
		
	public ArduRCT_RealTimeClock getRTC() {
		return eventManager.getRTC();
	}
	
	public void registerEventHandler(ArduRCT_EventHandler handler) {
		eventManager.registerEventHandler(handler);
	}
	
	public void registerSwitch(ArduRCT_Switch aSwitch) {
		eventManager.registerSwitch(aSwitch);	
	}

	public void registerEncoder(ArduRCT_Encoder anEncoder) {
		eventManager.registerEncoder(anEncoder);	
	}

	public void registerAnalog(ArduRCT_Analog anAnalog) {
		eventManager.registerAnalog(anAnalog);
	}
	
	uiManagementCB uiManager = new uiManagementCB();
	class uiManagementCB implements IEventCallback {
		public int handle(int eventId, int switchId) {
			int actionId = 0;
			if ((switchId != TOUSCRUINO_MENU) && (switchId != TOUSCRUINO_ENTER) && (switchId != TOUSCRUINO_UP) && 
					(switchId != TOUSCRUINO_DOWN)&& (switchId != TOUSCRUINO_RIGHT)) return EVENT_HANDLING_DONE;
			if (eventId == EVENT_SWITCH_RELEASED) {
				if (switchId == TOUSCRUINO_ENTER) actionId = GRAPHICS_UI_ACTION_RELEASE;
				else if (switchId == TOUSCRUINO_MENU) actionId = GRAPHICS_UI_ACTION_MENU;
			} else if (eventId == EVENT_SWITCH_REPEATING) {
				if (switchId == TOUSCRUINO_DOWN) actionId = GRAPHICS_UI_ACTION_DOWN;
				else if (switchId == TOUSCRUINO_UP) actionId = GRAPHICS_UI_ACTION_UP;
			} else if (eventId == EVENT_SWITCH_PRESSED) {
				if (switchId == TOUSCRUINO_DOWN) actionId = GRAPHICS_UI_ACTION_DOWN;
				else if (switchId == TOUSCRUINO_UP) actionId = GRAPHICS_UI_ACTION_UP;
				else if (switchId == TOUSCRUINO_ENTER) actionId = GRAPHICS_UI_ACTION_ENTER;
			}
			if (actionId == 0) return EVENT_HANDLING_DONE;
			if (handleGraphicsUI(actionId, 0, 0)) return EVENT_HANDLING_EXIT;
			return EVENT_HANDLING_DONE;
		}
		
		public int handle(int eventId, int value, int x, int y) { 
			int actionId = 0;
			if (eventId == EVENT_TOUCHPANEL_PRESSED) actionId = GRAPHICS_UI_ACTION_TOUCH;
			else if (eventId == EVENT_TOUCHPANEL_RELEASED) actionId = GRAPHICS_UI_ACTION_UNTOUCH;
			else if (eventId == EVENT_TOUCHPANEL_DRAGGED) actionId = GRAPHICS_UI_ACTION_DRAG;
			if (actionId == 0) return EVENT_HANDLING_DONE;
			if (handleGraphicsUI(actionId, x, y)) return EVENT_HANDLING_EXIT;
			return EVENT_HANDLING_DONE; 
		}
		
		public int handle(int type) { 
			return EVENT_HANDLING_VOID; 
		}
	}
}
