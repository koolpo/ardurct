package com.google.code.ardurct;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

import com.google.code.ardurct.libraries.graphics.ArduRCT_JAVA;

public class GraphicsPanel extends JPanel 
implements Runnable {

	private static final long serialVersionUID = 1L;
	
	private boolean running;
	private Thread t = null;
	
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
			if (ArduRCT_JAVA.getScreen() != null) repaint();
			try {
				Thread.sleep(20);
			} catch (Exception e) {				
			}	
		}
	}
	
	public BufferedImage getScreenPhoto() {
		BufferedImage bi = new BufferedImage(ArduRCT_JAVA.WIDTH+2, ArduRCT_JAVA.HEIGHT+2, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = (Graphics2D) bi.getGraphics();
		g2d.setColor(new Color(0, 0, 0));
		g2d.fillRect(0, 0, ArduRCT_JAVA.WIDTH+2, ArduRCT_JAVA.HEIGHT+2);	
		g2d.drawImage(ArduRCT_JAVA.getScreen(), 1, 1, this);
		return bi;
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());	
		g2d.setColor(Color.BLACK);
		if (ArduRCT_JAVA.getScreen() != null) 
			g2d.drawImage(ArduRCT_JAVA.getScreen(), (getWidth()-ArduRCT_JAVA.WIDTH)/2, getHeight()-ArduRCT_JAVA.HEIGHT - 5, this);
		else g2d.fillRect((getWidth()-ArduRCT_JAVA.WIDTH)/2, getHeight()-ArduRCT_JAVA.HEIGHT - 5, WIDTH, HEIGHT);
	}

	public Dimension getPreferredSize() {
		return new Dimension(Math.round(ArduRCT_JAVA.WIDTH), Math.round(ArduRCT_JAVA.HEIGHT));
	}

}
