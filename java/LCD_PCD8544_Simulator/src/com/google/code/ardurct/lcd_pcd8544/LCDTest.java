package com.google.code.ardurct.lcd_pcd8544;

import com.google.code.ardurct.Wiring;

public class LCDTest extends Wiring 
implements ILCDTest, Runnable {

	protected int LCD_PCD8544_WIDTH = LCDTextPanel.LCD_PCD8544_WIDTH;
	protected int LCD_PCD8544_COLUMNS = LCD_PCD8544_WIDTH/6;
	protected int LCD_PCD8544_HEIGHT = LCDTextPanel.LCD_PCD8544_HEIGHT;
	protected int LCD_PCD8544_LINES =  LCD_PCD8544_HEIGHT/8;
	protected int LCD_PCD8544_CHAR_WIDTH = LCDTextPanel.LCD_PCD8544_CHAR_WIDTH;
	
	protected LCDGraphicPanel lcdG;
	protected LCDTextPanel lcdT;

	private Thread t = null;
	
	public LCDTest() {
	}
	
	public void setDisplays(LCDGraphicPanel lcdG, LCDTextPanel lcdT) {
		this.lcdG = lcdG;
		this.lcdT = lcdT;
	}

	@Override
	public void loop() {
		// TODO Auto-generated method stub
	}

	@Override
	public void setup() {
		lcdG.setCursor(0, 1);
		lcdG.print("Select a test from the Tests menu");
	}

	@Override
	public void run() {
		while (true) {
			loop();
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
