package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JPanel;

public class RockerSwitch extends JPanel 
implements MouseListener {

	private static final int SWITCH_SIZE = 24;
	private static final int SWITCH_SPACING = 4;
	
	private static final long serialVersionUID = 1L;
	
	private Switch up = null;
	private Switch left = null;
	private Switch center = null;
	private Switch right = null;
	private Switch down = null;
	
	public RockerSwitch(int pinUp, int pinCenter, int pinDown) {
		this(pinUp, 0xFF, pinCenter, 0xFF, pinDown);
	}

	public RockerSwitch(int pinUp, int pinLeft, int pinRight, int pinDown) {
		this(pinUp, pinLeft, 0xFF, pinRight, pinDown);
	}
	
	public RockerSwitch(int pinUp, int pinLeft, int pinCenter, int pinRight, int pinDown) {
		if (pinUp != 0xFF) up = new Switch(pinUp);
		if (pinUp != 0xFF) left = new Switch(pinLeft);
		if (pinUp != 0xFF) center = new Switch(pinCenter);
		if (pinUp != 0xFF) right = new Switch(pinRight);
		if (pinUp != 0xFF) down = new Switch(pinDown);
		addMouseListener(this);
	}
	
	public void mousePressed(MouseEvent me) { 
		int x = (getWidth()-SWITCH_SIZE)/2;
		int y = (getHeight()-SWITCH_SIZE)/2;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE) 
				&& center != null) center.setPressed(true);
		x -= SWITCH_SIZE + SWITCH_SPACING;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& left != null) left.setPressed(true);
		x += 2*(SWITCH_SIZE + SWITCH_SPACING);
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& right != null) right.setPressed(true);
		x -= SWITCH_SIZE + SWITCH_SPACING;
		y -= SWITCH_SIZE + SWITCH_SPACING;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& up != null) up.setPressed(true);
		y += 2*(SWITCH_SIZE + SWITCH_SPACING);
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& down != null) down.setPressed(true);
		repaint();
	}

	public void mouseReleased(MouseEvent me) { 
		if (up != null) up.setPressed(false);
		if (left != null) left.setPressed(false);
		if (center != null) center.setPressed(false);
		if (right != null) right.setPressed(false);
		if (down != null) down.setPressed(false);
		repaint();
	}

	public Dimension getPreferredSize() {
		return new Dimension(3*SWITCH_SIZE+4*SWITCH_SPACING, 3*SWITCH_SIZE+4*SWITCH_SPACING);
	}
	
	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		int x = (getWidth()-SWITCH_SIZE)/2;
		int y = (getHeight()-SWITCH_SIZE)/2;
		if (center != null) paintSwitch(g2d, center.getPin(), x, y);
		if (left != null) paintSwitch(g2d, left.getPin(), x-SWITCH_SIZE-SWITCH_SPACING, y);
		if (right != null) paintSwitch(g2d, right.getPin(), x+SWITCH_SIZE+SWITCH_SPACING, y);
		if (up != null) paintSwitch(g2d, up.getPin(), x, y-SWITCH_SIZE-SWITCH_SPACING);
		if (down != null) paintSwitch(g2d, down.getPin(), x, y+SWITCH_SIZE+SWITCH_SPACING);
	}
	
	private void paintSwitch(Graphics2D g2d, int pin, int x, int y) {
		if (Switch.isPressed(pin)) g2d.setColor(new Color(130, 130, 130));
		else g2d.setColor(new Color(200, 200, 200));
		g2d.fillRect(x, y, SWITCH_SIZE, SWITCH_SIZE);
		g2d.setColor(Color.BLACK);
		g2d.drawRect(x, y, SWITCH_SIZE-1, SWITCH_SIZE-1);
		g2d.drawRect(x+1, y+1, SWITCH_SIZE-3, SWITCH_SIZE-3);
		x += (SWITCH_SIZE - g2d.getFontMetrics().getStringBounds(pin+"", g2d).getWidth())/2;
		y += (SWITCH_SIZE - g2d.getFontMetrics().getHeight())/2 + g2d.getFontMetrics().getAscent(); 
		g2d.drawString(pin+"", x, y);
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
}
