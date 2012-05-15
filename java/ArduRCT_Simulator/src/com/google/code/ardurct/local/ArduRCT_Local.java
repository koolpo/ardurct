package com.google.code.ardurct.local;

import com.google.code.ardurct.libraries.touchscreen.IUIActionCallback;
import com.google.code.ardurct.libraries.touchscreen.IUIDrawCallback;

public class ArduRCT_Local extends Radio {
	
	static final int BAUDRATES[] = { 9600, 19200, 38400, 57600 };
	
	static final int UI_OK_COLOR = GREEN;
	static final int UI_WARNING_COLOR = ORANGE;
	static final int UI_ALERT_COLOR = RED;
		
	static final long RADIO_WAIT_INACTION_BEFORE_COMMIT_DELAY = 5000;

	public static final int MAIN_LOOP_REPEATS_BEFORE_RESTART = 100;	// 2 seconds MAIN_LOOP_REPEATS_BEFORE_RESTART/(1/MAIN_LOOP_DELAY) 

	static final int UI_RADIO_BAUDRATE_9600 = 10;
	static final int UI_RADIO_BAUDRATE_19200 = 11;
	static final int UI_RADIO_BAUDRATE_38400 = 12;
	static final int UI_RADIO_BAUDRATE_57600 = 13;
	static final int UI_RADIO_BAUDRATE_GROUP = 1;
	
	static final int UI_RADIO_PAN_ID_LABEL = 19;
	static final int UI_RADIO_PAN_ID_1 = 20;
	static final int UI_RADIO_PAN_ID_2 = 21;
	static final int UI_RADIO_PAN_ID_3 = 22;
	static final int UI_RADIO_PAN_ID_4 = 23;
	static final int UI_RADIO_PAN_ID_PLUS = 24;
	static final int UI_RADIO_PAN_ID_MINUS = 25;
	static final int UI_RADIO_PAN_ID_GROUP = 2;
	
	static final int UI_RADIO_ADDRESS_RESET = 29;
	//public static final int UI_RADIO_LOCAL_ADDRESS = 30;
	//public static final int UI_RADIO_REMOTE_ADDRESS_1 = 31;
	//public static final int UI_RADIO_REMOTE_ADDRESS_2 = 32;
	//public static final int UI_RADIO_REMOTE_ADDRESS_3 = 33;
	
	static final int UI_TELEMETRY_DATA = 50;
	static final int UI_TELEMETRY_RADAR = 51;
	
	static final int UI_ENGINES_RUN = 60;
	static final int UI_ENGINES_STOP = 61;
	static final int UI_ENGINES_GROUP = 3;
	
	public int radioTab;
	public int telemetryTab;
	public int optionsTab;
	public int enginesTab;
	public long lastRadioUIChange;
	public int serial1BaudrateIndex;
	public int loopCounter;
	
	public void setup() {
	    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
	    touchscreen.setBacklight(180);
	    serial1BaudrateIndex = 0;
	    radioSetup(BAUDRATES[serial1BaudrateIndex]);
	    optionsSetup();
	    
		lastRadioUIChange = 0;
		loopCounter = 0;
		
	    touchscreen.setupUI(0, 0, touchscreen.getWidth(), touchscreen.getHeight());
	    telemetryTab = touchscreen.addUITab(stringToArray("    "), null, drawTelemetry);
	    radioTab = touchscreen.addUITab(stringToArray("    "), doRadioAction, drawRadio);
	    optionsTab = touchscreen.addUITab(stringToArray("    "), doOptionsAction, drawOptions);
	    enginesTab = touchscreen.addUITab(stringToArray("    "), doEnginesAction, drawEngines);
	    
	    setupRadioTab();
	    setupTelemetryTab();
	    setupEnginesTab();
	    touchscreen.setUITabEnabled(telemetryTab, false);
	    touchscreen.setUITabEnabled(optionsTab, false);
	    touchscreen.setUITabEnabled(enginesTab, false);
	    touchscreen.setUITab(radioTab);
	}

