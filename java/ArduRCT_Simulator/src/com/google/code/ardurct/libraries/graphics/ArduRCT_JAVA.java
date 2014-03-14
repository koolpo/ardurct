package com.google.code.ardurct.libraries.graphics;

import com.google.code.ardurct.hardware.TFTTouchPanel;

public class ArduRCT_JAVA extends ArduRCT_GraphicsUI {

	void initScreenImpl() {
		TFTTouchPanel.initPanel();
	}

	void setRotationImpl(int rotation) {
		TFTTouchPanel.setRotation(rotation);
	}

	void drawPixelImpl(int x, int y, int color) {
		TFTTouchPanel.setPixel(x, y, getARGBColor(color));
	}
		
	// Fills the area lx,ly -> hx, hy
	// this function assumes that lx <= hx and ly <= hy
	void fillAreaImpl(int lx, int ly, int hx, int hy, int color) {
		int argb = getARGBColor(color);
		for (int y=ly; y<=hy; y++) {
			for (int x=lx; x<=hx; x++) TFTTouchPanel.setPixel(x, y, argb);
		}
	}

	// Draws the bitmap
	void pasteBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		int buffer[] = new int[width*height];
		for (int i=0; i<width*height; i++) buffer[i] = getARGBColor(bitmap[i]);
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				if (x+c >= 0 && x+c < _width && y+l >= 0 && y+l < _height) TFTTouchPanel.setPixel(x+c, y+l, buffer[l*width+c]);
			}
		}
	}

	// retrieves a bitmap from the screen
	void retrieveBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				int color = TFTTouchPanel.getPixel(x+c, y+l);
				int red = ((color & 0x00FF0000) >> 19) << 11;
				int green = ((color & 0x0000FF00) >> 10) << 5;
				int blue = (color & 0x000000FF) >> 3;
				bitmap[l*width+c] = red + green + blue;
			}
		}
	}

	public int getWidthImpl() {
		return TFTTouchPanel.HARDWARE_WIDTH;
	}

	public int getHeightImpl() {
		return TFTTouchPanel.HARDWARE_HEIGHT;
	}
		
	public int getARGBColor(int color565) {
		int red = (color565 & 0xF800) >> 8;
		red = red * 0xFF / 0xF8;
		int green = (color565 & 0x07E0) >> 3;
		green = green * 0xFF / 0xFC;
		int blue = (color565 & 0x001F) << 3;
		blue = blue * 0xFF / 0xF8;
		return 0xFF000000 + (red << 16) + (green << 8) + blue;
	}
}
