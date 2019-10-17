/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* ADVANCED DISPLAY LIBRARY
* ---
* Advanced display buffers with special effects functions.
* Extension of Display class.
* ...
*/

#include "DisplayWithFX.h"
#include "TimerTask.h"
#include "string.h" // TODO ?.. Use c++ strings instead of old C library

DisplayWithFX::DisplayWithFX(Display basicDisplay) {
	// effects timers 
	dLine[0].fx_timer = new TimerTask(this->_stepFx0, (uint32_t)STD_DFX_DELAY, "R0 displFX");
	dLine[1].fx_timer = new TimerTask(this->_stepFx1, (uint32_t)STD_DFX_DELAY, "R1 displFX");
	// blink timers
	dLine[0].blink_timer = new TimerTask(this->_dBlink0, (uint32_t)STD_DFX_DELAY, "R0 displBlink");
	dLine[1].blink_timer = new TimerTask(this->_dBlink1, (uint32_t)STD_DFX_DELAY, "R1 displBlink");

	resetLineBlinkArea(0);
	resetLineBlinkArea(1);
	baseDisplay = &basicDisplay;
}

DisplayWithFX::~DisplayWithFX() {
	for (int i = 0; i < 2; i++) {
		dLine[i].fx_timer->disable();
		dLine[i].blink_timer->disable();
		delete dLine[i].fx_timer;
		delete dLine[i].blink_timer;
	}
	delete[] dLine;
}

void DisplayWithFX::setDisplayText(byte line, const char *text) {
	disableDisplayEffect(line);
	stopDisplayBlink(line);
	strncpy(dLine[line].msg, text, LINE_MAX_LEN);
	dLine[line].msgLen = strlen(text);
	dLine[line].chOffset = 0;
}

void DisplayWithFX::setDisplayText(byte line, const char *text, FX effectId) {
	setDisplayText(line, text);
	enableDisplayEffect(line, effectId);
}

void DisplayWithFX::enableDisplayEffect(byte line, FX effectId) {
	byte fxv = 0; // effect variable
	short int off = 0; // string offset
	uint16_t fxd = STD_DFX_DELAY; // effect delay

	line &= 1;
	if (effectId == BOUNCE && dLine[line].msgLen <= Display::DISPLAY_COLS)
		effectId = OFF;
	disableDisplayEffect(line);
	switch (effectId) {
	case OFF:
		off = 0;
		break;
	case BOUNCE:
		off = 0;
		fxd = 100;
		fxv = 0; // text direction: 0-3 = left; 4-7 = right
		break;
	case LEFT_SCRL:
		char buff[LINE_MAX_LEN + Display::DISPLAY_COLS + 1];
		// concatenates string on display with msg
		strcpy(buff, dLine[line].msg);
		strcpy(dLine[line].msg, baseDisplay->getRow(line));
		strcat(dLine[line].msg, buff);
		dLine[line].msgLen += Display::DISPLAY_COLS;
		strcpy(buff, dLine[line].blinkStr);
		strcpy(dLine[line].blinkStr, baseDisplay->getRow(line));
		strcat(dLine[line].blinkStr, buff);
		off = 0;
		fxd = 50;
		break;
	case RIGHT_SCRL:
		// concatenates msg with string on display
		strncat(dLine[line].msg, baseDisplay->getRow(line), LINE_MAX_LEN);
		off = -((short int)dLine[line].msgLen);
		dLine[line].msgLen += Display::DISPLAY_COLS;
		strncat(dLine[line].blinkStr, baseDisplay->getRow(line), LINE_MAX_LEN);
		fxd = 50;
		break;
	default:
		return;
	}
	dLine[line].displayEffect = effectId;
	dLine[line].fxVar = fxv;
	dLine[line].chOffset = off;
	if (effectId == OFF) return;
	dLine[line].fx_timer->setPeriod(fxd, true);
	dLine[line].fx_timer->enable();
}

inline void DisplayWithFX::disableDisplayEffect(byte line) {
	dLine[line & 1].fx_timer->disable();
}

inline void DisplayWithFX::setDisplayEffectPeriod(byte line, uint16_t period) {
	dLine[line & 1].fx_timer->period = period;
}

