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

import com.google.code.ardurct.libraries.eventManager.IEventDefines;

public class Joystick extends JPanel 
implements MouseListener, MouseMotionListener, IEventDefines {

	private final static int SIZE = 80;
	private final static int KNOB = 24;
	
	private static final long serialVersionUID = 1L;
	
	int pinH;
	int pinV;
	private String name;
	
	public Joystick(int pinH, int pinV, String name) {
		this(pinH, pinV, ANALOG_HARDWARE_RESOLUTION/2, ANALOG_HARDWARE_RESOLUTION/2, name);
	}

	public Joystick(int pinH, int pinV, int valueH, int valueV, String name) {
		this.name = name;
		this.pinH = pinH;
		this.pinV = pinV;
		Analog.write(pinH, valueH);
		Analog.write(pinV, valueV);
		addMouseListener(this);
		addMouseMotionListener(this);
	}

	
	public void mousePressed(MouseEvent me) {
		int x = me.getX() - (getWidth()- SIZE)/2;
		int y = me.getY() - (getHeight()- SIZE)/2;
		if (x >= 0 && x <= SIZE && y >= 0 && y <= SIZE) {
			Analog.write(pinH, x * 1023 / SIZE);
			Analog.write(pinV, 1023 - y * 1023 / SIZE);
			repaint();
		} else if (x >= 0 && x <= SIZE && y > SIZE + KNOB/2) {
			Analog.write(pinH, ANALOG_HARDWARE_RESOLUTION/2);
			Analog.write(pinV, ANALOG_HARDWARE_RESOLUTION/2);
			repaint();
		}
	}


	public void mouseDragged(MouseEvent me) {
		mousePressed(me);
	}

	public Dimension getPreferredSize() {
		return new Dimension(SIZE+KNOB+4, SIZE+KNOB+34);
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(240, 240, 240));
		g2d.fillRect(0, 0, getWidth(), getHeight());
		g2d.setColor(Color.BLACK);
		int x =  (getWidth()- SIZE)/2;
		int y =  (getHeight()- SIZE)/2;
		g2d.setColor(new Color(220, 220, 220));
		g2d.fillRect(x, y, SIZE, SIZE);
		g2d.setColor(Color.BLACK);
		g2d.drawRect(x, y, SIZE, SIZE);		
		x += Analog.read(pinH) * SIZE / 1023 - KNOB/2;
		y += (1023-Analog.read(pinV)) * SIZE / 1023 - KNOB/2;
		g2d.setColor(new Color(180, 180, 180));
		g2d.fillOval(x, y, KNOB, KNOB);
		//g2d.fillRect(x, y, KNOB, KNOB);
		g2d.setColor(Color.BLACK);
		g2d.drawOval(x, y, KNOB, KNOB);
		//g2d.drawRect(x, y, KNOB, KNOB);
		String pins = "H:" + pinH + " V:" + pinV; 
		Rectangle2D strBounds = g2d.getFontMetrics().getStringBounds(pins, g2d);
		y = (getHeight()- SIZE)/2 + SIZE + KNOB/2 + 2 + g2d.getFontMetrics().getAscent(); 
		g2d.drawString(pins, Math.round((getWidth()-strBounds.getWidth())/2), y);
		strBounds = g2d.getFontMetrics().getStringBounds(name, g2d);
		g2d.drawString(name, Math.round((getWidth()-strBounds.getWidth())/2), y+g2d.getFontMetrics().getHeight()-2);
	}
	
	public void mouseClicked(MouseEvent arg0) { }
	public void mouseEntered(MouseEvent arg0) { }
	public void mouseExited(MouseEvent arg0) { }
	public void mouseMoved(MouseEvent arg0) { }
	public void mouseReleased(MouseEvent arg0) { }


}
