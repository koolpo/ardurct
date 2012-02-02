package com.google.code.ardurct.touchscreen;

import com.google.code.ardurct.Eeprom;

class point_t {
	public int x;
	public int y;
}

class size_t {
	public int width;
	public int height;
}

class arc_t {
	public int radius;
	public int octant;
}

class color_t {
	public int rgb565;
	public int na;
}

class thickness_t {
	public int value;
	public int na;
}

class scale_t {
	public int mul;
	public int div;
}

class font_t {
	public int size;
	public int isBold;
	public int isOverlay;
}

class macroCommandParameter_t {
	point_t point = new point_t();
	size_t size = new size_t();;
	arc_t arc = new arc_t();;
	color_t color = new color_t();;
	thickness_t thickness = new thickness_t();;
	scale_t scale = new scale_t();;
	font_t font = new font_t();;
	
	public void unionize() {
		size.width = point.x;
		size.height = point.y;
		arc.radius = point.x;
		thickness.value = point.x;
		font.size = point.x;
	}
}

class macroCommand_t {
	int cmd;
	int nbParams;
	macroCommandParameter_t p1 = new macroCommandParameter_t();
	macroCommandParameter_t p2 = new macroCommandParameter_t();
	macroCommandParameter_t p3 = new macroCommandParameter_t();
	int text[] = new int[1];
	
	public void unionize() {
		p1.unionize();
		p2.unionize();
		p3.unionize();
	}
} 

public class MacroDrawing extends ScreenHAL {

	/* 
	 *	A macro is a serie of drawing commands which can chain together.
	 *  Each command is a few letters followed by a space and some parameters separated by spaces
	 *
	 *	Case:		There is no case sentitiveness. 
	 *	Chaining: 	If the number of parameters is not sufficient to finish the command, 
	 *				the last point reached by the previous command is used 
	 *	Numbers:	Colors are in hexadecimal
	 *				all other are in decimal
	 *
	 *	Examples
	 *	--------
	 *		Draw a thick red line of 3 segments
	 *			pt 3 pc F800 l 10 10 30 40 l 40 50 
	 *		Draw a blue seven
	 *			pt 1 pc 001F l 10 10 30 10 l 30 13 anw 10 l 15 40
	 *
	 *	Command syntax
	 *  --------------
	 * 	(items between brackets can be ommited)
	 *
	 *		p	Preset			can preset the thickness, color, background, scale, fontSize, fontBold, fontOverlay
	 *			pt thickness	preset the thickness
	 *			ps scale [div]	preset the scale, all further drawing will be scaled by scale/div
	 *			pc color		preset the foreground color
	 *			pb color		preset the background color
	 *			pf[b][o] size	preset the font
	 *				b			if present the font will be bold
	 *				o			if present the font will be overlaid
	 *		
	 *		l	Line
	 *			l x1 y1 [x2 y2]
	 *				x1 y1	start or next point in the line
	 *				x2 y2	next point in the line
	 *
	 *		a	Arc			can be drawn clockwise or counter clockwise and filled
	 *			a[r][f]DDD [x0 y0] radius
	 *				r		if present, the arc will be drawn reversed (counter clockwise). i.e.: if DDD=NE
	 *							if r the East end will be attached to the end of last line. 
	 *							else the North end will be attached
	 *				f		if present, the arc will be filled		
	 *				DDD 	can be n, s, e, w, ne, nw, se, sw, nne, nee, see, sse, ssw, sww, nww, nnw
	 *				radius	radius of the arc
	 *				x0 y0	center of the arc. If ommited the top (or bottom) of the arc will be connected to the last point drawn
	 *
	 *		r	Rectangle	can be rounded and filled
	 *			r[r][f] x1 y1 width height [radius]
	 *				r		if present, the rectangle will be rounded and radius is required
	 *				f		if present, the rectangle will be filled
	 *				x1 y1	top left of the rectangle
	 *				width
	 *				height	width and height of the rectangle
	 *
	 *		t	Triangle	can be filled
	 *			t[f] x1 y1 x2 y2 x3 y3
	 *				x1 y1	first point
	 *				x2 y2	second point
	 *				x3 y3	third point
	 *
	 *		c	Circle		can be filled
	 *			c[f] x0 y0 radius
	 *				x0 y0	center of the circle
	 *				radius	radius of the circle
	 * 
	 *		s	String
	 *			s x1 y1 "text"
	 *				x1 y1	top left of the first char drawn
	 *				"text"	text to draw. To draw a quote, some characters can be escaped \n, \", \\
	 *
	 *		w	Write		Write the macro in EEPROM, all following commands are written to the eeprom
	 *			w n
	 *				n		the macro number to write
	 *
	 *		e	Execute		Execute a macro from EEPROM
	 *			e[r] n
	 *				r		reset macro variables before executing
	 *				n		the macro number to execute
	 */

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public final static int SCREEN_MACRO_MAX_NUMBER  = 16;
	public final static int SCREEN_MACRO_MAX_SIZE  = 128;

