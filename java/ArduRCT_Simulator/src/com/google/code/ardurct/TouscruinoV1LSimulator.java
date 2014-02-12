package com.google.code.ardurct;

public class TouscruinoV1LSimulator extends GraphicsSimulator {
	
	private static final long serialVersionUID = -1L;
	
	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance("Touscruino", ".libraries.touscruino.examplesV1", 
				TFT_VERSION_1, LANDSCAPE_ORIENTATION,
				HAS_SWITCHES, !HAS_ROCKER_SWITCH);
		simulator.startExample("BasicExample");
	}

}
