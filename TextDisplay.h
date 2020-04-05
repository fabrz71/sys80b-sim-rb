/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

#ifndef _TextDisplay_h_
#define _TextDisplay_h_

#include "Arduino.h"

// Basic fixed-size alphanumeric display object which provides.:
// - both char and row (line) text set/get functions
// - vertical/horizontal shift and rotate functions
// - getRowCopy() function that returns a new string with row text
// rev: nov/19
class TextDisplay { 
public:
	static const byte MIN_ROWS_COUNT = 1;
	static const byte MIN_COLS_COUNT = 8;
	static const byte ERR_BYTE = (byte)'!';
	//bool clearBeforeWrite;

protected:
	char** textRow; // array of text rows
	String tempStr; // temporary string
	byte rows;
	byte columns;
	String spacesStr;

public:
	TextDisplay(byte rws, byte cols);
	void clear();
	void clearRow(byte row);
	void setText(byte row, const char *text, short offset, bool clearBefore = false);
	void setText(byte row, byte col, const char *text, bool clearBefore = false);
	void scrollLeft(byte row);
	void scrollRight(byte row);
	void rotateLeft(byte row);
	void rotateRight(byte row);
	inline void scrollUp() { rotateUp(); clearRow(rows - 1); }
	inline void scrollDown() { rotateDown(); clearRow(0); }
	void rotateUp();
	void rotateDown();
	//void writeChar(byte row, byte col, byte chr);
	inline String getRowCopy(byte row) { return (row >= rows) ? String("") : String(textRow[row]); }
	void getRowCopy(byte row, String& destStr);
	inline String getSpacesRow() { return String(spacesStr); }
	void setChar(byte row, byte col, byte ch);
	inline byte getChar(byte row, byte col) {
		return (validPosition(row, col)) ? (byte)textRow[row][col] : ERR_BYTE;
	}
	inline virtual byte getVisibleChar(byte row, byte col) { return getChar(row, col); }
	inline byte getHeight() { return rows; }
	inline byte getWidth() { return columns; }
	inline bool validRow(byte row) { return (row < rows); }
	inline bool validColumn(byte col) { return (col < rows); }
	inline bool validPosition(byte row, byte col) {
		return (row < rows && col < columns);
	}
	inline void setText(byte row, const char* text, bool clearBefore) {
		setText(row, 0, text, clearBefore);
	}
	inline void setText(byte row, String text, bool clearBefore = false) {
		setText(row, 0, text.c_str(), clearBefore);
	}
	inline void setText(byte row, String srcText, short offset, bool clearBefore = false) {
		setText(row, srcText.c_str(), offset, clearBefore);
	}
	inline void setText(byte row, byte col, String text, bool clearBefore = false) {
		setText(row, col, text.c_str(), clearBefore);
	}
	void setText(byte row, const __FlashStringHelper* text, short offset, bool clearBefore = false) {
		setText(row, (const char*)text, offset, clearBefore);
	}
	void setText(byte row, byte col, const __FlashStringHelper* text, bool clearBefore = false) {
		setText(row, col, (const char*)text, clearBefore);
	}
	String toString();
};

#endif
