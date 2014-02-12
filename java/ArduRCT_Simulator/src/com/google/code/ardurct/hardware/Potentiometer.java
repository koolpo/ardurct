package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;

public class Potentiometer extends JPanel 
implements MouseListener, MouseMotionListener {

	private final static int WIDTH = 50;
	private final static int HEIGHT = 80;
	private final static int KNOB = 20;
	private final static int LINE = 4;
	
	private static final long serialVersionUID = 1L;
	
	private int pin;
	private String name;

	public Potentiometer(int pin) {
		this(pin, 0, null);
	}

	public Potentiometer(int pin, int value) {
		this(pin, value, null);
	}

	public Potentiometer(int pin, String name) {
		this(pin, 0, name);
	}

	public Potentiometer(int pin, int value, String name) {
		this.pin = pin;
		Analog.write(pin, value);
		this.name = name;
		addMouseListener(this);
		addMouseMotionListener(this);
	}
	
	public void mousePressed(MouseEvent me) {
		int y = me.getY() - 2 - KNOB/2;
		int height = getHeight()-34-KNOB;
		if (y < 0) y = 0;
		if (y > height-1) y = height-1;
		Analog.write(pin, 1023 - y * 1023 / height);
		repaint();
	}


	public void mouseDragged(MouseEvent me) {
		mousePressed(me);
	}

	public Dimension getPreferredSize() {
		return new Dimension(WIDTH+4, HEIGHT+34);
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		g2d.setColor(Color.BLACK);
		g2d.fillRect((getWidth()-LINE)/2, KNOB/2+2, LINE, getHeight()-KNOB-34);
		int y = (1023-Analog.read(pin)) * (getHeight()-KNOB-34) / 1023 + 2;
		g2d.setColor(new Color(200, 200, 200));
		g2d.fillRect((getWidth()-KNOB)/2, y, KNOB, KNOB);
		g2d.setColor(Color.BLACK);
		g2d.drawRect((getWidth()-KNOB)/2, y, KNOB, KNOB);
		Rectangle2D strBounds = g2d.getFontMetrics().getStringBounds(pin+"", g2d);
		y = getHeight() - 2 * g2d.getFontMetrics().getHeight() + g2d.getFontMetrics().getAscent(); 
		g2d.drawString(pin+"", Math.round((getWidth()-strBounds.getWidth())/2), y);
		strBounds = g2d.getFontMetrics().getStringBounds(name, g2d);
		g2d.drawString(name, Math.round((getWidth()-strBounds.getWidth())/2), y+g2d.getFontMetrics().getHeight()-2);
		g2d.drawRect((int)Math.round((getWidth()-strBounds.getWidth()-8)/2), 0, (int)Math.round(strBounds.getWidth()+8), getHeight()-1);
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
	public void mouseMoved(MouseEvent arg0) { }
	public void mouseReleased(MouseEvent arg0) { }


}
