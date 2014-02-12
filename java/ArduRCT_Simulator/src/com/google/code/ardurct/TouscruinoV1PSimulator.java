package com.google.code.ardurct;

public class TouscruinoV1PSimulator extends GraphicsSimulator {
	
	private static final long serialVersionUID = -1L;
	
	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance("TouScruino", ".libraries.touscruino.examplesV1", 
				TFT_VERSION_1, PORTRAIT_ORIENTATION,
				HAS_SWITCHES, !HAS_ROCKER_SWITCH);
		simulator.startExample("BasicExample");
	}

}
