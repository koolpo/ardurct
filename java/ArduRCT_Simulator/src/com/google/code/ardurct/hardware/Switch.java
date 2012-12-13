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

	private static final long serialVersionUID = 1L;
	
	private boolean pressed = false;
	private int pin;
	private String name;
	
	public static Hashtable<Integer, Switch> switches = new Hashtable<Integer, Switch>();

	public Switch(int pin, String name) {
		this.pin = pin;
		this.name = name;
		this.pressed = false;
		switches.put(pin, this);
		addMouseListener(this);
	}

	public static boolean isPressed(int pin) {
		if (!switches.containsKey(pin)) return false;
		return switches.get(pin).isPressed();
	}
	
	public boolean isPressed() {
		return pressed;
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
		return new Dimension(40, 40);
	}
	
	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		if (pressed) g2d.setColor(new Color(130, 130, 130));
		else g2d.setColor(new Color(200, 200, 200));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		g2d.setColor(Color.BLACK);
		g2d.drawRect(0, 0, getWidth()-1, getHeight()-1);
		g2d.drawRect(1, 1, getWidth()-3, getHeight()-3);
		Rectangle2D strBounds = g2d.getFontMetrics().getStringBounds(pin+"", g2d);
		int y = (getHeight() - 2 * g2d.getFontMetrics().getHeight())/2 + g2d.getFontMetrics().getAscent(); 
		g2d.drawString(pin+"", Math.round((getWidth()-strBounds.getWidth())/2), y);
		strBounds = g2d.getFontMetrics().getStringBounds(name, g2d);
		g2d.drawString(name, Math.round((getWidth()-strBounds.getWidth())/2), y+g2d.getFontMetrics().getHeight());
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
}
