/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

// rev: nov/19

#ifndef _TextDisplayExt_
#define _TextDisplayExt_

#include "Arduino.h"
#include "TextDisplay.h"
//#include "ExtTextDisplayLine.h"

enum DFX_type { // Text-line Dynamic Effect identifier
	NONE, // static text shown
	BOUNCE_IN,  // short text bounce inside display margins
	BOUNCE_OUT, // for overwidth text only, bounces text from left to right and back
	LEFT_SCRL, // text left scroll
	RIGHT_SCRL, // text right scroll
	LEFT_INS, // left scroll with text insertion
	RIGHT_INS, // right scroll with text insertion
	LEFT_ROT, // left rotation (circular)
	RIGHT_ROT, // right rotation (circular)
	SPRAY // text "spraying" (only the head of overwidth lines)
};

// Extended alphanumeric Display virtual object which requires a target
// instance of TextDisplay object, adding extra functions and features
// to it:
// - extended line support (for text lines longer than display width)
// - special Dynamic Effects (DFX) for each line
// - text blink feature
// Periodic calls (i.e. every millisecond) of function update()
// are required for correct internal status updates.
class ExtTextDisplay {
public:
	static const byte LINE_MAX_LEN = 100;
	static const uint16_t STD_DFX_DELAY = 50;
	static const int DEF_BLINK_PERIOD = 500; // [ms]
	static const int DEF_BLINK_DURATION = 0; // [ms]

	struct LineBlinkParams {
		String mask; // should contain spaces where corresponding display char should not blink
		bool enabled = false;
		bool phase = true; // true when char is visible
		byte dutyCycle = 50; // values 1 to 99
		uint32_t startTime = 0; // blink effect start time
		uint16_t period = DEF_BLINK_PERIOD; // blink _period (on+off) [ms]
		uint16_t duration = DEF_BLINK_DURATION; // blink effect duration [ms]: 0 is forever
	};

	struct DFXTextLine {
		//public:
		String text; // full line content to display through DFX
		bool visible = true; // visibility flag
		DFX_type dfxType = NONE; // display effect type
		bool dfxEnabled = false; // display effect enable flag
		//uint32_t dfxStartTime; // effect start time
		uint32_t dfxLastStepTime = 0; // last effect step time
		uint16_t dfxStep = 0; // effect step
		byte dfxAux; // auxiliary value for further extensions
		uint16_t dfxDelay; // display effect step delay (_period)
	};

protected:
	TextDisplay *display = nullptr; // TextDisplay object to operate on
	DFXTextLine *extLine = nullptr; // extended display lines (array)
	LineBlinkParams* blinkParams; // blink parameters (one for each line)
	String inputText; // the text line to show on display (obj input)
	byte rows = 0; // dislpay lines count
	byte cols = 0; // dispay lines width

public:
	//ExtTextDisplay() {
	//	display = nullptr;
	//	rows = 0;
	//	cols = 0;
	//	extLine = nullptr;
	//}
	ExtTextDisplay(TextDisplay& d);
	~ExtTextDisplay() {
		delete[] extLine;
		delete[] blinkParams;
	}

	inline void clear() { for (byte i = 0; i < rows; i++) clearLine(i); }
	void clearLine(byte line);
	void reset();
	void putText(byte line, const char* text, byte pos = 0,	
		bool blinking = false, bool clearBefore = false);
	void putText(byte line, String& text, byte pos = 0, 
		bool blinking = false, bool clearBefore = false);
	void setDynamicFX(byte line, const char* text, DFX_type effectId);
	void setDynamicFX(byte line, String& text, DFX_type effectId);
	void setBlinkParams(byte line, uint16_t bDuration = 0, uint16_t bPeriod = 0, 
		byte bDutyCycle = 0);
	void startBlinking(byte line);
	void DFXStep(byte line);
	bool update(uint32_t t);
	String toString();
	//friend std::ostream& operator <<(std::ostream& os, ExtTextDisplay& d);

	inline void putText(byte line, const __FlashStringHelper* text, byte pos = 0,
		bool blinking = false, bool clearBefore = false) {
		putText(line, (const char*)text, pos, blinking, clearBefore);
	}

	inline void setDynamicFX(byte line, const __FlashStringHelper* text, DFX_type effectId) {
		setDynamicFX(line, (const char*)text, effectId);
	}

	inline void stopDynamicFX(byte line) {
		if (line < rows) extLine[line].dfxEnabled = false;
	}

	inline void setDFXPeriod(byte line, uint16_t period) {
		if (line < rows) extLine[line].dfxDelay = period;
	}

	inline void setBlinkMask(byte line, const char* mask) {
		for (int i = 0; i < cols && mask[i] != 0; i++) blinkParams[line].mask.setCharAt(i, mask[i]);
	}

	inline void setBlinkMask(byte line, String& mask) {
		blinkParams[line].mask = mask.substring(0, cols);
	}

	inline void stopBlinking(byte line) {
		blinkParams[line].enabled = false;
	}

protected:
	byte _getVisibleChar(byte row, byte col);
	void _enableDisplayEffect(byte line, DFX_type effectId);
	void _setBlinkMask(byte line, const char* text, byte pos);
	void _setBlinkMask(byte line, String& text, byte pos);
	void _clearBlinkMask(byte row);
};

#endif
