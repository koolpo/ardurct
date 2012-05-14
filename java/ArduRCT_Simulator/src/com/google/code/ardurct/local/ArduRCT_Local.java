package com.google.code.ardurct.local;

import com.google.code.ardurct.libraries.touchscreen.IUIActionCallback;

public class ArduRCT_Local extends Radio {
	
	static final int BAUDRATES[] = { 9600, 19200, 38400, 57600 };
	
	static final long RADIO_WAIT_INACTION_BEFORE_COMMIT_DELAY = 10000;
	
	static final int RADIO_SPEED_9600 = 10;
	static final int RADIO_SPEED_19200 = 11;
	static final int RADIO_SPEED_38400 = 12;
	static final int RADIO_SPEED_57600 = 13;
	static final int RADIO_SPEED = 1;
	
	static final int PAN_ID_LABEL = 19;
	static final int PAN_ID_1 = 20;
	static final int PAN_ID_2 = 21;
	static final int PAN_ID_3 = 22;
	static final int PAN_ID_4 = 23;
	static final int PAN_ID_PLUS = 24;
	static final int PAN_ID_MINUS = 25;
	static final int PAN_ID = 2;
	
	static final int ADDRESS_RESET = 29;
	//public static final int LOCAL_ADDRESS = 30;
	//public static final int REMOTE_ADDRESS_1 = 31;
	//public static final int REMOTE_ADDRESS_2 = 32;
	//public static final int REMOTE_ADDRESS_3 = 33;
	
	public int radioTab;
	public int telemetryTab;
	public int optionsTab;
	public long lastRadioUIChange;
	public int serial1Baudrate;
	
	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.setBacklight(180);
	    serial1Baudrate = 0;
	    radioSetup(BAUDRATES[serial1Baudrate]);
	    
		lastRadioUIChange = 0;
	    touchscreen.setupUI(0, 0, touchscreen.getWidth(), touchscreen.getHeight());

	    telemetryTab = touchscreen.addUITab(stringToArray("View"));
	    radioTab = touchscreen.addUITab(stringToArray("Radio"), doRadioAction);
	    optionsTab = touchscreen.addUITab(stringToArray("Options"));
	    
	    touchscreen.setUITabEnabled(telemetryTab, false);
	    touchscreen.setUITabEnabled(optionsTab, false);
	    
