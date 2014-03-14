package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;

import javax.swing.AbstractAction;
import javax.swing.JPanel;
import javax.swing.KeyStroke;

import com.google.code.ardurct.libraries.eventManager.IEventDefines;
import com.google.code.ardurct.libraries.graphics.IGraphicsDefines;

public class TFTTouchPanel extends JPanel 
implements Runnable, MouseListener, MouseMotionListener, IEventDefines, IGraphicsDefines {

	private static final long serialVersionUID = 1L;
	
	public static final boolean LANDSCAPE = true;
	public static final boolean PORTRAIT = false;

	public static final int SIZE_128x160 = 1;
	public static final int SIZE_240x320 = 2;

	private static final int SIZE_128x160_WIDTH = 128;
	private static final int SIZE_128x160_HEIGHT = 160;
	private static final int SIZE_240x320_WIDTH = 240;
	private static final int SIZE_240x320_HEIGHT = 320;
	
	public static int HARDWARE_SIZE = SIZE_240x320;
	public static int HARDWARE_WIDTH = SIZE_240x320_WIDTH;
	public static int HARDWARE_HEIGHT = SIZE_240x320_HEIGHT;
	public static int WIDTH = HARDWARE_WIDTH;
	public static int HEIGHT = HARDWARE_HEIGHT;
	
	public static boolean isLandscapeOrientation = false;
	
	public static int rotation = GRAPHICS_ROTATION_0;

	// only one instance at a time of TFTTouchPanel can exist
	private static BufferedImage img = null;
	private static int touchX = TOUCHPANEL_NO_TOUCH;
	private static int touchY = TOUCHPANEL_NO_TOUCH;
	private static int touchZ = TOUCHPANEL_NO_TOUCH;
	public static boolean dragged = false;
	
	private boolean running;
	private Thread t = null;	
	private static boolean enterPressed = false;
	
	public static void initPanel() {
		img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = (Graphics2D) img.getGraphics();
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, WIDTH, HEIGHT);
		TFTTouchPanel.setRotation(GRAPHICS_ROTATION_0);
	}
	
	public static void setOrientation(boolean isLandscape) {
		isLandscapeOrientation = isLandscape;
		if (isLandscape) {
			WIDTH = HARDWARE_HEIGHT;
			HEIGHT = HARDWARE_WIDTH;
		} else {
			WIDTH = HARDWARE_WIDTH;
			HEIGHT = HARDWARE_HEIGHT;				
		}
		cancelHighlight();
	}
	
	public static void setHardwareSize(int hardwareSize) {
		HARDWARE_SIZE = hardwareSize;
		if (hardwareSize == 1) {
			HARDWARE_WIDTH = SIZE_128x160_WIDTH;
			HARDWARE_HEIGHT = SIZE_128x160_HEIGHT;			
		} else {
			HARDWARE_WIDTH = SIZE_240x320_WIDTH;
			HARDWARE_HEIGHT = SIZE_240x320_HEIGHT;
		}
		setOrientation(isLandscapeOrientation);
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
		rotation = aRotation - (WIDTH != HARDWARE_WIDTH ? 1 : 0);
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
		getInputMap().put(KeyStroke.getKeyStroke("ENTER"), "pressEnter");
		getActionMap().put("pressEnter", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			public void actionPerformed(ActionEvent e) {
				enterPressed = true;
			}
		});
	}
	
	public static boolean getEnterPressed() {
		boolean pressed = enterPressed;
		enterPressed = false;
		return pressed;
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
		if (hasHighlight) {
			g2d.setColor(Color.RED);
			g2d.drawRect(hX + (getWidth()-WIDTH)/2, hY + (getHeight()-HEIGHT)/2, hWidth, hHeight);
		}
	}

	private static int hX, hY, hWidth, hHeight;
	private static boolean hasHighlight = false;
	public static void setHighlight(int x, int y, int width, int height) {
		hX = x;
		hY = y;
		hWidth = width;
		hHeight = height;
		hasHighlight = true;
	}
	
	public static void cancelHighlight() {
		hasHighlight = false;
	}
	
	public Dimension getPreferredSize() {
		return new Dimension(Math.max(HARDWARE_WIDTH, HARDWARE_HEIGHT) + 20, Math.max(HARDWARE_WIDTH, HARDWARE_HEIGHT) + 20);
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
		dragged = false;
	}


	public void mouseDragged(MouseEvent me) {
		mousePressed(me);
		dragged = true;
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

	public static int getRGB565(int red, int green, int blue) {
		return (((red & 0xFF) >> 3) << 11) + (((green & 0xFF) >> 2) << 5) + ((blue & 0xFF) >> 3);
	}

	public static int getRGB565() {
		return getRGB565((int)(Math.random()*0xFF), (int)(Math.random()*0xFF), (int)(Math.random()*0xFF));
	}

}
