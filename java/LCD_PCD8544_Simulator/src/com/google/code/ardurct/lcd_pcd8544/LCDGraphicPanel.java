package com.google.code.ardurct.lcd_pcd8544;

public class LCDGraphicPanel extends LCDTextPanel {
	
	private int font_3x5[][] = {
			  { 0x00, 0x00, 0x00 },  // 20 space
			  { 0x00, 0x17, 0x00 },	 // 21 !
			  { 0x03, 0x00, 0x03 },  // 22 "
			  { 0x1F, 0x0A, 0x1F },  // 23 #
			  { 0x1C, 0x14, 0x1F },  // 24 $
			  { 0x19, 0x04, 0x13 },  // 25 %
			  { 0x1C, 0x17, 0x04 },  // 26 &
			  { 0x00, 0x03, 0x00 },  // 27 '
			  { 0x0E, 0x1B, 0x11 },  // 28 (
			  { 0x11, 0x1B, 0x0E },  // 29 )
			  { 0x0A, 0x04, 0x0A },  // 2a *
			  { 0x04, 0x0E, 0x04 },  // 2b +
			  { 0x10, 0x0C, 0x00 },  // 2c ,
			  { 0x04, 0x04, 0x04 },  // 2d -
			  { 0x18, 0x18, 0x00 },  // 2e .
			  { 0x18, 0x04, 0x03 },  // 2f /
			  { 0x1F, 0x11, 0x1F },  // 30 0
			  { 0x12, 0x1F, 0x10 },  // 31 1
			  { 0x19, 0x15, 0x17 },  // 32 2
			  { 0x15, 0x15, 0x0A },  // 33 3
			  { 0x07, 0x04, 0x1F },  // 34 4
			  { 0x17, 0x15, 0x09 },  // 35 5
			  { 0x1F, 0x15, 0x1D },  // 36 6
			  { 0x01, 0x01, 0x1F },  // 37 7
			  { 0x1F, 0x15, 0x1F },  // 38 8
			  { 0x17, 0x15, 0x1F },  // 39 9
			  { 0x0A, 0x00, 0x00 },  // 3a :
			  { 0x10, 0x0A, 0x00 },  // 3b ;
			  { 0x04, 0x0A, 0x11 },  // 3c <
			  { 0x0A, 0x0A, 0x0A },  // 3d =
			  { 0x11, 0x0A, 0x04 },  // 3e >
			  { 0x01, 0x15, 0x03 },  // 3f ?
			  { 0x0E, 0x15, 0x12 },  // 40 @
			  { 0x1E, 0x05, 0x1E },  // 41 A
			  { 0x1F, 0x15, 0x0A },  // 42 B
			  { 0x0E, 0x11, 0x11 },  // 43 C
			  { 0x1F, 0x11, 0x0E },  // 44 D
			  { 0x1F, 0x15, 0x11 },  // 45 E
			  { 0x1F, 0x05, 0x01 },  // 46 F
			  { 0x1F, 0x11, 0x1D },  // 47 G
			  { 0x1F, 0x04, 0x1F },  // 48 H
			  { 0x11, 0x1F, 0x11 },  // 49 I
			  { 0x09, 0x11, 0x1F },  // 4a J
			  { 0x1F, 0x04, 0x1B },  // 4b K
			  { 0x1F, 0x10, 0x10 },  // 4c L
			  { 0x1F, 0x02, 0x1F },  // 4d M
			  { 0x1F, 0x02, 0x1E },  // 4e N
			  { 0x0E, 0x11, 0x0E },  // 4f O
			  { 0x1F, 0x05, 0x07 },  // 50 P
			  { 0x07, 0x05, 0x1F },  // 51 Q
			  { 0x1F, 0x05, 0x1A },  // 52 R
			  { 0x17, 0x15, 0x1D },  // 53 S
			  { 0x01, 0x1F, 0x01 },  // 54 T
			  { 0x1F, 0x10, 0x1F },  // 55 U
			  { 0x0F, 0x10, 0x0F },  // 56 V
			  { 0x1F, 0x08, 0x1F },  // 57 W
			  { 0x1B, 0x04, 0x1B },  // 58 X
			  { 0x07, 0x1C, 0x07 },  // 59 Y
			  { 0x19, 0x15, 0x13 },  // 5a Z
			  { 0x1F, 0x11, 0x11 },  // 5b [
			  { 0x03, 0x04, 0x18 },  // 5c backslash 
			  { 0x11, 0x11, 0x1F },  // 5d ]
			  { 0x02, 0x01, 0x02 },  // 5e ^
			  { 0x10, 0x10, 0x10 },  // 5f _
			  { 0x01, 0x02, 0x00 }   // 60 `
	};
	
