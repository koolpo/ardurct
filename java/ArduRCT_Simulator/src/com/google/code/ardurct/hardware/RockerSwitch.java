package com.google.code.ardurct.hardware;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JPanel;

import com.google.code.ardurct.libraries.IArduinoDefines;

public class RockerSwitch extends JPanel 
implements MouseListener, IArduinoDefines {

	private static final int SWITCH_SIZE = 24;
	private static final int SWITCH_SPACING = 4;
	
	private static final long serialVersionUID = 1L;
	
	private int upPin = 0xFF;
	private int downPin = 0xFF;
	private int leftPin = 0xFF;
	private int rightPin = 0xFF;
	private int centerPin = 0xFF;
	
	public RockerSwitch(int upPin, int centerPin, int downPin) {
		this(upPin, 0xFF, centerPin, 0xFF, downPin);
	}

	public RockerSwitch(int upPin, int leftPin, int rightPin, int downPin) {
		this(upPin, leftPin, 0xFF, rightPin, downPin);
	}
	
	public RockerSwitch(int upPin, int leftPin, int centerPin, int rightPin, int downPin) {
		this.upPin = upPin;
		this.downPin = downPin;
		this.centerPin = centerPin;
		this.rightPin = rightPin;
		this.leftPin = leftPin;
		if (upPin != 0xFF) Digital.write(upPin, HIGH);
		if (downPin != 0xFF) Digital.write(downPin, HIGH);
		if (centerPin != 0xFF) Digital.write(centerPin, HIGH);
		if (rightPin != 0xFF) Digital.write(rightPin, HIGH);
		if (leftPin != 0xFF) Digital.write(leftPin, HIGH);
		addMouseListener(this);
	}
	
	public void mousePressed(MouseEvent me) { 
		int x = (getWidth()-SWITCH_SIZE)/2;
		int y = (getHeight()-SWITCH_SIZE)/2;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE) 
				&& centerPin != 0xFF) Digital.write(centerPin, LOW);
		x -= SWITCH_SIZE + SWITCH_SPACING;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& leftPin != 0xFF) Digital.write(leftPin, LOW);
		x += 2*(SWITCH_SIZE + SWITCH_SPACING);
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& rightPin != 0xFF) Digital.write(rightPin, LOW);
		x -= SWITCH_SIZE + SWITCH_SPACING;
		y -= SWITCH_SIZE + SWITCH_SPACING;
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& upPin != 0xFF) Digital.write(upPin, LOW);
		y += 2*(SWITCH_SIZE + SWITCH_SPACING);
		if ((me.getX() > x) && (me.getX() < x+SWITCH_SIZE) && (me.getY() > y) && (me.getY() < y+SWITCH_SIZE)
				&& downPin != 0xFF) Digital.write(downPin, LOW);
		repaint();
	}

	public void mouseReleased(MouseEvent me) { 
		if (upPin != 0xFF) Digital.write(upPin, HIGH);
		if (leftPin != 0xFF) Digital.write(leftPin, HIGH);
		if (centerPin != 0xFF) Digital.write(centerPin, HIGH);
		if (rightPin != 0xFF) Digital.write(rightPin, HIGH);
		if (downPin != 0xFF) Digital.write(downPin, HIGH);
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
		paintSwitch(g2d, centerPin, x, y);
		paintSwitch(g2d, leftPin, x-SWITCH_SIZE-SWITCH_SPACING, y);
		paintSwitch(g2d, rightPin, x+SWITCH_SIZE+SWITCH_SPACING, y);
		paintSwitch(g2d, upPin, x, y-SWITCH_SIZE-SWITCH_SPACING);
		paintSwitch(g2d, downPin, x, y+SWITCH_SIZE+SWITCH_SPACING);
	}
	
	private void paintSwitch(Graphics2D g2d, int pin, int x, int y) {
		if (pin == 0xFF) return;
		if (Digital.read(pin) == LOW) g2d.setColor(new Color(130, 130, 130));
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
