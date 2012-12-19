package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Rectangle2D;
import java.util.Hashtable;

import javax.swing.JPanel;

public class Switch extends JPanel 
implements MouseListener {

	private final static int SWITCH_SIZE = 34;
	
	private static final long serialVersionUID = 1L;
	
	protected boolean pressed = false;
	private int pin;
	private String name;
	
	public static Hashtable<Integer, Switch> switches = new Hashtable<Integer, Switch>();

	public static boolean isPressed(int pin) {
		if (!switches.containsKey(pin)) return false;
		return switches.get(pin).isPressed();
	}

	public static void setPressed(int pin, boolean pressed) {
		if (!switches.containsKey(pin)) return;
		switches.get(pin).setPressed(pressed);
	}

	public Switch(int pin, String name) {
		this(pin);
		this.name = name;
		addMouseListener(this);
	}

	public Switch(int pin) {
		this.pin = pin;
		this.pressed = false;
		switches.put(pin, this);
	}

	public boolean isPressed() {
		return pressed;
	}

	public void setPressed(boolean pressed) {
		this.pressed = pressed;
	}
	
	public int getPin() {
		return pin;
	}
	
	public void mousePressed(MouseEvent arg0) { 
		pressed = true;
		repaint();
	}

	public void mouseReleased(MouseEvent arg0) { 
		pressed = false;
		repaint();
	}

	public Dimension getPreferredSize() {
		return new Dimension(SWITCH_SIZE+4, SWITCH_SIZE+4);
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		int x = (getWidth()-SWITCH_SIZE)/2;
		int y = (getHeight()-SWITCH_SIZE)/2;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		if (pressed) g2d.setColor(new Color(130, 130, 130));
		else g2d.setColor(new Color(200, 200, 200));
		g2d.fillRect(x, y, SWITCH_SIZE, SWITCH_SIZE);
		g2d.setColor(Color.BLACK);
		g2d.drawRect(x, y, SWITCH_SIZE-1, SWITCH_SIZE-1);
		g2d.drawRect(x+1, y+1, SWITCH_SIZE-3, SWITCH_SIZE-3);
		Rectangle2D strBounds = g2d.getFontMetrics().getStringBounds(pin+"", g2d);
		y = (getHeight() - 2 * g2d.getFontMetrics().getHeight())/2 + g2d.getFontMetrics().getAscent(); 
		g2d.drawString(pin+"", x+Math.round((SWITCH_SIZE-strBounds.getWidth())/2), y);
		strBounds = g2d.getFontMetrics().getStringBounds(name, g2d);
		g2d.drawString(name, x+Math.round((SWITCH_SIZE-strBounds.getWidth())/2), y+g2d.getFontMetrics().getHeight());
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
}