	private int font_4x6[][] = {
			  { 0x1E, 0x21, 0x21, 0x1E },  // 0
			  { 0x24, 0x22, 0x3F, 0x20 },  // 1
			  { 0x32, 0x29, 0x29, 0x26 },  // 2
			  { 0x21, 0x25, 0x25, 0x1A },  // 3
			  { 0x0F, 0x08, 0x3E, 0x08 },  // 4
			  { 0x27, 0x25, 0x25, 0x19 },  // 5
			  { 0x1E, 0x25, 0x25, 0x19 },  // 6
			  { 0x03, 0x01, 0x01, 0x3F },  // 7
			  { 0x1A, 0x25, 0x25, 0x1A },  // 8
			  { 0x26, 0x29, 0x29, 0x1E },  // 9
			  { 0x00, 0x00, 0x00, 0x00 }   // space
			};
	
	private static final long serialVersionUID = -3355396623405797901L;
	
	public LCDGraphicPanel() {
		for (int y=0; y<LCD_PCD8544_LINES; y++) {
			for (int x=0; x<LCD_PCD8544_WIDTH; x++)
				_buffer[y][x] = 0;
		}
	}
		
	private int drawMediumFontDigit(int val, int x, int y) {
		drawBitmap(font_4x6[val == 0x20 ? 10 : val], x, y, 4, 6);
		return x+5;
	}
	
	public int drawMediumFontValueRA(int value, int maxL, int space, int x, int y, boolean isTenTimes) {
		int lx = x;
		int val = value;
		int maxLength = maxL;
		if (isTenTimes) {
			val /= 10;
			if (val < 100) lx += 3;
			else maxLength += 2;
		}
		if (val >= 10000) return x;
		if (maxLength >= 4) {
			if (val >= 1000) lx = drawMediumFontDigit(val/1000, lx, y);
			else lx = drawMediumFontDigit(space, lx, y);
		}
		if (maxLength >= 3) {
			if (val >= 100) lx = drawMediumFontDigit(val/100-(10*(val/1000)), lx, y);
			else lx = drawMediumFontDigit(space, lx, y);
		}
		if (maxLength >= 2) {
			if (val >= 10)  lx = drawMediumFontDigit(val/10-(10*(val/100)), lx, y);
			else  lx = drawMediumFontDigit(space, lx, y);
		}
		lx = drawMediumFontDigit(val % 10, lx, y);
		if ((isTenTimes) && (val < 100)) {
			drawMediumFontDigit(' ', lx, y);
			bufferPixel(lx, y+5, true);
			drawMediumFontDigit(value%10, lx+2, y);			
		}
		return lx;
	}

	public void drawMediumFontTime(int hours, int minutes, int seconds, int x, int y) {
		int lx = x+10;
		if (hours > 0) {
			lx = drawMediumFontValueRA(hours, 2, ' ', x, y, false);
			drawMediumFontDigit(' ', lx, y);
			bufferPixel(lx, y+2, true);
			bufferPixel(lx, y+4, true);
		}
		lx = drawMediumFontValueRA(minutes, 2, hours > 0 ? 0 : ' ', lx+2, y, false);
		drawMediumFontDigit(' ', lx, y);
		bufferPixel(lx, y+2, true);
		bufferPixel(lx, y+4, true);
		drawMediumFontValueRA(seconds, 2, 0, lx+2, y, false);
	}

	public void drawSmallFontText(String s, int x, int y) {
		for (int i=0; i<s.length(); i++) {
			int c = s.charAt(i);
			if (c >= 0x20) {
				if (c > 0x60 && c < 0x7B) c -= 'a'-'A';
				drawBitmap(font_3x5[c-0x20], x+i*4, y, 3, 5);
			}
		}
	}
	
	public void setXCursor(int x, int line) {
		_column = x % LCD_PCD8544_WIDTH;
		_line = line % LCD_PCD8544_LINES;
	}

	private void bufferPixel(int x, int y, boolean on) {
		if ((x < 0) || (y < 0) || (x >= LCD_PCD8544_WIDTH) || (y >= LCD_PCD8544_HEIGHT)) return;
		
		int mask = 0x01 << (y & 0x07);
		int line = y >> 3;
		
		if (on) _buffer[line][x] |= mask;
		else _buffer[line][x] &= ~mask;	
	}
	
