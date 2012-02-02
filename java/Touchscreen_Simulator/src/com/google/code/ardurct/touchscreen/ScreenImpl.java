package com.google.code.ardurct.touchscreen;

import java.awt.Image;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;

public class ScreenImpl extends MacroDrawing {

	private static final long serialVersionUID = 1L;
	
	BufferedImage img = null;
	
	private double radianAngle = 0;
	private int maxSize = 0;
		
	public Image getScreen() {
		AffineTransform tx = new AffineTransform();
		tx.rotate(radianAngle, maxSize, maxSize);
		AffineTransformOp op = new AffineTransformOp(tx, AffineTransformOp.TYPE_BILINEAR);
		BufferedImage result = op.filter(img, null);
		return result.getScaledInstance(maxSize, maxSize, BufferedImage.SCALE_DEFAULT);
	}
	
	void initScreenImpl() {
		maxSize = Math.max(getWidthImpl(), getHeightImpl());
		img = new BufferedImage(maxSize, maxSize, BufferedImage.TYPE_INT_ARGB);
		radianAngle = 0;
	}

	void setRotationImpl(int rotation) {
		if (rotation == _rotation) return;
		AffineTransform tx = new AffineTransform();
		radianAngle = (double)(rotation-_rotation)* Math.PI/2;
		tx.rotate(radianAngle, maxSize/2, maxSize/2);
		AffineTransformOp op = new AffineTransformOp(tx, AffineTransformOp.TYPE_BILINEAR);
		img = op.filter(img, null);
	}

	int getARGBColor(int color565) {
		int red = (color565 & 0xF800) >> 8;
		red = red * 0xFF / 0xF8;
		int green = (color565 & 0x07E0) >> 3;
		green = green * 0xFF / 0xFC;
		int blue = (color565 & 0x001F) << 3;
		blue = blue * 0xFF / 0xF8;
		return 0xFF000000 + (red << 16) + (green << 8) + blue;
	}

	void drawPixelImpl(int x, int y, int color) {
		img.setRGB(x, y, getARGBColor(color));
	}
	
	// Fills the area lx,ly -> hx, hy
	// this function assumes that lx <= hx and ly <= hy
	void fillAreaImpl(int lx, int ly, int hx, int hy, int color) {
		int argb = getARGBColor(color);
		for (int y=ly; y<=hy; y++) {
			for (int x=lx; x<=hx; x++) img.setRGB(x, y, argb);
		}
	}

	// Draws the bitmap
	void pasteBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		int buffer[] = new int[width*height];
		for (int i=0; i<width*height; i++) buffer[i] = getARGBColor(bitmap[i]);
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				img.setRGB(x+c, y+l, buffer[l*width+c]);
			}
		}
	}

	// retrieves a bitmap from the screen
	void retrieveBitmapImpl(int bitmap[], int x, int y, int width, int height) {
		for (int l=0; l<height; l++) {
			for (int c=0; c<width; c++) {
				int color = img.getRGB(x+c, y+l);
				int red = ((color & 0x00FF0000) >> 19) << 11;
				int green = ((color & 0x0000FF00) >> 10) << 5;
				int blue = (color & 0x000000FF) >> 3;
				bitmap[l*width+c] = red + green + blue;
			}
		}
	}

	// returns the width of the non rotated screen
	int getWidthImpl() {
		return 240;
	}

	// returns the height of the non rotated screen
	int getHeightImpl() {
		return 320;
	}


}
