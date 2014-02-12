package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUITab extends ArduRCT_GraphicsUIOption 
implements IGraphicsDefines {

	public ArduRCT_GraphicsUITab(int id, int[] label, IUIActionCallback actionHandler, int group) {
		_init(id, group, label, null, actionHandler);
	}
	
	public ArduRCT_GraphicsUITab(int id, IUIDrawCallback drawHandler, IUIActionCallback actionHandler, int group) {
		_init(id, group, null, drawHandler, actionHandler);
	}

	public void autoSize(ArduRCT_Graphics graphics) {
		if (_text == null) return; 
		int fontSize = getFontSize(_text);
		height = graphics.getFontHeight(fontSize) + GRAPHICS_UI_TAB_TOP_MARGIN * 2;
		width =  graphics.getStringWidth(_text, fontSize) + GRAPHICS_UI_TAB_LEFT_MARGIN * 2;
		if ((getPrevious() == null) || (getPrevious().getGroup() != _group)) width += GRAPHICS_UI_TAB_LEFT_MARGIN;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		int bgColor = graphics.getBackgroundColor();
		int color = GRAPHICS_UI_COLOR_RELEASED;
		if (_value == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_BACKGROUND;
		if (_state == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_PRESSED;
		graphics.setBackgroundColor(color);
		int xStart = 0;
		if ((getPrevious() == null) || (getPrevious().getGroup() != _group)) xStart = GRAPHICS_UI_TAB_LEFT_MARGIN;
		graphics.fillRectangle(xStart+uiX+x, uiY+y+1, width-xStart, height-1, color);
		graphics.drawRectangle(xStart+uiX+x, uiY+y+1, width-xStart, height-1, 
				_state == GRAPHICS_UI_HIGHLIGHTED ? GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK, 1);	
		int lineY = uiY+y+height-1;
		if (xStart != 0) graphics.drawLine(uiX+x, lineY, uiX+x+xStart, lineY, BLACK, 1);
		// for the last tab, go to the end of the ui
		if ((getNext() == null) || (getNext().getGroup() != _group)) 
			graphics.drawLine(uiX+x+width, lineY, uiX+x+width+uiWidth, lineY, BLACK, 1);
		if (_value == GRAPHICS_UI_SELECTED) 
			graphics.drawLine(uiX+x+xStart+1, lineY, uiX+x+width-2, lineY, WHITE, 1);
		if (_text != null) {
			int fontSize = getFontSize(_text);
			graphics.drawString(_text, uiX+x+_textX+xStart/2, uiY+y+_textY+1, BLACK, fontSize, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
		} else if (_drawHandler != null) {
			 _drawHandler.draw(_id, _state, _value, x+uiX, y+uiY, width, height);
		}
		graphics.setBackgroundColor(bgColor);
	}


}
