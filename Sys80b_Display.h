/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

#ifndef _Sys80b_Display_h_
#define _Sys80b_Display_h_

#include "Arduino.h"
#include "Buffer.h"
#include "TextDisplay.h"

#define DISPLAY_COLS 20

//// System 80/B alphanumeric display of size
//// 2 line x 20 cahracters. Extension of ExtTextDisplay that allow
//// Sys80/b's display control. It provides:
//// - a buffer for byte-data to send to HW display
//// - getNextDisplayData() function for retrieving bytes to send to HW display
//// rev: nov/19
//class Sys80b_Display : public ExtTextDisplay {
//public:
//	//static const int DISPLAY_COLS = 20;
//	static const int CMD_BUF_LEN = 32; // Display command buffer length [bytes]
//
//	enum Mode { NORMAL, BLANK, INVERSE }; // 10939 chip display modes
//
//	// control data bytes
//	enum CDbyte_10939 {
//		CD_PREFIX = 0x01,
//		CD_DTIME_16CLK = 0x05, // no sw effect
//		CD_DTIME_32CLK = 0x06, // no sw effect
//		CD_DTIME_64CLK = 0x07, // no sw effect
//		CD_NORMAL_MODE = 0x08,
//		CD_BLANK_MODE = 0x09,
//		CD_INVRSE_MODE = 0x0a, // NOT IMPLEMENTED
//		CD_START_SCAN = 0x0e,
//		CD_DUTY_CYCLE = 0x40, // OR 0..3F (6 LSB)
//		CD_DIGIT_CNT = 0x80, // OR 0..1F (5 LSB)
//		CD_BUF_PTR = 0xc0 // OR 0..13 (5 LSB)
//	};
//
//	byte brightness; // brightness level, shared for both display lines
//	
//protected:
//	TextDisplay* d;
//	Mode mode[2];
//	Buffer<uint16_t, CMD_BUF_LEN> cmdBuffer;
//	byte currentCol;
//	byte currentRow;
//	uint16_t lastData;
//
//public:
//	Sys80b_Display();
//	void reset();
//	
//	inline void sendControlDataByte(byte row, CDbyte_10939 data) {
//		if (row <= 1) _pushCmd(row + 1, data);
//	}
//	inline byte getCurrentRow() {
//		return currentRow;
//	}
//
//	inline byte getCurrentColumn() {
//		return currentCol;
//	}
//
//	inline bool isHomePosition() {
//		return (currentCol == 0 && currentRow == 0);
//	}
//
//	inline bool isLastPosition() {
//		return (currentCol == DISPLAY_COLS - 1 && currentRow == 1);
//	}
//
//	// unified brightness levels: 00..3f (min..max)
//	inline void setBrightness(byte b) {
//		_pushCmd(3, CD_DUTY_CYCLE | (brightness & 0x3f));
//	}
//
//	inline uint16_t LastData() {
//		return lastData;
//	}
//
//	void setMode(byte row, Mode m);
//	String getRowText(byte row);
//	String getRowDots(byte row);
//	void putDotAt(byte row, byte col);
//	void clearDots(byte row);
//	uint16_t getNextDisplayData();
//	String toString();
//
//protected:
//	void _pushCmd(byte ld, byte cmd);
//	void _sendInitSequence();
//	byte _getVisibleChar(byte row, byte col);
//};
//
//#endif

// System 80/B alphanumeric display of size
// 2 line x 20 cahracters. Extension of ExtTextDisplay that allow
// Sys80/b's display control. It provides:
// - a buffer for byte-data to send to HW display
// - getNextDisplayData() function for retrieving bytes to send to HW display
// rev: nov/19
//class Sys80b_Display : public ExtTextDisplay {
class Sys80b_Display : public TextDisplay {
public:
	static const int CMD_BUF_LEN = 32; // Display command buffer length [bytes]

	enum Mode { NORMAL, BLANK, INVERSE }; // 10939 chip display modes

	// control data bytes
	enum CDbyte_10939 {
		CD_PREFIX = 0x01,
		CD_DTIME_16CLK = 0x05, // no sw effect
		CD_DTIME_32CLK = 0x06, // no sw effect
		CD_DTIME_64CLK = 0x07, // no sw effect
		CD_NORMAL_MODE = 0x08,
		CD_BLANK_MODE = 0x09,
		CD_INVRSE_MODE = 0x0a, // NOT IMPLEMENTED
		CD_START_SCAN = 0x0e,
		CD_DUTY_CYCLE = 0x40, // OR 0..3F (6 LSB)
		CD_DIGIT_CNT = 0x80, // OR 0..1F (5 LSB)
		CD_BUF_PTR = 0xc0 // OR 0..13 (5 LSB)
	};

	byte brightness; // brightness level, shared for both display lines

protected:
	Mode mode[2];
	Buffer<uint16_t, CMD_BUF_LEN> cmdBuffer;
	byte currentCol;
	byte currentRow;
	uint16_t lastData;

public:
	Sys80b_Display();
	void reset();

	inline void sendControlDataByte(byte row, CDbyte_10939 data) {
		if (row <= 1) _pushCmd(row + 1, data);
	}
	inline byte getCurrentRow() {
		return currentRow;
	}

	inline byte getCurrentColumn() {
		return currentCol;
	}

	inline bool isHomePosition() {
		return (currentCol == 0 && currentRow == 0);
	}

	inline bool isLastPosition() {
		return (currentCol == DISPLAY_COLS - 1 && currentRow == 1);
	}

	// unified brightness levels: 00..3f (min..max)
	inline void setBrightness(byte b) {
		_pushCmd(3, CD_DUTY_CYCLE | (brightness & 0x3f));
	}

	inline uint16_t LastData() {
		return lastData;
	}

	void setMode(byte row, Mode m);
	String getRowText(byte row);
	String getRowDots(byte row);
	void putDotAt(byte row, byte col);
	void clearDots(byte row);
	uint16_t getNextDisplayData();
	String toString();

protected:
	void _pushCmd(byte ld, byte cmd);
	void _sendInitSequence();
	byte _getVisibleChar(byte row, byte col);
};

#endif
