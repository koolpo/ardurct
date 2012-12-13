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

	public static final int SCREEN_ARC_ALL =(GRAPHICS_ARC_N+GRAPHICS_ARC_S);

	public static final int GRAPHICS_CORNER_NW = 0;
	public static final int GRAPHICS_CORNER_NE = 1;
	public static final int GRAPHICS_CORNER_SE = 2;
	public static final int GRAPHICS_CORNER_SW = 3;
	
	public static final int GRAPHICS_STYLE_SIMPLE = 0;
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
	
	public static final int UI_ERROR = -1;
	
	public static final int UI_AUTO_SIZE = 0;
	public static final int UI_SELECTED = 1;
	public static final int UI_UNSELECTED = 0;
	public static final int UI_REFRESH = 2;
	public static final int UI_NO_GROUP = -1;
	
	public static final int UI_SAME_AS = 0x8000;
	public static final int UI_WITH_MARGIN = 0x1000;
	public static final int UI_LEFT_OF = 0x4000;
	public static final int UI_RIGHT_OF = 0x2000;
	public static final int UI_RIGHT_OF_WITH_MARGIN = 0x3000;
	public static final int UI_TOP_OF = 0x4000;
	public static final int UI_BOTTOM_OF = 0x2000;
	public static final int UI_BOTTOM_OF_WITH_MARGIN = 0x3000;
	public static final int UI_PLACE_MODIFIER_MASK = 0xF000;
		
	public static final int CONFIGURATION_UI_LOOP_LENGTH = 20;
	public static final int CONFIGURATION_UI_DEBOUNCE_STEPS = 6;

	public static final int CONFIGURATION_UI_MAX_ELEMENTS = 30;
	public static final int CONFIGURATION_UI_MAX_TABS = 6;
	
	public static final int UI_DRAW_CALLBACK_TAB_ID = 0xFF;


	public static final int EEND = 0x1000-1;
}
