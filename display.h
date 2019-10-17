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

#ifndef _Display_h_
#define _Display_h_

class Display {
public:
	enum Mode { NORMAL, BLANK, INVERSE };

	static const int CMD_BUF_LEN = 10;
	static const int DISPLAY_COLS = 20;

	// control data bytes
	static const int CD_PREFIX = 0x01;
	static const int CD_DTIME_16CLK = 0x05; // no sw effect
	static const int CD_DTIME_32CLK = 0x06; // no sw effect
	static const int CD_DTIME_64CLK = 0x07; // no sw effect
	static const int CD_NORMAL_MODE = 0x08;
	static const int CD_BLANK_MODE = 0x09;
	static const int CD_INVRSE_MODE = 0x0a; // not implemented
	static const int CD_START_SCAN = 0x0e;
	static const int CD_DUTY_CYCLE = 0x40; // OR 0..3F (6 LSB)
	static const int CD_DIGIT_CNT = 0x80; // OR 0..1F (5 LSB)
	static const int CD_BUF_PTR = 0xc0; // OR 0..13 (5 LSB)

	bool overWriteMode;

protected:
	char txtBuff[2][DISPLAY_COLS + 1]; // bidimensional array (2 rows)
	char tmpBuff[DISPLAY_COLS + 1];
	bool dVisible[2]; // array
	Mode mode[2];
	uint16_t cmdBuff[CMD_BUF_LEN]; // array
	byte cmdBufLen;
	byte cmdBufPtr;
	byte dCol;
	byte dRow;

public:
	Display();
	void reset();
	void clear();
	void setBrightness(byte b);
	void setMode(byte row, Mode m);
	void clearRow(byte row);
	void setText(byte row, const char *text);
	void setText(byte row, const char *text, short int offset);
	void setText(byte row, byte col, const char *text);
	void setText(byte row, String text);
	void setText(byte row, String text, short int offset);
	void setText(byte row, byte col, String text);
	void scrollLeft(byte row);
	void scrollRight(byte row);
	void rotateLeft(byte row);
	void rotateRight(byte row);
	void shiftUp();
	void shiftDown();
	uint16_t getNextDisplayData();
	void writeChar(byte row, byte col, byte chr);
	void setVisible(byte row, byte visible);
	char *getRow(byte row);
	char *getRowText(byte row);
	char *getRowDots(byte row);
	String getRowStr(byte row);
	char getChar(byte row, byte col);
	void putDotAt(byte row, byte col);
	friend std::ostream& operator <<(std::ostream& os, Display& d);

protected:
	void _initSequence();
	void _pushCmd(byte ld, byte cmd);
};

#endif