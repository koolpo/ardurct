package com.google.code.ardurct.remote;

import com.google.code.ardurct.libraries.XBee;
import com.google.code.ardurct.libraries.touchscreen.ArduinoFirmware;

public class Radio extends ArduinoFirmware {
	
	public static final int MAIN_LOOP_DELAY = 20;

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
	
	public int[] radioFrame = new int[64];
	public int radioFramePtr;
	
	public int battery;
	
	public void radioSetup(int baudrate) {
		xBee.begin(Serial1, baudrate);
	    radioStatus = RADIO_STATUS_UNDEFINED;
	    radioBaudrate = baudrate;
	    radioFramePtr = 0;
	}
	
	public void radioProcess() {	
		if (radioStatus <= RADIO_STATUS_ERROR) return;

		// process pending commands
	    xBee.processCommand();
	    if (xBee.isProcessingCommand()) return;

	    switch (radioStatus) {
	    		
	    	case RADIO_STATUS_READY:
	    		if (xBee.available() <= 0) return;
	    		while (xBee.available() > 0) {
	    			radioFrame[radioFramePtr++] = xBee.read();
	    			if (radioFrame[radioFramePtr-1] == '\n') break;
	    		}
	    		if (radioFrame[radioFramePtr-1] == '\n') radioProcessFrame();
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

	
	public void radioProcessFrame() {
		int i = 0;
		int temp;
		if ((radioFramePtr > 2) && (radioFrame[radioFramePtr-1] == '\n') && (radioFrame[0] == '$')) {
			radioFramePtr = 0;
			// we are receiving ArduRC data
			switch (radioFrame[1]) {
				case 'T':	// testing
					xBee.print("$RX\n");
					break;
				case 'B':	// battery
					xBee.print("$B");
					temp = (battery >> 4) + '0';
					xBee.print((char)(temp > '9' ? temp + 'A' - '0' - 10 : temp));
					temp = (battery & 0x0F) + '0';					
					xBee.print((char)(temp > '9' ? temp + 'A' - '0' - 10 : temp));
					xBee.print((char)'\n');
					break;
				default:
					break;
			}
		}
		while (i < radioFramePtr) Serial.print((char)radioFrame[i++]);
		radioFramePtr = 0;
	}
	

	public void setup() {}
	public void loop() { }

}
