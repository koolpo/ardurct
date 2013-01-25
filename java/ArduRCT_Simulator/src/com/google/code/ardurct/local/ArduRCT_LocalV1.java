package com.google.code.ardurct.local;

import com.google.code.ardurct.IArduRCTDefines;
import com.google.code.ardurct.libraries.ArduinoFirmware;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Analog;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_Encoder;
import com.google.code.ardurct.libraries.eventManager.ArduRCT_EventHandler;
import com.google.code.ardurct.libraries.eventManager.IEventCallback;
import com.google.code.ardurct.libraries.eventManager.IEventDefines;
import com.google.code.ardurct.libraries.graphics.IGraphicsDefines;
import com.google.code.ardurct.libraries.touscruino.ArduRCT_TouScruinoV1;
import com.google.code.ardurct.libraries.touscruino.ITouscruinoDefines;

public class ArduRCT_LocalV1  extends ArduinoFirmware 
implements IGraphicsDefines, IEventDefines, ITouscruinoDefines, IArduRCTDefines {

	// ------------------ Java specific ------------------------- //
	private switchesCB handleSwitches = new switchesCB();
	private analogCB handleAnalogs = new analogCB();
	private encoderCB handleEncoders = new encoderCB();
	// ------------------ Java specific ------------------------- //

	ArduRCT_TouScruinoV1 touscruino = new ArduRCT_TouScruinoV1();

	ArduRCT_Analog yaw = new ArduRCT_Analog(YAW);
	ArduRCT_Analog roll = new ArduRCT_Analog(ROLL);
	ArduRCT_Analog pitch = new ArduRCT_Analog(PITCH);
	ArduRCT_Analog speed = new ArduRCT_Analog(SPEED);
	ArduRCT_Analog battery = new ArduRCT_Analog(BATTERY);

	ArduRCT_Encoder encoder = new ArduRCT_Encoder(ENCODER_A, ENCODER_B);

	ArduRCT_EventHandler switchesEventHandler = new ArduRCT_EventHandler(EVENT_SWITCH, EVENT_ANY_VALUE, handleSwitches);
	ArduRCT_EventHandler analogEventHandler = new ArduRCT_EventHandler(EVENT_ANALOG, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, handleAnalogs);
	ArduRCT_EventHandler encoderEventHandler = new ArduRCT_EventHandler(EVENT_ENCODER, EVENT_ANY_VALUE, EVENT_ANY_VALUE, EVENT_ANY_VALUE, handleEncoders);

	public void setup() {
		Serial.begin(9600);
		touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touscruino.setMargin(10);
		
		touscruino.registerAnalog(yaw);
		touscruino.registerAnalog(roll);
		touscruino.registerAnalog(pitch);
		touscruino.registerAnalog(speed);
		touscruino.registerAnalog(battery);

		touscruino.registerEncoder(encoder);

		touscruino.registerEventHandler(switchesEventHandler);
		touscruino.registerEventHandler(analogEventHandler);		
		touscruino.registerEventHandler(encoderEventHandler);	
		
		encoder.setMinMax(0, 23);
		encoder.setValue(-1);
		drawValue(0, 0);
	}


	public void loop() {
		delay(1);
		touscruino.update();
	}

	public void drawValue(int value, int change) {
		touscruino.setCursor(0, 0);
		touscruino.print("Position = ");
		touscruino.print(value);
		touscruino.print(" [");
		touscruino.print(change);
		touscruino.print("]    ");		
	}
	
	class switchesCB implements IEventCallback {
		public int handle(int eventType, int buttonId) {
			if (eventType == EVENT_SWITCH_PRESSED) Serial.print("B" + buttonId + "P.");
			else if (eventType == EVENT_SWITCH_REPEATING) Serial.print(".");
			else if (eventType == EVENT_SWITCH_RELEASED) Serial.println("R");
			return EVENT_HANDLING_DONE;
		}
		public int handle(int type, int value, int x, int y) { return EVENT_HANDLING_VOID; }
		public int handle(int type) { return EVENT_HANDLING_VOID; }
	}

	class analogCB implements IEventCallback {
		public int handle(int eventType, int channel, int value, int change) { 
			Serial.println("A" + channel + ":" + value);
			return EVENT_HANDLING_DONE; 
		}
		public int handle(int type) { return EVENT_HANDLING_VOID; }
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
	}

	class encoderCB implements IEventCallback {
		public int handle(int eventType, int id, int value, int change) { 
			drawValue(value, change);
			if (change < 0) Serial.print("-");
			else Serial.print("+");
			return EVENT_HANDLING_DONE; 
		}
		public int handle(int type) { return EVENT_HANDLING_VOID; }
		public int handle(int type, int value) { return EVENT_HANDLING_VOID; }
	}
}