	    touchscreen.addUIPushButton(radioTab, RADIO_SPEED_9600, RADIO_SPEED, 7, 10, 57, UI_AUTO_SIZE, "96OO", UI_SELECTED);
	    touchscreen.addUIPushButton(radioTab, RADIO_SPEED_19200, RADIO_SPEED, UI_RIGHT_OF + RADIO_SPEED_9600, UI_SAME_AS + RADIO_SPEED_9600, 
	    		UI_SAME_AS + RADIO_SPEED_9600, UI_AUTO_SIZE, "192OO", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, RADIO_SPEED_38400, RADIO_SPEED, UI_RIGHT_OF + RADIO_SPEED_19200, UI_SAME_AS + RADIO_SPEED_9600, 
	    		UI_SAME_AS + RADIO_SPEED_9600, UI_AUTO_SIZE, "384OO", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, RADIO_SPEED_57600, RADIO_SPEED, UI_RIGHT_OF + RADIO_SPEED_38400, UI_SAME_AS + RADIO_SPEED_9600, 
	    		UI_SAME_AS + RADIO_SPEED_9600, UI_AUTO_SIZE, "576OO", UI_UNSELECTED);
	    
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + RADIO_SPEED_9600, UI_AUTO_SIZE, UI_AUTO_SIZE, "ID");
	    touchscreen.addUIButton(radioTab, PAN_ID_MINUS, 30, UI_BOTTOM_OF_WITH_MARGIN + RADIO_SPEED_9600, 32, UI_AUTO_SIZE, "-");
	    touchscreen.addUIPushButton(radioTab, PAN_ID_1, PAN_ID, 72, UI_SAME_AS + PAN_ID_MINUS, 30, UI_AUTO_SIZE, "0", UI_SELECTED);
	    touchscreen.addUIPushButton(radioTab, PAN_ID_2, PAN_ID, UI_RIGHT_OF + PAN_ID_1, UI_SAME_AS + PAN_ID_MINUS, 
	    		UI_SAME_AS + PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, PAN_ID_3, PAN_ID, UI_RIGHT_OF + PAN_ID_2, UI_SAME_AS + PAN_ID_MINUS, 
	    		UI_SAME_AS + PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, PAN_ID_4, PAN_ID, UI_RIGHT_OF + PAN_ID_3, UI_SAME_AS + PAN_ID_MINUS, 
	    		UI_SAME_AS + PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIButton(radioTab, PAN_ID_PLUS, 200, UI_SAME_AS + PAN_ID_MINUS, 32, UI_AUTO_SIZE, "+");
	    
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + PAN_ID_PLUS, UI_AUTO_SIZE, UI_AUTO_SIZE, "L");
	    touchscreen.addUIPushButton(-1, LOCAL_ADDRESS, UI_NO_GROUP, UI_SAME_AS + PAN_ID_MINUS, UI_BOTTOM_OF_WITH_MARGIN + PAN_ID_PLUS, 
	    		160, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + LOCAL_ADDRESS, UI_AUTO_SIZE, UI_AUTO_SIZE, "R");
		touchscreen.addUIPushButton(-1, REMOTE_1_ADDRESS, UI_NO_GROUP, UI_SAME_AS + LOCAL_ADDRESS, UI_BOTTOM_OF_WITH_MARGIN + LOCAL_ADDRESS, 
				UI_SAME_AS + LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
		touchscreen.addUIPushButton(-1, REMOTE_2_ADDRESS, UI_NO_GROUP, UI_SAME_AS + LOCAL_ADDRESS, UI_BOTTOM_OF + REMOTE_1_ADDRESS, 
				UI_SAME_AS + LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
		touchscreen.addUIPushButton(-1, REMOTE_3_ADDRESS, UI_NO_GROUP, UI_SAME_AS + LOCAL_ADDRESS, UI_BOTTOM_OF + REMOTE_2_ADDRESS, 
				UI_SAME_AS + LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
	    touchscreen.addUIButton(-1, ADDRESS_RESET, 200, UI_SAME_AS + LOCAL_ADDRESS, 32, UI_AUTO_SIZE, "X");
	    
	    for (int i=0; i<4; i++) touchscreen.setUIElementEditable(RADIO_SPEED_9600 + i, false);
	    for (int i=0; i<6; i++) touchscreen.setUIElementEditable(PAN_ID_1 + i, false);
	    touchscreen.setUITab(radioTab);
	}

	public void loop() {
	    delay(20);
	    touchscreen.handleUI();
	    radioProcess();
	    handleRadioResponse();
	    if (lastRadioUIChange != 0 && ((long)(millis() - lastRadioUIChange) > RADIO_WAIT_INACTION_BEFORE_COMMIT_DELAY)) {
	    	lastRadioUIChange = 0;
	    	radioCommitChanges();
	    }
	}
	
	public void handleRadioResponse() {
		if ((radioStatus == RADIO_STATUS_READY) || radioStatus == RADIO_STATUS_ERROR) return;
		switch (radioStatus) {
			case RADIO_STATUS_CONNECTION_ERROR:
				touchscreen.showUIPopup("Connection error", null, RED);
    	    	radioStatus = RADIO_STATUS_SEND_TEST_TO_REMOTE;
				break;
			case RADIO_STATUS_BAUDRATE_ERROR:
		    	if (serial1Baudrate < 3) {
		    		touchscreen.setUIElementValue(RADIO_SPEED_9600 + serial1Baudrate, UI_UNSELECTED);
			    	serial1Baudrate++;
			    	touchscreen.setUIElementValue(RADIO_SPEED_9600 + serial1Baudrate, UI_SELECTED);
			    	radioSetup(BAUDRATES[serial1Baudrate]);			    	
		    	} else {
	    	    	touchscreen.setBackgroundColor(WHITE);
					touchscreen.showUIPopup("Plug in local xBee", "Could not communicate with the XBee module at any baudrate.\n" +
							"Switch off ArduRCT and plug in local XBee module", BLUE);
	    	    	radioStatus = RADIO_STATUS_ERROR;
		    	}
		    	break;
			case RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_REMOTE:
				setXBeeLabel(LOCAL_ADDRESS, radioLocalAddress);
				touchscreen.showUIPopup("Plug in remote xBee", "Switch off ArduRCT and plug in the remote XBee.", BLUE);
    	    	radioStatus = RADIO_STATUS_ERROR;
    	    	break;
	    	case RADIO_STATUS_CHECKING_ADDRESS:
			case RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL:
				setXBeeLabel(LOCAL_ADDRESS, radioLocalAddress);
				setXBeeLabel(REMOTE_1_ADDRESS, radioRemote1Address);
				if (radioAddressIsDefined(radioRemote2Address)) setXBeeLabel(REMOTE_2_ADDRESS, radioRemote2Address);
				if (radioAddressIsDefined(radioRemote3Address)) setXBeeLabel(REMOTE_3_ADDRESS, radioRemote3Address);
			    for (int i=0; i<4; i++) touchscreen.setUIElementEditable(RADIO_SPEED_9600 + i, true);
			    for (int i=0; i<6; i++) touchscreen.setUIElementEditable(PAN_ID_1 + i, true);
			    if (radioStatus == RADIO_STATUS_CHECKING_ADDRESS) return;
				touchscreen.showUIPopup("Plug in local xBee", "Remote module configured.\nSwitch off ArduRCT and plug in the local XBee" +
						" or change id or baudrate.", BLUE);
    	    	radioStatus = RADIO_STATUS_ERROR;
    	    	// we can now change the panID and speed
    	    	break;
			case RADIO_STATUS_FINALIZING:
		    	touchscreen.setUITabEnabled(telemetryTab, true);
		    	touchscreen.setUITabEnabled(optionsTab, true);
			    for (int i=0; i<4; i++) touchscreen.setUIElementEditable(RADIO_SPEED_9600 + i, true);
			    for (int i=0; i<6; i++) touchscreen.setUIElementEditable(PAN_ID_1 + i, true);
			    radioStatus = RADIO_STATUS_READY;
		    	break;
			default:
				break;
		}
	}

	public void setXBeeLabel(int labelId, int[] address) {
		int[] buffer = touchscreen.getUIElementLabel(labelId);
		arrayToHexString(address, buffer, 8);
		touchscreen.setUIElementLabel(labelId, buffer);
		touchscreen.setUIElementTab(labelId, radioTab);

	}
	public void setPanIdLabels() {
		for (int i=0; i<4; i++) {
			int[] label = touchscreen.getUIElementLabel(PAN_ID_1 + i);
			label[0] = radioPanId[i];
			touchscreen.setUIElementLabel(PAN_ID_1 + i, label);
		}
	}
	
	private RadioActionCB doRadioAction = new RadioActionCB();
	class RadioActionCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			lastRadioUIChange = millis();
			if ((id == PAN_ID_MINUS) || (id == PAN_ID_PLUS)) {
				int index = touchscreen.getUIGroupElement(PAN_ID) - PAN_ID_1;
				if (id == PAN_ID_MINUS) {
					radioPanId[index] --;
					if (radioPanId[index] > '9' && radioPanId[index] < 'A') radioPanId[index] = '9';
					else if (radioPanId[index] < '0') radioPanId[index] = 'F';
				} else {
					radioPanId[index] = radioPanId[index] + 1;
					if (radioPanId[index] > '9' && radioPanId[index] < 'A') radioPanId[index] = 'A';
					else if (radioPanId[index] > 'F') radioPanId[index] = '0';
				}
				int[] label = touchscreen.getUIElementLabel(PAN_ID_1 + index);
				label[0] = radioPanId[index];
				touchscreen.setUIElementLabel(PAN_ID_1 + index, label);
			} else if ((id >= RADIO_SPEED_9600) && (id <= RADIO_SPEED_57600)) {
				radioBaudrate = BAUDRATES[id-RADIO_SPEED_9600];
			}
		}
	}


}
