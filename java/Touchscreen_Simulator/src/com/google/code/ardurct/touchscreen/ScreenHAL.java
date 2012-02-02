package com.google.code.ardurct.touchscreen;


public class ScreenHAL extends FontBitmaps {

	private static final long serialVersionUID = 1L;

	public static final int SCREENHAL_MAX_BITMAP_SPACE = (32*32);		

	public static final int SCREEN_ROTATION_0	= 0;
	public static final int SCREEN_ROTATION_90	= 1;
	public static final int SCREEN_ROTATION_180	= 2;
	public static final int SCREEN_ROTATION_270	= 3;

	public static final int SCREEN_ARC_SSE = (1 << 0);
	public static final int SCREEN_ARC_SEE = (1 << 1);
	public static final int SCREEN_ARC_NEE = (1 << 2);
	public static final int SCREEN_ARC_NNE = (1 << 3);
	public static final int SCREEN_ARC_SSW = (1 << 4);
	public static final int SCREEN_ARC_SWW = (1 << 5);
	public static final int SCREEN_ARC_NWW = (1 << 6);
	public static final int SCREEN_ARC_NNW = (1 << 7);

	public static final int SCREEN_ARC_NE = (SCREEN_ARC_NNE+SCREEN_ARC_NEE);
	public static final int SCREEN_ARC_SE = (SCREEN_ARC_SSE+SCREEN_ARC_SEE);
	public static final int SCREEN_ARC_SW = (SCREEN_ARC_SSW+SCREEN_ARC_SWW);
	public static final int SCREEN_ARC_NW = (SCREEN_ARC_NNW+SCREEN_ARC_NWW);

	public static final int SCREEN_ARC_N = (SCREEN_ARC_NE+SCREEN_ARC_NW);
	public static final int SCREEN_ARC_S = (SCREEN_ARC_SE+SCREEN_ARC_SW);
	public static final int SCREEN_ARC_E = (SCREEN_ARC_SE+SCREEN_ARC_NE);
	public static final int SCREEN_ARC_W = (SCREEN_ARC_NW+SCREEN_ARC_SW);

	public static final int SCREEN_ARC_ALL =(SCREEN_ARC_N+SCREEN_ARC_S);

	public static final int PATTERN_ORIENTATION_HORIZONTAL = 0;
	public static final int PATTERN_ORIENTATION_VERTICAL = 1;

	public static final boolean OVERLAY = true;
	public static final boolean NO_OVERLAY = false;

	
	int _width;
	int _height;
	int _x;
	int _y;
	int _margin;
	int _rotation;
	int _backgroundColor;
	int _foregroundColor;
	int _fontSize;
	boolean _isFontBold;
	boolean _isFontOverlay;
	int _fontScale;
	int _thickness;
	int _mThickness;
	int _mX;
	int _mY;
	int _mScaleMul;
	int _mScaleDiv;
	int _mBackgroundColor;
	int _mForegroundColor;
	int _mFontSize;
	boolean _mIsFontBold;
	boolean _mIsFontOverlay;
	fontDefinition_t _fontDef;
	
	ScreenHAL() {
	}

	void initScreen() {		
		_width = getWidthImpl();
		_height = getHeightImpl();
		_rotation = 0;
		_margin = 0;
		setFont(1, FONT_PLAIN, NO_OVERLAY);
		initScreenImpl();
		_initializeMacros();
	}

	void _initializeMacros() {
		_mX = 0;
		_mY = 0;
		_mScaleMul = 1;
		_mScaleDiv = 1;
		_mBackgroundColor = _backgroundColor;
		_mForegroundColor = _foregroundColor;
		_mFontSize = _fontSize;
		_mIsFontOverlay = _isFontOverlay;
		_mIsFontBold = _isFontBold;
		_mThickness = _thickness;
	}
	
	public void setRotation(int rotation) {
		if (rotation > SCREEN_ROTATION_270) rotation = SCREEN_ROTATION_0;
		if ((rotation == SCREEN_ROTATION_0) || (rotation == SCREEN_ROTATION_180)) {
			_width = getWidthImpl();
			_height = getHeightImpl();
		} else {
			_width = getHeightImpl();
			_height = getWidthImpl();
		}
		setRotationImpl(rotation);
		_rotation = rotation;
	}


