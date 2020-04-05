#include "ExtTextDisplay.h"

ExtTextDisplay::ExtTextDisplay(TextDisplay& d) {
	display = &d;
	rows = display->getHeight();
	cols = display->getWidth();
	extLine = new DFXTextLine[rows];
	blinkParams = new LineBlinkParams[rows];
	LineBlinkParams* bl;
	for (byte i = 0; i < rows; i++) {
		bl = &(blinkParams[i]);
		bl->mask = display->getSpacesRow();
		bl->enabled = false;
		bl->phase = true;
		bl->dutyCycle = 50;
		bl->period = DEF_BLINK_PERIOD;
		bl->duration = DEF_BLINK_DURATION;
	}
	clear();
}

void ExtTextDisplay::clearLine(byte line) {
	display->clearRow(line);
	stopDynamicFX(line);
	stopBlinking(line);
	extLine[line].text = "";
	_clearBlinkMask(line);
}

void ExtTextDisplay::reset() {
	for (byte i = 0; i < rows; i++) extLine[i].visible = true;
	clear();
}

//void ExtTextDisplay::setLineText(byte line, const char* text) {
//	if (line >= rows) return;
//	stopDynamicFX(line);
//	stopBlinking(line);
//	extLine[line].copy(text, LINE_MAX_LEN);
//	setText(line, text);
//}

 void ExtTextDisplay::putText(byte line, const char* text, byte pos,
	bool blinking, bool clearBefore) {
	if (line >= rows) return;
	extLine[line].text = text;
	extLine[line].dfxType = NONE;
	extLine[line].dfxStep = pos;
	extLine[line].dfxEnabled = false;
	display->setText(line, pos, text, clearBefore);
	if (blinking) {
		_setBlinkMask(line, text, pos);
		setBlinkParams(line, DEF_BLINK_DURATION, DEF_BLINK_PERIOD, 50);
		startBlinking(line);
	}
	else stopBlinking(line);
}

 void ExtTextDisplay::putText(byte line, String & text, byte pos,
	bool blinking, bool clearBefore) {
	//putText(line, text.c_str, pos, blinking, clearBefore);
	if (line >= rows) return;
	extLine[line].text = text;
	extLine[line].dfxType = NONE;
	extLine[line].dfxStep = pos;
	extLine[line].dfxEnabled = false;
	display->setText(line, pos, text, clearBefore);
	if (blinking) {
		_setBlinkMask(line, text, pos);
		setBlinkParams(line, DEF_BLINK_DURATION, DEF_BLINK_PERIOD, 50);
		startBlinking(line);
	}
	else stopBlinking(line);
}

void ExtTextDisplay::setDynamicFX(byte line, const char* text, DFX_type effectId) {
	if (line >= rows) return;
	//if (effectId == NONE) putText(line, text);
	//stopBlinking(line);
	inputText = text;
	_enableDisplayEffect(line, effectId);
}

 void ExtTextDisplay::setDynamicFX(byte line, String& text, DFX_type effectId) {
	if (line >= rows) return;
	//if (effectId == NONE) putText(line, text);
	//stopBlinking(line);
	inputText = text;
	_enableDisplayEffect(line, effectId);
}

