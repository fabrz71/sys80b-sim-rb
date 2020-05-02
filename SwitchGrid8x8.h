/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

#ifndef _SwitchGrid_h_
#define _SwitchGrid_h_

#include "Arduino.h"

// 8x8software switch grid read / set functions with
// strobe advance feature and chances tracking functions.
// rev: jan/20
class SwitchGrid8x8 {
	// comment next line if changes tracking is not required
	//#define SWGRID_TRACK_CHANGES

protected:
	//byte lastRowChanged; // last switch-matrix row changed during sequential scan
	byte lastRowChanges; // bits set to 1 shows changed switch states
	byte lastSwitchChanged; // number of switch that has changed at last - starts with 0xff (none)
	bool lastSwitchChState;
	uint32_t swChangeTime[8][8]; // last switch change time (strobe (row), switch (column))
	uint32_t swChangeCount[8][8]; // switch state changes count (strobe (row), switch (column))
	byte returns_latch[8]; // last switch-matrix state 
	byte returns_changed[8]; // returns change bits

public:
	static const uint16_t SW_ONOFF_DELAY = 50; // ms

protected:
	byte _strobe;

public:
	SwitchGrid8x8();
	~SwitchGrid8x8();

	//byte getNextReturns(uint32_t t);
	byte incrementStrobe();
	inline byte getCurrentStrobe() { return _strobe; }
	inline byte getCurrentRow() { return returns_latch[_strobe]; }
	inline byte getLastChanges() { return lastRowChanges; }
	// return 0xff when no changes occurred on current row
	inline byte getLastChangedSwitch() { return lastSwitchChanged; }
	inline bool getLastChangedSwitchState() { return lastSwitchChState; }
	inline byte getRow(byte r) { return returns_latch[r & 7]; }
	inline byte setCurrentRow(byte val, uint32_t& tm) { return setRow(_strobe, val, tm); }
	byte setRow(byte r, byte val, uint32_t& tm);
	inline bool currentRowChanged() { return (lastRowChanges > 0) ? true : false; }
	bool getSwitchState(byte switchNum);
	bool getSwitchState(byte col, byte row);
	bool setSwitch(byte switchNum, bool state, bool seamless = false);
	inline uint32_t getSwitchLastChangeTime(byte col, byte row) { return swChangeTime[row & 7][col & 7]; }
	inline byte getSwitchNumber(byte col, byte row) { return (row & 7) * 10 + (col & 7); }
	inline uint32_t getSwitchChangeCount(byte col, byte row) { return swChangeCount[row & 7][col & 7]; }
	void clearLastSwitchChanged() { lastSwitchChanged = 0xff; }

#ifdef SWGRID_TRACK_CHANGES
	inline byte getCurrentChanges() { return returns_changed[strobe]; }
	inline byte getChanges(byte r) { return returns_changed[r & 7]; }
	inline void clearRowChanges(byte& col) { clearCurrentChangeBits((byte)1 << (col & 7)); }
	void clearCurrentRowChanges(byte& bits) { returns_changed[strobe] &= !bits; }
	void clearSwitchChange(byte& sw);
#endif
		
};

#endif
