/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* DISPLAY LIBRARY
* ---
* Display buffers and basic output functions.
*/

#include "mcp_io.h"

#define CMD_BUF_LEN 20
#define DISPLAY_COLS 20

// control data bytes --- da spostare!
#define CD_BYTE01      0x01
#define CD_NORMAL_MODE 0x08
#define CD_BLANK_MODE  0x09
#define CD_INVRSE_MODE 0x0a
#define CD_START_SCAN  0x0e
#define CD_DUTY_CYCLE  0x40 // OR 0..3F (6 LSB)
#define CD_DIGIT_CNT   0x80 // OR 0..1F (5 LSB)
#define CD_BUF_PTR     0x80 // char pointer = lower 5 bits

#define setOverWriteMode(m) overWriteMode = m
#define writeDisplayChar(r,c,a) txtBuff[r & 1][c % DISPLAY_COLS] = a
#define setDisplayVisible(r,v) dVisible[r] = (v) ? true : false
#define getDisplayRow(r) txtBuff[r]
#define getDisplayRowStr(r) String(txtBuff[r])
#define getDisplayChar(r,c) txtBuff[r][c]
#define putDotAt(r,c) txtBuff[r & 1][c % DISPLAY_COLS] |= 0x80
#define writeDisplayData(b)  mcpWritePB(3, b)
#define writeDisplayLD1(b)  digitalWrite(D_LD1_PIN, ((b) ? HIGH : LOW)
#define writeDisplayLD2(b)  digitalWrite(D_LD2_PIN, ((b) ? HIGH : LOW)

const byte displayInitSequence[] = { // display init sequence (after reset)
  CD_DIGIT_CNT, 20,
  CD_DUTY_CYCLE, 0x20,
  CD_NORMAL_MODE,
  CD_START_SCAN,
  0xff }; // end of sequence

extern const byte D_LD1_PIN;
extern const byte D_LD2_PIN;

char txtBuff[2][DISPLAY_COLS+1];
bool dVisible[2];
uint16_t cmdBuff[CMD_BUF_LEN];
byte cmdBufLen;
byte cmdBufPtr;
byte dCol;
byte dRow;
bool overWriteMode;

void initDisplay();
void resetDispl();
void clearDispl();
void clearDisplayRow(byte row);
void writeDisplayText(byte row, const char *text);
void writeDisplayText(byte row, const char *text, short int offset);
void writeDisplayText(byte row, byte col, const char *text);
void writeDisplayText(byte row, String text);
void writeDisplayText(byte row, String text, short int offset);
void writeDisplayText(byte row, byte col, String text);
void displayScrollLeft(byte row);
void displayScrollRight(byte row);
void displayRotateLeft(byte row);
void displayRotateRight(byte row);
void displayShiftUp();
void displayShiftDown();
void dPushCmd(byte ld, byte cmd);
uint16_t getNextDisplayData();
void pushByteOnDisplayRows();

void initDisplay() {
  byte i, c;

  txtBuff[0][DISPLAY_COLS] = 0;
  txtBuff[1][DISPLAY_COLS] = 0;
  resetDispl();
  clearDispl();
  overWriteMode = false;
  c = 0;
  for (i=0; ; i++) {
    c = displayInitSequence[i];
    if (c == 0xff) break;
    dPushCmd(3, c);
  }
}

void resetDispl() {
  cmdBufPtr = 0;
  cmdBuff[cmdBufPtr] = 0x0300;
  cmdBufLen = 1;
  dCol = 0; // current display column
  dRow = 0; // current display row
  dVisible[0] = true;
  dVisible[1] = true;
  dPushCmd(3, CD_BUF_PTR);
}

void clearDispl() {
  clearDisplayRow(0);
  clearDisplayRow(1);
}

void clearDisplayRow(byte row) {
  int i;

  row &= 1;
  for (i=0; i<DISPLAY_COLS; i++) txtBuff[row][i] = ' ';
}

void writeDisplayText(byte row, const char *text) {
  writeDisplayText(row, 0, text);
}

// writes string text on specified display line with offset:
// with offset < 0 = text starts before first display char (left)
// with offset > 0 = text starts after first display char (right)
void writeDisplayText(byte row, const char *text, short int offset) {
  if (offset >= 0) { // right
    if (offset >= DISPLAY_COLS) return;
    writeDisplayText(row, (byte)offset, text);
  }
  else { // offset < 0  = left
    if (-offset > (short int)strlen(text)) return;
    writeDisplayText(row, 0, text - offset);
  }
}