	public void loop() {
	    delay(MAIN_LOOP_DELAY);
	    touchscreen.handleUI();
	    radioProcess();
	    handleRadioResponse();
	    serialProcess();
	    if (lastRadioUIChange != 0 && ((long)(millis() - lastRadioUIChange) > RADIO_WAIT_INACTION_BEFORE_COMMIT_DELAY)) {
	    	lastRadioUIChange = 0;
	    	radioCommitChanges();
	    }
	    if (radioStatus != RADIO_STATUS_READY) return;
	    
	    switch (loopCounter) {
	    	case 0:
	    		radioMeasureRemoteBattery();
	    		break;
	    	case 20:
	    		handleUIIndicators();
	    		break;
	    }
	    loopCounter ++;
	    if (loopCounter == MAIN_LOOP_REPEATS_BEFORE_RESTART) loopCounter = 0;
	}
	
	public void handleRadioResponse() {
		if ((radioStatus == RADIO_STATUS_READY) || radioStatus == RADIO_STATUS_ERROR) return;
		switch (radioStatus) {
			case RADIO_STATUS_PREPARE_TO_SEND_TEST_TO_REMOTE:
				touchscreen.showUIPopup("Connecting...", null, BLUE);
				radioStatus = RADIO_STATUS_SEND_TEST_TO_REMOTE;
				break;
			case RADIO_STATUS_FIRST_CONNECTION_ERROR:
				touchscreen.showUIPopup("Connection error", "Start the remote part, then the" +
						"local part.", RED);
			case RADIO_STATUS_CONNECTION_ERROR:
    	    	radioStatus = RADIO_STATUS_SEND_TEST_TO_REMOTE;
				break;
			case RADIO_STATUS_BAUDRATE_ERROR:
		    	if (serial1BaudrateIndex < 3) {
		    		touchscreen.setUIElementValue(UI_RADIO_BAUDRATE_9600 + serial1BaudrateIndex, UI_UNSELECTED);
			    	serial1BaudrateIndex++;
			    	touchscreen.setUIElementValue(UI_RADIO_BAUDRATE_9600 + serial1BaudrateIndex, UI_SELECTED);
			    	radioSetup(BAUDRATES[serial1BaudrateIndex]);			    	
		    	} else {
	    	    	touchscreen.setBackgroundColor(WHITE);
					touchscreen.showUIPopup("Plug in local xBee", "Could not communicate with the XBee module at any baudrate.\n" +
							"Switch off ArduRCT and plug in local XBee module", BLUE);
	    	    	radioStatus = RADIO_STATUS_ERROR;
		    	}
		    	break;
			case RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_REMOTE:
				setXBeeLabel(UI_RADIO_LOCAL_ADDRESS, radioLocalAddress);
				touchscreen.showUIPopup("Plug in remote xBee", "Switch off ArduRCT and plug in the remote XBee.", BLUE);
    	    	radioStatus = RADIO_STATUS_ERROR;
    	    	break;
	    	case RADIO_STATUS_CHECKING_ADDRESS:
			case RADIO_STATUS_REQUESTING_MODULE_SWITCH_TO_LOCAL:
				setXBeeLabel(UI_RADIO_LOCAL_ADDRESS, radioLocalAddress);
				setXBeeLabel(UI_RADIO_REMOTE_1_ADDRESS, radioRemote1Address);
				if (radioAddressIsDefined(radioRemote2Address)) setXBeeLabel(UI_RADIO_REMOTE_2_ADDRESS, radioRemote2Address);
				if (radioAddressIsDefined(radioRemote3Address)) setXBeeLabel(UI_RADIO_REMOTE_3_ADDRESS, radioRemote3Address);
			    for (int i=0; i<4; i++) touchscreen.setUIElementEditable(UI_RADIO_BAUDRATE_9600 + i, true);
			    for (int i=0; i<6; i++) touchscreen.setUIElementEditable(UI_RADIO_PAN_ID_1 + i, true);
			    if (radioStatus == RADIO_STATUS_CHECKING_ADDRESS) return;
				touchscreen.showUIPopup("Plug in local xBee", "Remote module configured.\nSwitch off ArduRCT and plug in the local XBee" +
						" or change id or baudrate.", BLUE);
    	    	radioStatus = RADIO_STATUS_ERROR;
    	    	// we can now change the panID and speed
    	    	break;
			case RADIO_STATUS_FINALIZING:
				touchscreen.hideUIPopups();
		    	touchscreen.setUITabEnabled(telemetryTab, true);
		    	touchscreen.setUITabEnabled(optionsTab, true);
		    	touchscreen.setUITabEnabled(enginesTab, true);
			    radioStatus = RADIO_STATUS_READY;
		    	break;
			default:
				break;
		}
	}

