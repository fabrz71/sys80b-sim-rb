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

	lastRowChanged = 0xff;
	for (i = 0; i < 8; i++) {
		returns_latch[i] = 0;
		for (j=0; j<8; j++) swChangeTime[i][j] = 0;
	}
}

SwitchGrid8x8::~SwitchGrid8x8() {
	delete[] returns_latch;
	delete[] swChangeTime;
}

inline byte SwitchGrid8x8::incrementStrobe() {
	strobe = (++strobe) & 7;
	return strobe;
}

inline byte SwitchGrid8x8::getSequentialRow() {
	return returns_latch[strobe & 7];
}

inline byte SwitchGrid8x8::getRow(byte r) {
	return returns_latch[r&7];
}

inline byte SwitchGrid8x8::setSequentialRow(byte val, uint32_t tm) {
	return setRow(strobe, val, tm);
}

// Changes the state of a row of switches at one time.
// Has unbounce effect on on->off transitions, with delay defined by SW_ONOFF_DELAY [ms].
// returns changed bits (0 if no bit has changed)
inline byte SwitchGrid8x8::setRow(byte r, byte val, uint32_t tm) {
	byte diff, b, bv;

	r &= 7;
	diff = returns_latch[r] ^ val;
	if (diff) { // changes detected
		lastRowChanged = r;
		lastRowChanges = diff;

		// checks which switch has changed
		for (b = 0; b < 8; b++) { // for each bit...
			bv = 1 << b; // bit value
			if (diff & bv) { // bit has changed!
				swChangeCount[r][b]++;
				if (val & bv) { // switch has just closed
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
		}
	}
	return diff;
}

// switchNum = 0..63
inline bool SwitchGrid8x8::getSwitch(byte switchNum) {
	return ((returns_latch[switchNum / 10] & bit(switchNum % 10) > 0) ? true : false);
}

inline bool SwitchGrid8x8::getSwitch(byte col, byte row) {
	return ((returns_latch[row & 7] & bit(col & 7) > 0) ? true : false);
}

inline uint32_t SwitchGrid8x8::getSwitchLastChangeTime(byte col, byte row) {
	return swChangeTime[row&7][col&7];
}

inline uint32_t SwitchGrid8x8::getSwitchChangeCount(byte col, byte row) {
	return swChangeCount[row & 7][col & 7];
}

inline static byte getSwitchNumber(byte col, byte row) {
	return ((col << 3) | (row & 7)) & 0b00111111;
}