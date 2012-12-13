package com.google.code.ardurct.libraries.eventManager;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import com.google.code.ardurct.GraphicsPanel;

public class TouchPanel 
implements MouseListener, MouseMotionListener {

	public static final int TOUCHSCREEN_NO_TOUCH = -1;

	int mouseX;
	int mouseY;

	public TouchPanel(GraphicsPanel javaScreen) {
		javaScreen.addMouseListener(this);
		javaScreen.addMouseMotionListener(this);
	}
	
	// should be in Touchscreen.cpp
	public int getTouchXYZ(int[] x, int[] y, int[] z) {
		x[0] = mouseX;
		y[0] = mouseY;
		if (mouseX == TOUCHSCREEN_NO_TOUCH) return TOUCHSCREEN_NO_TOUCH;
		return 10;
	}
	
	public void mousePressed(MouseEvent me) {
		mouseX = me.getX();
		mouseY = me.getY();
	}

	public void mouseReleased(MouseEvent me) {
		mouseX = TOUCHSCREEN_NO_TOUCH;
		mouseY = TOUCHSCREEN_NO_TOUCH;
	}

	public void mouseDragged(MouseEvent me) {
		mouseX = me.getX();
		mouseY = me.getY();		
	}

	/* Unused */
	public void mouseClicked(MouseEvent arg0) { }

	public void mouseEntered(MouseEvent arg0) { }

	public void mouseExited(MouseEvent arg0) { }

	public void mouseMoved(MouseEvent arg0) { }

}