// private
void DisplayWithFX::_stepDisplayEffect(byte line) {
	byte fxv; // effect variable (optional)
	bool fxEnd; // effect end reached
	byte len; // msg length
	short int off;

	fxv = dLine[line].fxVar;
	off = dLine[line].chOffset;
	fxEnd = false;
	len = dLine[line].msgLen;
	switch (dLine[line].displayEffect) {
	case LEFT_SCRL:
		baseDisplay->scrollLeft(line);
		if (--off < Display::DISPLAY_COLS - len) fxEnd = true;
		updateDisplayLine(line);
		break;
	case RIGHT_SCRL:
		baseDisplay->scrollRight(line);
		if (++off > 0) fxEnd = true;
		updateDisplayLine(line);
		break;
	case BOUNCE:
		if (fxv < 4) {
			if (fxv == 0 && off > -(short int)(len - Display::DISPLAY_COLS)) off--;
			else ++fxv;
		}
		else { // fxv >= 4
			if (fxv == 4 && off < 0) off++;
			else if (++fxv > 7) fxv = 0;
		}
		updateDisplayLine(line);
		break;
	}
	dLine[line].fxVar = fxv;
	dLine[line].chOffset = off;
	if (fxEnd) disableDisplayEffect(line);
}

void DisplayWithFX::startDisplayBlink(byte line, uint16_t bDelay, uint16_t bPeriod) {
	line &= 1;
	dLine[line].blink_timer->setPeriod(bDelay, true);
	dLine[line].blink_timer->enable();
	dLine[line].blinkState = false;
	dLine[line].blinkActivePeriod = bPeriod;
	dLine[line].blinkEnableTime = millis();
}

void DisplayWithFX::startDisplayBlink(byte line, uint16_t bDelay) {
	startDisplayBlink(line, bDelay, 0);
}

void DisplayWithFX::startDisplayBlink(byte line) {
	startDisplayBlink(line, STD_BLK_DELAY, 0);
}

void DisplayWithFX::stopDisplayBlink(byte line) {
	line &= 1;
	dLine[line].blink_timer->disable();
	dLine[line].blinkState = true;
	updateDisplayLine(line);
}

void DisplayWithFX::setDisplayBlink(byte line, bool enabled) {
	if (enabled) startDisplayBlink(line);
	else stopDisplayBlink(line);
}

void DisplayWithFX::setDisplayBlinkInterval(byte line, byte firstCh, byte lastCh) {
	dLine[line & 1].blinkStartCol = firstCh;
	dLine[line & 1].blinkEndCol = lastCh;
}

void DisplayWithFX::resetLineBlinkArea(byte line) {
	setDisplayBlinkInterval(line, 0, Display::DISPLAY_COLS - 1);
}

void DisplayWithFX::setDisplayBlinkPeriod(byte line, uint16_t bPeriod) {
	dLine[line & 1].blinkActivePeriod = bPeriod;
}

void DisplayWithFX::updateDisplayLine(byte line) {
	char *st;
	DisplayLineText &dl = dLine[line];
	if (dl.blink_timer->enabled && dl.blinkState) st = dl.blinkStr;
	else st = dl.msg;
	baseDisplay->setText(line, st, dl.chOffset);
}

void DisplayWithFX::_stepFx0(uint32_t t) {
	_stepDisplayEffect(0);
}

void DisplayWithFX::_stepFx1(uint32_t t) {
	_stepDisplayEffect(1);
}

void DisplayWithFX::_dBlink0(uint32_t t) {
	_dBlinkSwitch(0);
}

void DisplayWithFX::_dBlink1(uint32_t t) {
	_dBlinkSwitch(1);
}

void DisplayWithFX::_dBlinkSwitch(byte line) {
	bool st;
	uint32_t dt;

	st = dLine[line].blinkState;
	st = !st;
	if (dLine[line].blinkActivePeriod > 0) { // limited time blink
		dt = millis() - dLine[line].blinkEnableTime;
		if (dt > dLine[line].blinkActivePeriod) { // blink period is over
			st = true;
			stopDisplayBlink(line);
		}
	}
	dLine[line].blinkState = st;
	updateDisplayLine(line);
}

void DisplayWithFX::_updBlinkedTxt(byte line, byte startCh, byte endCh) {
	char *oc, *dc;
	byte i, l;

	l = dLine[line].msgLen;
	if (startCh >= l || endCh < startCh) return;
	if (endCh >= l) endCh = l - 1;
	oc = dLine[line].msg;
	dc = dLine[line].blinkStr;
	strcpy(dc, oc);
	for (i = startCh; i <= endCh; i++) dc[i] = ' ';
}