	int getRotation() {
		return _rotation;
	}


	void getRotatedXY(int x[], int y[], int rotation) {
		if (rotation == SCREEN_ROTATION_0) return;

		int X = x[0];
		int Y = y[0];
		
		if (rotation == SCREEN_ROTATION_180) {
			x[0] = getWidthImpl() - X;
			y[0] = getHeightImpl() - Y;
		} else if (rotation == SCREEN_ROTATION_90) {
			x[0] = getHeightImpl() - Y;
			y[0] = X;
		} else if (rotation == SCREEN_ROTATION_270) {
			x[0] = Y;
			y[0] = getWidthImpl() - X;
		} 
	}


	boolean isVertical() {
		return ((_rotation == SCREEN_ROTATION_0) || (_rotation == SCREEN_ROTATION_180));
	}

	int getForegroundColor() {
		return _foregroundColor;
	}

	public void setForegroundColor(int color) {
		_foregroundColor = color;
	}


	int getBackgroundColor() {
		return _backgroundColor;
	}

	void setBackgroundColor(int color) {
		_backgroundColor = color;
	}

	
	public void setThickness(int thickness) {
		_thickness = thickness;
	}

	int getThickness(int thickness) {
		return _thickness;
	}

	int getMargin() {
		return _margin;
	}

	void setMargin(int margin) {
		_margin = margin;
	}


	int getFontSize() {
		return _fontSize;
	}

	boolean isFontBold() {
		return _isFontBold;
	}

	boolean isFontOverlay() {
		return _isFontOverlay;
	}

	void setFont(int size, boolean bold, boolean overlay) {
		_fontSize = size;
		_isFontBold = bold;
		_isFontOverlay = overlay;
		if (_fontSize > FONT_LAST_DEF*2) _fontSize = FONT_LAST_DEF*2;
		if (_fontSize == 0) _fontSize = 1;
		_fontScale = (_fontSize <= FONT_LAST_DEF ? 1 : 2);
		_fontDef = fontDefinition_small;
		if ((_fontSize == 2) || (_fontSize == 4)) _fontDef = fontDefinition_medium;
	}


	public void setCursor(int x, int y) {
		_x = x;
		_y = y;
	}


	public int getWidth() {
		return _width;
	}

	public int getHeight() {
		return _height;
	}


