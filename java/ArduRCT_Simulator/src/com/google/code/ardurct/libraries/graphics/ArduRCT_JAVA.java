package com.google.code.ardurct.libraries.graphics;

import java.awt.Color;
import java.awt.Image;
import java.awt.image.BufferedImage;

public class ArduRCT_JAVA extends ArduRCT_GraphicsMacros {

	private static int PHYSICAL_WIDTH = 128;
	private static int PHYSICAL_HEIGHT = 160;

	void initScreenImpl() {
		img = new BufferedImage(getWidthImpl(), getHeightImpl(), BufferedImage.TYPE_INT_ARGB);
		img.getGraphics().setColor(Color.BLACK);
		img.getGraphics().fillRect(0, 0, getWidthImpl()-1, getHeightImpl()-1);
	}

	void setRotationImpl(int rotation) {
	}

	void drawPixelImpl(int x, int y, int color) {
		_setPixel(x, y, getARGBColor(color));
	}
		
	// Fills the area lx,ly -> hx, hy
	// this function assumes that lx <= hx and ly <= hy
	void fillAreaImpl(int lx, int ly, int hx, int hy, int color) {
		int argb = getARGBColor(color);
		for (int y=ly; y<=hy; y++) {
			for (int x=lx; x<=hx; x++) _setPixel(x, y, argb);
		}
	}

	// Draws the bitmap
	void pasteBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		int buffer[] = new int[width*height];
		for (int i=0; i<width*height; i++) buffer[i] = getARGBColor(bitmap[i]);
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				if (x+c >= 0 && x+c < _width && y+l >= 0 && y+l < _height) _setPixel(x+c, y+l, buffer[l*width+c]);
			}
		}
	}

	// retrieves a bitmap from the screen
	void retrieveBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				int color = _getPixel(x+c, y+l);
				int red = ((color & 0x00FF0000) >> 19) << 11;
				int green = ((color & 0x0000FF00) >> 10) << 5;
				int blue = (color & 0x000000FF) >> 3;
				bitmap[l*width+c] = red + green + blue;
			}
		}
	}

	// returns the width of the non rotated screen
	public int getWidthImpl() {
		return WIDTH;
	}

	// returns the height of the non rotated screen
	public int getHeightImpl() {
		return HEIGHT;
	}

	/* ------------- Java specific ---------------------------------------------- */
	
	public static int WIDTH = 128;
	public static int HEIGHT = 160;

	public static void setLandscapeOrientation() {
		WIDTH = PHYSICAL_HEIGHT;
		HEIGHT = PHYSICAL_WIDTH;
	}

	public static void setPortraitOrientation() {
		WIDTH = PHYSICAL_WIDTH;
		HEIGHT = PHYSICAL_HEIGHT;		
	}

	private static final long serialVersionUID = 1L;
	
	// only one instance at a time of ArduRCT_JAVA can exist
	private static BufferedImage img = null;
	
	public ArduRCT_JAVA() {
		ArduRCT_JAVA.img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
	}

	private void _setPixel(int x, int y, int argb) {
		int X[] = { x }; int Y[] = { y };
		getRotatedXY(X, Y);
		img.setRGB(X[0], Y[0], argb);
	}

	private int _getPixel(int x, int y) {
		int X[] = { x }; int Y[] = { y };
		getRotatedXY(X, Y);
		return img.getRGB(X[0], Y[0]);
	}
	

	public static Image getScreen() {
		return img;
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
