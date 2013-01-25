package com.google.code.ardurct.hardware;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;

import com.google.code.ardurct.libraries.IArduinoDefines;

public class RotaryEncoder extends JPanel 
implements MouseListener, IArduinoDefines, Runnable {

	private final static int ENCODER_STEPS = 24;

	public final static int ENCODER_SIZE = 54;
	public final static int ENCODER_MARGIN = 10;

	private final static float ENCODER_RPS = 0.5f;
	
	private static final long serialVersionUID = 1L;
	
	private int pinA;
	private int pinB;
	private boolean cRotation;
	private boolean ccRotation;
	private int a;
	private int b;
	private int count;

	public RotaryEncoder(int pinA, int pinB) {
		this.pinA = pinA;
		this.pinB = pinB;
		Digital.write(pinA, HIGH);
		Digital.write(pinB, HIGH);
		addMouseListener(this);
		a = 1;
		b = 0;
		count = 0;
		Thread t = new Thread(this);
		t.start();
	}


	public void run() {
		while (true) {
			if (cRotation) {
				a --;
				if (a < 0) a = 3;
				b --;
				if (b < 0) b = 3;
				count ++;
				if (count == ENCODER_STEPS) count = 0;
			} else if (ccRotation) {
				a ++;
				if (a > 3) a = 0;
				b ++;
				if (b > 3) b = 0;
				count --;
				if (count < 0) count = ENCODER_STEPS-1;
			}
			if (cRotation || ccRotation) {
				if (a < 2) Digital.write(pinA, HIGH);
				else Digital.write(pinA, LOW);
				if (b < 2) Digital.write(pinB, HIGH);
				else Digital.write(pinB, LOW);
				repaint();
			}
			try {
				Thread.sleep(Math.round(1000/ENCODER_RPS/ENCODER_STEPS));
			} catch (Exception e) {				
			}
		}
	}
		
	public void mousePressed(MouseEvent me) { 
		if (me.getX() < getWidth()/2) {
			ccRotation = true;
			cRotation = false;
		} else {
			cRotation = true;
			ccRotation = false;
		}
	}

	public void mouseReleased(MouseEvent arg0) { 
		ccRotation = false;
		cRotation = false;
		repaint();
	}

	public Dimension getPreferredSize() {
		return new Dimension(ENCODER_SIZE+ENCODER_MARGIN, ENCODER_SIZE+ENCODER_MARGIN);
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setStroke(new BasicStroke(1.0f));
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		g2d.setColor(new Color(220, 220, 220));
		if (cRotation) g2d.fillRect(getWidth()/2, 0, getWidth()/2, getHeight());
		else if (ccRotation) g2d.fillRect(0, 0, getWidth()/2, getHeight());
		int x = getWidth()/2;
		int y = getHeight()/2;
		g2d.setColor(new Color(180, 180, 180));
		g2d.fillOval(x-ENCODER_SIZE/2, y-ENCODER_SIZE/2, ENCODER_SIZE, ENCODER_SIZE);
		g2d.setColor(Color.BLACK);
		g2d.drawOval(x-ENCODER_SIZE/2, y-ENCODER_SIZE/2, ENCODER_SIZE, ENCODER_SIZE);
		g2d.setStroke(new BasicStroke(2.0f));
		g2d.drawLine(x, y, 
				(int)(x+ENCODER_SIZE/2*Math.cos(count*Math.PI*2/ENCODER_STEPS-Math.PI/2)), 
				(int)(y+ENCODER_SIZE/2*Math.sin(count*Math.PI*2/ENCODER_STEPS-Math.PI/2)));
		Rectangle2D strBounds = g2d.getFontMetrics().getStringBounds(pinA+":"+pinB, g2d);
		x -= Math.round(strBounds.getWidth()/2);
		y -= g2d.getFontMetrics().getHeight()/2; 
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(x-2, y-2, (int)(strBounds.getWidth()+4), g2d.getFontMetrics().getHeight()+4);
		g2d.setColor(Color.BLACK);
		g2d.setStroke(new BasicStroke(1.0f));
		g2d.drawRect(x-2, y-2, (int)(strBounds.getWidth()+4), g2d.getFontMetrics().getHeight()+4);
		g2d.drawString(pinA+":"+pinB, x+1, y + g2d.getFontMetrics().getAscent());
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
}