void ExtTextDisplay::_enableDisplayEffect(byte line, DFX_type effectId) {
	if (line >= rows) return;
	//String buff;
	String spaces = display->getSpacesRow();
	DFXTextLine& ln = extLine[line];
	byte txLen = inputText.length();
	byte columns = display->getWidth();

	//delete &ln.text;
	ln.dfxDelay = STD_DFX_DELAY;
	ln.dfxType = effectId;

	// BOUNCE checks
	if (effectId == BOUNCE_IN) {
		if (txLen > rows) effectId = BOUNCE_OUT;
		else if (txLen == rows) effectId = NONE;
	}
	if (effectId == BOUNCE_OUT) {
		if (txLen < rows) effectId = BOUNCE_IN;
		else if (txLen == rows) effectId = NONE;
	}

	switch (effectId) {
	case NONE:
		ln.text = inputText;
		break;
	case BOUNCE_IN:
		ln.dfxStep = 0; // first char position on display
		ln.dfxAux = 1; // text direction: 0 = left, 1 = right
		//display->setText(line, ln.text);
		break;
	case BOUNCE_OUT:
		ln.dfxStep = 0; // first char of displayed extended line 
		ln.dfxAux = 0; // text direction: 0 = left, 1 = right
		//display->setText(line, ln.text);
		break;
	case LEFT_SCRL:
		ln.text = display->getRowCopy(line) + spaces;
		ln.dfxStep = 0; // first char of displayed extended line 
		break;
	case RIGHT_SCRL:
		ln.text = spaces + display->getRowCopy(line);
		ln.dfxStep = columns; // first char of displayed extended line 
		break;
	case LEFT_INS:
		ln.text = display->getRowCopy(line) + inputText;
		ln.dfxStep = 0; // first char of displayed extended line 
		break;
	case RIGHT_INS:
		ln.text = inputText + display->getRowCopy(line);
		ln.dfxStep = inputText.length(); // first char of displayed extended line 
		break;
	case LEFT_ROT:
		ln.text = (txLen > 0) ? inputText : display->getRowCopy(line);
		ln.dfxStep = txLen-1; // rightmost text character index on display
	case RIGHT_ROT:
		ln.text = (txLen > 0) ? inputText : display->getRowCopy(line);
		ln.dfxStep = 0; // leftmost text character index on display
		break;
	case SPRAY:
		ln.text = inputText; // cuts overwidth lines
		display->clearRow(line);
		ln.dfxStep = 0; // completed line characters
		break;
	default:
		ln.dfxEnabled = false;
		ln.dfxStep = 0; // chars completed
		return;
	}
	ln.dfxEnabled = true;
	//display->clearRow(line);
	DFXStep(line);
	//delete &spaces;
}

//void ExtTextDisplay::setBlinkParams(byte line, uint16_t bDuration = 0,
//	uint16_t bPeriod = 0, byte bDutyCycle = 0) {
void ExtTextDisplay::setBlinkParams(byte line, uint16_t bDuration,
	uint16_t bPeriod, byte bDutyCycle) {
	blinkParams[line].duration = bDuration;
	if (bPeriod > 0) blinkParams[line].period = bPeriod % 5000;
	if (bDutyCycle > 0) blinkParams[line].dutyCycle = bDutyCycle % 100;
}

 void ExtTextDisplay::startBlinking(byte line) {
	blinkParams[line].enabled = true;
	blinkParams[line].startTime = millis();
}

// private
void ExtTextDisplay::DFXStep(byte line) {
	if (line >= rows) return;
	DFXTextLine& ln = extLine[line];
	if (!ln.dfxEnabled) return;
	byte tLen = ln.text.length();
	byte dWidth = display->getWidth();
	byte i;
	char ch;

	switch (ln.dfxType) {
	case NONE:
		display->setText(line, ln.dfxStep, ln.text);
		ln.dfxEnabled = false;
		break;
	case BOUNCE_IN: // endless fx
		display->setText(line, ln.dfxStep, ln.text);
		if (ln.dfxAux == 0) { // text direction: left
			if (--ln.dfxStep == 0) ln.dfxAux = 1;
		}
		else { // text direction: right
			if (++ln.dfxStep == rows - tLen) ln.dfxAux = 0;
		}
		break;
	case BOUNCE_OUT: // endless fx
		display->setText(line, ln.text, (short)ln.dfxStep);
		if (ln.dfxAux == 0) { // text direction: left
			if (++ln.dfxStep == tLen - rows) ln.dfxAux = 1;
		}
		else { // text direction: right
			if (--ln.dfxStep == 0) ln.dfxAux = 0;
		}
		break;
	case LEFT_SCRL:
	case LEFT_INS:
		display->setText(line, ln.text, (short)ln.dfxStep++, true);
		if (ln.dfxStep > tLen - dWidth) ln.dfxEnabled = false;
		break;
	case RIGHT_SCRL:
	case RIGHT_INS:
		display->setText(line, ln.text, (short)ln.dfxStep--, true);
		if (ln.dfxStep > tLen) ln.dfxEnabled = false;
		break;
	case LEFT_ROT: // endless fx
		if (ln.dfxStep == tLen-1) { // rightmost text character index on display
			display->setText(line, ln.text);
			ln.dfxStep = 0;
		}
		else {
			display->scrollLeft(line);
			ch = ln.text.charAt(ln.dfxStep++);
			display->setChar(line, dWidth-1, ch);
		}
		break;
	case RIGHT_ROT: // endless fx
		if (ln.dfxStep == 0) { // leftmost text character index on display
			display->setText(line, ln.text);
			ln.dfxStep = tLen - 1;
		}
		else {
			display->scrollRight(line);
			ch = ln.text.charAt(ln.dfxStep--);
			display->setChar(line, 0, ch);
		}
		break;
	case SPRAY:
		i = (byte)(micros() & 0xff) % dWidth; // pseudo-random index
		while (1) {
			if (display->getChar(line, i) == ' ') {
				ch = ln.text.charAt(i);
				display->setChar(line, i, ch);
				ln.dfxStep++;
				break;
			}
			else if (++i == dWidth) i = 0;
		}
		if (ln.dfxStep == dWidth) {
			display->setText(line, ln.text);
			ln.dfxEnabled = false; // effect process completed
		}
		break;
	default:
		ln.dfxType = NONE;
		ln.dfxEnabled = false;
		return;
	}
}

