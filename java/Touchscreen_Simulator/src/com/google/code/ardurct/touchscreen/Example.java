package com.google.code.ardurct.touchscreen;

import com.google.code.ardurct.Wiring;

public class Example extends Wiring 
implements IExample, Runnable {
	
	private Thread t = null;
	public Touchscreen touchscreen = null;
	
	public Example() {
	}
	
	public void setTouchscreen(Touchscreen touchscreen) {
		this.touchscreen = touchscreen;
	}

	@Override
	public void loop() {
		// TODO Auto-generated method stub
	}

	@Override
	public void setup() {
		touchscreen.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
		touchscreen.setCursor(10, 10);
		touchscreen.print("Select a test from the Examples menu");
	}

	@Override
	public void run() {
		while (true) {
			loop();
			touchscreen.repaint();
		}
	}
	
	public void start() {
		t = new Thread(this);
		t.start();
	}
	
	public void stop() {
		if (t != null) t.interrupt();
	}
}