	void drawChar(int chr, int x, int y, int color, int fontSize, boolean isBold, boolean overlay, 
			boolean grabAndReleaseBus) {
		if ((x < 0) || (y < 0)) return;
		int validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
		int fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
		fontDefinition_t fontDef = fontDefinition_small;
		if ((validFontSize == 2) || (validFontSize == 4)) fontDef = fontDefinition_medium;
		if ((x + fontDef.width * fontScale >= _width) || (y + fontDef.height * fontScale >= _height)) return;
		if ((chr < fontDef.firstChar) || (chr > fontDef.lastChar)) return;
		if (grabAndReleaseBus) _grabBus();
		_drawValidChar(chr, x, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
		if (grabAndReleaseBus) _releaseBus();
	}
	
	void drawString(int s[], int x, int y, int color, int fontSize, boolean isBold, boolean overlay, 
			boolean grabAndReleaseBus) {
		if ((x < 0) || (y < 0)) return;
		int validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
		int fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
		fontDefinition_t fontDef = fontDefinition_small;
		if ((validFontSize == 2) || (validFontSize == 4)) fontDef = fontDefinition_medium;
		if ((x + fontDef.width * fontScale >= _width) || (y + fontDef.height * fontScale >= _height)) return;
		
		if (grabAndReleaseBus) _grabBus();
		int i = 0;
		while (s[i] != 0) {
			if (s[i] == '\n') {
				y += (fontDef.height + fontDef.lineSpacing + (isBold ? 1 : 0)) * fontScale;
				if (y >= _height) break;
				x = 0;
			} else if (s[i] == '\r') {
				// skip the character
			} else {
				int validChr = s[i];
				if ((validChr < fontDef.firstChar) || (validChr > fontDef.lastChar)) validChr = '?';
				_drawValidChar(validChr, x, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
				x += (fontDef.width + fontDef.charSpacing) * fontScale;
				if (x > _width) {
					x = 0;
					y += (fontDef.height + fontDef.lineSpacing + (isBold ? 1 : 0)) * fontScale;
					if (y >= _height) break;
				}
			}
			i++;
		}
		if (grabAndReleaseBus) _releaseBus();
	}

	public void drawString(String s, int x, int y, int color, int fontSize, boolean isBold, boolean overlay) {
		int buffer[] = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) buffer[i] = s.charAt(i);
		buffer[s.length()] = 0;	
		drawString(buffer , x, y, color, fontSize, isBold, overlay, true);
	}
			
	void print(int[] s) {
		int i = 0;
		while (s[i] != 0) {
			if (s[i] == '\n') {
				_y += (_fontDef.height + _fontDef.lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
				if (_y >= _height) break;
				_x = 0;
			} else if (s[i] == '\r') {
				// skip the character
			} else {
				int validChr = s[i];
				if ((validChr < _fontDef.firstChar) || (validChr > _fontDef.lastChar)) validChr = '?';
				_drawValidChar(validChr, _x, _y, _foregroundColor, _fontSize, _fontDef, _fontScale, _isFontBold, _isFontOverlay);
				_x += (_fontDef.width + _fontDef.charSpacing) * _fontScale;
				if (_x > _width) {
					_x = 0;
					_y += (_fontDef.height + _fontDef.lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
					if (_y >= _height) _y = 0;
				}
			}
			i++;
		}
	}

	public void print(int n) {
		print(n + "");
	}

	public void print(long n) {
		print(n + "");
	}

	public void print(int n, int base) {
		print(Integer.toString(n, base));
	}

	public void print(String s) {
		int buffer[] = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) buffer[i] = s.charAt(i);
		buffer[s.length()] = 0;	
		print(buffer);
	}

	void drawPixel(int x, int y, int color, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();
		_drawBoundedPixel(x, y, color);
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawHorizontalLine(int x1, int x2, int y, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();
		if (thickness <= 1) _fillBoundedArea(x1, y, x2, y, color);
		else {
			int t1 = (thickness-1)/2;
			int t2 = thickness/2;
			_fillBoundedArea(x1-t1, y-t1, x2+t2, y+t2, color);
		}
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawVerticalLine(int x, int y1, int y2, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();
		if (thickness <= 1) _fillBoundedArea(x, y1, x, y2, color);
		else {
			int t1 = (thickness-1)/2;
			int t2 = thickness/2;
			_fillBoundedArea(x-t1, y1-t1, x+t2, y2+t2, color);
		}
		if (grabAndReleaseBus) _releaseBus();
	}


	/*
	 *	From Wikipedia - Bresenham's line algorithm
	 */
	void drawLine(int x1, int y1, int x2, int y2, int color, int thickness, boolean grabAndReleaseBus) {
		int dx = Math.abs(x2-x1);
		if (dx == 0) {
			drawVerticalLine(x1, y1, y2, color, thickness, grabAndReleaseBus);
			return;
		}
		int dy = Math.abs(y2-y1);
		if (dy == 0) {
			drawHorizontalLine(x1, x2, y1, color, thickness, grabAndReleaseBus);
			return;
		}
		
		int sx = -1;
		if (x1 < x2) sx = 1;
		int sy = -1;
		if (y1 < y2) sy = 1;
		int err = dx - dy;
		int d1 = (thickness-1) / 2;
		int d2 = thickness / 2;

		if (grabAndReleaseBus) _grabBus();	
		while (true) {
			if (thickness == 1) drawPixelImpl(x1, y1, color);
			else _fillBoundedArea(x1-d1, y1-d1, x1+d2, y1+d2, color);
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
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		drawLine(x0, y0, x1, y1, color, thickness, false);
		drawLine(x1, y1, x2, y2, color, thickness, false);
		drawLine(x2, y2, x0, y0, color, thickness, false);
		if (grabAndReleaseBus) _releaseBus();	
	}

	void swap(int a, int b) {
		int t = a; 
		a = b; 
		b = t; 
	}

	void swap(long a, long b) {
		long t = a; 
		a = b; 
		b = t; 
	}

	void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color, boolean grabAndReleaseBus) {
		if (y0 > y1) {
			swap(y0, y1); 
			swap(x0, x1);
		}
		if (y1 > y2) {
			swap(y2, y1); 
			swap(x2, x1);
		}
		if (y0 > y1) {
			swap(y0, y1); 
			swap(x0, x1);
		}

		int dx1, dx2, dx3;
		int sx1, sx2, sy;

		sx2 = (int)x0 * (int)1000;
		sx1 = sx2;
		sy = y0;

		// Calculate interpolation deltas
		if (y1-y0 > 0) dx1 = ((x1-x0)*1000)/(y1-y0);
		else dx1 = 0;
		if (y2-y0 > 0) dx2 = ((x2-x0)*1000)/(y2-y0);
		else dx2 = 0;
		if (y2-y1 > 0) dx3 = ((x2-x1)*1000)/(y2-y1);
		else dx3 = 0;

		// Render scanlines 
		if (grabAndReleaseBus) _grabBus();	
		if (dx1 > dx2) {
			for (; sy<=y1; sy++, sx1+=dx2, sx2+=dx1) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
			sx2 = x1 * 1000;
			sy = y1;
			for (; sy<=y2; sy++, sx1+=dx2, sx2+=dx3) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
		} else {
			for (; sy<=y1; sy++, sx1+=dx1, sx2+=dx2) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
			sx1 = x1*1000;
			sy = y1;
			for (; sy<=y2; sy++, sx1+=dx3, sx2+=dx2) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
		}
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawRectangle(int x, int y, int width, int height, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();
		drawHorizontalLine(x, x+width-1, y, color, thickness, false);
		drawHorizontalLine(x, x+width-1, y+height-1, color, thickness, false);
		drawVerticalLine(x, y, y+height-1, color, thickness, false);
		drawVerticalLine(x+width-1, y, y+height-1, color, thickness, false);
		if (grabAndReleaseBus) _releaseBus();
	}


	void fillRectangle(int x, int y, int width, int height, int color, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();
		_fillBoundedArea(x, y, x+width-1, y+height-1, color);
		if (grabAndReleaseBus) _releaseBus();
	}


	void fillScreen(int color, boolean grabAndReleaseBus) {
		fillRectangle(0, 0, getWidth(), getHeight(), color, grabAndReleaseBus);
	}


	void drawArc(int x0, int y0, int r, int octant, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x0, y0, r, octant, color, thickness, false);
		if (grabAndReleaseBus) _releaseBus();
	}


	void fillArc(int x0, int y0, int r, int octant, int color, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x0, y0, r, octant, color, 0, true);
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawCircle(int x0, int y0, int r, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, thickness, false);
		if (grabAndReleaseBus) _releaseBus();
	}


	public void fillCircle(int x0, int y0, int r, int color, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, 0, true);
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawRoundedRectangle(int x, int y, int width, int height, int r, int color, int thickness, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x+r, y+r, r, SCREEN_ARC_NW, color, thickness, false);
		drawHorizontalLine(x+r, x+width-1-r, y, color, thickness, false);
		_drawOrFillOctant(x+width-1-r, y+r, r, SCREEN_ARC_NE, color, thickness, false);
		drawVerticalLine(x+width-1, y+r, y+height-1-r, color, thickness, false);
		_drawOrFillOctant(x+r, y+height-1-r, r, SCREEN_ARC_SW, color, thickness, false);
		drawHorizontalLine(x+r, x+width-1-r, y+height-1, color, thickness, false);
		_drawOrFillOctant(x+width-1-r, y+height-1-r, r, SCREEN_ARC_SE, color, thickness, false);
		drawVerticalLine(x, y+r, y+height-1-r, color, thickness, false);
		if (grabAndReleaseBus) _releaseBus();
	}


	void fillRoundedRectangle(int x, int y, int width, int height, int r, int color, boolean grabAndReleaseBus) {
		if (grabAndReleaseBus) _grabBus();	
		_drawOrFillOctant(x+r, y+r, r, SCREEN_ARC_NW, color, 0, true);
		_drawOrFillOctant(x+width-1-r, y+r, r, SCREEN_ARC_NE, color, 0, true);
		_drawOrFillOctant(x+r, y+height-1-r, r, SCREEN_ARC_SW, color, 0, true);
		_drawOrFillOctant(x+width-1-r, y+height-1-r, r, SCREEN_ARC_SE, color, 0, true);
		_fillBoundedArea(x, y+r, x+width-1, y+height-1-r, color);
		_fillBoundedArea(x+r, y, x+width-1-r, y+r, color);
		_fillBoundedArea(x+r, y+height-1-r, x+width-1-r, y+height-1, color);
		if (grabAndReleaseBus) _releaseBus();
	}


	int[] retrieveBitmap(int buffer[], int x, int y, int width, int height, boolean grabAndReleaseBus) {
		if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height) || (width*height > SCREENHAL_MAX_BITMAP_SPACE)) return new int[0];
		if (grabAndReleaseBus) _grabBus();
		retrieveBitmapImpl(buffer, x, y, width, height);
		if (grabAndReleaseBus) _releaseBus();
		return buffer;
	}