	public final static int SCREEN_MACRO_CMD_NONE =						0x00;
	public final static int SCREEN_MACRO_CMD_PRESET_FOREGROUND =		0x11;
	public final static int SCREEN_MACRO_CMD_PRESET_BACKGROUND =		0x12;
	public final static int SCREEN_MACRO_CMD_PRESET_FONT =				0x13;
	public final static int SCREEN_MACRO_CMD_PRESET_THICKNESS =			0x14;
	public final static int SCREEN_MACRO_CMD_PRESET_SCALE	=			0x15;
	public final static int SCREEN_MACRO_CMD_LINE 		=				0x20;
	public final static int SCREEN_MACRO_CMD_ARC 	=					0x30;
	public final static int SCREEN_MACRO_CMD_ARC_FILLED 	=			0x31;
	public final static int SCREEN_MACRO_CMD_ARC_REVERSED 	=			0x32;
	public final static int SCREEN_MACRO_CMD_ARC_FILLED_REVERSED 	=	0x33;
	public final static int SCREEN_MACRO_CMD_RECTANGLE	=				0x40;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_FILLED	=		0x41;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_ROUNDED	=		0x42;
	public final static int SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED=	0x43;
	public final static int SCREEN_MACRO_CMD_TRIANGLE	=				0x50;
	public final static int SCREEN_MACRO_CMD_TRIANGLE_FILLED	=		0x51;
	public final static int SCREEN_MACRO_CMD_CIRCLE		=				0x60;
	public final static int SCREEN_MACRO_CMD_CIRCLE_FILLED	=			0x61;
	public final static int SCREEN_MACRO_CMD_STRING	=					0x70;
	public final static int SCREEN_MACRO_CMD_WRITE	=					0xE0;
	public final static int SCREEN_MACRO_CMD_EXECUTE	=				0xF0;
	public final static int SCREEN_MACRO_CMD_EXECUTE_WITH_RESET	=		0xF1;

	public final static int SCREEN_MACRO_FORMAT_ERROR = -1;
	public final static int SCREEN_MACRO_NUMBER_ERROR = -0xFFFF;

	public final static int SCREEN_MACRO_CMD_GROUP_MASK = 0xF0;

	public final static int SCREEN_MACRO_CMD_GROUP_NONE		=			0x00;
	public final static int SCREEN_MACRO_CMD_GROUP_PRESET	=			0x10;
	public final static int SCREEN_MACRO_CMD_GROUP_LINE 	=			0x20;
	public final static int SCREEN_MACRO_CMD_GROUP_ARC	 	=			0x30;
	public final static int SCREEN_MACRO_CMD_GROUP_RECTANGLE=			0x40;
	public final static int SCREEN_MACRO_CMD_GROUP_TRIANGLE	=			0x50;
	public final static int SCREEN_MACRO_CMD_GROUP_CIRCLE	=			0x60;
	public final static int SCREEN_MACRO_CMD_GROUP_STRING	=			0x70;
	public final static int SCREEN_MACRO_CMD_GROUP_EXECUTE	=			0xF0;

	public final static int SCREEN_MACRO_COS45_LSH16	= 46340;

	int eeprom_read_uint8_t(int address) {
		return Eeprom.read_uint8_t(address);
	}
	
	void eeprom_write_uint8_t(int address, int value) {
		Eeprom.write_uint8_t(address, value);
	}

	public void executeMacro(String macro, int x, int y, int scaleMul, int scaleDiv, boolean isScaleThickness, boolean continueLastMacro) {
		int buffer[] = new int[macro.length()+1];
		for (int i=0; i<macro.length(); i++) buffer[i] = macro.charAt(i);
		buffer[macro.length()] = 0;	
		executeMacro(buffer, x, y, scaleMul, scaleDiv, isScaleThickness, continueLastMacro, true);
	}

