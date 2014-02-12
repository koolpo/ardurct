package com.google.code.ardurct.local;


public class Options extends Telemetry {
	
	public boolean remoteBatteryIsMeasured;
	public int remoteBattery;				// max 255
	public int remoteBatteryAlert;
	
	public boolean enginesAreRunning;
	public long enginesStartTime;


	public void optionsSetup() {
		remoteBatteryIsMeasured = false;
		remoteBattery = 0;
		remoteBatteryAlert = 255 / 3;
		enginesAreRunning = false;
	}
	
	public void setup() {}
	public void loop() {}

}
