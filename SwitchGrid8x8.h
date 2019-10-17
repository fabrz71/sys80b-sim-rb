/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* 8x8 SWITCH GRID HANDLER
* ---
* 8x8 pinball switch grid I/O functions.
*/

#ifndef _SwitchGrid_h_
#define _SwitchGrid_h_

class SwitchGrid8x8 {
protected:
	byte lastRowChanged; // last switch-matrix row changed during sequential scan
	byte lastRowChanges; // 1-bits show changed switch state
	uint32_t swChangeTime[8][8]; // last switch change time (strobe, switch)
	uint32_t swChangeCount[8][8]; // switch state changes count (strobe, switch)
	byte returns_latch[8]; // updated switch-matrix state 

public:
	static const uint16_t SW_ONOFF_DELAY = 50; // ms

	byte strobe;

	SwitchGrid8x8();
	~SwitchGrid8x8();

	//byte getNextReturns(uint32_t t);
	byte incrementStrobe();
	byte getSequentialRow();
	byte getRow(byte r);
	byte setSequentialRow(byte val, uint32_t tm);
	byte setRow(byte r, byte val, uint32_t tm);
	bool getSwitch(byte switchNum);
	bool getSwitch(byte col, byte row);
	uint32_t getSwitchLastChangeTime(byte col, byte row);
	uint32_t getSwitchChangeCount(byte col, byte row);

	static byte getSwitchNumber(byte col, byte row);
};

#endif