	void executeMacro(int macro[], int x, int y, int scaleMul, int scaleDiv, boolean isScaleThickness, boolean continueLastMacro, boolean grabAndReleaseBus) {
		int s[] = macro;
		macroCommand_t mc = new macroCommand_t();
		boolean drawMode = true;
		int wBuffer[] = new int[256];
		int wBufferPtr = 0;
		int wBufferNb = 0;
		
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		int i = 0;
		// convert to upper case, transform all char below space as space
		_formatMacroSentence(s);
		while (s[i] != 0) {
			// get the command
			switch (s[i]) {
				case 'L':
					mc.cmd = SCREEN_MACRO_CMD_LINE;
					if (s[i+1] != ' ') return;
					break;
				case 'A':
					mc.cmd = SCREEN_MACRO_CMD_ARC;
					if (s[i+1] == 'R') {
						mc.cmd = SCREEN_MACRO_CMD_ARC_REVERSED;
						i++;
					}
					if (s[i+1] == 'F') {
						if (mc.cmd == SCREEN_MACRO_CMD_ARC) mc.cmd = SCREEN_MACRO_CMD_ARC_FILLED;
						else mc.cmd = SCREEN_MACRO_CMD_ARC_FILLED_REVERSED;
						i++;
					} 
					if (s[i+1] == 'N') {
						if (s[i+2] == 'N') {
							if (s[i+3] == 'E') mc.p2.arc.octant = SCREEN_ARC_NNE;
							else if (s[i+3] == 'W') mc.p2.arc.octant = SCREEN_ARC_NNW;
							else if (s[i+3] != ' ') return;
						} else if (s[i+2] == 'E') {
							if (s[i+3] == 'E') mc.p2.arc.octant = SCREEN_ARC_NEE;
							else if (s[i+3] == ' ') mc.p2.arc.octant = SCREEN_ARC_NE;
							else return;
						} else if (s[i+2] == 'W') {
							if (s[i+3] == 'W') mc.p2.arc.octant = SCREEN_ARC_NWW;
							else if (s[i+3] == ' ') mc.p2.arc.octant = SCREEN_ARC_NW;
							else return;
						} else if (s[i+2] == ' ') mc.p2.arc.octant = SCREEN_ARC_N;
						else return;
					} else if (s[i+1] == 'S') {
						if (s[i+2] == 'S') {
							if (s[i+3] == 'E') mc.p2.arc.octant = SCREEN_ARC_SSE;
							else if (s[i+3] == 'W') mc.p2.arc.octant = SCREEN_ARC_SSW;
							else if (s[i+3] != ' ') return;
						} else if (s[i+2] == 'E') {
							if (s[i+3] == 'E') mc.p2.arc.octant = SCREEN_ARC_SEE;
							else if (s[i+3] == ' ') mc.p2.arc.octant = SCREEN_ARC_SE;
							else return;
						} else if (s[i+2] == 'W') {
							if (s[i+3] == 'W') mc.p2.arc.octant = SCREEN_ARC_SWW;
							else if (s[i+3] == ' ') mc.p2.arc.octant = SCREEN_ARC_SW;
							else return;
						} else if (s[i+2] == ' ') mc.p2.arc.octant = SCREEN_ARC_S;
						else return;
					} else if ((s[i+1] == 'E') && (s[i+2] == ' ')) mc.p2.arc.octant = SCREEN_ARC_E;
					else if ((s[i+1] == 'W') && (s[i+2] == ' ')) mc.p2.arc.octant = SCREEN_ARC_W;
					else return;
					i++;
					if (s[i+2] != ' ') i++;
					if (s[i+3] != ' ') i++;
					break;
				case 'R':
					if (s[i+1] == 'R') {
						i++;
						if (s[i+1] == 'F') {
							i++;
							mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED;
						} else if (s[i+1] == ' ') mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_ROUNDED;
					} else if (s[i+1] == 'F') {
						mc.cmd = SCREEN_MACRO_CMD_RECTANGLE_FILLED;
						i++;
					} else 
					if (s[i+1] != ' ') return;
					break;
				case 'S':
					mc.cmd = SCREEN_MACRO_CMD_STRING;
					if (s[i+1] != ' ') return;
					break;
				case 'C':
					if (s[i+1] == 'F') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_CIRCLE_FILLED;
					} else mc.cmd = SCREEN_MACRO_CMD_CIRCLE;
					if (s[i+1] != ' ') return;
					break;
				case 'T':
					if (s[i+1] == 'F') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_TRIANGLE_FILLED;
					} else mc.cmd = SCREEN_MACRO_CMD_TRIANGLE;
					if (s[i+1] != ' ') return;
					break;
				case 'P':
					if (s[i+1] == 'T') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_THICKNESS;
						i++;
						if (s[i+1] != ' ') return;
					} else if (s[i+1] == 'S') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_SCALE;
						i++;
						if (s[i+1] != ' ') return;
					} else if (s[i+1] == 'F') {
						mc.cmd = SCREEN_MACRO_CMD_PRESET_FONT;
						i++;
						if (s[i+1] == 'B') {
							mc.p1.font.isBold = 1;
							i++;
						} 
						if (s[i+1] == 'O') {
							mc.p1.font.isOverlay = 1;
							i++;
						} 
						if (s[i+1] != ' ') return;
					} else if (s[i+1] == 'C') {
						int len = _parseHexUInt16(s, i+2, mc.p1.color);
						if (len == SCREEN_MACRO_FORMAT_ERROR) return;
						i += len+1;
						mc.cmd = SCREEN_MACRO_CMD_PRESET_FOREGROUND;
					} else if (s[i+1] == 'B') {
						int len = _parseHexUInt16(s, i+2, mc.p1.color);
						if (len == SCREEN_MACRO_FORMAT_ERROR) return;
						i += len+1;
						mc.cmd = SCREEN_MACRO_CMD_PRESET_BACKGROUND;
					} else return;
					break;
				case 'E':
					if (s[i+1] == 'R') {
						i++;
						mc.cmd = SCREEN_MACRO_CMD_EXECUTE_WITH_RESET;
					} else mc.cmd = SCREEN_MACRO_CMD_EXECUTE;
					if (s[i+1] != ' ') return;
					break;
				case 'W':
					drawMode = false;
					mc.cmd = SCREEN_MACRO_CMD_WRITE;
					if (s[i+1] != ' ') return;
					break;
				case ' ':
					mc.cmd = SCREEN_MACRO_CMD_NONE;
					break;
				default:
					return;
			}
			i++;
			
			if (mc.cmd == SCREEN_MACRO_CMD_NONE) continue;
			
			// parse the command parameters
			int len = _parseMacroCommandParameters(s, i, mc);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return;
			i += len;
			mc.unionize();	// for java only
			
			// Execute the command
			if (drawMode) drawMacroCommand(mc, x, y, scaleMul, scaleDiv, isScaleThickness, true, grabAndReleaseBus);
			else if (mc.cmd == SCREEN_MACRO_CMD_WRITE) {
				wBufferNb = mc.p1.point.x;
				if (wBufferNb > SCREEN_MACRO_MAX_NUMBER) return;
			}

			// At this stage, we have a struct containing the command to execute
			// this can be stored in EEPROM for future execution
			if (mc.cmd != SCREEN_MACRO_CMD_WRITE) {
				int endWrite = _compressMacroCommand(mc, wBuffer, wBufferPtr);
				if (endWrite != SCREEN_MACRO_FORMAT_ERROR) wBufferPtr = endWrite;
			}
		}
		if (!drawMode) {
			// write to EEPROM
			if ((wBufferPtr > 0) && (wBufferPtr < SCREEN_MACRO_MAX_SIZE)) {
				eeprom_write_uint8_t(wBufferNb, wBufferPtr);
				for (int j=0; j<wBufferPtr; j++) eeprom_write_uint8_t(SCREEN_MACRO_MAX_NUMBER + wBufferNb*SCREEN_MACRO_MAX_SIZE+j, wBuffer[j]);
			}
		}
		debugArray(wBuffer, wBufferPtr, 1, 2);
	}

	void drawMacroCommand(macroCommand_t mc, int x, int y, int scaleMul, int scaleDiv, boolean isScaleThickness, boolean continueLastMacro, boolean grabAndReleaseBus) {
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		if (grabAndReleaseBus) _grabBus();
		_executeMacroCommand(mc, x, y, scaleMul, scaleDiv, isScaleThickness);
		if (grabAndReleaseBus) _releaseBus();
	}


	void executeEepromMacro(int macroNb, int x, int y, int scaleMul, int scaleDiv, boolean isScaleThickness, boolean continueLastMacro, boolean grabAndReleaseBus) {
		macroCommand_t mc = new macroCommand_t();;
		// initialize relative origin
		if (!continueLastMacro) _initializeMacros();
		mc.cmd = SCREEN_MACRO_CMD_EXECUTE;
		mc.nbParams = macroNb;
		if (grabAndReleaseBus) _grabBus();
		_executeMacroCommand(mc, x, y, scaleMul, scaleDiv, isScaleThickness);
		if (grabAndReleaseBus) _releaseBus();
	}


	void _executeMacroCommand(macroCommand_t mc, int x, int y, int scaleMul, int scaleDiv, boolean isScaleThickness) {
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) _mForegroundColor = mc.p1.color.rgb565;
			else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND) _mBackgroundColor = mc.p1.color.rgb565;
			else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) _mThickness = mc.p1.thickness.value;
			else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				_mFontSize = mc.p1.font.size;
				_mIsFontBold = (mc.p1.font.isBold != 0);
				_mIsFontOverlay = (mc.p1.font.isOverlay != 0);
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_SCALE) {
				_mScaleMul = mc.p1.scale.mul;
				_mScaleDiv = 1;
				if ((mc.nbParams > 1) && (mc.p1.scale.div != 0)) _mScaleDiv = mc.p1.scale.div;
			}
			return;
		}
		
		int sMul = _mScaleMul * (scaleMul == 0 ? 1 : scaleMul);
		int sDiv = _mScaleDiv * (scaleDiv == 0 ? 1 : scaleDiv);
		int sX = x + mc.p1.point.x * sMul/sDiv;
		int sY = y + mc.p1.point.y * sMul/sDiv;		
		int sThickness = _mThickness;
		if (isScaleThickness) sThickness = sThickness * sMul/sDiv;	
		
		// lines
		if (mc.cmd == SCREEN_MACRO_CMD_LINE) {
			int sX2, sY2;
			if (mc.nbParams < 2) return;
			if (mc.nbParams > 2) {
				sX2 = x + mc.p2.point.x * sMul/sDiv;
				sY2 = y + mc.p2.point.y * sMul/sDiv;
				// p2 becomes the end point
				_mX = mc.p2.point.x;
				_mY = mc.p2.point.y;
			} else {
				sX2 = sX;
				sY2 = sY;
				sX = x + _mX * sMul/sDiv;
				sY = y + _mY * sMul/sDiv;	
				// p1 becomes the end point
				_mX = mc.p1.point.x;
				_mY = mc.p1.point.y;
			}
			drawLine(sX, sY, sX2, sY2, _mForegroundColor, sThickness, false);
			return;
		}	
		
		// arcs
		if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			if (mc.nbParams < 1) return;
			boolean reversed = (mc.cmd == SCREEN_MACRO_CMD_ARC_REVERSED) || (mc.cmd == SCREEN_MACRO_CMD_ARC_FILLED_REVERSED);
			int sRadius = 0;
			if (mc.nbParams == 1) {
				sRadius = mc.p1.arc.radius * sMul/sDiv;
				int sArcStartX = _getArcEnd(sRadius, mc.p2.arc.octant, !reversed, true);
				int sArcStartY = _getArcEnd(sRadius, mc.p2.arc.octant, !reversed, false);
				int sArcEndX = _getArcEnd(sRadius, mc.p2.arc.octant, reversed, true);
				int sArcEndY = _getArcEnd(sRadius, mc.p2.arc.octant, reversed, false);
				sX = x + _mX * sMul/sDiv - sArcStartX;
				sY = y + _mY * sMul/sDiv - sArcStartY;
				_mX += (sArcEndX - sArcStartX) * sDiv/sMul;
				_mY += (sArcEndY - sArcStartY) * sDiv/sMul;

			} else if (mc.nbParams == 3) {
				sRadius = mc.p2.arc.radius * sMul/sDiv;
				_mX = mc.p1.point.x + _getArcEnd(mc.p2.arc.radius, mc.p2.arc.octant, reversed, true);
				_mY = mc.p1.point.y + _getArcEnd(mc.p2.arc.radius, mc.p2.arc.octant, reversed, false);
			} else return;
			if ((mc.cmd == SCREEN_MACRO_CMD_ARC_REVERSED) || (mc.cmd == SCREEN_MACRO_CMD_ARC)) drawArc(sX, sY, sRadius, mc.p2.arc.octant, _mForegroundColor, sThickness, false);
			else fillArc(sX, sY, sRadius, mc.p2.arc.octant, _mForegroundColor, false);			
			return;
		}
		
		// circles
		if (group == SCREEN_MACRO_CMD_GROUP_CIRCLE) {
			// we need at least 3 parameters
			if (mc.nbParams < 3) return;
			int sRadius = mc.p2.arc.radius * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_CIRCLE) drawCircle(sX, sY, sRadius, _mForegroundColor, sThickness, false);
			else fillCircle(sX, sY, sRadius, _mForegroundColor, false);
			return;
		}
		
		// rectangles
		if (group == SCREEN_MACRO_CMD_GROUP_RECTANGLE) {
			// we need at least 4 parameters
			if (mc.nbParams < 4) return;
			int sWidth = mc.p2.size.width * sMul/sDiv;
			int sHeight = mc.p2.size.height * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE) drawRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_FILLED) fillRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, false);
			// we need at least 1 more parameter to display the rounded rectangle
			if (mc.nbParams < 5) return;
			int sRadius = mc.p3.arc.radius * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_ROUNDED) drawRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED) fillRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, false);
			return;
		}
		
		// triangles
		if (group == SCREEN_MACRO_CMD_GROUP_TRIANGLE) {
			// we need at least 6 parameters
			if (mc.nbParams < 6) return;
			int sX2 = mc.p2.point.x * sMul/sDiv;
			int sY2 = mc.p2.point.y * sMul/sDiv;
			int sX3 = mc.p3.point.x * sMul/sDiv;
			int sY3 = mc.p3.point.y * sMul/sDiv;
			if (mc.cmd == SCREEN_MACRO_CMD_TRIANGLE) drawTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, sThickness, false);
			else if (mc.cmd == SCREEN_MACRO_CMD_TRIANGLE_FILLED) fillTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, false);
			return;
		}
			
		// strings
		if (mc.cmd == SCREEN_MACRO_CMD_STRING) {
			if (mc.nbParams < 2) return;
			int bc = _backgroundColor;
			if (!_mIsFontOverlay) setBackgroundColor(_mBackgroundColor);
			drawString(mc.text, sX, sY, _mForegroundColor, _mFontSize, _mIsFontBold, _mIsFontOverlay, false);
			if (!_mIsFontOverlay) setBackgroundColor(bc);
			return;
		}
		
		// executes
		if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			mc.nbParams = mc.p1.point.x;
			if (mc.nbParams >= SCREEN_MACRO_MAX_NUMBER) return;
			// read the length in the EEPROM allocation table
			int length = eeprom_read_uint8_t(mc.nbParams);
			if (length == 0xFF) return;
			// read the EEPROM pointers table to get the start
			int start = SCREEN_MACRO_MAX_NUMBER + mc.nbParams * SCREEN_MACRO_MAX_SIZE;
			// get the compressed macro
			int buffer[] = new int[SCREEN_MACRO_MAX_SIZE];
			int i=0;
			while (i < length) {
				buffer[i] = eeprom_read_uint8_t(start+i);
				i++;
			}
			buffer[i] = 0;
			macroCommand_t emc = new macroCommand_t();
			// uncompress the macro commands and execute them
			if (mc.cmd == SCREEN_MACRO_CMD_EXECUTE_WITH_RESET) _initializeMacros();
			i = 0;
			while (i < length) {
				int len = _uncompressMacroCommand(buffer, i, emc);
				if (len == SCREEN_MACRO_FORMAT_ERROR) return;
				_executeMacroCommand(emc, x, y, scaleMul, scaleDiv, isScaleThickness);
				i = len;
			}
		}
	}


	int _parseMacroCommandParameters(int s[], int n, macroCommand_t mc) {
		int i = n;
		
		mc.nbParams = 0;
		// parse numbers until there is an error
		int len = _parseInt16(s, i, mc.p1.point, true);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams = 1;
		len = _parseInt16(s, i, mc.p1.point, false);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams ++;
		if (mc.cmd == SCREEN_MACRO_CMD_STRING) {
			// for text, always 2 coordinates, followed by text
			while ((s[i] != 0) && (s[i] != '"')) i++;
			if (s[i] != '"') return SCREEN_MACRO_FORMAT_ERROR;
			i++;
			int txtStart = i;
			// search for end of string
			while ((s[i] != 0) && (s[i] != '"')) i++;
			if (s[i] != '"') return SCREEN_MACRO_FORMAT_ERROR;
			s[i] = 0;
			mc.text = new int[i-txtStart+2];
			memcpy_P(mc.text, s, txtStart, i-txtStart+1);
			mc.nbParams = i - txtStart;
			return i+1-n;
		}
		len = _parseInt16(s, i, mc.p2.point, true);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams ++;
		len = _parseInt16(s, i, mc.p2.point, false);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams ++;
		len = _parseInt16(s, i, mc.p3.point, true);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams ++;
		len = _parseInt16(s, i, mc.p3.point, false);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return i-n;
		i += len;
		mc.nbParams ++;
		return i-n;
	}

	void _formatMacroSentence(int s[]) {
		int i=0;
		while (s[i] != 0) {
			// before quotes, convert lower into higher
			while ((s[i] != 0) && (s[i] != '"')) {
				if ((s[i] >= 'a') && (s[i] <= 'z')) s[i] = s[i] + 'A' - 'a';
				// replace characters not allowed by spaces
				if ((s[i] < '0') || (s[i] > 'Z') || ((s[i] > '9') && (s[i] < 'A'))) s[i] = ' ';
				i++;
			}
			if (s[i] == 0) break;
			i++;
			// inside the quotes, replace escapes
			while ((s[i] != 0) && (s[i] != '"')) {
				if (s[i] == '\\') {
					if (s[i+1] == 'n') s[i+1] = '\n';
					else if (s[i+1] == 't') s[i+1] = ' ';
					int j = i;
					while (s[j] != 0) s[j] = s[++j];
					i++;
				}
				i++;
			}
			if (s[i] == 0) break;
			i++;
		}
		i=0;
		while (s[i] != 0) System.out.print((char)s[i++]);
		System.out.println();	
	}
	
	int _parseHexUInt16(int s[], int n, color_t number) {
		int i = n;
		number.rgb565 = 0;
		// remove front spaces
		while ((s[i] != 0) && (s[i] <= ' ')) i++;
		if (s[i] == 0) return SCREEN_MACRO_FORMAT_ERROR;
		// get the digits	
		while ((s[i] != 0) && (s[i] > ' ')) {
			// check for unknown digits
			if ((s[i] < '0') || (s[i] > 'F') || ((s[i] > '9') && (s[i] < 'A'))) return SCREEN_MACRO_FORMAT_ERROR;
			number.rgb565 = (number.rgb565 << 4) + s[i] - (s[i] < 'A' ? '0' :  'A' - 10);
			i++;
		}
		return i-n;
	}

	int _parseInt16(int s[], int n, point_t number, boolean isX) {
		int i = n;
		boolean negative = false;
		// remove front spaces
		while ((s[i] != 0) && (s[i] <= ' ')) i++;
		// if no value, return error
		if (s[i] == 0) return SCREEN_MACRO_FORMAT_ERROR;
		int value = 0;
		// get the digits	
		if (s[i] == '-') {
			negative = true;
			i++;
		}
		while ((s[i] != 0) && (s[i] > ' ')) {
			// check for unknown digits
			if ((s[i] < '0') || (s[i] > '9')) return SCREEN_MACRO_FORMAT_ERROR;
			value = value * 10 + s[i] - '0';
			i++;
		}
		if (negative) value = -value;
		if (isX) number.x = value;
		else number.y = value;
		return i-n;
	}

	int _getArcEnd(int radius, int octant, boolean isReversed, boolean isX) {
		int value = (radius * SCREEN_MACRO_COS45_LSH16) >> 16;
		switch (octant) {
			case SCREEN_ARC_SSE:
				if (isReversed && isX) return value;
				if (isReversed) return value;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_SEE:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return value;
				return value;
			case SCREEN_ARC_NEE:
				if (isReversed && isX) return value;
				if (isReversed) return -value;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_NNE:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return value;
				return -value;
			case SCREEN_ARC_SSW:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return -value;
				return value;
			case SCREEN_ARC_SWW:
				if (isReversed && isX) return -value;
				if (isReversed) return value;
				if (isX) return -radius;
				return 0;
			case SCREEN_ARC_NWW:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return -value;
				return -value;
			case SCREEN_ARC_NNW:
				if (isReversed && isX) return -value;
				if (isReversed) return -value;
				if (isX) return 0;
				return -radius;
			case SCREEN_ARC_NE:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_SE:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_SW:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_NW:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return 0;
				return -radius;
			case SCREEN_ARC_N:
				if (isReversed && isX) return -radius;
				if (isReversed) return 0;
				if (isX) return radius;
				return 0;
			case SCREEN_ARC_S:
				if (isReversed && isX) return radius;
				if (isReversed) return 0;
				if (isX) return -radius;
				return 0;
			case SCREEN_ARC_E:
				if (isReversed && isX) return 0;
				if (isReversed) return -radius;
				if (isX) return 0;
				return radius;
			case SCREEN_ARC_W:
				if (isReversed && isX) return 0;
				if (isReversed) return radius;
				if (isX) return 0;
				return -radius;
		}
		return 0;
	}

	/*
	 * Compression format for commands
	 *	dependant on command
	 *		cmd [nbParams] param1 .. paramN
	 *
	 * Compression format for numbers
	 *		NSxxxxxx
	 *			N	if N is null, only one byte of data
	 *			S	if S is null, number is positive
	 *		1Snnnnnn nnnnnnnn
	 *			From -16383 to 16383
	 *		0Snnnnnn
	 *			From -63 to 63
	 */
	int _compressMacroCommand(macroCommand_t mc, int buffer[], int bufferPtr) {
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		
		int i = bufferPtr;
		buffer[i++] = mc.cmd;
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if ((mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) || (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND)) { 
				buffer[i++] = mc.p1.color.rgb565 >> 8;
				buffer[i++] = mc.p1.color.rgb565 & 0x0FF;
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) { 
				buffer[i++] = mc.p1.thickness.value & 0x0FF;
			} else if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				buffer[i++] = (mc.p1.font.size & 0x0F) + (mc.p1.font.isBold != 0 ? 0x40 : 0) + (mc.p1.font.isOverlay != 0 ? 0x80 : 0);
			} 	
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_SCALE) {
				if ((mc.nbParams > 1) && (mc.p1.scale.div == 0)) mc.nbParams = 1;
			}
			// for the other presets, we can return, nothing more to do
			else return i;
		} else if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			buffer[i++] = mc.p2.arc.octant;
		} else if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			if (mc.nbParams >= SCREEN_MACRO_MAX_NUMBER) return SCREEN_MACRO_FORMAT_ERROR;
			buffer[i++] = mc.nbParams;
			return i;
		}

		// store the number of parameters
		buffer[i++] = mc.nbParams;
		
		// then we compress the parameters
		int len = _compressNumber(mc.p1.point.x, buffer, i);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
		i += len;
		if (mc.nbParams > 1) {
			len = _compressNumber(mc.p1.point.y, buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 2) {
			len = _compressNumber(mc.p2.point.x, buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 3) {
			len = _compressNumber(mc.p2.point.y, buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 4) {
			len = _compressNumber(mc.p3.point.x, buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 5) {
			len = _compressNumber(mc.p3.point.y, buffer, i);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		return i;
	}

	int _uncompressMacroCommand(int buffer[], int n, macroCommand_t mc) {
		int i = n;
		mc.cmd = buffer[i++];
		
		int group = mc.cmd & SCREEN_MACRO_CMD_GROUP_MASK;
		// presets
		if (group == SCREEN_MACRO_CMD_GROUP_PRESET) {
			if ((mc.cmd == SCREEN_MACRO_CMD_PRESET_FOREGROUND) || (mc.cmd == SCREEN_MACRO_CMD_PRESET_BACKGROUND)) { 
				mc.p1.color.rgb565 = (buffer[i] << 8) + buffer[i+1];
				return i+2;
			} 
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_THICKNESS) { 
				mc.p1.thickness.value = buffer[i++];
				return i;
			}
			if (mc.cmd == SCREEN_MACRO_CMD_PRESET_FONT) {
				mc.p1.font.size = buffer[i++];
				mc.p1.font.isBold = ((mc.p1.font.size & 0x40) != 0 ? 1 : 0);
				mc.p1.font.isOverlay = ((mc.p1.font.size & 0x80) != 0 ? 1 : 0);
				mc.p1.font.size = mc.p1.font.size & 0x0F;
				return i;
			} 	
		} else if (group == SCREEN_MACRO_CMD_GROUP_ARC) {
			mc.p2.arc.octant = buffer[i++];
		} else if (group == SCREEN_MACRO_CMD_GROUP_EXECUTE) {
			mc.nbParams = buffer[i++];
			if (mc.nbParams >= SCREEN_MACRO_MAX_NUMBER) return SCREEN_MACRO_FORMAT_ERROR;
			return i;
		}

		mc.nbParams = buffer[i++];

		// then we uncompress the parameters
		int len = _uncompressNumber(buffer, i, mc.p1.point);
		if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
		mc.p1.point.x = mc.p1.point.y; 
		i += len;
		if (mc.nbParams > 1) {
			len = _uncompressNumber(buffer, i, mc.p1.point);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 2) {
			len = _uncompressNumber(buffer, i, mc.p2.point);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			mc.p2.point.x = mc.p2.point.y; 
			i += len;
		}
		if (mc.nbParams > 3) {
			len = _uncompressNumber(buffer, i, mc.p2.point);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}
		if (mc.nbParams > 4) {
			len = _uncompressNumber(buffer, i, mc.p3.point);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			mc.p3.point.x = mc.p3.point.y; 
			i += len;
		}
		if (mc.nbParams > 5) {
			len = _uncompressNumber(buffer, i, mc.p3.point);
			if (len == SCREEN_MACRO_FORMAT_ERROR) return SCREEN_MACRO_FORMAT_ERROR;
			i += len;
		}

		mc.unionize();
		
		return i;
	}

	int _compressNumber(int in, int out[], int n) {
		boolean negative = false;
		if (in < 0) {
			negative = true;
			in = -in;
		}
		if (in > (0x01 << 14)-1) return SCREEN_MACRO_FORMAT_ERROR;
		if (in > (0x01 << 6)-1) {
			// 2 bytes
			out[n] = (in >> 8) | (negative ? (0x01 << 6) : 0x00) | (0x01 << 7);
			out[n+1] = (in & 0x0FF);
			return 2;
		}
		// 1 byte
		out[n] = in | (negative ? (0x01 << 6) : 0x00);
		return 1;
	}

	// will return -0xFFFF if problem
	int _uncompressNumber(int in[], int n, point_t number) {
		boolean negative = false;
		if ((in[n] & (0x01 << 6)) != 0) negative = true;
		number.y = in[n] & 0x3F;
		if ((in[n] & (0x01 << 7)) != 0) {
			number.y = (number.y << 8) | in[n+1];
			if (negative) number.y = -number.y;
			return 2;
		}
		if (negative) number.y = -number.y;
		return 1;	
	}

}
