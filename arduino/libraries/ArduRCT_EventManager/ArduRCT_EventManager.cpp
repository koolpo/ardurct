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

void ArduRCT_EventManager::registerRTC(ArduRCT_RealTimeClock *rtc) {
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

void ArduRCT_EventManager::registerEncoder(ArduRCT_Encoder *anEncoder) {
	if (_encoder == 0) _encoder = anEncoder;
	else {
		ArduRCT_Encoder *nEncoder = _encoder;
		while (nEncoder->getNext() != 0) nEncoder = nEncoder->getNext();
		nEncoder->setNext(anEncoder);
	}
}

void ArduRCT_EventManager::registerAnalog(ArduRCT_Analog *anAnalog) {
	if (_analog == 0) _analog = anAnalog;
	else {
		ArduRCT_Analog *nAnalog = _analog;
		while (nAnalog->getNext() != 0) nAnalog = nAnalog->getNext();
		nAnalog->setNext(anAnalog);
	}
}

void ArduRCT_EventManager::registerTouchPanel(ArduRCT_TouchPanel *touchPanel) {
	_touchPanel = touchPanel;
}

ArduRCT_TouchPanel *ArduRCT_EventManager::getRegisteredTouchPanel() {
    return _touchPanel;
}

void ArduRCT_EventManager::registerEventHandler(ArduRCT_EventHandler *handler) {
    handler->setNext(0);
	if (_handler == 0) _handler = handler;
	else {
		ArduRCT_EventHandler *nHandler = _handler;
		while (nHandler->getNext() != 0) nHandler = nHandler->getNext();
		nHandler->setNext(handler);
	}
}

void ArduRCT_EventManager::manageEvents() {
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
        delay(1);
    }

    // update the time and generate corresponding events
    delay(5);
    if (_rtc != 0) _updateTime();

    // look after the switches
    ArduRCT_Switch *aSwitch = _switch;
    while (aSwitch != 0) {
        aSwitch->updateState();
        if (aSwitch->isPressed()) processEvent(EVENT_SWITCH_PRESSED, aSwitch->getPin());
        else if (aSwitch->isRepeating()) processEvent(EVENT_SWITCH_REPEATING, aSwitch->getPin());
        else if (aSwitch->isReleased()) processEvent(EVENT_SWITCH_RELEASED, aSwitch->getPin());
        aSwitch = aSwitch->getNext();
    }

    // look after the touchPanel
    if (_touchPanel != 0) {
        _touchPanel->update();
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
        if (_touchPanel->isCalibrating()) {
            if (_touchPanel->getCalibrationCrossNumber()) {
                uint8_t crossNumber = _touchPanel->getCalibrationCrossNumber();
                uint16_t crossX, crossY;
                _touchPanel->getCalibrationCrossXY(crossNumber, &crossX, &crossY);
                processEvent(EVENT_TOUCHPANEL_CALIBRATION, crossNumber, crossX, crossY);
            }
        } else {
#endif
            uint16_t x = _touchPanel->getPenX();
            uint16_t y = _touchPanel->getPenY();
            uint8_t z = _touchPanel->getPenZ();
            if (_touchPanel->isPenPressed()) {
                if (_touchPanel->isPenDragged()) processEvent(EVENT_TOUCHPANEL_DRAGGED, z, x, y);
                else if (!_touchPanel->isPenPressedMotionless()) processEvent(EVENT_TOUCHPANEL_PRESSED, z, x, y);
            } else if (_touchPanel->isPenReleased()) processEvent(EVENT_TOUCHPANEL_RELEASED, 0, 0, 0);
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
        }
#endif
    }
    
    // look after the analogs
    ArduRCT_Analog *anAnalog = _analog;
    while (anAnalog != 0) {
        uint16_t value = anAnalog->updateValue();
        // change can only been read once
        int16_t change = anAnalog->getChange();
        // send an event if the value changed
        if (change != 0) processEvent(change > 0 ? EVENT_ANALOG_INCREASE : EVENT_ANALOG_DECREASE, anAnalog->getPin(), value, change);
        anAnalog = anAnalog->getNext();
    }
    
    // look after the encoders
    ArduRCT_Encoder *anEncoder = _encoder;
    while (anEncoder != 0) {
        int16_t value = anEncoder->updateValue();
        // change can only been read once
        int16_t change = anEncoder->getChange();
        // send an event if the value changed
        if (change != 0) processEvent(change > 0 ? EVENT_ENCODER_INCREASE : EVENT_ENCODER_DECREASE, anEncoder->getPinA(), value, change);
        anEncoder = anEncoder->getNext();
    }

}

int8_t ArduRCT_EventManager::processEvent(uint8_t type, uint8_t value) {
    int8_t processStatus = EVENT_HANDLING_VOID;
    ArduRCT_EventHandler *handler = _handler;
    while (handler != 0) {
        processStatus = handler->handle(type, value);
        if (processStatus == EVENT_HANDLING_VOID) processStatus = handler->handle(type);
        if (processStatus == EVENT_HANDLING_EXIT) return EVENT_HANDLING_EXIT;
        handler = handler->getNext();
    }
    return processStatus;
}


int8_t ArduRCT_EventManager::processEvent(uint8_t type, uint8_t value, uint16_t x, uint16_t y) {
    int8_t processStatus = EVENT_HANDLING_VOID;
    ArduRCT_EventHandler *handler = _handler;
    while (handler != 0) {
        processStatus = handler->handle(type, value, x, y);
        if (processStatus == EVENT_HANDLING_VOID) processStatus = handler->handle(type, value);
        if (processStatus == EVENT_HANDLING_VOID) processStatus = handler->handle(type);
        if (processStatus == EVENT_HANDLING_EXIT) return EVENT_HANDLING_EXIT;
        handler = handler->getNext();
    }
    return processStatus;
}

void ArduRCT_EventManager::_updateTime() {
	if (_rtc->updateTime()) {
		processEvent(EVENT_TIME_SECOND, _rtc->getSecond());
		if (_rtc->isAlarmStartingToRing()) processEvent(EVENT_TIME_ALARM, _rtc->getDayOfWeek());
		if (_rtc->getSecond() == 0) {
			processEvent(EVENT_TIME_MINUTE, _rtc->getMinute());
			if (_rtc->getMinute() == 0) {
				processEvent(EVENT_TIME_MINUTE, _rtc->getHour());
				if (_rtc->getHour() == 0) {
					processEvent(EVENT_TIME_DAY, _rtc->getDay());
					if (_rtc->getDay() == 1) {
						processEvent(EVENT_TIME_MONTH, _rtc->getMonth());
						if (_rtc->getMonth() == 1) processEvent(EVENT_TIME_YEAR, _rtc->getYear());
					}
				}
			}
		}
	}		
}
