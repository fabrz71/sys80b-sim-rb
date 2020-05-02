#ifndef _CmdExecutor_h_
#define _CmdExecutor_h_

#include "Arduino.h"

class Board_Sys80b;
class PinGame;

class CmdExecutor {
protected:
	static const int MAX_PARTS = 4;
	static const int MAX_LENGTH = 128;
	const __FlashStringHelper* soundCmd = F("sound");
	const __FlashStringHelper* solenoidCmd = F("solenoid");
	const __FlashStringHelper* pulsesolenoidCmd = F("pulsesolenoid");
	const __FlashStringHelper* lampCmd = F("lamp");
	const __FlashStringHelper* lightCmd = F("light");
	const __FlashStringHelper* blinklightCmd = F("blinklight");
	const __FlashStringHelper* pulselightCmd = F("pulselight");
	const __FlashStringHelper* ledgridCmd = F("ledgrid");
	const __FlashStringHelper* ledgridmodeCmd = F("ledgridmode");
	const __FlashStringHelper* lcdCmd = F("lcd");
	const __FlashStringHelper* displayCmd = F("display");
	const __FlashStringHelper* factorysettingsCmd = F("factorysettings");
	const __FlashStringHelper* freememCmd = F("freemem");
	const __FlashStringHelper* revCmd = F("rev");
	const __FlashStringHelper* pocCmd = F("poc");
	const __FlashStringHelper* resetCmd = F("reset");
	const __FlashStringHelper* helpCmd = F("help");

	Board_Sys80b* _hw;
	PinGame* _sys;
	String _cmdPartStr[MAX_PARTS];
	byte _cmdPartIdx[MAX_PARTS];
	int _partsCount;
	//byte firstArgIdx; // first argument index (0 if none)

public:
	CmdExecutor() { _hw = nullptr; _sys = nullptr; };
	CmdExecutor(Board_Sys80b* board, PinGame* sys) { _hw = board; _sys = sys; };
	~CmdExecutor() { delete[] _cmdPartStr; };
	bool execCmd(String cmd);
protected:
	void _getParts(String& cmd);
};

#endif