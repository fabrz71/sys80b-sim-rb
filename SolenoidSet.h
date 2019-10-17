/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* SOLENOID SET
* ---
* Defines the setPeriod of all solenoids of the pinball table.
*/

#ifndef _SolenoidSet_h_
#define _SolenoidSet_h_

class Solenoid;

class SolenoidSet {
public:
	static const int MAX_COUNT = 32;

	uint32_t solenBits; // solenoid state bits (1 bit = 1 solenoid) (0..count-1)
	Solenoid *solen; // array of solenoids

protected:
	byte count;
	byte lastChanged;

public:
	SolenoidSet();
	SolenoidSet(byte n);
	~SolenoidSet();

	byte getCount();
	uint32_t set(byte n, bool active);
	void setAll(uint32_t bits);
	uint32_t activate(byte n, uint16_t actPeriod);
	uint32_t activateDelayed(byte n, uint16_t swDelay);
	uint32_t activateDelayed(byte n, uint16_t actPeriod, uint16_t swDelay);
	void resetAll();
	bool check(uint32_t t);
	uint32_t getBits();
	bool isActive(byte n);
	byte getLastChanged();
};

#endif
