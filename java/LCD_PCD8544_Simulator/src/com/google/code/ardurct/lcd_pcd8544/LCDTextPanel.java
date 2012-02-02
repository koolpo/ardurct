package com.google.code.ardurct.lcd_pcd8544;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

public class LCDTextPanel extends JPanel {

	// Change this to have the frame replicate the LCD llok
	public final int DOT_SIZE = 1;
	public final static float SCALE_X = 1.0f;
	public final static float SCALE_Y = 1.0f;	// non square pixels
	
	public final static int BORDER = 10;

	public static final int LCD_PCD8544_WIDTH = 84;
	public static final int LCD_PCD8544_COLUMNS = LCD_PCD8544_WIDTH/6;
	public static final int LCD_PCD8544_HEIGHT = 48;
	public static final int LCD_PCD8544_LINES = LCD_PCD8544_HEIGHT/8;
	public static final int LCD_PCD8544_DATA = 0;
	public static final int LCD_PCD8544_CHAR_WIDTH = 6;
		
	private static final long serialVersionUID = -3355396623405797901L;

	private int font_5x7[][] = {
			  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 20 space
			  { 0x00, 0x00, 0x5f, 0x00, 0x00 },  // 21 !
			  { 0x00, 0x07, 0x00, 0x07, 0x00 },  // 22 "
			  { 0x14, 0x7f, 0x14, 0x7f, 0x14 },  // 23 #
			  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },  // 24 $
			  { 0x23, 0x13, 0x08, 0x64, 0x62 },  // 25 %
			  { 0x36, 0x49, 0x55, 0x22, 0x50 },  // 26 &
			  { 0x00, 0x05, 0x03, 0x00, 0x00 },  // 27 '
			  { 0x00, 0x1c, 0x22, 0x41, 0x00 },  // 28 (
			  { 0x00, 0x41, 0x22, 0x1c, 0x00 },  // 29 )
			  { 0x14, 0x08, 0x3e, 0x08, 0x14 },  // 2a *
			  { 0x08, 0x08, 0x3e, 0x08, 0x08 },  // 2b +
			  { 0x00, 0x50, 0x30, 0x00, 0x00 },  // 2c ,
			  { 0x08, 0x08, 0x08, 0x08, 0x08 },  // 2d -
			  { 0x00, 0x60, 0x60, 0x00, 0x00 },  // 2e .
			  { 0x20, 0x10, 0x08, 0x04, 0x02 },  // 2f /
			  { 0x3e, 0x51, 0x49, 0x45, 0x3e },  // 30 0
			  { 0x00, 0x42, 0x7f, 0x40, 0x00 },  // 31 1
			  { 0x42, 0x61, 0x51, 0x49, 0x46 },  // 32 2
			  { 0x21, 0x41, 0x45, 0x4b, 0x31 },  // 33 3
			  { 0x18, 0x14, 0x12, 0x7f, 0x10 },  // 34 4
			  { 0x27, 0x45, 0x45, 0x45, 0x39 },  // 35 5
			  { 0x3c, 0x4a, 0x49, 0x49, 0x30 },  // 36 6
			  { 0x01, 0x71, 0x09, 0x05, 0x03 },  // 37 7
			  { 0x36, 0x49, 0x49, 0x49, 0x36 },  // 38 8
			  { 0x06, 0x49, 0x49, 0x29, 0x1e },  // 39 9
			  { 0x00, 0x36, 0x36, 0x00, 0x00 },  // 3a :
			  { 0x00, 0x56, 0x36, 0x00, 0x00 },  // 3b ;
			  { 0x08, 0x14, 0x22, 0x41, 0x00 },  // 3c <
			  { 0x14, 0x14, 0x14, 0x14, 0x14 },  // 3d =
			  { 0x00, 0x41, 0x22, 0x14, 0x08 },  // 3e >
			  { 0x02, 0x01, 0x51, 0x09, 0x06 },  // 3f ?
			  { 0x32, 0x49, 0x79, 0x41, 0x3e },  // 40 @
			  { 0x7e, 0x11, 0x11, 0x11, 0x7e },  // 41 A
			  { 0x7f, 0x49, 0x49, 0x49, 0x36 },  // 42 B
			  { 0x3e, 0x41, 0x41, 0x41, 0x22 },  // 43 C
			  { 0x7f, 0x41, 0x41, 0x22, 0x1c },  // 44 D
			  { 0x7f, 0x49, 0x49, 0x49, 0x41 },  // 45 E
			  { 0x7f, 0x09, 0x09, 0x09, 0x01 },  // 46 F
			  { 0x3e, 0x41, 0x49, 0x49, 0x7a },  // 47 G
			  { 0x7f, 0x08, 0x08, 0x08, 0x7f },  // 48 H
			  { 0x00, 0x41, 0x7f, 0x41, 0x00 },  // 49 I
			  { 0x20, 0x40, 0x41, 0x3f, 0x01 },  // 4a J
			  { 0x7f, 0x08, 0x14, 0x22, 0x41 },  // 4b K
			  { 0x7f, 0x40, 0x40, 0x40, 0x40 },  // 4c L
			  { 0x7f, 0x02, 0x0c, 0x02, 0x7f },  // 4d M
			  { 0x7f, 0x04, 0x08, 0x10, 0x7f },  // 4e N
			  { 0x3e, 0x41, 0x41, 0x41, 0x3e },  // 4f O
			  { 0x7f, 0x09, 0x09, 0x09, 0x06 },  // 50 P
			  { 0x3e, 0x41, 0x51, 0x21, 0x5e },  // 51 Q
			  { 0x7f, 0x09, 0x19, 0x29, 0x46 },  // 52 R
			  { 0x46, 0x49, 0x49, 0x49, 0x31 },  // 53 S
			  { 0x01, 0x01, 0x7f, 0x01, 0x01 },  // 54 T
			  { 0x3f, 0x40, 0x40, 0x40, 0x3f },  // 55 U
			  { 0x1f, 0x20, 0x40, 0x20, 0x1f },  // 56 V
			  { 0x3f, 0x40, 0x38, 0x40, 0x3f },  // 57 W
			  { 0x63, 0x14, 0x08, 0x14, 0x63 },  // 58 X
			  { 0x07, 0x08, 0x70, 0x08, 0x07 },  // 59 Y
			  { 0x61, 0x51, 0x49, 0x45, 0x43 },  // 5a Z
			  { 0x00, 0x7f, 0x41, 0x41, 0x00 },  // 5b [
			  { 0x02, 0x04, 0x08, 0x10, 0x20 },  // 5c backslash 
			  { 0x00, 0x41, 0x41, 0x7f, 0x00 },  // 5d ]
			  { 0x04, 0x02, 0x01, 0x02, 0x04 },  // 5e ^
			  { 0x40, 0x40, 0x40, 0x40, 0x40 },  // 5f _
			  { 0x00, 0x01, 0x02, 0x04, 0x00 },  // 60 `
			  { 0x20, 0x54, 0x54, 0x54, 0x78 },  // 61 a
			  { 0x7f, 0x48, 0x44, 0x44, 0x38 },  // 62 b
			  { 0x38, 0x44, 0x44, 0x44, 0x20 },  // 63 c
			  { 0x38, 0x44, 0x44, 0x48, 0x7f },  // 64 d
			  { 0x38, 0x54, 0x54, 0x54, 0x18 },  // 65 e
			  { 0x08, 0x7e, 0x09, 0x01, 0x02 },  // 66 f
			  { 0x0c, 0x52, 0x52, 0x52, 0x3e },  // 67 g
			  { 0x7f, 0x08, 0x04, 0x04, 0x78 },  // 68 h
			  { 0x00, 0x44, 0x7d, 0x40, 0x00 },  // 69 i
			  { 0x20, 0x40, 0x44, 0x3d, 0x00 },  // 6a j 
			  { 0x7f, 0x10, 0x28, 0x44, 0x00 },  // 6b k
			  { 0x00, 0x41, 0x7f, 0x40, 0x00 },  // 6c l
			  { 0x7c, 0x04, 0x18, 0x04, 0x78 },  // 6d m
			  { 0x7c, 0x08, 0x04, 0x04, 0x78 },  // 6e n
			  { 0x38, 0x44, 0x44, 0x44, 0x38 },  // 6f o
			  { 0x7c, 0x14, 0x14, 0x14, 0x08 },  // 70 p
			  { 0x08, 0x14, 0x14, 0x18, 0x7c },  // 71 q
			  { 0x7c, 0x08, 0x04, 0x04, 0x08 },  // 72 r
			  { 0x48, 0x54, 0x54, 0x54, 0x20 },  // 73 s
			  { 0x04, 0x3f, 0x44, 0x40, 0x20 },  // 74 t
			  { 0x3c, 0x40, 0x40, 0x20, 0x7c },  // 75 u
			  { 0x1c, 0x20, 0x40, 0x20, 0x1c },  // 76 v
			  { 0x3c, 0x40, 0x30, 0x40, 0x3c },  // 77 w
			  { 0x44, 0x28, 0x10, 0x28, 0x44 },  // 78 x
			  { 0x0c, 0x50, 0x50, 0x50, 0x3c },  // 79 y
			  { 0x44, 0x64, 0x54, 0x4c, 0x44 },  // 7a z
			  { 0x00, 0x08, 0x36, 0x41, 0x00 },  // 7b {
			  { 0x00, 0x00, 0x7f, 0x00, 0x00 },  // 7c |
			  { 0x00, 0x41, 0x36, 0x08, 0x00 },  // 7d }
			  { 0x10, 0x08, 0x08, 0x10, 0x08 },  // 7e arrow left
			  { 0x00, 0x06, 0x09, 0x09, 0x06 }   // 7f arrow right
			};
	

	int _buffer[][] = new int[LCD_PCD8544_LINES][LCD_PCD8544_WIDTH]; 
	int _column = 0;
	int _line = 0;
	
	public LCDTextPanel() {
	}
	
	public void begin() {	
		for (int y=0; y<LCD_PCD8544_LINES; y++) {
			for (int x=0; x<LCD_PCD8544_WIDTH; x++)
				_buffer[y][x] = 0;
		}
	}
	
	public void begin(int bias) {	
		begin();
	}
	
	public void clear() {
		begin();
		repaint();
	}
	
	public void clearLine(int line) {
		for (int x=0; x<LCD_PCD8544_WIDTH; x++) _buffer[line][x] = 0;
		repaint();
	}
    
	public void write(int c) {
		if (c < 32 || c > 127) return;
		for (int i=0; i<5; i++) send(LCD_PCD8544_DATA, font_5x7[c-32][i]);
		send(LCD_PCD8544_DATA, 0x00);
		repaint();
	    _column = _column % LCD_PCD8544_WIDTH;
	    if (_column == 0) _line = (_line + 1) % LCD_PCD8544_LINES;
	}
	
	public void print(String s) {
		for (int i=0; i<s.length(); i++) write(s.charAt(i));
	}
		
	public void setCursor(int column, int line) {
		_column = (column * 6) % LCD_PCD8544_WIDTH;
		_line = line % LCD_PCD8544_LINES;
	}

	
	public void drawBitmap(int data[], int x, int y, int width, int height) {
		if ((x > LCD_PCD8544_WIDTH) || (y > LCD_PCD8544_HEIGHT)) return;
		
		// clip the bitmap to the screen
		int line = y / 8;
		int stopLine = (y + height > LCD_PCD8544_HEIGHT ? LCD_PCD8544_HEIGHT-y : height) / 8;
		if (stopLine == 0) stopLine = 1;
		int stopColumn = (x + width > LCD_PCD8544_WIDTH ? LCD_PCD8544_WIDTH-x : width);
		
		for (int l = 0; l < stopLine; l++) {
			setXYAddress(x, line + l);
			for (int c = 0; c < stopColumn; c++) {
				send(LCD_PCD8544_DATA, data[l*width+c]);
			}
		}
		repaint();
		setCursor(_column, _line);
	}
	
	protected void send(int type, int value) {
		_buffer[_line][_column++] = value;
	}
	
	protected void setXYAddress(int x, int line) {
	    _column = (x % LCD_PCD8544_WIDTH);
	    _line = (line % LCD_PCD8544_LINES);
	}
	
	public void drawProgressBar(int column, int line, int size, int percentage, boolean filled) {
		// sanity checks
		if ((size < 2) || (size > LCD_PCD8544_COLUMNS) || (column + size > LCD_PCD8544_COLUMNS) || (percentage > 100) || (line >= LCD_PCD8544_LINES)) return;

		int range = 4 + 6 * (size - 2);
		int filling = percentage * range / 100;
		
		setXYAddress(column * LCD_PCD8544_CHAR_WIDTH, line);
		for (int i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
		send(LCD_PCD8544_DATA, 0x7F);	
		if (filled) {
			for (int i=0; i<filling; i++) send(LCD_PCD8544_DATA, 0x7F);
			for (int i=filling; i<range; i++) send(LCD_PCD8544_DATA, 0x41);			
		} else {
			// draw an empty bar
			for (int i=0; i<range; i++) send(LCD_PCD8544_DATA, 0x41);
			// draw the double bar
			if (filling < 2) setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4, line);
			else setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4 + filling - 2, line);
			send(LCD_PCD8544_DATA, 0x7F);
			send(LCD_PCD8544_DATA, 0x7F);
			setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4 + range, line);
		}
		send(LCD_PCD8544_DATA, 0x7F);
		for (int i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
		repaint();
	}
	
	public void drawToggleSwitch(int column, int line, int size, int nbPositions, int position) {
		// sanity checks
		if ((size < 2) || (size > LCD_PCD8544_COLUMNS) || (column + size > LCD_PCD8544_COLUMNS) || (position >= nbPositions) || (line >= LCD_PCD8544_LINES)) return;

		int range = 4 + 6 * (size - 2);
		int fillingStart = position * range / nbPositions;
		int filling = range / nbPositions;
		while (filling * nbPositions < range) filling ++;
		
		setXYAddress(column * LCD_PCD8544_CHAR_WIDTH, line);
		for (int i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
		// draw an empty bar
		send(LCD_PCD8544_DATA, 0x7F);	
		for (int i=0; i<range; i++) send(LCD_PCD8544_DATA, 0x41);
		send(LCD_PCD8544_DATA, 0x7F);
		for (int i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
		// draw the toggle
		setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4 + fillingStart, line);
		for (int i=0; i<filling; i++) send(LCD_PCD8544_DATA, 0x7F);
	}

	
	public Dimension getPreferredSize() {
		return new Dimension(Math.round((LCD_PCD8544_WIDTH*DOT_SIZE+2*BORDER)*SCALE_X), 
				Math.round((LCD_PCD8544_HEIGHT*DOT_SIZE+2*BORDER)*SCALE_Y));
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(230, 255, 200));
		g2d.fillRect(0, 0, getWidth(), getHeight());	
		g2d.scale(SCALE_X, SCALE_Y);
		g2d.setColor(new Color(223, 247, 189));
		g2d.fillRect(BORDER, BORDER, LCD_PCD8544_WIDTH*DOT_SIZE, LCD_PCD8544_HEIGHT*DOT_SIZE);	
		g2d.setColor(Color.BLACK);
		for (int y=0; y<LCD_PCD8544_LINES; y++) {
			for (int x=0; x<LCD_PCD8544_WIDTH; x++) {
				for (int p=0; p<8; p++) {
					if ((_buffer[y][x] & (1 << p)) != 0) 
						g.fillRect(x*DOT_SIZE+BORDER, y*(8*DOT_SIZE)+p*DOT_SIZE+BORDER, DOT_SIZE, DOT_SIZE);
				}
			}
		}
	}

}