	private boolean bufferHLine(int x1, int x2, int y, boolean on) {
		if (y < 0 || y >= LCD_PCD8544_HEIGHT) return false;
		if (x1 < 0 || x1 >= LCD_PCD8544_WIDTH) x1 = LCD_PCD8544_WIDTH-1;
		if (x2 < 0 || x2 >= LCD_PCD8544_WIDTH) x2 = LCD_PCD8544_WIDTH-1;
		
		int mask = 0x01 << (y & 0x07);
		int line = y >> 3;
		
		if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
		for (int x = x1; x <= x2; x++) {
			if (on) _buffer[line][x] |= mask;
			else _buffer[line][x] &= ~mask;
		}
		return true;
	}

	private void bufferVLine(int x, int y1, int y2, boolean on) {
		if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
		for (int y = y1; y <= y2; y++) bufferPixel(x, y, on);
	}
	
	private void bufferQuadrants(int x0, int y0, int x, int y, boolean on, boolean filled) {
		if (filled) {
			bufferHLine(x0-x, x0+x, y0+y, on);
			bufferHLine(x0-x, x0+x, y0-y, on);
		} else {
			bufferPixel(x0+x, y0+y, on);
			bufferPixel(x0+x, y0-y, on);
			bufferPixel(x0-x, y0-y, on);
			bufferPixel(x0-x, y0+y, on);
		}
	}

