package com.google.code.ardurct.touchscreen;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class Touchscreen extends ScreenImpl {

	public final int SCALE_X = 2;
	public final int SCALE_Y = 2;
	
	public final int BLACK 	=	0x0000;		/* 000000 */
	public final int RED 	=	0xF800;		/* FF0000 */
	public final int GREEN 	=	0x07E0;		/* 00FF00 */
	public final int BLUE 	=	0x001F; 		/* 0000FF */
	public final int CYAN 	=	0x07FF; 		/* 00FFFF */
	public final int MAGENTA =	0xF81F; 		/* FF00FF */
	public final int YELLOW =	0xFFE0; 		/* FFFF00 */
	public final int WHITE 	=	0xFFFF; 		/* FFFFFF */
	public final int LIGHT_GREY =0xCE79; 	/* CCCCCC */
	public final int GREY 	=	0x9CD3; 		/* 999999 */
	public final int DARK_GREY =0x632C; 		/* 666666 */
	public final int ORANGE =	0xFBE0; 		/* FF7F00 */
	public final int FUSCHIA =	0xF80F; 		/* FF007F */
	public final int VIOLET =	0x780F; 		/* 7F007F */
	public final int BROWN 	=	0x83E0; 		/* 827F00 */
	public final int DARK_GREEN =0x03E0; 	/* 007F00 */
	public final int DARK_BLUE= 0x000F; 		/* 00007F */
	public final int DARK_RED = 0x7800; 		/* 7F0000 */
	public final int DARK_CYAN =0x03EF; 		/* 007F7F */
	
	private static final long serialVersionUID = 1L;

	public void begin(int foregroundColor, int backgroundColor, int fontSize, boolean fontBold, boolean fontOverlay) {
		initScreen();
		setRotation(0);
		setFont(fontSize, fontBold, fontOverlay);
		setBackgroundColor(backgroundColor);
		setForegroundColor(foregroundColor);
		if (backgroundColor != BLACK) fillRectangle(0, 0, getWidth(), getHeight(), backgroundColor, true);
	}

	void setupBacklight(int backlightPin) { 	}

	public void setBacklight(int value) { }

	int getBacklight() {
		return 0;
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D)g;
		g2d.setColor(new Color(255, 255, 255));
		g2d.fillRect(0, 0, getWidthImpl()*SCALE_X, getHeightImpl()*SCALE_Y);	
		//g2d.scale(SCALE_X, SCALE_Y);
		g2d.drawImage(getScreen(), 0, 0, this);
	}

	public Dimension getPreferredSize() {
		return new Dimension(Math.round(getWidthImpl()), Math.round(getHeightImpl()));
	}

}
