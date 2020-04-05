#include "Sys80b_Display.h"

//Sys80b_Display::Sys80b_Display() {
//	d = new TextDisplay(2, DISPLAY_COLS);
//	_init(*d);
//}
//
//void Sys80b_Display::reset() {
//	currentCol = 0; // current display column
//	currentRow = 0; // current display row
//	cmdBuffer.Clear();
//	cmdBuffer.put(0x0300u); // reset signal (active low) to both rows
//	_sendInitSequence();
//	for (byte i = 0; i < 2; i++) mode[i] = NORMAL;
//}
//
//void Sys80b_Display::setMode(byte row, Mode m) {
//	row &= 1;
//	mode[row] = m;
//	switch (m) {
//	case NORMAL:
//		_pushCmd(row+1, CD_NORMAL_MODE);
//		break;
//	case BLANK:
//		_pushCmd(row+1, CD_BLANK_MODE);
//		break;
//	case INVERSE:
//		_pushCmd(row+1, CD_INVRSE_MODE);
//		break;
//	}
//}
//
//String Sys80b_Display::getRowText(byte row) {
//	if (row > 1) return String("");
//	String retStr; // = display->getSpacesRow();
//	byte dWidth = display->getWidth();
//	for (byte i = 0; i < dWidth; i++) retStr += (char)_getVisibleChar(row, i);
//	return retStr;
//}
//
//String Sys80b_Display::getRowDots(byte row) {
//	if (row > 1) return String("");
//	char ch;
//	String retStr = display->getSpacesRow();
//	byte dWidth = display->getWidth();
//	for (byte i = 0; i < dWidth; i++) {
//		ch = display->getChar(row, i);
//		retStr[i] = ((ch & 0x80) > 0) ? '.' : ' ';
//	}
//	return retStr;
//}
//
//void Sys80b_Display::putDotAt(byte row, byte col) {
//	if (!display->validPosition(row, col)) return;
//	char ch = display->getChar(row, col);
//	if ((ch & 0x80) == 0) display->setChar(row, col, ch | 0x80);
//}
//
//void Sys80b_Display::clearDots(byte row) {
//	char ch;
//	byte dWidth = display->getWidth();
//	for (byte i = 0; i < dWidth; i++) {
//		ch = display->getChar(row, i);
//		if (ch & 0x80) display->setChar(row, i, ch & 0x7F);
//	}
//}
//
//// function to call periodically (1 ms) to feed the display unit with sequential bytes.
//// returns a word with format 00000RLL.DDDDDDDD where:
//// R = reset bit (active low)
//// LL = LD2, LD1 bits (active high)
//// DDDDDDDD = data byte
//// at least LD1 or LD2 (or Reset) should be setPeriod for any effect
//uint16_t Sys80b_Display::getNextDisplayData() {
//
//	// prioritary pending command
//	if (!cmdBuffer.isEmpty()) return cmdBuffer.get();
//	
//	// standard char
//	char ch = _getVisibleChar(currentRow, currentCol);
//	uint16_t data = (currentRow == 0) ? 0x500 : 0x600; // LD1 or LD2 bits with reset signal high
//	data |= ch;
//	if (ch == CD_PREFIX) cmdBuffer.put(data); // repeats value output (char #1 case)
//	if (++currentRow > 1) {
//		currentRow = 0;
//		if (++currentCol == DISPLAY_COLS) {
//			currentCol = 0;
//			_pushCmd(3, CD_BUF_PTR); // CR on both lines
//		}
//	}
//	lastData = data;
//	return data;
//}
//
//String Sys80b_Display::toString() {
//	String str;
//	for (int line = 0; line < rows; line++) str += "[" + getRowText(line) + "]\n";
//	return str;
//}
//
//byte Sys80b_Display::_getVisibleChar(byte row, byte col) {
//	byte ch = ExtTextDisplay::_getVisibleChar(row, col);
//	byte tag = ch & 0x80;
//	if (mode[row] == BLANK && tag > 0) return ' ';
//	return ch & (byte)0x7f;
//}
//
//void Sys80b_Display::_pushCmd(byte ld, byte cmd) {
//	if (cmdBuffer.getCount() >= CMD_BUF_LEN) return; // refused
//	uint16_t base = ((uint16_t)(0x04u | (ld & 0x03u))) << 8;
//	cmdBuffer.put(base | CD_PREFIX);
//	cmdBuffer.put(base | cmd);
//}
//
//void Sys80b_Display::_sendInitSequence() {
//	_pushCmd(3, CD_DIGIT_CNT | DISPLAY_COLS);
//	_pushCmd(3, CD_DTIME_16CLK); // no sw effect
//	_pushCmd(3, CD_DUTY_CYCLE | 0x3F); // maximum brightess
//	_pushCmd(3, CD_NORMAL_MODE);
//	_pushCmd(3, CD_BUF_PTR);
//	_pushCmd(3, CD_START_SCAN);
//}

