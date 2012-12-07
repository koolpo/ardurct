package com.google.code.ardurct.libraries.touchscreen;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;

public class ScreenImpl extends ScreenPL 
implements MouseListener, MouseMotionListener {

	private static final long serialVersionUID = 1L;
	
	BufferedImage img = new BufferedImage(10, 10, BufferedImage.TYPE_INT_ARGB);
	
	private double radianAngle = 0;
	private int maxSize = 0;
	int mouseX;
	int mouseY;

	public void clear() {
		fillAreaImpl(0, 0, getWidthImpl()-1, getHeightImpl()-1, BLACK);
		repaint();
	}

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

	public BufferedImage getScreenPhoto() {
		BufferedImage bi = new BufferedImage(getWidthImpl()+2, getHeightImpl()+2, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = (Graphics2D) bi.getGraphics();
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, getWidthImpl()+2, getHeightImpl()+2);	
		g2d.drawImage(getScreen(), 1, 1, this);
		return bi;

	}
	
	void setRotationImpl(int rotation) {
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
				if (x+c >= 0 && x+c < getWidthImpl() && y+l >= 0 && y+l < getHeightImpl()) img.setRGB(x+c, y+l, buffer[l*width+c]);
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
		return 160;
	}

	// returns the height of the non rotated screen
	int getHeightImpl() {
		return 128;
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, getBounds().width, getBounds().height);	
		//g2d.scale(SCALE_X, SCALE_Y);
		g2d.drawImage(getScreen(), 0, 0, this);
	}

	public Dimension getPreferredSize() {
		return new Dimension(Math.round(getWidthImpl()), Math.round(getHeightImpl()));
	}

	// should be in Touchscreen.cpp
	public int getTouchXYZ(int[] x, int[] y, int[] z) {
		x[0] = mouseX;
		y[0] = mouseY;
		if (mouseX == TOUCHSCREEN_NO_TOUCH) return TOUCHSCREEN_NO_TOUCH;
		return 10;
	}
	
	public void mousePressed(MouseEvent me) {
		mouseX = me.getX();
		mouseY = me.getY();
	}

	public void mouseReleased(MouseEvent me) {
		mouseX = TOUCHSCREEN_NO_TOUCH;
		mouseY = TOUCHSCREEN_NO_TOUCH;
	}

	public void mouseDragged(MouseEvent me) {
		mouseX = me.getX();
		mouseY = me.getY();		
	}

	/* Unused */
	public void mouseClicked(MouseEvent arg0) { }

	public void mouseEntered(MouseEvent arg0) { }

	public void mouseExited(MouseEvent arg0) { }

	public void mouseMoved(MouseEvent arg0) { }

}