bool ExtTextDisplay::update(uint32_t t) {
	bool changed = false;

	// DFX
	DFXTextLine* ln;
	for (byte i = 0; i < rows; i++) {
		ln = &extLine[i];
		if (ln->dfxEnabled) {
			if ((t - ln->dfxLastStepTime) >= ln->dfxDelay) {
				ln->dfxLastStepTime = t;
				DFXStep(i);
				changed = true;
			}
		}
	}

	// blink
	LineBlinkParams* bl;
	uint32_t elapsedT;
	for (byte i = 0; i < rows; i++) {
		bl = &blinkParams[i];
		elapsedT = t - bl->startTime;
		if (bl->enabled) {
			if (bl->duration > 0 && elapsedT > bl->duration) bl->enabled = false;
			else {
				bool lastState = bl->phase;
				bl->phase = (((elapsedT % (uint32_t)bl->period) * (uint32_t)100u / (uint32_t)bl->period)
					<= bl->dutyCycle);
				if (bl->phase != lastState) changed = true;
			}
		}
	}

	return changed;
}

byte ExtTextDisplay::_getVisibleChar(byte row, byte col) {
	byte ch = display->getVisibleChar(row, col);
	LineBlinkParams& bl = blinkParams[row];
	if (bl.enabled) return ((bl.mask.charAt(col) != ' ') && !bl.phase) ? ' ' : ch;
	return ch;
}

 void ExtTextDisplay::_clearBlinkMask(byte row) {
	 LineBlinkParams& lbp = blinkParams[row];
	 for (byte i = 0; i < cols; i++) lbp.mask[i] = ' ';
}

void ExtTextDisplay::_setBlinkMask(byte line, const char* text, byte pos) {
	char ch;
	_clearBlinkMask(line);
	for (byte i = pos; i < cols; i++) {
		ch = text[i-pos];
		if (ch == 0) break;
		blinkParams[line].mask[i] = (ch == ' ' ? ch : 'x');
	}
}

void ExtTextDisplay::_setBlinkMask(byte line, String& text, byte pos) {
	char ch;
	byte max = text.length() + pos;
	_clearBlinkMask(line);
	for (byte i = pos; i < cols && i < max; i++) {
		ch = text.charAt(i - pos);
		blinkParams[line].mask[i] = (ch == ' ' ? ch : 'x');
	}
}

String ExtTextDisplay::toString() {
	//return display->toString();
	int i;
	String str;
	byte wth = display->getWidth();
	for (int line = 0; line < rows; line++) {
		str += "[";
		for (i = 0; i < wth; i++) str += _getVisibleChar(line, i);
		str += "]";
	}
	return str;
}

//std::ostream& operator<<(std::ostream& os, ExtTextDisplay& d) {
//	return os << d.toString();
//}