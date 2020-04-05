/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* SWITCH GRID HANDLER
* ---
* 8x8 pinball switch grid I/O functions.
*/

#include "SwitchGrid8x8.h"

SwitchGrid8x8::SwitchGrid8x8() {
	int i, j;

	//lastRowChanged = 0xff;
	for (i = 0; i < 8; i++) {
		returns_latch[i] = 0;
		returns_changed[i] = 0;
		for (j=0; j<8; j++) swChangeTime[i][j] = 0;
	}
	lastSwitchChanged = 0xff;
	lastSwitchChState = false;
	_strobe = 0;
}

SwitchGrid8x8::~SwitchGrid8x8() {
	delete[] returns_latch;
	delete[] returns_changed;
	delete[] swChangeTime;
}

byte SwitchGrid8x8::incrementStrobe() {
	if (++_strobe > 7) _strobe = 0;
	return _strobe;
}

// Changes the state of a row of switches.
// Has unbounce effect on on->off transitions, with delay defined by SW_ONOFF_DELAY [ms].
// <r> = row [0..7]
// <val> = value [0..255]
// tm = current system time [ms]
// returns changed bits (0 if no bit has changed)
 byte SwitchGrid8x8::setRow(byte r, byte val, uint32_t& tm) {
	byte diff, b, bv;

	r &= 7;
	diff = returns_latch[r] ^ val;
	if (diff > 0) { // changes detected!
		//lastRowChanged = r;
		lastRowChanges = diff;
		// checks which switch has changed
		bv = 1; // bit value
		for (b = 0; b < 8; b++) { // for each bit...
			if (diff & bv) { // bit has changed!
				swChangeCount[r][b]++;
				lastSwitchChanged = getSwitchNumber(b, r);
				lastSwitchChState = ((val & bv) > 0);
				if (lastSwitchChState) { // switch has just closed
					returns_latch[r] |= bv; 
					swChangeTime[r][b] = tm;
				}
				else { // switch now open
					if (tm - swChangeTime[r][b] >= SW_ONOFF_DELAY) { // debounce condition
						returns_latch[r] &= ~bv;
						swChangeTime[r][b] = tm;
					}
					else diff &= ~bv; // correction in case of delayed transition: clears corresponding bit
				}
			}
			bv <<= 1;
		}
	#ifdef SWGRID_TRACK_CHANGES
		returns_changed[r] |= diff;
	#endif
	}
	return diff;
}

// switchNum = 0..77
 bool SwitchGrid8x8::getSwitchState(byte switchNum) {
	byte r = switchNum % 10;
	byte c = switchNum - r;
	if (r > 7 || c > 7) return false;
	return ((returns_latch[r] & (1u<<c)) > 0) ? true : false;
}

bool SwitchGrid8x8::getSwitchState(byte col, byte row) {
	return (((returns_latch[row & 7] & (1u << (col & 7))) > 0) ? true : false);
}

bool SwitchGrid8x8::setSwitch(byte switchNum, bool state, bool seamless) {
	if (getSwitchState(switchNum) == state) return false;
	byte r = switchNum % 10;
	byte c = switchNum - r;
	if (state) returns_latch[r] |= 1u << c;
	else returns_latch[r] &= ~(1u << c);
	if (!seamless) {
		returns_changed[r] |= 1u << c;
		swChangeTime[r][c] = millis();
		swChangeCount[r][c]++;
	}
	return true;
}

#ifdef SWGRID_TRACK_CHANGES

void SwitchGrid8x8::clearSwitchChange(byte& sw) {
	byte r = sw % 10;
	byte c = sw - r;
	if (c > 7 || r > 7) return;
	returns_changed[r] &= !(1u << c);
}

#endif
