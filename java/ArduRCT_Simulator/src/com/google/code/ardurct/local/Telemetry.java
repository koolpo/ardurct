package com.google.code.ardurct.local;

import com.google.code.ardurct.libraries.ArduinoFirmware;
import com.google.code.ardurct.libraries.HMC6352;
import com.google.code.ardurct.libraries.graphics.TouchScreen_UserInterface.uiRectangle_t;
import com.google.code.ardurct.libraries.touscruino.TouscruinoFirmware;

public class Telemetry extends TouscruinoFirmware {

	static final int UI_OK_COLOR = GREEN;
	static final int UI_WARNING_COLOR = ORANGE;
	static final int UI_ALERT_COLOR = RED;
	
	static final double COMPASS_SHIFT = -Math.PI/2;
	
	static final int UI_TELEMETRY_DATA = 50;
	static final int UI_TELEMETRY_RADAR = 51;
	static final int UI_TELEMETRY_ALTITUDE = 52;
	
	static final int DISPLAY_METER_LENGTH = 5;
	
	public int telemetryTab;
	public int radioStrength = 40;			// 0 to 63
	public boolean telemetryGPSHasLock = false;
	public double telemetryCompassHeading = 0;
	public boolean telemetryDataTypeIsGPS = false;
	
	public int[] telemetryLatitudeDisplay = { ' ', ' ', 'x', 0x7e, 'x', 'x', '\'', 'x', 'x', '"', 'x', 'x', 'x', ' ', 'N', 0}; 		// 15
	public int[] telemetryLongitudeDisplay = { ' ', ' ', 'x', 0x7e, 'x', 'x', '\'', 'x', 'x', '"', 'x', 'x', 'x', ' ', 'E', 0};		// 15
	public int[] telemetryAltitudeDisplay ={ 'x', 'm', 0, 0, 0, 0, 0 };       
	public int[] telemetrySpeedDisplay = { ' ', ' ', ' ', ' ', '0', 'm', '/', 's', 0 };          
	public int[] telemetryDistanceDisplay = { ' ', ' ', ' ', ' ', '0', 'm', 0 };            
	public int[] telemetryHeightDisplay = { ' ', ' ', ' ', ' ', '0', 'm', 0 };        
	public int[] telemetryRunTimeDisplay = { '0', '0', ':', '0', '0', ':', '0', '0', '.', '0', 0 };      
	
	public long telemetryRunTime = 0;
	public int telemetryDistance = 0;
	public int telemetryAngle = 0;
	public int telemetryHeight = 0;

	public boolean telemetryDataHasChanged = false;
	
	public void telemetrySetup() {
		
	}
	
	public void telemetryDrawTabIcon(int x, int y, int width, int height, int value) {
		// draw a GPS
		x += (width - 26)/2;
		y += (height - 24)/2;
		if (value != UI_REFRESH) {
			drawAndFillRotatedRectangle(x+3, y+2, 6, 3, value == UI_SELECTED ? LIGHT_GREY : WHITE);
			drawAndFillRotatedRectangle(x+17, y+16, 6, 3,  value == UI_SELECTED ? LIGHT_GREY : WHITE);
			touchscreen.drawArc(x+10, y+14, 5, GRAPHICS_ARC_SW, BLACK, 1);
			touchscreen.drawArc(x+10, y+14, 8, GRAPHICS_ARC_SW, BLACK, 1);
			touchscreen.drawArc(x+10, y+14, 11, GRAPHICS_ARC_SW, BLACK, 1);
		}
		drawAndFillRotatedRectangle(x+7, y+11, 7, 9, telemetryGPSHasLock ? UI_OK_COLOR : UI_ALERT_COLOR);
	}
	