	void pasteBitmap(int bitmap[], int x, int y, int width, int height, boolean hasTransparency, 
			int transparentColor, boolean grabAndReleaseBus) {
		int buffer[] = new int[SCREENHAL_MAX_BITMAP_SPACE];

		if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
		// check for overflow
		if (hasTransparency && (width * height > SCREENHAL_MAX_BITMAP_SPACE)) return;
		if (grabAndReleaseBus) _grabBus();
		if (hasTransparency) {
			// get the background image
			retrieveBitmapImpl(buffer, x, y, width, height);
			// set all non transparent pixels of the bitmap in the buffer
			for (int i=0; i<width*height; i++) {
				if (bitmap[i] != transparentColor) buffer[i] = bitmap[i];
			}
			pasteBitmapImpl(buffer, x, y, width, height);
		} else pasteBitmapImpl(bitmap, x, y, width, height);
		if (grabAndReleaseBus) _releaseBus();
	}


	void drawPattern(int pattern[], int orientation, int x, int y, int width, int height, 
			int color, int backColor, int scale, boolean overlay, boolean grabAndReleaseBus) {
		int buffer[] = new int[SCREENHAL_MAX_BITMAP_SPACE];
		int unpacked[] = new int[SCREENHAL_MAX_BITMAP_SPACE];

		if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
		if (width * height * scale * scale > SCREENHAL_MAX_BITMAP_SPACE) return;
		// unpack the pattern
		_unpackPattern(pattern, unpacked, width, height, 0, 0, orientation);
		if (grabAndReleaseBus) _grabBus();
		// if overlay, get the bitmap from the screen
		if (overlay) retrieveBitmapImpl(buffer, x, y, width*scale, height*scale);
		// scale and colorize it, setting a background color if no need to overlay
		_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, backColor, width, height, scale, 0, 0, overlay != true);
		// draw it
		pasteBitmapImpl(buffer, x, y, width*scale, height*scale);
		if (grabAndReleaseBus) _releaseBus();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void drawChar(int chr, int x, int y, int color, int fontSize, boolean isBold, boolean overlay) {
		drawChar(chr, x, y, color, fontSize, isBold, overlay, true);
	}

