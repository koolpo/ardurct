package com.google.code.ardurct.remote;

import com.google.code.ardurct.libraries.XBee;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class Radio extends ArduinoFirmware {
	
	public static final int RADIO_STATUS_ERROR = -1;
	public static final int RADIO_STATUS_BAUDRATE_ERROR = -2;
	public static final int RADIO_STATUS_CONNECTION_ERROR = -3;
	public static final int RADIO_STATUS_UNDEFINED = 0;
	public static final int RADIO_STATUS_FINALIZING = 110;
	public static final int RADIO_STATUS_READY = 100;
	public static final int RADIO_STATUS_ENTERING_COMMAND_MODE = 1;
			
	public XBee xBee = new XBee();
	
	public int radioStatus;
	public int radioBaudrate;
	
	public void radioSetup(int baudrate) {
		xBee.begin(Serial1, baudrate);
	    radioStatus = RADIO_STATUS_UNDEFINED;
	    radioBaudrate = baudrate;
	}
	
	public void radioProcess() {	
		if (radioStatus <= RADIO_STATUS_ERROR) return;

		// process pending commands
	    xBee.processCommand();
	    if (xBee.isProcessingCommand()) return;

	    switch (radioStatus) {
	    		
	    	case RADIO_STATUS_READY:
	    		radioProcessData();
	    		break;
	    		
	    	case RADIO_STATUS_UNDEFINED:
	    		xBee.enterCommandMode();
	    		radioStatus = RADIO_STATUS_ENTERING_COMMAND_MODE;
	    		break;
	    		
	    	case RADIO_STATUS_ENTERING_COMMAND_MODE:
	    		if (!xBee.isInCommandMode()) {
	    			radioStatus = RADIO_STATUS_BAUDRATE_ERROR;
	    			return;
	    		}
	    		xBee.exitCommandMode();
	    		radioStatus = RADIO_STATUS_FINALIZING;
	    		break;
	    		
	    	case RADIO_STATUS_FINALIZING:
	    		radioStatus = RADIO_STATUS_READY;
	    		break;

	    	default:
	    		break;
	    }
	}

	
	public void radioProcessData() {
		if (xBee.available() > 0) return;
		while (xBee.available() > 0) Serial.print((char)xBee.read());
	}
	

	public void setup() {}
	public void loop() { }

}
