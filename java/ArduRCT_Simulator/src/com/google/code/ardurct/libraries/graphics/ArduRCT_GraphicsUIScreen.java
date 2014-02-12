package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIScreen implements IGraphicsDefines {

	int _x;
	int _y;
	int _width;
	int _height;
	int _topX;
	int _topY;
	int _position;
	int _backgroundColor;
	int _headerHeight;
	int _footerHeight;
	
	ArduRCT_GraphicsUI _graphics;
	
	ArduRCT_GraphicsUIElement _firstHeaderElement;
	ArduRCT_GraphicsUIElement _lastHeaderElement;
	ArduRCT_GraphicsUIElement _firstFooterElement;
	ArduRCT_GraphicsUIElement _firstElement;
	ArduRCT_GraphicsUIElement _lastElement;
	ArduRCT_GraphicsUIElement _currentElement;
	ArduRCT_GraphicsUIElement _selectedElement;
	ArduRCT_GraphicsUIElement _firstDisplayedElement;
	ArduRCT_GraphicsUIElement _lastDisplayedElement;

	private int _currentElementSection;
	private boolean _enabled;

	ArduRCT_GraphicsUIScreen _previousScreen;
	
	public ArduRCT_GraphicsUIScreen() {
		this(0, 0, 0, 0);
	}
	
	public ArduRCT_GraphicsUIScreen(int x, int y, int width, int height) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_topX = 0;
		_topY = 0;
		_enabled = false;
		_previousScreen = null;
		_position = GRAPHICS_POSITION_NW;
		_backgroundColor = GRAPHICS_UI_COLOR_BACKGROUND;
		_headerHeight = 0;
		_footerHeight = 0;
	}

	public void setGraphics(ArduRCT_GraphicsUI graphics) {
		_graphics = graphics;		
	}
	
	public void setBackgroundColor(int color) {
		_backgroundColor = color;
	}

	public void setPreviousScreen(ArduRCT_GraphicsUIScreen previous) {
		_previousScreen = previous;
	}

	public ArduRCT_GraphicsUIScreen getPreviousScreen() {
		return _previousScreen;
	}

	public void addElement(ArduRCT_GraphicsUIElement element, int x, int y, int width, int height) {
		addElement(element, x, y, width, height, GUI_MAIN);
	}
	
	public void addElement(ArduRCT_GraphicsUIElement element) {
		addElement(element, element.x, element.y, element.width, element.height, element.section);
	}

	public void addElement(ArduRCT_GraphicsUIElement elt, int x, int y, int width, int height, int section) {
		if (_width == 0) _width = _graphics.getWidth();
		if (_height == 0) _height = _graphics.getHeight();
		if (section == GUI_HEADER) {
			if (_firstHeaderElement == null) _firstHeaderElement = elt;
			else _lastHeaderElement.setNext(elt);
			_lastHeaderElement = elt;
		} else if (section == GUI_FOOTER) {
			if (_firstFooterElement == null) _firstFooterElement = elt;
			else {
				ArduRCT_GraphicsUIElement next = _firstFooterElement;
				while (next.getNext() != null) next = next.getNext();
				next.setNext(elt);
			}
		} else {
			if (_firstElement == null) _firstElement = elt;
			else _lastElement.setNext(elt);
			_lastElement = elt;
		}
		elt.section = section;
		ArduRCT_GraphicsUIElement previous = elt.getPrevious();
		if ((width == GRAPHICS_UI_AUTO_SIZE) || (height == GRAPHICS_UI_AUTO_SIZE)) elt.autoSize(_graphics);
		if (width == GRAPHICS_UI_WIDTH) elt.width = _graphics.getWidth();
		else if (width != GRAPHICS_UI_AUTO_SIZE) elt.width = width;
		if (height == GRAPHICS_UI_HEIGHT) elt.height = _graphics.getHeight();
		else if (height != GRAPHICS_UI_AUTO_SIZE) elt.height = height;
		if (previous != null) {
			if (width == GRAPHICS_UI_SAME_AS_PREVIOUS) elt.width = previous.width;
			if (height == GRAPHICS_UI_SAME_AS_PREVIOUS) elt.height = previous.height;
		}
		elt.x = x;
		elt.y = y;
		if (previous != null) {
			if (x == GRAPHICS_UI_SAME_AS_PREVIOUS) elt.x = previous.x;
			else if (x == GRAPHICS_UI_LEFT_OF_PREVIOUS) elt.x = previous.x - elt.width;
			else if (x == GRAPHICS_UI_RIGHT_OF_PREVIOUS) elt.x = previous.x + previous.width - 1;	
			else if (x == GRAPHICS_UI_RIGHT_OF_PREVIOUS_WITH_MARGIN) elt.x = previous.x + previous.width + GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
			if (y == GRAPHICS_UI_SAME_AS_PREVIOUS) elt.y = previous.y;
			else if (y == GRAPHICS_UI_TOP_OF_PREVIOUS) elt.y = previous.y - elt.height;
			else if (y == GRAPHICS_UI_BOTTOM_OF_PREVIOUS) elt.y = previous.y + previous.height - 1;	
			else if (y == GRAPHICS_UI_BOTTOM_OF_PREVIOUS_WITH_MARGIN) elt.y = previous.y + previous.height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
		}
		elt.placeLabel(_graphics);
	}

	public void deactivate() {
		_enabled = false;
	}
	
	public void activate() {
		_enabled = true;
		_reset();
		_draw();
	}
	
	public void pack(int position) {
	    int width = 0;
	    int height = 0;
	    _width = _graphics.getWidth();
	    _height = _graphics.getHeight();
	    ArduRCT_GraphicsUIElement elt = _firstElement;
	    if (elt == null) return;
	    boolean hasVerticalScroll = false;
	    while (elt != null) {
	        if (elt.x + elt.width > width) width = elt.x+elt.width;
	        if (elt.y + elt.height < _height) height = elt.y + elt.height;
	        else hasVerticalScroll = true;
	        elt = elt.getNext();
	    }
	    if (width+GRAPHICS_UI_ELEMENT_LEFT_MARGIN < _width) _width = width+GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
	    if (height < _height) _height = height;
	    // take care of scrolling for tall screens		
	    if (hasVerticalScroll && (_width + 10 < _graphics.getWidth())) _width += 10;
	    // adjust the position
	    if (position == GRAPHICS_POSITION_NW) return;
	    _position = position;
		if ((position == GRAPHICS_POSITION_NE) || (position == GRAPHICS_POSITION_SE)) 
			_x = _graphics.getWidth()-_width;
		if ((position == GRAPHICS_POSITION_SE) || (position == GRAPHICS_POSITION_SW) || (position == GRAPHICS_POSITION_S)) 
			_y = _graphics.getHeight()-_height;
		if ((position == GRAPHICS_POSITION_N) || (position == GRAPHICS_POSITION_S) || (position == GRAPHICS_POSITION_CENTER)) 
			_x = (_graphics.getWidth()-_width)/2;
		if (position == GRAPHICS_POSITION_CENTER) {
			_y = (_graphics.getHeight()-_height)/2;
			// when centered, leave some space at the bottom during packing
			if (_height < _graphics.getHeight()) _height += GRAPHICS_UI_ELEMENT_TOP_MARGIN;
		}
	}
	
	public boolean process(int actionId, int x, int y) {
		if (actionId == GRAPHICS_UI_ACTION_TOUCH) {
			if (_selectedElement != null) {
				_selectedElement.release();
				_selectedElement.escape();
				_drawElement(_selectedElement);
			}
			// find an item at these coordinates
			_selectedElement = _findElementAt(x, y);
			if (_selectedElement == null) return _processOutOfScreenTouch(x, y);
			int touchX = x-_selectedElement.x;
			int touchY = y-_selectedElement.y;
			if (_selectedElement.section == GUI_MAIN) {
				touchX -= _topX;
				touchY -= _topY;
			} else if (_selectedElement.section == GUI_FOOTER) {
				touchY -= _height - _firstFooterElement.height;
			}
			ArduRCT_GraphicsUIElement modifiedElement = _selectedElement.touch(touchX, touchY);
			if (modifiedElement != null) _drawElement(modifiedElement);
			_drawElement(_selectedElement);
			return true;
		} else if (actionId == GRAPHICS_UI_ACTION_UNTOUCH) {
			// release the item
			if (_selectedElement == null) return false;
			_selectedElement.release();
			boolean runOnEscape = _selectedElement.escape();
			_drawElement(_selectedElement);
			if (runOnEscape) _selectedElement.run();
			_selectedElement = null;
			return true;
		} else if (actionId == GRAPHICS_UI_ACTION_DRAG) {
	        ArduRCT_GraphicsUIElement draggedElement = _findElementAt(x, y);
	        if ((_selectedElement != null) && ((draggedElement == null) || (draggedElement != _selectedElement))) {
	            _selectedElement.release();
	            _selectedElement.escape();
	            _drawElement(_selectedElement);
	        }
	        _selectedElement = draggedElement;
	        if (_selectedElement == null) return false;
	        int touchX = x-_selectedElement.x;
	        int touchY = y-_selectedElement.y;
	        if (_selectedElement.section == GUI_MAIN) {
	            touchX -= _topX;
	            touchY -= _topY;
	        } else if (_selectedElement.section == GUI_FOOTER) {
	            touchY -= _height - _firstFooterElement.height;
	        }
	        _selectedElement.touch(touchX, touchY);
	        _drawElement(_selectedElement);
	        return true;
			
		} else if ((actionId == GRAPHICS_UI_ACTION_UP) || (actionId == GRAPHICS_UI_ACTION_DOWN)) {
			if (_currentElement == null) {
				_currentElement = _findNextElement(false);
				if (_currentElement == null) return false;
				_currentElement.highlight();
				if (_adjustTopToDisplayCurrentElement()) _draw();
				else _drawElement(_currentElement);
			} else if (_selectedElement != null) {
				// we are inside a selected element
				boolean runOnIncreaseOrDecrease = false;
				int value = _selectedElement.getValue();
				if (actionId == GRAPHICS_UI_ACTION_UP) runOnIncreaseOrDecrease = _selectedElement.increase();
				else runOnIncreaseOrDecrease = _selectedElement.decrease();
				if (_selectedElement.getValue() != value) _drawElement(_selectedElement);
				if (runOnIncreaseOrDecrease) _selectedElement.run();
			} else {
				ArduRCT_GraphicsUIElement nextElement = _findNextElement(actionId == GRAPHICS_UI_ACTION_UP);
				if (nextElement != null) {
					_currentElement.escape();
					_drawElement(_currentElement);
					_currentElement = nextElement;
					_currentElement.highlight();
					if (_adjustTopToDisplayCurrentElement()) _draw();
					else _drawElement(_currentElement);
				}
			}
			return true;
		} else if (actionId == GRAPHICS_UI_ACTION_ENTER) {
			if (_currentElement == null) return false;
			_selectedElement = _currentElement;
			ArduRCT_GraphicsUIElement modifiedElement = _selectedElement.enter();
			if (modifiedElement != null) _drawElement(modifiedElement);
			_drawElement(_selectedElement);
			return true;
		} else if (actionId == GRAPHICS_UI_ACTION_MENU) {
			if (_selectedElement == null) return false;
			boolean runOnEscape = _selectedElement.escape();
			_selectedElement.highlight();
			_drawElement(_selectedElement);
			if (runOnEscape) _selectedElement.run();
			_selectedElement = null;
			return true;
		} else if (actionId == GRAPHICS_UI_ACTION_RELEASE) {
			if (_selectedElement == null) return false;
			boolean escapeAfterRelease = _selectedElement.release();
			boolean runOnEscape = false;
			if (escapeAfterRelease) {
				runOnEscape = _selectedElement.escape();
				_selectedElement.highlight();				
			}
			_drawElement(_selectedElement);
			if (escapeAfterRelease) {
				if (runOnEscape) _selectedElement.run();
				_selectedElement = null;
			}
			return true;
		}
		return false;
	}
	
	public ArduRCT_GraphicsUIElement getElement(int id) {
		ArduRCT_GraphicsUIElement elt = _firstElement;
		while (elt != null) {
			if (elt.getId() == id) return elt;
			elt = elt.getNext();
		}
		elt = _firstHeaderElement;
		while (elt != null) {
			if (elt.getId() == id) return elt;
			elt = elt.getNext();
		}
		elt = _firstFooterElement;
		while (elt != null) {
			if (elt.getId() == id) return elt;
			elt = elt.getNext();
		}
		return null;
	}
	
	public void setElementValue(int id, int value) {
		ArduRCT_GraphicsUIElement elt = getElement(id);
		if (elt == null) return;
		ArduRCT_GraphicsUIElement modifiedElement = elt.setValue(value);
		if ((modifiedElement != null) && _enabled) _drawElement(modifiedElement);
		if (_enabled) _drawElement(elt);
	}

	public int getElementValue(int id) {
		ArduRCT_GraphicsUIElement elt = getElement(id);
		if (elt == null) return 0;
		return elt.getValue();
	}

	private void _reset() {
		ArduRCT_GraphicsUIElement elt = _firstElement;
		while (elt != null) {
			elt.setState(GRAPHICS_UI_RELEASED);
			elt = elt.getNext();
		}
		elt = _firstHeaderElement;
		while (elt != null) {
			elt.setState(GRAPHICS_UI_RELEASED);
			elt = elt.getNext();
		}
		elt = _firstFooterElement;
		while (elt != null) {
			elt.setState(GRAPHICS_UI_RELEASED);
			elt = elt.getNext();
		}
		if (_firstHeaderElement != null) {
			_headerHeight = _firstHeaderElement.height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
			_topY = _headerHeight;
		} else _topY = 0;
		if (_firstFooterElement != null) _footerHeight = _firstFooterElement.height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
		_currentElement = null;
		_selectedElement = null;
		_currentElementSection = 0;
	}
	
	private void _draw() {
		if (_position == GRAPHICS_POSITION_CENTER) {
			_graphics.setBackgroundColor(_backgroundColor);
			_graphics.fillRoundedRectangle(_x, _y, _width, _height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, _backgroundColor);
			_graphics.drawRoundedRectangle(_x, _y, _width, _height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, BLACK, 1);
		} else {
			_graphics.setBackgroundColor(_backgroundColor);
			_graphics.fillRectangle(_x, _y, _width, _height, _backgroundColor);
			if ((_x != 0) || (_width != _graphics.getWidth()) || (_y != 0) || (_height != _graphics.getHeight())) 
				_graphics.drawRectangle(_x, _y, _width, _height, BLACK, 1);
		}
		ArduRCT_GraphicsUIElement elt = _firstElement;
		_firstDisplayedElement = null;
		while (elt != null) {
			if (_topY+elt.y+elt.height > _height-_footerHeight) break;
			if (elt.y+_topY >= _headerHeight) {
				if (_firstDisplayedElement == null) _firstDisplayedElement = elt;
				_lastDisplayedElement = elt;
				elt.draw(_graphics, _x + _topX, _y + _topY, _width);
			}
			elt = elt.getNext();
		}
		elt = _firstHeaderElement;
		while (elt != null) {
			elt.draw(_graphics,  _x, _y, _width);
			elt = elt.getNext();
		}
		elt = _firstFooterElement;
		while (elt != null) {
			elt.draw(_graphics, _x, _y + _height - _firstFooterElement.height, _width);
			elt = elt.getNext();
		}	
		if (_firstDisplayedElement != null) {
			if (_firstDisplayedElement != _firstElement) {
				int headerHeight = _headerHeight;
				if (headerHeight > 0) headerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
				_graphics.fillCorner(_x+_width-2, _y+headerHeight, GRAPHICS_UI_SCROLLER_ARROW_SIZE, GRAPHICS_POSITION_SW, GRAPHICS_UI_COLOR_GAUGE);
			}
			if (_lastDisplayedElement != _lastElement) { 
				int footerHeight = _footerHeight;
				if (footerHeight > 0) footerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
				_graphics.fillCorner(_x+_width-2, _y+_height-footerHeight, GRAPHICS_UI_SCROLLER_ARROW_SIZE, GRAPHICS_POSITION_NW, GRAPHICS_UI_COLOR_GAUGE);
			}
		}
		if (_currentElement == null) return;
		_drawElement(_currentElement);
	}

	private void _drawElement(ArduRCT_GraphicsUIElement elt) {
		int topX = 0;
		int topY = 0;
		if (elt.section == GUI_MAIN) {
			if (elt.y+_topY < _headerHeight) return;
			if (_topY+elt.y+elt.height > _height-_footerHeight) return;
			topX = _topX;
			topY = _topY;
		} else if (elt.section == GUI_FOOTER) {
			topY = _height - _firstFooterElement.height;
		}
		elt.draw(_graphics, _x + topX, _y + topY, _width);
	}

	private boolean _processOutOfScreenTouch(int x, int y) {
		// check if we are outside the boundaries of the screen
		if ((y < _y) || (y > _y+_height) || (x < _x) || (x > _x+_width)) {
			// close it and return to previous screen
			if (_previousScreen != null) _graphics.setGraphicsUIScreen(_previousScreen, _previousScreen.getPreviousScreen());
	        return true;		}
		// check if we have to scroll
		if (x > _width-20) {
			// check if we have to scroll up
			int headerHeight = _headerHeight;
			if (headerHeight > 0) headerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
			int footerHeight = _footerHeight;
			if (footerHeight > 0) footerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
			if ((_firstDisplayedElement != _firstElement) && (y-_y > headerHeight) && (y-_y < headerHeight+20)) {
				_currentElementSection = 0;
				_currentElement = _firstDisplayedElement.getPrevious();
				_adjustTopToDisplayCurrentElement();
				_draw();
			} else if ((_lastDisplayedElement != _lastElement) && (y-_y > _height-footerHeight-20) && (y-_y < _height-footerHeight)) {
			// check if we have to scroll down
				_currentElementSection = 0;
				_currentElement = _lastDisplayedElement.getNext();
				_adjustTopToDisplayCurrentElement();
				_draw();
			}
		}
		return true;
	}

	private ArduRCT_GraphicsUIElement _findNextEditableElementInSection(ArduRCT_GraphicsUIElement current, boolean upDirection) {
		if (current == null) return null;
		if (current.editable) return current;
		if (upDirection) {
			ArduRCT_GraphicsUIElement elt = current.getPrevious();
			while (elt != null) {
				if (elt.editable) return elt;
				elt = elt.getPrevious();
			}
			return null;
		} else {
			ArduRCT_GraphicsUIElement elt = current.getNext();
			while (elt != null) {
				if (elt.editable) return elt;
				elt = elt.getNext();
			}
			return null;
		}
	}
	
	private ArduRCT_GraphicsUIElement _findNextElement(boolean upDirection) {
		ArduRCT_GraphicsUIElement elt = null;
		if (_currentElement == null) {
			_currentElementSection = 0;
			elt = _findNextEditableElementInSection(_firstElement, upDirection);
			if (elt != null) return elt;
			_currentElementSection = -1;
			elt = _findNextEditableElementInSection(_firstHeaderElement, upDirection);
			if (elt != null) return elt;
			_currentElementSection = 1;
			return _findNextEditableElementInSection(_firstFooterElement, upDirection);
		}
		if (upDirection) {
			elt = _findNextEditableElementInSection(_currentElement.getPrevious(), true);
			if (elt != null) return elt;
			if (_currentElementSection == -1) return null;
			if (_currentElementSection == 0) {
				elt = _findNextEditableElementInSection(_lastHeaderElement, true);
				if (elt != null) _currentElementSection = -1;
				return elt;
			} else if (_currentElementSection == 1) {
				elt = _findNextEditableElementInSection(_lastElement, true);
				if (elt != null) _currentElementSection = 0;
				return elt;
			}
			return null;
		}
		elt = _findNextEditableElementInSection(_currentElement.getNext(), false);
		if (elt != null) return elt;
		if (_currentElementSection == 1) return null;
		if (_currentElementSection == -1) {
			elt = _findNextEditableElementInSection(_firstElement, false);
			if (elt != null) _currentElementSection = 0;
			return elt;
		} else if (_currentElementSection == 0) {
			elt = _findNextEditableElementInSection(_firstFooterElement, false);
			if (elt != null) _currentElementSection = 1;
			return elt;
		}
		return null;
	}
	
	private boolean _adjustTopToDisplayCurrentElement() {
		if (_currentElementSection != 0) return false;
		int adjustedTopY = _topY;
		if (_currentElement.y+_currentElement.height+_topY > _height-_footerHeight) {
			adjustedTopY = -_currentElement.y-_currentElement.height+_height-_footerHeight;
		} else if (_currentElement.y+_topY < _headerHeight) {
			adjustedTopY =  _headerHeight-_currentElement.y;
		}
		if (adjustedTopY == _topY) return false;
		_topY = adjustedTopY;
		return true;
	}
	
	private ArduRCT_GraphicsUIElement _findElementAt(int x, int y) {
		ArduRCT_GraphicsUIElement elt = _firstElement;
		while (elt != null) {
			if (_topY+elt.y+elt.height > _height-_footerHeight) break;
			if (elt.editable && (elt.y+_topY >= _headerHeight) && (y-_y > elt.y+_topY) && (y-_y < elt.y+elt.height+_topY) &&
					(x-_x > elt.x+_topX) && (x-_x < elt.x+elt.width+_topX)) return elt;
			elt = elt.getNext();
		}
		elt = _firstHeaderElement;
		while (elt != null) {
			if (y > _headerHeight) break;
			if (elt.editable && (y > elt.y) && (y < elt.y+elt.height) && (x > elt.x) && (x < elt.x+elt.width)) return elt;
			elt = elt.getNext();
		}
		elt = _firstFooterElement;
		while (elt != null) {
			if (y < _height-_footerHeight) break;
			if (elt.editable && (y+_height-_footerHeight > elt.y) && (y+_height-_footerHeight < elt.y+elt.height) && 
					(x > elt.x) && (x < elt.x+elt.width)) return elt;
			elt = elt.getNext();
		}	
		return null;
	}

}