Sys80b_Display::Sys80b_Display() : TextDisplay(2, DISPLAY_COLS) {
	//_init(*d);
}

void Sys80b_Display::reset() {
	currentCol = 0; // current display column
	currentRow = 0; // current display row
	cmdBuffer.Clear();
	cmdBuffer.put(0x0300u); // reset signal (active low) to both rows
	_sendInitSequence();
	for (byte i = 0; i < 2; i++) mode[i] = NORMAL;
}

void Sys80b_Display::setMode(byte row, Mode m) {
	row &= 1;
	mode[row] = m;
	switch (m) {
	case NORMAL:
		_pushCmd(row + 1, CD_NORMAL_MODE);
		break;
	case BLANK:
		_pushCmd(row + 1, CD_BLANK_MODE);
		break;
	case INVERSE:
		_pushCmd(row + 1, CD_INVRSE_MODE);
		break;
	}
}

String Sys80b_Display::getRowText(byte row) {
	if (row > 1) return String("");
	String retStr; // = display->getSpacesRow();
	for (byte i = 0; i < columns; i++) retStr += (char)_getVisibleChar(row, i);
	return retStr;
}

String Sys80b_Display::getRowDots(byte row) {
	if (row > 1) return String("");
	char ch;
	String retStr = spacesStr;
	for (byte i = 0; i < columns; i++) {
		ch = textRow[row][i];
		retStr[i] = ((ch & 0x80) > 0) ? '.' : ' ';
	}
	return retStr;
}

void Sys80b_Display::putDotAt(byte row, byte col) {
	if (!validPosition(row, col)) return;
	char ch = textRow[row][col];
	if ((ch & 0x80) == 0) textRow[row][col] = ch | 0x80;
}

void Sys80b_Display::clearDots(byte row) {
	char ch;
	for (byte i = 0; i < columns; i++) {
		ch = textRow[row][i];
		if (ch & 0x80) textRow[row][i] = ch & 0x7F;
	}
}

// function to call periodically (1 ms) to feed the display unit with sequential bytes.
// returns a word with format 00000RLL.DDDDDDDD where:
// R = reset bit (active low)
// LL = LD2, LD1 bits (active high)
// DDDDDDDD = data byte
// at least LD1 or LD2 (or Reset) should be setPeriod for any effect
uint16_t Sys80b_Display::getNextDisplayData() {

	// prioritary pending command
	if (!cmdBuffer.isEmpty()) return cmdBuffer.get();

	// standard char
	char ch = _getVisibleChar(currentRow, currentCol);
	uint16_t data = (currentRow == 0) ? 0x500 : 0x600; // LD1 or LD2 bits with reset signal high
	data |= ch;
	if (ch == CD_PREFIX) cmdBuffer.put(data); // repeats value output (char #1 case)
	if (++currentRow > 1) {
		currentRow = 0;
		if (++currentCol == DISPLAY_COLS) {
			currentCol = 0;
			_pushCmd(3, CD_BUF_PTR); // CR on both lines
		}
	}
	lastData = data;
	return data;
}

String Sys80b_Display::toString() {
	String str;
	for (int line = 0; line < rows; line++) str += "[" + getRowText(line) + "]\n";
	return str;
}

byte Sys80b_Display::_getVisibleChar(byte row, byte col) {
	byte ch = getChar(row, col);
	byte tag = ch & 0x80;
	if (mode[row] == BLANK && tag > 0) return ' ';
	return ch & (byte)0x7f;
}

void Sys80b_Display::_pushCmd(byte ld, byte cmd) {
	if (cmdBuffer.getCount() >= CMD_BUF_LEN) return; // refused
	uint16_t base = ((uint16_t)(0x04u | (ld & 0x03u))) << 8;
	cmdBuffer.put(base | CD_PREFIX);
	cmdBuffer.put(base | cmd);
}

void Sys80b_Display::_sendInitSequence() {
	_pushCmd(3, CD_DIGIT_CNT | DISPLAY_COLS);
	_pushCmd(3, CD_DTIME_16CLK); // no sw effect
	_pushCmd(3, CD_DUTY_CYCLE | 0x3F); // maximum brightess
	_pushCmd(3, CD_NORMAL_MODE);
	_pushCmd(3, CD_BUF_PTR);
	_pushCmd(3, CD_START_SCAN);
	_pushCmd(3, CD_START_SCAN);
}