	void drawString(int s[], int x, int y, int color, int fontSize, boolean isBold, boolean overlay) {
		drawString(s, x, y, color, fontSize, isBold, overlay, true);
	}

	void drawPixel(int x, int y, int color) {
		drawPixel(x, y, color, true);
	}


	void drawHorizontalLine(int x1, int x2, int y, int color, int thickness) {
		drawHorizontalLine(x1, x2, y, color, thickness, true);
	}


	void drawVerticalLine(int x, int y1, int y2, int color, int thickness) {
		drawVerticalLine(x, y1, y2, color, thickness, true);
	}

	public void drawLine(int x1, int y1, int x2, int y2, int color, int thickness) {
		drawLine(x1, y1, x2, y2, color, thickness, true);
	}


	public void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color, int thickness) {
		drawTriangle(x0, y0, x1, y1, x2, y2, color, thickness, true);
	}

	public void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) {
		fillTriangle(x0, y0, x1, y1, x2, y2, color, true);
	}


	public void drawRectangle(int x, int y, int width, int height, int color, int thickness) {
		drawRectangle(x, y, width, height, color, thickness, true);
	}


	public void fillRectangle(int x, int y, int width, int height, int color) {
		fillRectangle(x, y, width, height, color, true);
	}


	void fillScreen(int color) {
		fillScreen(color, true);
	}


	public void drawArc(int x0, int y0, int r, int octant, int color, int thickness) {
		drawArc(x0,  y0,  r,  octant,  color,  thickness, true);
	}


	public void fillArc(int x0, int y0, int r, int octant, int color) {
		fillArc( x0,  y0,  r,  octant,  color, true);
	}


	public void drawCircle(int x0, int y0, int r, int color, int thickness) {
		_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, thickness, false);
	}


	public void fillCircle(int x0, int y0, int r, int color) {
		_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, 0, true);
	}


	public void drawRoundedRectangle(int x, int y, int width, int height, int r, int color, int thickness) {
		drawRoundedRectangle( x,  y,  width,  height,  r,  color,  thickness, true);
	}


	public void fillRoundedRectangle(int x, int y, int width, int height, int r, int color) {
		fillRoundedRectangle( x,  y,  width,  height,  r,  color, true);
	}


	int[] retrieveBitmap(int buffer[], int x, int y, int width, int height) {
		retrieveBitmap(buffer,  x,  y,  width,  height, true);
		return buffer;
	}


	public void pasteBitmap(int bitmap[], int x, int y, int width, int height, boolean hasTransparency, 
			int transparentColor) {
		pasteBitmap(bitmap,  x,  y,  width,  height,  hasTransparency, transparentColor, true);
	}


	void drawPattern(int pattern[], int orientation, int x, int y, int width, int height, 
			int color, int backColor, int scale, boolean overlay) {
		drawPattern(pattern, orientation,  x,  y,  width,  height, color,  backColor,  scale, overlay, true);
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	
	/* ---------------- Protected functions ------------------------ */

	/* ---------------- Virtual functions -------------------------- */
	/* ---------------- NEED TO BE IMPLEMENTED BY CHILD CLASS ------ */

	void initScreenImpl() {
		// needs to be implemented by the class inheriting from this class
	}

	void setRotationImpl(int rotation) {
		// needs to be implemented by the class inheriting from this class
	}

	void drawPixelImpl(int x, int y, int color) {
		// needs to be implemented by the class inheriting from this class
	}

	// Fills the area lx,ly -> hx, hy
	// this function assumes that lx <= hx and ly <= hy
	void fillAreaImpl(int lx, int ly, int hx, int hy, int color) {
		// needs to be implemented by the class inheriting from this class
	}

	// Draws the bitmap
	void pasteBitmapImpl(int bitmap[], int x, int y, int width, int height) {
	}

	// retrieves a bitmap from the screen
	void retrieveBitmapImpl(int bitmap[], int x, int y, int width, int height) {
	}

	// returns the width of the non rotated screen
	int getWidthImpl() {
		return 240;
	}

	// returns the height of the non rotated screen
	int getHeightImpl() {
		return 160;
	}


	/* ---------------- Private functions ------------------------ */

	void _grabBus() {
	}

	void _releaseBus() {
	}

	public void memcpy_P(int to[], int from[], int start, int len) {
		for (int i=0; i<len; i++) to[i] = from[i+start];
	}
	
	void _drawValidChar(int chr, int x, int y, int color, int fontSize, fontDefinition_t fontDef, 
			int fontScale, boolean isBold, boolean overlay) {
		
		int pattern[] = new int[FONT_MAX_PATTERN];
		int unpacked[] = new int[FONT_MAX_SPACE];
		int buffer[] = new int[FONT_MAX_SPACE*2*2];

		if ((fontSize % 2) == 1) memcpy_P(pattern, font_small[chr - fontDef.firstChar], 0, fontDef.orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef.height : fontDef.width);
		else if ((fontSize % 2) == 0) memcpy_P(pattern, font_medium[chr - fontDef.firstChar], 0, fontDef.orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef.height : fontDef.width);
				
		// unpack the pattern
		_unpackPattern(pattern, unpacked, fontDef.width, fontDef.height, fontDef.charSpacing, fontDef.lineSpacing, fontDef.orientation);	

		// if overlay, get the bitmap from the screen
		if (overlay) retrieveBitmapImpl(buffer, x, y, (fontDef.width+fontDef.charSpacing)*fontScale, (fontDef.height+fontDef.lineSpacing)*fontScale);
		// scale and colorize it
		_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef.width+fontDef.charSpacing, fontDef.height+fontDef.lineSpacing, fontScale, 0, 0, overlay != true);

		if (isBold) {
			_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef.width+fontDef.charSpacing, fontDef.height+fontDef.lineSpacing, fontScale, 1, 0, false);
			_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef.width+fontDef.charSpacing, fontDef.height+fontDef.lineSpacing, fontScale, 0, 1, false);
		}
		// draw it
		pasteBitmapImpl(buffer, x, y, (fontDef.width+fontDef.charSpacing)*fontScale, (fontDef.height+fontDef.lineSpacing)*fontScale);
	}

	// Unpacks the bits contained in pattern into unpacked: each byte of unpacked is a bit of pattern
	// the bits of the pattern can be stored horizontaly of verticaly
	// we can add a border on the right and on the bottom
	void _unpackPattern(int pattern[], int unpacked[], int width, int height, int borderRight, int borderBottom, int orientation) {
		for (int i=0; i<(width+borderRight)*(height+borderBottom); i++) unpacked[i] = 0;
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				int patternByte;
				int mask;
				if (orientation == PATTERN_ORIENTATION_HORIZONTAL) {
					patternByte = pattern[l*(1+width/8)+c/8];
					mask = 0x01 << (c & 0x07);
					unpacked[l*(width+borderRight)+c] = patternByte & mask;
				} else {
					patternByte = pattern[c*(1+height/8)+l/8];
					mask = 0x01 << (l & 0x07);
					unpacked[l*(width+borderRight)+c] = patternByte & mask;
				}
			}
		}
	}

	// draws an unpacked pattern into the buffer, replacing all the result pixels by onColor if the corresponding byte is not null in unpacked
	// can scale and shift the unpacked pattern before setting the pixels in the result
	// can also fill the result with offColor before replacing
	void _scaleShiftAndColorizeUnpackedPattern(int unpacked[], int result[], int onColor, int offColor, int width, int height, int scale,
		int xShift, int yShift, boolean blankFirst) {
		if (blankFirst) {
			for (int i=0; i<(width*height)*scale*scale; i++) result[i] = offColor;
		}
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				if (scale == 1) {
					if (unpacked[l*width+c] != 0) result[(l+yShift)*width+(c+xShift)] = onColor;
				} else {
					for (int pl=0; pl<scale; pl++) {
						for (int pc=0; pc<scale; pc++) {
							if (unpacked[l*width+c] != 0) result[(l*scale+pl+yShift)*width*scale+c*scale+pc+xShift] = onColor;
						}
					}
				}
			}
		}
	}

	public void debugArray(int array[], int width, int height, int padding) {
		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				String hex = Integer.toHexString(array[y*width+x]).toUpperCase();
				while (hex.length() < padding) hex = "0" + hex;
				System.out.print(hex + " ");
			}
			System.out.println();
		}
		System.out.println();
	}

	void _fillBoundedArea(int x1, int y1, int x2, int y2, int color) {
		if (x2 < x1) swap(x1, x2);
		if (x1 >= _width) return;
		if (y2 < y1) swap(y1, y2);
		if (y1 >= _height) return;
		if (x1 < 0) x1 = 0;
		if (x2 >= _width) x2 = _width-1;
		if (y1 < 0) y1 = 0;
		if (y2 >= _height) y2 = _height-1;
		fillAreaImpl(x1, y1, x2, y2, color);
	}


	void _drawBoundedPixel(int x, int y, int color) {
		if ((x < 0) || (x >= _width)) return;
		if ((y < 0) || (y >= _height)) return;
		drawPixelImpl(x, y, color);
	}

	/*
	 * From Wikipedia: Andres arc drawing algorithm, extended to 
	 * 	- draw thick lines and 
	 * 	- fill with optimal use of horizontal lines
	 */	
	void _drawOrFillOctant(int x0, int y0, int r, int octant, int color, int thickness, boolean fill) {
		int x = 0;
		int y = r;
		int d = r - 1;
		int t1 = (thickness-1)/2;
		int t2 = thickness/2;
		
		while (y >= x) {
			if (fill) {
				if ((octant & (SCREEN_ARC_SSW + SCREEN_ARC_SSE)) != 0) _fillBoundedArea(x0-x, y0+y, x0+x, y0+y, color);
				else if ((octant & SCREEN_ARC_SSW) != 0) _fillBoundedArea(x0, y0+y, x0-x, y0+y, color);
				else if ((octant & SCREEN_ARC_SSE) != 0) _fillBoundedArea(x0, y0+y, x0+x, y0+y, color);
				if ((octant & (SCREEN_ARC_SWW | SCREEN_ARC_SEE)) != 0) _fillBoundedArea(x0-y, y0+x, x0+y, y0+x, color);
				else if ((octant & SCREEN_ARC_SWW) != 0) _fillBoundedArea(x0, y0+x, x0-y, y0+x, color);
				else if ((octant & SCREEN_ARC_SEE) != 0) _fillBoundedArea(x0, y0+x, x0+y, y0+x, color);
				if ((octant & (SCREEN_ARC_NNW | SCREEN_ARC_NNE)) != 0) _fillBoundedArea(x0-x, y0-y, x0+x, y0-y, color);
				else if ((octant & SCREEN_ARC_NNW) != 0) _fillBoundedArea(x0, y0-y, x0-x, y0-y, color);
				else if ((octant & SCREEN_ARC_NNE) != 0) _fillBoundedArea(x0, y0-y, x0+x, y0-y, color);
				if ((octant & (SCREEN_ARC_NWW | SCREEN_ARC_NEE)) != 0) _fillBoundedArea(x0-y, y0-x, x0+y, y0-x, color);		
				else if ((octant & SCREEN_ARC_NWW) != 0) _fillBoundedArea(x0, y0-x, x0-y, y0-x, color);		
				else if ((octant & SCREEN_ARC_NEE) != 0) _fillBoundedArea(x0, y0-x, x0+y, y0-x, color);
			} else if (thickness != 1) {
				if ((octant & SCREEN_ARC_SSE) != 0) _fillBoundedArea(x0+x-t1, y0+y-t1, x0+x+t2, y0+y+t2, color);
				if ((octant & SCREEN_ARC_SEE) != 0) _fillBoundedArea(x0+y-t1, y0+x-t1, x0+y+t2, y0+x+t2, color);
				if ((octant & SCREEN_ARC_SSW) != 0) _fillBoundedArea(x0-x-t1, y0+y-t1, x0-x+t2, y0+y+t2, color);
				if ((octant & SCREEN_ARC_SWW) != 0) _fillBoundedArea(x0-y-t1, y0+x-t1, x0-y+t2, y0+x+t2, color);
				if ((octant & SCREEN_ARC_NNE) != 0) _fillBoundedArea(x0+x-t1, y0-y-t1, x0+x+t2, y0-y+t2, color);
				if ((octant & SCREEN_ARC_NEE) != 0) _fillBoundedArea(x0+y-t1, y0-x-t1, x0+y+t2, y0-x+t2, color);
				if ((octant & SCREEN_ARC_NNW) != 0) _fillBoundedArea(x0-x-t1, y0-y-t1, x0-x+t2, y0-y+t2, color);
				if ((octant & SCREEN_ARC_NWW) != 0) _fillBoundedArea(x0-y-t1, y0-x-t1, x0-y+t2, y0-x+t2, color);		
			} else {
				if ((octant & SCREEN_ARC_SSE) != 0) _drawBoundedPixel(x0+x, y0+y, color);
				if ((octant & SCREEN_ARC_SEE) != 0) _drawBoundedPixel(x0+y, y0+x, color);
				if ((octant & SCREEN_ARC_SSW) != 0) _drawBoundedPixel(x0-x, y0+y, color);
				if ((octant & SCREEN_ARC_SWW) != 0) _drawBoundedPixel(x0-y, y0+x, color);
				if ((octant & SCREEN_ARC_NNE) != 0) _drawBoundedPixel(x0+x, y0-y, color);
				if ((octant & SCREEN_ARC_NEE) != 0) _drawBoundedPixel(x0+y, y0-x, color);
				if ((octant & SCREEN_ARC_NNW) != 0) _drawBoundedPixel(x0-x, y0-y, color);
				if ((octant & SCREEN_ARC_NWW) != 0) _drawBoundedPixel(x0-y, y0-x, color);
			}
			if (d >= 2*x) {
				d = d - 2*x - 1;
				x = x + 1;
			} else if (d <= 2*(r-y)) {
				d = d + 2*y -1;
				y = y - 1;
			} else {
				d = d + 2*(y-x-1);
				y = y - 1;
				x = x + 1;
			}
		}
	}
}
