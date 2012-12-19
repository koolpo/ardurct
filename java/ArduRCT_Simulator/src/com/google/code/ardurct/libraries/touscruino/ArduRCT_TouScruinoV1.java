package com.google.code.ardurct.libraries.touscruino;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventManager;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Switch;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_TouchPanel;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;


public class ArduRCT_TouScruinoV1 extends ArduRCT_JAVA
implements ITouscruinoDefines {
			
	ArduRCT_EventManager eventManager = new ArduRCT_EventManager();
	ArduRCT_RealTimeClock rtc = new ArduRCT_RealTimeClock();
	
	ArduRCT_Switch menu = new ArduRCT_Switch(TOUSCRUINO_MENU);
	ArduRCT_Switch up = new ArduRCT_Switch(TOUSCRUINO_UP);
	ArduRCT_Switch down = new ArduRCT_Switch(TOUSCRUINO_DOWN);
	ArduRCT_Switch right = new ArduRCT_Switch(TOUSCRUINO_RIGHT);
	ArduRCT_Switch enter = new ArduRCT_Switch(TOUSCRUINO_ENTER);
	
	ArduRCT_TouchPanel touchPanel = new ArduRCT_TouchPanel();
		
	public ArduRCT_TouScruinoV1() {
		eventManager.setRTC(rtc);
		eventManager.registerSwitch(up);
		eventManager.registerSwitch(down);
		eventManager.registerSwitch(menu);
		eventManager.registerSwitch(right);
		eventManager.registerSwitch(enter);
		eventManager.registerTouchPanel(touchPanel);
	}
	
	public void update() {
		eventManager.update();
	}
		
	public ArduRCT_RealTimeClock getRTC() {
		return eventManager.getRTC();
	}
	
	public void registerEventHandler(ArduRCT_EventHandler handler) {
		eventManager.registerEventHandler(handler);
	}
}
