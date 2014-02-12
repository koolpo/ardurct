package com.google.code.ardurct;

public class TouscruinoV2PSimulator extends GraphicsSimulator {
	
	private static final long serialVersionUID = -1L;
	
	public static void main(final String[] args) {
		GraphicsSimulator simulator = createAndShowInstance("Touscruino", ".libraries.touscruino.examplesV2", 
				TFT_VERSION_2, PORTRAIT_ORIENTATION,
				!HAS_SWITCHES, !HAS_ROCKER_SWITCH);
		simulator.startExample("BasicExample");
	}

}
