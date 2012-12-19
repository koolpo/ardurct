/*
 * EventManager - Manages events in a coordinated way
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "ArduRCT_EventManager.h"

ArduRCT_EventManager::ArduRCT_EventManager() {
}

ArduRCT_EventManager::ArduRCT_EventManager(ArduRCT_RealTimeClock *rtc) {
	_rtc = rtc;
}

void ArduRCT_EventManager::setRTC(ArduRCT_RealTimeClock *rtc) {
	_rtc = rtc;
}

ArduRCT_RealTimeClock *ArduRCT_EventManager::getRTC() {
	return _rtc;
}

void ArduRCT_EventManager::registerSwitch(ArduRCT_Switch *aSwitch) {
	if (_switch == 0) _switch = aSwitch;
	else {
		ArduRCT_Switch *nSwitch = _switch;
		while (nSwitch->getNext() != 0) nSwitch = nSwitch->getNext();
		nSwitch->setNext(aSwitch);
	}
}

void ArduRCT_EventManager::registerTouchPanel(ArduRCT_TouchPanel *touchPanel) {
	_touchPanel = touchPanel;
}

void ArduRCT_EventManager::registerEventHandler(ArduRCT_EventHandler *handler) {
	if (_handler == 0) _handler = handler;
	else {
		ArduRCT_EventHandler *nHandler = _handler;
		while (nHandler->getNext() != 0) nHandler = nHandler->getNext();
		nHandler->setNext(handler);
	}
}

void ArduRCT_EventManager::update() {
	// check if we have to run or not
	if ((int32_t)(millis() - _nextUpdate) < 0) return;
	_nextUpdate = millis() + EVENT_MANAGER_CYCLE;

	// update the tick counter
	_tick ++;
	// and the handlers attached to it
	ArduRCT_EventHandler *handler = _handler;
	while (handler != 0) {
		handler->handle(EVENT_SYSTEM_TICK, _tick);
		handler = handler->getNext();
	}

	// update the time and generate corresponding events
	if (_rtc != 0) _updateTime();
	
	// look after the switches
	ArduRCT_Switch *aSwitch = _switch;
	while (aSwitch != 0) {
		aSwitch->updateState();
		if (aSwitch->isPressed()) addEvent(EVENT_SWITCH_PRESSED, aSwitch->getPin());
		else if (aSwitch->isRepeating()) addEvent(EVENT_SWITCH_REPEATING, aSwitch->getPin());
		else if (aSwitch->isReleased()) addEvent(EVENT_SWITCH_RELEASED, aSwitch->getPin());
		aSwitch = aSwitch->getNext();
	}
	
	// look after the touchPanel
	if (_touchPanel != 0) {
		_touchPanel->update();
		uint16_t x = _touchPanel->getPenX();
		uint16_t y = _touchPanel->getPenY();
		uint8_t z = _touchPanel->getPenZ();
		if (_touchPanel->isPenPressed()) {
			if (_touchPanel->isPenDragged()) addEvent(EVENT_TOUCHPANEL_DRAGGED, z, x, y);
			else if (!_touchPanel->isPenPressedMotionless())  addEvent(EVENT_TOUCHPANEL_PRESSED, z, x, y);
		} else if (_touchPanel->isPenReleased()) addEvent(EVENT_TOUCHPANEL_RELEASED, 0, 0, 0);
	}
}

bool ArduRCT_EventManager::addEvent(uint8_t type, uint8_t value) {
	return addEvent(type, value, EVENT_ANY_VALUE, EVENT_ANY_VALUE);
}


bool ArduRCT_EventManager::addEvent(uint8_t type, uint8_t value, uint16_t x, uint16_t y) {
	boolean atLeastOneHandlerHasRun = false;
	ArduRCT_EventHandler *handler = _handler;
	while (handler != 0) {
		if (handler->handle(type, value, x, y)) atLeastOneHandlerHasRun = true;
		else if (handler->handle(type, value)) atLeastOneHandlerHasRun = true;
		else if (handler->handle(type)) atLeastOneHandlerHasRun = true;
		handler = handler->getNext();
	}
	return atLeastOneHandlerHasRun;
}

void ArduRCT_EventManager::_updateTime() {
	if (_rtc->updateTime()) {
		addEvent(EVENT_TIME_SECOND, _rtc->getSecond());
		if (_rtc->isAlarmStartingToRing()) addEvent(EVENT_TIME_ALARM, _rtc->getDayOfWeek());
		if (_rtc->getSecond() == 0) {
			addEvent(EVENT_TIME_MINUTE, _rtc->getMinute());
			if (_rtc->getMinute() == 0) {
				addEvent(EVENT_TIME_MINUTE, _rtc->getHour());
				if (_rtc->getHour() == 0) {
					addEvent(EVENT_TIME_DAY, _rtc->getDay());
					if (_rtc->getDay() == 1) {
						addEvent(EVENT_TIME_MONTH, _rtc->getMonth());
						if (_rtc->getMonth() == 1) addEvent(EVENT_TIME_YEAR, _rtc->getYear());
					}
				}
			}
		}
	}		
}
