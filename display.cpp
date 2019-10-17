/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* DISPLAY LIBRARY
* ---
* Display object that provides the following functions:
* - display text output functions
* - text buffer that contains the content wanted
* - provides sequential data bytes to send to hardware display bus
* This object does not operate direclty on hardware but
* provides data to send directly to hardware through sub
* getNextDisplayData().
* Sys80b display is composed by 2 rows of 20 alphanumeric chars.
*/

#include "Display.h"

Display::Display() {
	txtBuff[0][DISPLAY_COLS] = 0;
	txtBuff[1][DISPLAY_COLS] = 0;
	tmpBuff[DISPLAY_COLS] = 0;
	clear();
	reset();
}

void Display::_initSequence() {
	_pushCmd(3, CD_DIGIT_CNT | DISPLAY_COLS);
	_pushCmd(3, CD_DTIME_16CLK); // no sw effect
	_pushCmd(3, CD_DUTY_CYCLE | 0x3F); // maximum brightess
	_pushCmd(3, CD_NORMAL_MODE);
	_pushCmd(3, CD_BUF_PTR);
	_pushCmd(3, CD_START_SCAN);
}

void Display::reset() {
	cmdBufPtr = 0;
	cmdBuff[0] = 0x0300; // reset signal (active low) to both rows
	cmdBufLen = 1;
	_initSequence();
	dCol = 0; // current display column
	dRow = 0; // current display row
	mode[0] = NORMAL;
	mode[1] = NORMAL;
	overWriteMode = false;
	dVisible[0] = true;
	dVisible[1] = true;
}

void Display::clear() {
	clearRow(0);
	clearRow(1);
}

// 0..3f
inline void Display::setBrightness(byte b) {
	_pushCmd(3, CD_DUTY_CYCLE | (b & 0x3F));
}

// to both display rows
void Display::setMode(byte row, Mode m) {
	byte r = (byte)(1 << (row & 1));
	switch (m) {
	case NORMAL:
		_pushCmd(r, CD_NORMAL_MODE);
		break;
	case BLANK:
		_pushCmd(r, CD_BLANK_MODE);
		break;
	case INVERSE:
		_pushCmd(r, CD_INVRSE_MODE);
		break;
	}
	mode[row & 1] = m;
}

void Display::clearRow(byte row) {
	int i;

	row &= 1;
	for (i = 0; i < DISPLAY_COLS; i++) txtBuff[row][i] = ' ';
}

void Display::setText(byte row, const char *text) {
	setText(row, 0, text);
}

// writes string text on specified display line with offset:
// with offset < 0 = text starts before first display char (left)
// with offset > 0 = text starts after first display char (right)
void Display::setText(byte row, const char *text, short int offset) {
	if (offset >= 0) { // right
		if (offset >= DISPLAY_COLS) return;
		setText(row, (byte)offset, text);
	}
	else { // offset < 0  = left
		if (-offset > (short int)strlen(text)) return;
		setText(row, 0, text - offset);
	}
}

void Display::setText(byte row, byte col, const char *text) {
	int i = 0;

	row = row & 1;
	col = col % DISPLAY_COLS;
	if (!overWriteMode) clearRow(row);
	while (col < DISPLAY_COLS && text[i] != 0) txtBuff[row][col++] = text[i++];
}

void Display::setText(byte row, String text) {
	setText(row, text.c_str());
}

void Display::setText(byte row, String text, short int offset) {
	setText(row, text.c_str(), offset);
}

void Display::setText(byte row, byte col, String text) {
	setText(row, col, text.c_str());
}

// scrolls text 1 char left
void Display::scrollLeft(byte row) {
	for (int i = 0; i<(DISPLAY_COLS - 1); i++) txtBuff[row][i] = txtBuff[row][i + 1];
	txtBuff[row][DISPLAY_COLS - 1] = ' ';
}

// scrolls text 1 char right
void Display::scrollRight(byte row) {
	for (int i = DISPLAY_COLS - 1; i>0; i--) txtBuff[row][i] = txtBuff[row][i - 1];
	txtBuff[row][0] = ' ';
}

void Display::rotateLeft(byte row) {
	char c;
	c = txtBuff[row][0];
	scrollLeft(row);
	txtBuff[row][DISPLAY_COLS - 1] = c;
}