	public void telemetryRefreshData(int x, int y, int width, int height, boolean runTimeOnly) {
		int lineHeight = touchscreen.getLineHeight(FONT_MEDIUM) + 4;
		y += 6;
		x += 6 + 10 * touchscreen.getStringWidth("  ", FONT_MEDIUM) / 2;
		touchscreen.setBackgroundColor(WHITE);
		// draw run time
		runTimeToRunTimeDisplay();
		touchscreen.drawString(telemetryRunTimeDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
		if (!telemetryDataHasChanged || runTimeOnly) return;	
		telemetryDataHasChanged = false;
		y += lineHeight;
		if (telemetryDataTypeIsGPS) {
			// draw longitude, latitude and altitude
			touchscreen.drawString(telemetryLatitudeDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
			y += lineHeight;
			touchscreen.drawString(telemetryLongitudeDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
			y += lineHeight;
			touchscreen.fillRectangle(x, y, width-x, lineHeight, WHITE);
			touchscreen.drawString(telemetryAltitudeDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
		} else {
			touchscreen.fillRectangle(x, y, width-x, 3*lineHeight, WHITE);
			// draw speed, distance and height
			touchscreen.drawString(telemetrySpeedDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
			y += lineHeight;
			touchscreen.drawString(telemetryDistanceDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
			y += lineHeight;
			touchscreen.drawString(telemetryHeightDisplay, x, y, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
		}
	}
	
	public void telemetryRefreshRadar(int x, int y, int width, int height) {
		width -= 6;
		int x0 = x+2+width/2;
		int y0 = y+2+width/2;
		int r = width/2;
		int x1 = x0 + (int)(Math.cos(telemetryCompassHeading) * (r-2));
		int y1 = y0 + (int)(Math.sin(telemetryCompassHeading) * (r-2));
		touchscreen.drawLine(x0, y0, x1, y1, WHITE, 1);
		telemetryCompassHeading = HMC6352.getHeading();
		telemetryCompassHeading = (COMPASS_SHIFT + Math.PI/2) + (telemetryCompassHeading - 180) * Math.PI / 180;
		x1 = x0 + (int)(Math.cos(telemetryCompassHeading) * (r-2));
		y1 = y0 + (int)(Math.sin(telemetryCompassHeading) * (r-2));
		touchscreen.drawLine(x0, y0, x1, y1, RED, 1);
	}
	
	public void telemetryRefreshAltitude(int x, int y, int width, int height) {
		touchscreen.setBackgroundColor(WHITE);
		touchscreen.drawString("9999", x+width-2-touchscreen.getStringWidth("9999", FONT_SMALL), y, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
	}
	
	public void telemetryRefreshScreen(boolean runTimeOnly) {
		if (touchscreen.getUISelectedTab() != telemetryTab) return;
		uiRectangle_t bounds = touchscreen.getUIElementBounds(UI_TELEMETRY_DATA);
		telemetryRefreshData(bounds.x, bounds.y, bounds.width, bounds.height, runTimeOnly);
		if (runTimeOnly) return;
		bounds = touchscreen.getUIElementBounds(UI_TELEMETRY_RADAR);
		telemetryRefreshRadar(bounds.x, bounds.y, bounds.width, bounds.height);
		bounds = touchscreen.getUIElementBounds(UI_TELEMETRY_ALTITUDE);
		telemetryRefreshAltitude(bounds.x, bounds.y, bounds.width, bounds.height);
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
	
	private void runTimeToRunTimeDisplay() {
		long rt = telemetryRunTime;
		long rtH = rt / (60*60*1000);
		telemetryRunTimeDisplay[0] = (int) (rtH > 10 ? (rtH / 10) + '0' : ' ');
		telemetryRunTimeDisplay[1] = (int) ((rtH % 10) + '0');
		rt -= rtH * 60*60*1000;
		long rtM = rt / (60*1000);
		telemetryRunTimeDisplay[3] = (int) ((rtM / 10) + '0');
		telemetryRunTimeDisplay[4] = (int) ((rtM % 10) + '0');
		rt -= rtM * 60*1000;
		long rtS = rt / 1000;
		telemetryRunTimeDisplay[6] = (int) ((rtS / 10) + '0');
		telemetryRunTimeDisplay[7] = (int) ((rtS % 10) + '0');
		rt -= rtS * 1000;
		telemetryRunTimeDisplay[9] = (int) ((rt / 100) + '0');
	}
	
	public void setup() {}
	public void loop() {}

}