	/*
	 *	From Wikipedia - Bresenham's line algorithm
	 */
	private void bufferCircle(int x0, int y0, int r, boolean on, boolean filled) {
		int f = 1 - r;
		int ddF_x = 1;
		int ddF_y = -2 * r;
		int x = 0;
		int y = r;

		bufferPixel(x0, y0+r, on);
		bufferPixel(x0, y0-r, on);
		bufferPixel(x0+r, y0, on);
		bufferPixel(x0-r, y0, on);
		while (x < y) {
			if (f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			bufferQuadrants(x0, y0, x, y, on, filled);
			bufferQuadrants(x0, y0, y, x, on, filled);
		}
	}

	private void bufferEllipse(int x0, int y0, int a, int b, boolean on, boolean filled) {
		int aa = a*a;
		int bb = b*b;
		int aa2 = aa*2;
		int bb2 = bb*2;
		int x = 0;
		int y = b;
		int fx = 0;
		int fy = aa2*b;
		int p = (int)(bb-(aa*b)+(0.25*aa)+0.5);
		
		bufferPixel(x0, y0+b, on);
		bufferPixel(x0, y0-b, on);
		bufferPixel(x0+a, y0, on);
		bufferPixel(x0-a, y0, on);
		while (fx < fy) {
			x++;
			fx += bb2;
			if (p < 0) p += fx + bb;
			else {
				y--;
				fy -= aa2;
				p += fx + bb - fy;
			}
			bufferQuadrants(x0, y0, x, y, on, filled);
		}
		p = (int)((bb*(x+0.5)*(x+0.5))+(aa*(y-1)*(y-1))-(aa*bb)+0.5);
		while (y > 0) {
			y--;
			fy -= aa2;
			if (p >= 0) p += aa - fy;
			else {
				x++;
				fx += bb2;
				p += fx + aa - fy;
			}
			bufferQuadrants(x0, y0, x, y, on, filled);
		}
	}
		
	public void drawBitmap(int data[], int x, int y, int width, int height) {
		// draw in the frame
		if ((x > LCD_PCD8544_WIDTH) || (y > LCD_PCD8544_HEIGHT)) return;
		
		// clip the bitmap to the screen
		int line = y / 8;
		int shift = y % 8;
		int mask = 0;
		for (int i=0; i<height && i<8; i++) mask = mask*2+1;
		byte lsmask = (byte)~(mask << shift);
		byte rsmask = (byte)~(mask >> (8-shift));
		int stopLine = (y + height > LCD_PCD8544_HEIGHT ? LCD_PCD8544_HEIGHT-y : height) / 8;
		if (stopLine == 0) stopLine = 1;
		int stopColumn = (x + width > LCD_PCD8544_WIDTH ? LCD_PCD8544_WIDTH-x : width);
		
		int pix8;
		
		for (int l = 0; l < stopLine; l++) {
			for (int c = 0; c < stopColumn; c++) {
				pix8 = data[l*width+c] << shift;
				_buffer[line+l][x+c] &= lsmask;
				_buffer[line+l][x+c] |= pix8;
				if ((shift != 0) && (line+l+1 < LCD_PCD8544_LINES)) {
					pix8 = data[l*width+c] >> (8-shift);
					_buffer[line+l+1][x+c] &= rsmask;
					_buffer[line+l+1][x+c] |= pix8;
				}
			}
		}
	}

	/*
	 *	From Wikipedia - Bresenham's line algorithm
	 */
	public boolean drawLine(int x1, int y1, int x2, int y2, boolean on, int thickness) {
		int d1 = (thickness-1) / 2;
		int d2 = thickness / 2;
		int dx = Math.abs(x2-x1);
		if (dx == 0) {
			if ((d1 > 0) || (d2 > 0)) fillRectangle(x1-d1, y1, x1+d2, y2, on);
			else bufferVLine(x1, y1, y2, on); 
			return false;
		}
		int dy = Math.abs(y2-y1);
		if (dy == 0) {
			if ((d1 > 0) || (d2 > 0)) fillRectangle(x1, y1-d1, x2, y2+d2, on);
			else bufferHLine(x1, x2, y1, on); 
			return false;
		}
		
		int sx = -1;
		if (x1 < x2) sx = 1;
		int sy = -1;
		if (y1 < y2) sy = 1;
		int err = dx - dy;
		
		while (true) {
			if ((d1 > 0) || (d2 > 0)) fillRectangle(x1-d1, y1-d1, x1+d2, y1+d2, on);
			else bufferPixel(x1, y1, on);
			if ((x1 == x2) && (y1 == y2)) break;
			int e2 = 2 * err;
			if (e2 > -dy) {
				err = err - dy;
				x1 = x1 + sx;
			}
			if (e2 < dx) {
				err = err + dx;
				y1 = y1 + sy;
			}
		}
		return true;
	}

	
	public void drawRectangle(int x1, int y1, int x2, int y2, boolean on) {	
		for (int x = x1; x <= x2; x++) {
			bufferPixel(x, y1, on);
			bufferPixel(x, y2, on);
		}
		for (int y = y1; y <= y2; y++) {
			bufferPixel(x1, y, on);
			bufferPixel(x2, y, on);
		}
	}
	
	public void drawEllipse(int x1, int y1, int a, int b, boolean on) {
		bufferEllipse(x1, y1, a, b, on, false);
	}

	public void fillRectangle(int x1, int y1, int x2, int y2, boolean on) {
		if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
		for (int y = y1; y <= y2; y++) bufferHLine(x1, x2, y, on);
	}
	
	public void fillEllipse(int x1, int y1, int a, int b, boolean on) {
		bufferEllipse(x1, y1, a, b, on, true);
	}

	public void drawCircle(int x0, int y0, int r, boolean on) {
		bufferCircle(x0, y0, r, on, false);
	}

	public void fillCircle(int x0, int y0, int r, boolean on) {
		bufferCircle(x0, y0, r, on, true);
	}
		
	public void setXYAddress(int x, int line) {
	    _column = (x % LCD_PCD8544_WIDTH);
	    _line = (line % LCD_PCD8544_LINES);
	}
	
	public void drawProgressBar(int column, int line, int size, int percentage, boolean filled) {
		// sanity checks
		if ((size < 2) || (size > LCD_PCD8544_COLUMNS) || (column + size > LCD_PCD8544_COLUMNS) || (percentage > 100) || (line >= LCD_PCD8544_LINES)) return;

		int x1 = 3 + column * LCD_PCD8544_CHAR_WIDTH;
		int x2 = (column + size) * LCD_PCD8544_CHAR_WIDTH - 3;
		int y1 = line * 8;
		int y2 = y1 + 6;
		fillRectangle(x1-3, y1, x2+3, y2, false);
		drawRectangle(x1, y1, x2, y2, true);
		
		x2 = 3 + percentage * (x2 - x1) / 100;
		if (filled) fillRectangle(x1, y1, x2, y2, true);
		else {
			bufferVLine(x2, y1, y2, true);
			bufferVLine(x2-1, y1, y2, true);
		}
	}
	
	public void drawToggleSwitch(int x, int y, int width, int nbPositions, int position) {
		// sanity checks
		if ((width < 8) || (width > LCD_PCD8544_WIDTH) || (x + width > LCD_PCD8544_WIDTH) || (position >= nbPositions) || (y >= LCD_PCD8544_HEIGHT)) return;

		int x1 = 3 + x;
		int x2 = x + width - 3;
		int y1 = y;
		int y2 = y1 + 6;
		fillRectangle(x1-3, y1, x2+3, y2, false);
		drawRectangle(x1, y1, x2, y2, true);
		
		x2 = (width - 6) / nbPositions;
		while (x2 * nbPositions < width - 6) x2++;
		x1 = position * (width - 6) / nbPositions;
		x2 += x1;
		fillRectangle(x1, y1, x2, y2, true);
	}

	
	public void updateDisplay() {
		repaint();
	}
}
