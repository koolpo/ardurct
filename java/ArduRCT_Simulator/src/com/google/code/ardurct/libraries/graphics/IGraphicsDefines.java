package com.google.code.ardurct.libraries.graphics;

public interface IGraphicsDefines {

	public static final int BLACK =		0x0000;		/* 000000 */
	public static final int RED =		0xF800;		/* FF0000 */
	public static final int GREEN =		0x07E0;		/* 00FF00 */
	public static final int BLUE =		0x001F; 	/* 0000FF */
	public static final int CYAN =		0x07FF; 	/* 00FFFF */
	public static final int MAGENTA =	0xF81F; 	/* FF00FF */
	public static final int YELLOW =	0xFFE0; 	/* FFFF00 */
	public static final int WHITE =		0xFFFF; 	/* FFFFFF */
	public static final int LIGHT_GREY= 0xDEFB;		/* DDDDDD */
	public static final int VLIGHT_GREY=0xEF7D;		/* EEEEEE */
	public static final int GREY =		0x9CD3; 	/* 999999 */
	public static final int DARK_GREY =	0x632C; 	/* 666666 */
	public static final int ORANGE =	0xFDA0; 	/* FF7F00 */
	public static final int FUSCHIA =	0xF80F; 	/* FF007F */
	public static final int VIOLET =	0x780F; 	/* 7F007F */
	public static final int BROWN =		0x83E0; 	/* 827F00 */
	public static final int DARK_GREEN =0x03E0; 	/* 007F00 */
	public static final int DARK_BLUE = 0x000F; 	/* 00007F */
	public static final int DARK_RED = 	0x7800; 	/* 7F0000 */
	public static final int DARK_CYAN =	0x03EF; 	/* 007F7F */
	public static final int LIGHT_BLUE=	0xB6DC; 	/* B0DBE3 */
	
	public static final int F_LIGHT_GREY =	0xDEDE; /* DEDAF6 */
	public static final int F_GREY =	0x9494; 	/* 9491A4 */
	
	public static final int GRAPHICS_ROTATION_0	= 0;
	public static final int GRAPHICS_ROTATION_90	= 1;
	public static final int GRAPHICS_ROTATION_180	= 2;
	public static final int GRAPHICS_ROTATION_270	= 3;

	public static final int GRAPHICS_ARC_SSE = (1 << 0);
	public static final int GRAPHICS_ARC_SEE = (1 << 1);
	public static final int GRAPHICS_ARC_NEE = (1 << 2);
	public static final int GRAPHICS_ARC_NNE = (1 << 3);
	public static final int GRAPHICS_ARC_SSW = (1 << 4);
	public static final int GRAPHICS_ARC_SWW = (1 << 5);
	public static final int GRAPHICS_ARC_NWW = (1 << 6);
	public static final int GRAPHICS_ARC_NNW = (1 << 7);

	public static final int GRAPHICS_ARC_NE = (GRAPHICS_ARC_NNE+GRAPHICS_ARC_NEE);
	public static final int GRAPHICS_ARC_SE = (GRAPHICS_ARC_SSE+GRAPHICS_ARC_SEE);
	public static final int GRAPHICS_ARC_SW = (GRAPHICS_ARC_SSW+GRAPHICS_ARC_SWW);
	public static final int GRAPHICS_ARC_NW = (GRAPHICS_ARC_NNW+GRAPHICS_ARC_NWW);

	public static final int GRAPHICS_ARC_N = (GRAPHICS_ARC_NE+GRAPHICS_ARC_NW);
	public static final int GRAPHICS_ARC_S = (GRAPHICS_ARC_SE+GRAPHICS_ARC_SW);
	public static final int GRAPHICS_ARC_E = (GRAPHICS_ARC_SE+GRAPHICS_ARC_NE);
	public static final int GRAPHICS_ARC_W = (GRAPHICS_ARC_NW+GRAPHICS_ARC_SW);

	public static final int GRAPHICS_ARC_ALL =(GRAPHICS_ARC_N+GRAPHICS_ARC_S);

	public static final int GRAPHICS_POSITION_NONE = 0;
	public static final int GRAPHICS_POSITION_NW = GRAPHICS_ARC_NW;
	public static final int GRAPHICS_POSITION_NE = GRAPHICS_ARC_NE;
	public static final int GRAPHICS_POSITION_SE = GRAPHICS_ARC_SE;
	public static final int GRAPHICS_POSITION_SW = GRAPHICS_ARC_SW;
	public static final int GRAPHICS_POSITION_N = GRAPHICS_ARC_N;
	public static final int GRAPHICS_POSITION_S = GRAPHICS_ARC_S;
	public static final int GRAPHICS_POSITION_E = GRAPHICS_ARC_E;
	public static final int GRAPHICS_POSITION_W = GRAPHICS_ARC_W;
	public static final int GRAPHICS_POSITION_CENTER = GRAPHICS_ARC_ALL;
	
	public static final int GRAPHICS_STYLE_BASIC = 0;
	public static final int GRAPHICS_STYLE_NORMAL = 1;
	public static final int GRAPHICS_STYLE_ADVANCED = 2;
	
	public static final boolean OVERLAY = true;
	public static final boolean NO_OVERLAY = false;
	
	public static final int FONT_SMALL 	= 1;
	public static final int FONT_MEDIUM = 2;
	public static final int FONT_BIG = 3;
	public static final int FONT_HUGE = 4;

	public static final boolean FONT_PLAIN = false;
	public static final boolean FONT_BOLD = true;

