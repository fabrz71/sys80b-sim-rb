/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LAMP SET OBJECT
* ---
* Low-level representation of a complete pinball lamp setPeriod 
* of given dimension, mapped on an array of nybbles.
* Each lamp state is simply represented by a state bit (ON/OFF).
*/

#ifndef _LampSet_h_
#define _LampSet_h_

class LampSet
{
public:
	static const int MAX_COUNT = 256;

protected:
	byte *_lamps; // lamp states nybbles array (groups of 4 lamps)
	byte _size; // size of lamps array
	byte _groups4; // count of groups of 4
	byte *_changed; // hardware lamp-group update-request flags (nybbles)
	byte _lastChanged;

public:
	LampSet();
	LampSet(byte n);
	~LampSet();

	byte getSize();
	bool setLamp(byte n, bool state);
	bool setGroup(byte set, byte states);
	byte getGroup(byte set);
	void setSequence(byte idx, uint32_t bits, byte cnt);
	bool getLamp(byte n);
	void resetAll();
	bool isGroupChanged(byte set);
	void validateGroup(byte set);
	void invalidate();
protected:
	void _init(byte n);
};

#endif