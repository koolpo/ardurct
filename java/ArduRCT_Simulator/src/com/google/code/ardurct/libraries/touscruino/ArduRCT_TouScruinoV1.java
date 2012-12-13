package com.google.code.ardurct.libraries.touscruino;

import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventManager;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_RealTimeClock;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Switch;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventManager.event_t;
import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;


public class ArduRCT_TouScruinoV1 extends ArduRCT_JAVA
implements ITouscruinoDefines {
			
	public ArduRCT_EventManager eventManager = new ArduRCT_EventManager();
	public ArduRCT_RealTimeClock rtc = new ArduRCT_RealTimeClock();
	public ArduRCT_Switch menu = new ArduRCT_Switch(MENU);
	public ArduRCT_Switch up = new ArduRCT_Switch(UP);
	public ArduRCT_Switch down = new ArduRCT_Switch(DOWN);
	public ArduRCT_Switch right = new ArduRCT_Switch(RIGHT);
	public ArduRCT_Switch enter = new ArduRCT_Switch(ENTER);
	
	public ArduRCT_TouScruinoV1() {
		eventManager.setRTC(rtc);
		eventManager.registerSwitch(up);
		eventManager.registerSwitch(down);
		eventManager.registerSwitch(menu);
		eventManager.registerSwitch(right);
		eventManager.registerSwitch(enter);
	}
	
	public void update() {
		eventManager.update();
	}
	
	public boolean hasEvent() {
		return eventManager.hasEvent();
	}
	
	public event_t getEvent() {
		return eventManager.getEvent();
	}
	
	public ArduRCT_RealTimeClock getRTC() {
		return eventManager.getRTC();
	}
	
	public void registerEventHandler(ArduRCT_EventHandler handler) {
		eventManager.registerEventHandler(handler);
	}
}