void Display::rotateRight(byte row) {
	char c;
	c = txtBuff[row][DISPLAY_COLS - 1];
	scrollRight(row);
	txtBuff[row][0] = c;
}

void Display::shiftUp() {
	for (int i = 0; i<(DISPLAY_COLS); i++) {
		txtBuff[0][i] = txtBuff[1][i];
		txtBuff[1][i] = ' ';
	}
}

void Display::shiftDown() {
	for (int i = 0; i<(DISPLAY_COLS); i++) {
		txtBuff[1][i] = txtBuff[0][i];
		txtBuff[0][i] = ' ';
	}
}

void Display::_pushCmd(byte ld, byte cmd) {
	byte ptr;
	uint16_t base;

	if (cmdBufLen >= CMD_BUF_LEN - 1) return; // refused
	ptr = (cmdBufPtr + cmdBufLen) % CMD_BUF_LEN;
	base = 0x0400 | (((uint16_t)(ld & 0x03)) << 8);
	cmdBuff[ptr++] = base | CD_PREFIX;
	if (ptr >= CMD_BUF_LEN) ptr = 0;
	cmdBuff[ptr] = base | cmd;
	cmdBufLen += 2;
}

// function to call periodically (1 ms) to feed the display unit with sequential bytes.
// returns a word with format 00000RLL.DDDDDDDD where:
// R = reset bit (active low)
// LL = LD2, LD1 bits (active high)
// DDDDDDDD = data byte
// at least LD1 or LD2 (or Reset) should be setPeriod for any effect
uint16_t Display::getNextDisplayData() {
	uint16_t r = 0; // return value

					// pending command ?
	if (cmdBufLen > 0) {
		r = cmdBuff[cmdBufPtr++];
		if (cmdBufPtr == CMD_BUF_LEN) cmdBufPtr = 0;
		cmdBufLen--;
		return r;
	}

	// standard char
	byte ch = (byte)txtBuff[dRow][dCol];
	if (ch == CD_PREFIX) ch = 0x20;
	if (mode[dRow] == BLANK && (ch & 0x80) > 0) ch = 0x20;
	r = (dRow == 0) ? 0x500 : 0x600; // LD1 or LD2 bits with reset high signal
	if (dVisible[dRow]) r |= ch; else r |= 0x20;
	if (++dRow > 1) {
		dRow = 0;
		dCol++;
		if (dCol == DISPLAY_COLS) {
			_pushCmd(3, CD_BUF_PTR); // CR on both lines
			dCol = 0;
		}
	}
	return r;
}

inline void Display::writeChar(byte row, byte col, byte chr) {
	txtBuff[row & 1][col % DISPLAY_COLS] = chr;
}

inline void Display::setVisible(byte row, byte visible) {
	dVisible[row] = (visible) ? true : false;
}

inline char* Display::getRow(byte row) {
	return txtBuff[row];
}

char* Display::getRowText(byte row) {
	switch (mode[row]) {
	case NORMAL:
	case INVERSE: // unimplemented!
		for (int i = 0; i < DISPLAY_COLS; i++) tmpBuff[i] = txtBuff[row][i] & 0x7f;
		break;
	case BLANK:
		byte ch;
		for (int i = 0; i < DISPLAY_COLS; i++) {
			ch = (byte)txtBuff[row][i];
			if (ch & 0x80) tmpBuff[i] = 0x20;
			else tmpBuff[i] = ch;
		}
		break;
	}
	return tmpBuff;
}

char* Display::getRowDots(byte row) {
	for (int i = 0; i < DISPLAY_COLS; i++) tmpBuff[i] = (txtBuff[row][i] & 0x80) ? '.' : ' ';
	return tmpBuff;
}
inline String Display::getRowStr(byte row) {
	return String(txtBuff[row]);
}

inline char Display::getChar(byte row, byte col) {
	return txtBuff[row][col];
}

void Display::putDotAt(byte row, byte col) {
	txtBuff[row & 1][col % DISPLAY_COLS] |= 0x80;
}

std::ostream& operator<<(std::ostream& os, Display& d) {
	for (int i = 0; i<2; i++) {
		os << '[' << d.getRowText(i) << ']';
		if (d.mode[i] == Display::BLANK) os << 'B';
		else if (d.mode[i] == Display::INVERSE) os << "I";
		os << endl;
	}
	return os;
}