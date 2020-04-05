/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

#ifndef _SolenoidSet_h_
#define _SolenoidSet_h_

#include "Arduino.h"
#include "BitMappedSet.h"

//class Solenoid;

struct Solenoid {
	static const uint16_t DEF_ACTIVE_TIME = 250;
	static const uint16_t MAX_ACTIVE_TIME = 5000;
	bool active = false;
	uint32_t settingTime = 0; // time when solenoid has been set
	uint16_t activePeriod = 0; // standard active state _period, not set = 0 [ms]
	uint16_t maxActivePeriod = MAX_ACTIVE_TIME; // maximum safe active _period, 0 = not set [ms]
	uint16_t swOnDelay = 0; // switch-on delay
	uint32_t swOnTime; // switch-on event time [ms]
	uint32_t swOffTime; // switch-off event time [ms]
};

// update: nov/19
class SolenoidSet {
public:
	static const byte MAX_COUNT = 32;

protected:
	Solenoid* _solenoid; // array of solenoids objects
	BitMappedSet* _state; // solenoid state bits (1 bit = 1 solenoid) (0.._size-1)
	byte _size; // solenoids count of the set
	byte _lastChanged; // last-changed solenoid number

public:
	SolenoidSet();
	SolenoidSet(byte n);
	~SolenoidSet();

	byte getSize();
	bool setState(byte n, bool active);
	bool getState(byte n);
	//void setAll(uint32_t bits);
	//bool activate(byte n, uint16_t actPeriod);
	bool activateDelayed(byte n, uint16_t swDelay);
	bool activate(byte n, uint16_t actPeriod, uint16_t swDelay = 0);
	void resetAll();
	bool update(uint32_t& t);
	uint32_t getStates();
	uint32_t getChangesBits();
	void clearChanges();
	bool isActive(byte n);
	byte getLastChanged();
	Solenoid& get(byte n);
	String toString();

protected:
	bool _switch(byte n, bool active, uint32_t& t);
	//bool _checkSolenoid(byte n, uint32_t t);
};

#endif