	public static final int GRAPHICS_UI_NO_GROUP = -1;

	public static final int GRAPHICS_UI_ACTION_UP = 1;
	public static final int GRAPHICS_UI_ACTION_DOWN = 2;
	public static final int GRAPHICS_UI_ACTION_RIGHT = 3;
	public static final int GRAPHICS_UI_ACTION_LEFT = 4;
	public static final int GRAPHICS_UI_ACTION_MENU = 5;
	public static final int GRAPHICS_UI_ACTION_ENTER = 6;
	public static final int GRAPHICS_UI_ACTION_RELEASE = 7;
	public static final int GRAPHICS_UI_ACTION_TOUCH = 8;
	public static final int GRAPHICS_UI_ACTION_UNTOUCH = 9;
	public static final int GRAPHICS_UI_ACTION_DRAG = 10;
	
	public static final int GRAPHICS_UI_RELEASED = 0;
	public static final int GRAPHICS_UI_SELECTED = 1;
	public static final int GRAPHICS_UI_HIGHLIGHTED = 2;

	public static final int GRAPHICS_UI_AUTO_SIZE = 0;
	public static final int GRAPHICS_UI_SAME_AS_PREVIOUS = 0x8000;
	public static final int GRAPHICS_UI_WIDTH = 0x1000;
	public static final int GRAPHICS_UI_LEFT_OF_PREVIOUS = 0x4000;
	public static final int GRAPHICS_UI_RIGHT_OF_PREVIOUS = 0x2000;
	public static final int GRAPHICS_UI_RIGHT_OF_PREVIOUS_WITH_MARGIN = 0x3000;
	public static final int GRAPHICS_UI_HEIGHT = 0x1000;
	public static final int GRAPHICS_UI_TOP_OF_PREVIOUS = 0x4000;
	public static final int GRAPHICS_UI_BOTTOM_OF_PREVIOUS = 0x2000;
	public static final int GRAPHICS_UI_BOTTOM_OF_PREVIOUS_WITH_MARGIN = 0x3000;

	public static final int GUI_AS = GRAPHICS_UI_AUTO_SIZE;
	public static final int GUI_SAP = GRAPHICS_UI_SAME_AS_PREVIOUS;
	public static final int GUI_W = GRAPHICS_UI_WIDTH;
	public static final int GUI_H = GRAPHICS_UI_HEIGHT;
	public static final int GUI_LOP = GRAPHICS_UI_LEFT_OF_PREVIOUS;
	public static final int GUI_ROP = GRAPHICS_UI_RIGHT_OF_PREVIOUS;
	public static final int GUI_ROPWM = GRAPHICS_UI_RIGHT_OF_PREVIOUS_WITH_MARGIN;
	public static final int GUI_TOP = GRAPHICS_UI_TOP_OF_PREVIOUS;
	public static final int GUI_BOP = GRAPHICS_UI_BOTTOM_OF_PREVIOUS;
	public static final int GUI_BOPWM = GRAPHICS_UI_BOTTOM_OF_PREVIOUS_WITH_MARGIN;

	public static int GRAPHICS_UI_COLOR_BACKGROUND = WHITE;
	public static int GRAPHICS_UI_COLOR_POPUP_BACKGROUND = 0xF6F6;
	public static int GRAPHICS_UI_COLOR_TAB_UNSELECTED = VLIGHT_GREY;
	public static int GRAPHICS_UI_COLOR_HIGHLIGHTED = RED;
	public static int GRAPHICS_UI_COLOR_PRESSED = F_GREY;
	public static int GRAPHICS_UI_COLOR_RELEASED = VLIGHT_GREY;
	public static int GRAPHICS_UI_COLOR_GAUGE = 0x7C7C;
	public static int GRAPHICS_UI_COLOR_SELECTED = LIGHT_BLUE;

	public static int GRAPHICS_UI_POPUP_MARGIN = 5;
	public static int GRAPHICS_UI_POPUP_PADDING = 3;
	public static int GRAPHICS_UI_POPUP_TITLE_FONT = FONT_SMALL;
	public static boolean GRAPHICS_UI_POPUP_TITLE_FONT_IS_BOLD = FONT_BOLD;
	public static int GRAPHICS_UI_POPUP_MESSAGE_FONT = FONT_SMALL;
	public static boolean GRAPHICS_UI_POPUP_MESSAGE_FONT_IS_BOLD = FONT_PLAIN;

	public static int GRAPHICS_UI_TAB_FONT = FONT_MEDIUM;
	public static boolean GRAPHICS_UI_TAB_FONT_IS_BOLD = true;
	public static int GRAPHICS_UI_TAB_TOP_MARGIN = 4;
	public static int GRAPHICS_UI_TAB_LEFT_MARGIN = 5;

	public static int GRAPHICS_UI_ELEMENT_FONT = FONT_MEDIUM;
	public static boolean GRAPHICS_UI_ELEMENT_FONT_IS_BOLD = true;

	public static int GRAPHICS_UI_ELEMENT_TOP_MARGIN = 5;
	public static int GRAPHICS_UI_ELEMENT_LEFT_MARGIN = 6;
	
	public static int GRAPHICS_UI_SLIDER_KNOB_SIZE = 10;
	
	public static int GRAPHICS_UI_SCROLLER_ARROW_SIZE = 8;

	public static int GUI_HEADER = -1;
	public static int GUI_MAIN = 0;
	public static int GUI_FOOTER = 1;

}