	public void setupRadioTab() {
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_BAUDRATE_9600, UI_RADIO_BAUDRATE_GROUP, 7, 10, 57, UI_AUTO_SIZE, "96OO", UI_SELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_BAUDRATE_19200, UI_RADIO_BAUDRATE_GROUP, UI_RIGHT_OF + UI_RADIO_BAUDRATE_9600, UI_SAME_AS + UI_RADIO_BAUDRATE_9600, 
	    		UI_SAME_AS + UI_RADIO_BAUDRATE_9600, UI_AUTO_SIZE, "192OO", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_BAUDRATE_38400, UI_RADIO_BAUDRATE_GROUP, UI_RIGHT_OF + UI_RADIO_BAUDRATE_19200, UI_SAME_AS + UI_RADIO_BAUDRATE_9600, 
	    		UI_SAME_AS + UI_RADIO_BAUDRATE_9600, UI_AUTO_SIZE, "384OO", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_BAUDRATE_57600, UI_RADIO_BAUDRATE_GROUP, UI_RIGHT_OF + UI_RADIO_BAUDRATE_38400, UI_SAME_AS + UI_RADIO_BAUDRATE_9600, 
	    		UI_SAME_AS + UI_RADIO_BAUDRATE_9600, UI_AUTO_SIZE, "576OO", UI_UNSELECTED);
	    
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_BAUDRATE_9600, UI_AUTO_SIZE, UI_AUTO_SIZE, "ID");
	    touchscreen.addUIButton(radioTab, UI_RADIO_PAN_ID_MINUS, 30, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_BAUDRATE_9600, 32, UI_AUTO_SIZE, "-");
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_PAN_ID_1, UI_RADIO_PAN_ID_GROUP, 72, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, 30, UI_AUTO_SIZE, "0", UI_SELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_PAN_ID_2, UI_RADIO_PAN_ID_GROUP, UI_RIGHT_OF + UI_RADIO_PAN_ID_1, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, 
	    		UI_SAME_AS + UI_RADIO_PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_PAN_ID_3, UI_RADIO_PAN_ID_GROUP, UI_RIGHT_OF + UI_RADIO_PAN_ID_2, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, 
	    		UI_SAME_AS + UI_RADIO_PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIPushButton(radioTab, UI_RADIO_PAN_ID_4, UI_RADIO_PAN_ID_GROUP, UI_RIGHT_OF + UI_RADIO_PAN_ID_3, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, 
	    		UI_SAME_AS + UI_RADIO_PAN_ID_1, UI_AUTO_SIZE, "0", UI_UNSELECTED);
	    touchscreen.addUIButton(radioTab, UI_RADIO_PAN_ID_PLUS, 200, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, 32, UI_AUTO_SIZE, "+");
	    
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_PAN_ID_PLUS, UI_AUTO_SIZE, UI_AUTO_SIZE, "L");
	    touchscreen.addUIPushButton(-1, UI_RADIO_LOCAL_ADDRESS, UI_NO_GROUP, UI_SAME_AS + UI_RADIO_PAN_ID_MINUS, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_PAN_ID_PLUS, 
	    		160, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
	    touchscreen.addUILabel(radioTab, -1, 7, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_LOCAL_ADDRESS, UI_AUTO_SIZE, UI_AUTO_SIZE, "R");
		touchscreen.addUIPushButton(-1, UI_RADIO_REMOTE_1_ADDRESS, UI_NO_GROUP, UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_BOTTOM_OF_WITH_MARGIN + UI_RADIO_LOCAL_ADDRESS, 
				UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
		touchscreen.addUIPushButton(-1, UI_RADIO_REMOTE_2_ADDRESS, UI_NO_GROUP, UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_BOTTOM_OF + UI_RADIO_REMOTE_1_ADDRESS, 
				UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
		touchscreen.addUIPushButton(-1, UI_RADIO_REMOTE_3_ADDRESS, UI_NO_GROUP, UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_BOTTOM_OF + UI_RADIO_REMOTE_2_ADDRESS, 
				UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, UI_AUTO_SIZE, "                ", UI_UNSELECTED);
	    touchscreen.addUIButton(radioTab, UI_RADIO_ADDRESS_RESET, 200, UI_SAME_AS + UI_RADIO_LOCAL_ADDRESS, 32, UI_AUTO_SIZE, "X");
	    
	    for (int i=0; i<4; i++) touchscreen.setUIElementEditable(UI_RADIO_BAUDRATE_9600 + i, false);
	    for (int i=0; i<6; i++) touchscreen.setUIElementEditable(UI_RADIO_PAN_ID_1 + i, false);
	}
	
	public void setupTelemetryTab() {
		touchscreen.addUIArea(telemetryTab, UI_TELEMETRY_DATA, 5, 5, touchscreen.getWidth(), 40);
		touchscreen.addUIArea(telemetryTab, UI_TELEMETRY_RADAR, 20, 90, touchscreen.getWidth()-40, touchscreen.getWidth()-40);
	}
	
	public void setupEnginesTab() {
		touchscreen.addUIPushButton(enginesTab, UI_ENGINES_RUN, UI_ENGINES_GROUP, 40, 20, touchscreen.getWidth()-80, 50, "Run", UI_UNSELECTED);
		touchscreen.addUIPushButton(enginesTab, UI_ENGINES_STOP, UI_ENGINES_GROUP, UI_SAME_AS + UI_ENGINES_RUN, 
				UI_BOTTOM_OF_WITH_MARGIN + UI_ENGINES_RUN, UI_SAME_AS + UI_ENGINES_RUN, UI_SAME_AS + UI_ENGINES_RUN, "Stop", UI_SELECTED);
	}
	
	public void setXBeeLabel(int labelId, int[] address) {
		int[] buffer = touchscreen.getUIElementLabel(labelId);
		arrayToHexString(address, buffer, 8);
		touchscreen.setUIElementLabel(labelId, buffer);
		touchscreen.setUIElementTab(labelId, radioTab);
	}
	
	public void setPanIdLabels() {
		for (int i=0; i<4; i++) {
			int[] label = touchscreen.getUIElementLabel(UI_RADIO_PAN_ID_1 + i);
			label[0] = radioPanId[i];
			touchscreen.setUIElementLabel(UI_RADIO_PAN_ID_1 + i, label);
		}
	}

	public void serialProcess() {
		while (Serial.available() > 0) xBee.print((char) Serial.read());
	}

	private RadioActionCB doRadioAction = new RadioActionCB();
	class RadioActionCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			if ((id == UI_RADIO_PAN_ID_MINUS) || (id == UI_RADIO_PAN_ID_PLUS)) {
				lastRadioUIChange = millis();
				int index = touchscreen.getUIGroupElement(UI_RADIO_PAN_ID_GROUP) - UI_RADIO_PAN_ID_1;
				if (id == UI_RADIO_PAN_ID_MINUS) {
					radioPanId[index] --;
					if (radioPanId[index] > '9' && radioPanId[index] < 'A') radioPanId[index] = '9';
					else if (radioPanId[index] < '0') radioPanId[index] = 'F';
				} else {
					radioPanId[index] = radioPanId[index] + 1;
					if (radioPanId[index] > '9' && radioPanId[index] < 'A') radioPanId[index] = 'A';
					else if (radioPanId[index] > 'F') radioPanId[index] = '0';
				}
				int[] label = touchscreen.getUIElementLabel(UI_RADIO_PAN_ID_1 + index);
				label[0] = radioPanId[index];
				touchscreen.setUIElementLabel(UI_RADIO_PAN_ID_1 + index, label);
			} else if ((id >= UI_RADIO_BAUDRATE_9600) && (id <= UI_RADIO_BAUDRATE_57600)) {
				lastRadioUIChange = millis();
				radioBaudrate = BAUDRATES[id-UI_RADIO_BAUDRATE_9600];
			} else if (id == UI_RADIO_ADDRESS_RESET) {
				resetAddressIfRequired(UI_RADIO_LOCAL_ADDRESS, radioLocalAddress, RADIO_LOCAL_EEPROM_ADDRESS);
				resetAddressIfRequired(UI_RADIO_REMOTE_1_ADDRESS, radioRemote1Address, RADIO_REMOTE_1_EEPROM_ADDRESS);
				resetAddressIfRequired(UI_RADIO_REMOTE_2_ADDRESS, radioRemote2Address, RADIO_REMOTE_2_EEPROM_ADDRESS);
				resetAddressIfRequired(UI_RADIO_REMOTE_3_ADDRESS, radioRemote3Address, RADIO_REMOTE_3_EEPROM_ADDRESS);
			}
		}
	}

	private EnginesActionCB doEnginesAction = new EnginesActionCB();
	class EnginesActionCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			if (id == UI_ENGINES_RUN) {
				enginesAreRunning = true;
				radioCommandEngines();
			} else if (id == UI_ENGINES_STOP) {
				enginesAreRunning = false;
				radioCommandEngines();
			}
		}
	}

	private OptionsActionCB doOptionsAction = new OptionsActionCB();
	class OptionsActionCB implements IUIActionCallback {
		public void uiActionCallback(int id) {
			
		}
	}

	public void resetAddressIfRequired(int id, int[] address, int eepromAddress) {
		if (touchscreen.getUIElementValue(id) == UI_SELECTED) {
			radioResetAddress(address, eepromAddress);
			touchscreen.setUIElementTab(id, -1);
			radioStatus = RADIO_STATUS_UNDEFINED;
		}
	}
	
	public void handleUIIndicators() {
		drawTelemetry.uiDrawCallback(UI_DRAW_CALLBACK_TAB_ID, touchscreen.getUITabX(telemetryTab), touchscreen.getUITabY(telemetryTab), 
				touchscreen.getUITabWidth(telemetryTab), touchscreen.getUITabHeight(), UI_REFRESH);
		drawRadio.uiDrawCallback(UI_DRAW_CALLBACK_TAB_ID, touchscreen.getUITabX(radioTab), touchscreen.getUITabY(radioTab), 
				touchscreen.getUITabWidth(radioTab), touchscreen.getUITabHeight(), UI_REFRESH);
		drawOptions.uiDrawCallback(UI_DRAW_CALLBACK_TAB_ID, touchscreen.getUITabX(optionsTab), touchscreen.getUITabY(optionsTab), 
				touchscreen.getUITabWidth(optionsTab), touchscreen.getUITabHeight(), UI_REFRESH);
		drawEngines.uiDrawCallback(UI_DRAW_CALLBACK_TAB_ID, touchscreen.getUITabX(enginesTab), touchscreen.getUITabY(enginesTab), 
				touchscreen.getUITabWidth(enginesTab), touchscreen.getUITabHeight(), UI_REFRESH);
	}


	private DrawTelemetryCB drawTelemetry = new DrawTelemetryCB();
	class DrawTelemetryCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == UI_DRAW_CALLBACK_TAB_ID) {
				// draw a GPS
				x += (width - 26)/2;
				y += (height - 24)/2;
				if (value != UI_REFRESH) {
					drawAndFillRotatedRectangle(x+3, y+2, 6, 3, value == UI_SELECTED ? LIGHT_GREY : WHITE);
					drawAndFillRotatedRectangle(x+17, y+16, 6, 3,  value == UI_SELECTED ? LIGHT_GREY : WHITE);
					touchscreen.drawArc(x+10, y+14, 5, SCREEN_ARC_SW, BLACK, 1);
					touchscreen.drawArc(x+10, y+14, 8, SCREEN_ARC_SW, BLACK, 1);
					touchscreen.drawArc(x+10, y+14, 11, SCREEN_ARC_SW, BLACK, 1);
				}
				drawAndFillRotatedRectangle(x+7, y+11, 7, 9, radioGPSHasLock ? UI_OK_COLOR : UI_ALERT_COLOR);
			} else if (id == UI_TELEMETRY_DATA) {
				touchscreen.fillRectangle(x, y, width, height, YELLOW);
			} else if (id == UI_TELEMETRY_RADAR) {
				touchscreen.fillRectangle(x, y, width, height, GREY);				
			}
		}
	}

	// Draw  and fill a rectangle rotated 45°
	// x,y denote the bottom right corner of the non rotated rectangle
	private void drawAndFillRotatedRectangle(int x, int y, int dx, int dy, int color) {
		for (int i=0; i<dx; i++) {
			touchscreen.drawLine(x+i, y+i, x+dy+i, y-dy+i, (i == 0) || (i == dx-1) ? BLACK : color, 1);
			if (i < dx-1) touchscreen.drawLine(x+1+i, y+i, x+1+dy+i, y-dy+i, color, 1);
		}
		touchscreen.drawLine(x, y, x+dx-1, y+dx-1, BLACK, 1);
		touchscreen.drawLine(x+dy+1, y-dy-1, x+dy+dx, y-dy+dx-2, BLACK, 1);		
	}
	
	private DrawRadioCB drawRadio = new DrawRadioCB();
	class DrawRadioCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == UI_DRAW_CALLBACK_TAB_ID) {
				x += (width - 36)/2;
				y += (height - 22)/2;
				if (value != UI_REFRESH) {
					// draw the antenna
					touchscreen.drawTriangle(x, y, x+6, y, x+3, y+3, BLACK, 1);
					touchscreen.drawLine(x+3, y+3, x+3, y+21, BLACK, 1);
					// draw the signal strength
					for (int i=0; i<5; i++) touchscreen.drawRectangle(x+7+i*6, y+16-i*4, 5, (i+1)*4+2, BLACK, 1);
				}
				for (int i=0; i<5; i++) touchscreen.fillRectangle(x+8+i*6, y+17-i*4, 3, (i+1)*4, radioStrength > (i+1)*10 ? BLUE : WHITE);
			} 
		}
	}
	
	private DrawOptionsCB drawOptions = new DrawOptionsCB();
	class DrawOptionsCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == UI_DRAW_CALLBACK_TAB_ID) {
				x += (width - 34)/2;
				y += (height - 16)/2;
				if (value != UI_REFRESH) {
					touchscreen.drawRectangle(x, y, 33, 16, BLACK, 1);
					touchscreen.fillRectangle(x+33, y+4, 3, 8, BLACK);
				}
				if (remoteBatteryIsMeasured) {
					int batteryColor = UI_OK_COLOR;
					if (remoteBattery < remoteBatteryAlert) batteryColor = UI_ALERT_COLOR;
					else if (remoteBattery < remoteBatteryAlert + (255-remoteBatteryAlert)/3) batteryColor = UI_WARNING_COLOR;
					int filling = remoteBattery >> 3;						
					touchscreen.fillRectangle(x+1+filling, y+1, 31-filling, 14, WHITE);
					touchscreen.fillRectangle(x+1, y+1, filling, 14, batteryColor);
				}
			} 
		}
	}
	
	private DrawEnginesCB drawEngines = new DrawEnginesCB();
	class DrawEnginesCB implements IUIDrawCallback {
		public void uiDrawCallback(int id, int x, int y, int width, int height, int value) {
			if (id == UI_DRAW_CALLBACK_TAB_ID) {
				x += (width - 24)/2;
				y += (height - 16)/2;
				if (value != UI_REFRESH) touchscreen.drawRectangle(x, y, 24, 16, BLACK, 1);
				touchscreen.fillRectangle(x+1, y+1, 22, 14, enginesAreRunning ? UI_OK_COLOR : UI_ALERT_COLOR);
			} 
		}
	}
}
