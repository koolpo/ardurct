package com.google.code.ardurct.libraries;

public class HMC6352 implements Runnable {
	
	static int heading;
	static Thread t = null;
	static boolean running = false;
	
	public HMC6352() {
	}
	
	public static void begin() {
		if (t == null) {
			heading = (int)Math.floor(359.99 * Math.random());
			t = new Thread(new HMC6352());
			t.start();
		}
	}
	
	public static void stop() {
		running = false;
		if (t != null) t.interrupt();
	}
	
	public void run() {
		running = true;
		while (running) {
			try {
				heading += (int)Math.round(3 * Math.random());
				heading = (heading % 360);
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				return;
			}
		}
	}

	public static int getHeading() {
		return heading;
	}
}