void writeDisplayText(byte row, byte col, char *text) {
  int i = 0;

  row = row & 1;
  col = col % DISPLAY_COLS;
  while (col < DISPLAY_COLS && text[i] != 0) txtBuff[row][col++] = text[i++];
}

void writeDisplayText(byte row, byte col, const char *text) {
  int i = 0;

  row = row & 1;
  col = col % DISPLAY_COLS;
  if (!overWriteMode) clearDisplayRow(row);
  while (col < DISPLAY_COLS && text[i] != 0) txtBuff[row][col++] = text[i++];
}

void writeDisplayText(byte row, String text) {
  writeDisplayText(row, text.c_str());
}

void writeDisplayText(byte row, String text, short int offset) {
  writeDisplayText(row, text.c_str(), offset);
}

void writeDisplayText(byte row, byte col, String text) {
  writeDisplayText(row, col, text.c_str());
}

// scrolls text 1 char left
void displayScrollLeft(byte row) {
  for (int i=0; i<(DISPLAY_COLS-1); i++) txtBuff[row][i] = txtBuff[row][i+1];
  txtBuff[row][DISPLAY_COLS-1] = ' ';
}

// scrolls text 1 char right
void displayScrollRight(byte row) {
  for (int i=DISPLAY_COLS-1; i>0; i--) txtBuff[row][i] = txtBuff[row][i-1];
  txtBuff[row][0] = ' ';
}

void displayRotateLeft(byte row) {
  char c;
  c = txtBuff[row][0];
  displayScrollLeft(row);
  txtBuff[row][DISPLAY_COLS-1] = c;
}

void displayRotateRight(byte row) {
  char c;
  c = txtBuff[row][DISPLAY_COLS-1];
  displayScrollRight(row);
  txtBuff[row][0] = c;
}

void displayShiftUp() {
  for (int i=0; i<(DISPLAY_COLS); i++) {
    txtBuff[0][i] = txtBuff[1][i];
    txtBuff[1][i] = ' ';
  }
}

void displayShiftDown() {
  for (int i=0; i<(DISPLAY_COLS); i++) {
    txtBuff[1][i] = txtBuff[0][i];
    txtBuff[0][i] = ' ';
  }
}

void dPushCmd(byte ld, byte cmd) {
  byte ptr;

  if (cmdBufLen >= CMD_BUF_LEN) return; // refused
  ptr = (cmdBufPtr + cmdBufLen) % CMD_BUF_LEN;
  cmdBuff[ptr] = 0x0400 | (((uint16_t)(ld & 0x03))<<8) | cmd;
  cmdBufLen++;
}

// function to call periodically (1 ms) to feed the display unit with sequential bytes.
// returns a word with format 00000RLL.DDDDDDDD where:
// R = reset bit (active low)
// LL = LD2, LD1 bits (active high)
// DDDDDDDD = data byte
// at least LD1 or LD2 (or Reset) should be set for any effect
uint16_t getNextDisplayData() {
  uint16_t r = 0; // return value

  if (cmdBufLen > 0) { // pending command ?
    r = cmdBuff[cmdBufPtr++];
    if (cmdBufPtr == CMD_BUF_LEN) cmdBufPtr = 0;
    cmdBufLen--;
    return r;
  }
  // standard char
  r = (dRow == 0) ? 0x500 : 0x600; // LD1 or LD2 bits
  if (dVisible[dRow]) r |= txtBuff[dRow][dCol]; else r |= ' ';
  if (++dRow > 1) {
    dRow = 0;
    dCol++;
    if (dCol == DISPLAY_COLS) {
      dPushCmd(3, CD_BUF_PTR); // CR on both lines
      dCol = 0;
    }
  }
  return r;
}

void pushByteOnDisplayRows() {
  uint16_t dd;

  // FIRST OR BOTH DISPLAYS
  dd = getNextDisplayData(); // row 0
  writeDisplayData((byte)(dd & 0xff));
  digitalWrite(D_LD1_PIN, ((dd & 0x100)>0) ? HIGH : LOW);
  digitalWrite(D_LD2_PIN, ((dd & 0x200)>0) ? HIGH : LOW);
  delayMicroseconds(10);
  digitalWrite(D_LD1_PIN, LOW);
  digitalWrite(D_LD2_PIN, LOW);
  
  if ((dd & 0x0300) == 0x0300) return; // CMD for both lines has already been sent
  delayMicroseconds(100);

  // SECOND DISPLAY  
  dd = getNextDisplayData(); // row 1
  writeDisplayData((byte)(dd & 0xff));
  digitalWrite(D_LD2_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(D_LD2_PIN, LOW);
}
