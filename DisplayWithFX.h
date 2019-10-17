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

#ifndef _DisplayWithFX_
#define _DisplayWithFX_

#include "Display.h"

class TimerTask;

#define LINE_MAX_LEN 50
#define STD_DFX_DELAY 100
#define STD_BLK_DELAY 500

// A line of text to put on display with special display mode/fx.
// Text length may be either shorter or longer than display width
struct DisplayLineText {
	char msg[LINE_MAX_LEN + 1]; // line text
	byte msgLen; // display text length
	short int chOffset; // where to write the msg
	byte displayEffect; // line display effect (FX) id
	byte fxVar;
	TimerTask *fx_timer;
	bool blinkState; // cycle phase (on/off)
	uint32_t blinkEnableTime;
	uint16_t blinkActivePeriod;
	byte blinkStartCol; // first blinking char
	byte blinkEndCol; // last blinking char
	char blinkStr[LINE_MAX_LEN + 1]; // alternate string for line blinking
	TimerTask *blink_timer;
};

class DisplayWithFX {
public:
	enum FX { // single line text display effects:
		OFF, // efffects disabled
		BOUNCE,  // for overwidth msg only, bounces msg from left to right and back
		LEFT_SCRL, // left scroll text insertion
		RIGHT_SCRL, // right scroll text insertion
	};

	Display *baseDisplay = NULL;
	DisplayLineText dLine[2]; // display lines' content (complete)

	DisplayWithFX(Display basicDisplay);
	~DisplayWithFX();
	void setDisplayText(byte line, const char *text);
	void setDisplayText(byte line, const char *text, FX effectId);
	void enableDisplayEffect(byte line, FX effectId);
	void disableDisplayEffect(byte line);
	void setDisplayEffectPeriod(byte line, uint16_t period);
	void _stepDisplayEffect(byte line);
	void startDisplayBlink(byte line, uint16_t bDelay, uint16_t bPeriod);
	void startDisplayBlink(byte line, uint16_t bDelay);
	void startDisplayBlink(byte line);
	void stopDisplayBlink(byte line);
	void setDisplayBlink(byte line, bool enabled);
	void setDisplayBlinkInterval(byte line, byte firstCh, byte lastCh);
	void resetLineBlinkArea(byte line);
	void setDisplayBlinkPeriod(byte line, uint16_t bPeriod);
	void updateDisplayLine(byte line);
protected:
	void _stepFx0(uint32_t);
	void _stepFx1(uint32_t);
	void _dBlink0(uint32_t t);
	void _dBlink1(uint32_t t);
	void _dBlinkSwitch(byte line);
	void _updBlinkedTxt(byte line, byte startCh, byte endCh);
	
};

#endif
