package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIScreen implements IGraphicsDefines {

	int _x;
	int _y;
	int _width;
	int _height;
	int _topY;
	
	ArduRCT_GraphicsUIElement _headerElement;
	ArduRCT_GraphicsUIElement _footerElement;
	ArduRCT_GraphicsUIElement _firstElement;
	ArduRCT_GraphicsUIElement _currentElement;
	ArduRCT_GraphicsUIElement _selectedElement;

	ArduRCT_GraphicsUIScreen(int x, int y, int width, int height) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_topY = 0;
	}
	
	public void addElement(ArduRCT_GraphicsUIElement element, int x, int y, int width, int height) {
		if (_firstElement == null) _firstElement = element;
		else {
			ArduRCT_GraphicsUIElement next = _firstElement;
			while (next.getNext() != null) next = next.getNext();
			next.setNext(element);
		}
		
	}
	
	public void setFooter(ArduRCT_GraphicsUIElement footer) {
		_footerElement = footer;
	}

	public void setHeader(ArduRCT_GraphicsUIElement header) {
		_headerElement = header;
	}
	
	public void draw(ArduRCT_Graphics graphics) {
		graphics.fillRectangle(_x, _y, _width, _height, graphics.getBackgroundColor());
		if (_headerElement != null) _headerElement.draw(graphics, GRAPHICS_UI_STATE_RELEASED);
	}
	
	public boolean process(int actionId, int x, int y) {
		if (actionId == GRAPHICS_UI_ACTION_TOUCH) {
			
		} else if (actionId == GRAPHICS_UI_ACTION_UP) {
			
		}
		return false;
	}
}
