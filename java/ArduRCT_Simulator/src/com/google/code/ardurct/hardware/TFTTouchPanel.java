package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

import com.google.code.ardurct.libraries.eventManager.IEventDefines;
import com.google.code.ardurct.libraries.graphics.IGraphicsDefines;

public class TFTTouchPanel extends JPanel 
implements Runnable, MouseListener, MouseMotionListener, IEventDefines, IGraphicsDefines {

	private static final long serialVersionUID = 1L;

	public static int WIDTH = PHYSICAL_WIDTH;
	public static int HEIGHT = PHYSICAL_HEIGHT;
	public static int rotation = GRAPHICS_ROTATION_0;

	// only one instance at a time of TFTTouchPanel can exist
	private static BufferedImage img = null;
	private static int touchX = TOUCHPANEL_NO_TOUCH;
	private static int touchY = TOUCHPANEL_NO_TOUCH;
	private static int touchZ = TOUCHPANEL_NO_TOUCH;

	private boolean running;
	private Thread t = null;	
	
	
	public static void initPanel() {
		img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = (Graphics2D) img.getGraphics();
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, WIDTH, HEIGHT);
		TFTTouchPanel.setRotation(GRAPHICS_ROTATION_0);
	}
	
	public static void setLandscapeOrientation() {
		WIDTH = PHYSICAL_HEIGHT;
		HEIGHT = PHYSICAL_WIDTH;
	}

	public static void setPortraitOrientation() {
		WIDTH = PHYSICAL_WIDTH;
		HEIGHT = PHYSICAL_HEIGHT;	
	}	
	
	private static void getRotatedXY(int X[], int Y[]) {
		if (rotation == GRAPHICS_ROTATION_0) return;
		int x = X[0];
		int y = Y[0];
		if (rotation == GRAPHICS_ROTATION_90) {
			X[0] = WIDTH - 1 - y;
			Y[0] = x;
		} else if (rotation == GRAPHICS_ROTATION_180) {
			X[0] = WIDTH - 1 - x;
			Y[0] = HEIGHT - 1 - y;
		} else if (rotation == GRAPHICS_ROTATION_270) {
			X[0] = y;
			Y[0] = HEIGHT - 1 - x;
		} 
	}

	public static void setRotation(int aRotation) {
		rotation = aRotation - (WIDTH != PHYSICAL_WIDTH ? 1 : 0);
		if (rotation < 0) rotation = GRAPHICS_ROTATION_270;
	}
	
	public static void setPixel(int x, int y, int argb) {
		int X[] = { x }; int Y[] = { y };
		getRotatedXY(X, Y);
		img.setRGB(X[0], Y[0], argb);
	}

	public static int getPixel(int x, int y) {
		int X[] = { x }; int Y[] = { y };
		getRotatedXY(X, Y);
		return img.getRGB(X[0], Y[0]);
	}
	
	public TFTTouchPanel() {
		addMouseListener(this);
		addMouseMotionListener(this);
	}
	
	public void start() {
		running = true;
		t = new Thread(this);
		t.start();
	}
	
	public void stop() {
		running = false;
		if (t != null) t.interrupt();
	}
	
	public void run() {
		while (running) {
			if (img != null) repaint();
			try {
				Thread.sleep(20);
			} catch (Exception e) {				
			}	
		}
	}

	public BufferedImage getContentImage() {
		BufferedImage bi = new BufferedImage(WIDTH+2, HEIGHT+2, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = (Graphics2D) bi.getGraphics();
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, WIDTH+2, HEIGHT+2);	
		g2d.drawImage(img, 1, 1, this);
		return bi;
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());	
		g2d.setColor(Color.BLACK);
		if (img != null) g2d.drawImage(img, (getWidth()-WIDTH)/2, (getHeight()-HEIGHT)/2, this);
		else g2d.fillRect((getWidth()-WIDTH)/2, (getHeight()-HEIGHT)/2, WIDTH, HEIGHT);
	}

	public Dimension getPreferredSize() {
		return new Dimension(Math.max(PHYSICAL_WIDTH, PHYSICAL_HEIGHT) + 20, Math.max(PHYSICAL_WIDTH, PHYSICAL_HEIGHT) + 20);
	}

	public void mouseExited(MouseEvent me) { 
		touchX = TOUCHPANEL_NO_TOUCH;
		touchY = TOUCHPANEL_NO_TOUCH;
		touchZ = TOUCHPANEL_NO_TOUCH;
	}

	public void mouseReleased(MouseEvent me) {
		touchX = TOUCHPANEL_NO_TOUCH;
		touchY = TOUCHPANEL_NO_TOUCH;
		touchZ = TOUCHPANEL_NO_TOUCH;
	}

	public void mousePressed(MouseEvent me) {
		int x = me.getX() - (getWidth()-WIDTH)/2;
		int y = me.getY() - (getHeight()-HEIGHT)/2;
		if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) return;
		int X[] = {x}; int Y[] = {y};
		// calculate the invert rotation of x,y since the point will be drawn rotated
		if (rotation == GRAPHICS_ROTATION_90) {
			X[0] = y;
			Y[0] = WIDTH - 1 - x;
		} else if (rotation == GRAPHICS_ROTATION_180) {
			X[0] = WIDTH - 1 - x;
			Y[0] = HEIGHT -1 - y;			
		} else if (rotation == GRAPHICS_ROTATION_270) {
			X[0] = HEIGHT - 1 - y;
			Y[0] = x;			
		}
		touchX = X[0];
		touchY = Y[0];
		touchZ = 10;
	}


	public void mouseDragged(MouseEvent me) {
		mousePressed(me);
	}

	public void mouseMoved(MouseEvent arg0) { }
	public void mouseClicked(MouseEvent me) {	}
	public void mouseEntered(MouseEvent me) { }


	public static int getTouchX() {
		return touchX;
	}

	public static int getTouchY() {
		return touchY;
	}

	public static int getTouchZ() {
		return touchZ;
	}

}
