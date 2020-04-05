#include "TextDisplay.h"

TextDisplay::TextDisplay(byte rws = MIN_ROWS_COUNT, byte cols = MIN_COLS_COUNT) {
	// dimensions check
	if (rws < MIN_ROWS_COUNT) rws = MIN_ROWS_COUNT;
	rows = rws;
	if (cols == MIN_COLS_COUNT) cols = MIN_COLS_COUNT;
	columns = cols;
	// creates "empty" display text lines filled with spaces
	textRow = new char*[rows];
	for (byte i = 0; i < rows; i++) textRow[i] = new char[columns + 1];
	clear();
	spacesStr = String(textRow[0]); // empty text line string for further use
}

void TextDisplay::clear() {
	for (byte i = 0; i < rows; i++) {
		for (byte j = 0; j < columns; j++) textRow[i][j] = ' ';
		textRow[i][columns] = 0;
	}
}

void TextDisplay::clearRow(byte row) {
	if (row >= rows) return;
	for (byte i = 0; i < columns; i++) textRow[row][i] = ' ';
}

// Copy <srcText> in <row>-th text buffer, starting from column <col>.
// Redundant characters in <srcText> will be ignored.
void TextDisplay::setText(byte row, byte col, const char *srcText, bool clearBefore) {
	if (row > rows) return;
	byte i;
	if (clearBefore) for (i = 0; i < columns; i++) textRow[row][i] = ' ';
	if (col > columns) return;
	byte c = col;
	char* tRow = textRow[row];
	for (i = 0; srcText[i] != 0 && c < columns; i++) tRow[c++] = srcText[i];
}

// Copy <srcText> in <row>-th text buffer, starting from <offset>-th char of <srcText>.
// If <offset> < 0, header spaces will be displayed before first <srcText> character.
// Redundant characters in <srcText> will be ignored.
void TextDisplay::setText(byte row, const char* srcText, short offset, bool clearBefore) {
	if (row >= rows) return;
	short i;
	if (clearBefore) for (i = 0; i < columns; i++) textRow[row][i] = ' ';
	if (offset <= -((short)columns)) return; // too large negative offset 
	uint16_t ln = 0;
	while (srcText[ln] != 0) ln++;
	if (offset >= ln) return; // too large offset 

	byte c = 0;
	char* tRow = textRow[row];
	for (i = offset; c < columns; i++) {
		if (i < 0) tRow[c++] = 32;
		else {
			if (srcText[i] == 0) break;
			tRow[c++] = srcText[i];
		}
	}
}

// scrolls _text 1 char left
void TextDisplay::scrollLeft(byte row) {
	if (row >= rows) return;
	byte i;
	char* tRow = textRow[row];
	for (i=0; i<columns; i++) tRow[i] = tRow[i+1];
	tRow[columns-1] = ' ';
}

// scrolls _text 1 char right
void TextDisplay::scrollRight(byte row) {
	if (row >= rows) return;
	byte i;
	char* tRow = textRow[row];
	for (i = columns-1; i > 0; i--) tRow[i] = tRow[i - 1];
	tRow[0] = ' ';
}

void TextDisplay::rotateLeft(byte row) {
	if (row >= rows) return;
	byte i;
	char* tRow = textRow[row];
	char firstCh = tRow[0];
	for (i = 0; i < columns; i++) tRow[i] = tRow[i + 1];
	tRow[columns-1] = firstCh;
}

void TextDisplay::rotateRight(byte row) {
	if (row >= rows) return;
	byte i;
	char* tRow = textRow[row];
	char lastCh = tRow[columns - 1];
	for (i = columns - 1; i > 0; i--) tRow[i] = tRow[i - 1];
	tRow[0] = lastCh;
}

void TextDisplay::rotateUp() {
	char* firstLine = textRow[0];
	for (byte i = 0; i < rows; i++) textRow[i] = textRow[i + 1];
	textRow[rows - 1] = firstLine;
}

void TextDisplay::rotateDown() {
	char* lastLine = textRow[rows - 1];
	for (byte i = rows - 1; i > 0; i--) textRow[i] = textRow[i - 1];
	textRow[0] = lastLine;
}

//void TextDisplay::writeChar(byte row, byte col, byte chr) {
//	if (!validPosition(row, col)) return;
//	textRow[row][col] = chr;
//}

void TextDisplay::getRowCopy(byte row, String& destStr) {
	if (row >= rows) return;
	destStr.copy((const char*)textRow[row], columns);
}

void TextDisplay::setChar(byte row, byte col, byte ch) {
	if (!validPosition(row, col)) return; 
	textRow[row][col] = ch;
}

String TextDisplay::toString() {
	String s;
	for (int r = 0; r < rows; r++) s += "[" + getRowCopy(r) + "]\n";
	return s;
}