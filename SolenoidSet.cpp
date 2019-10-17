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

#include "SolenoidSet.h"
#include "Solenoid.h"

SolenoidSet::SolenoidSet() {
	solen = NULL;
	count = 0;
}

SolenoidSet::SolenoidSet(byte n) {
	if (n = 0 || n >= MAX_COUNT) {
		solen = NULL;
		count = 0;
		return;
	}
	solen = new Solenoid[n-1];
	count = n;
	solenBits = 0;
}

SolenoidSet::~SolenoidSet() {
	delete[] solen;
}

inline byte SolenoidSet::getCount() {
	return count;
}

// sets a solenoid state immediately, updating internal variables
// note:
// n = [1..9] standard solenoid
// n = [10..15] no effect
// n = [16..31] "lamp" solenoid 0..15
uint32_t SolenoidSet::set(byte n, bool active) {
	if (n >= count) return;
	if (solen[n].isActive() == active) return solenBits;
	solen[n].set(active);
	if (active) solenBits |= bit(n);
	else solenBits &= ~bit(n);
	lastChanged = n;
	return solenBits;
}

void SolenoidSet::setAll(uint32_t bits) {
	for (int i=0; i < count; i++) solen[i].set((bits & bit(i)) > 0);
	solenBits = bits;
}

// sets a solenoid state, defining active period.
// actPeriod updates solenoid attribute also when active = false
inline uint32_t SolenoidSet::activate(byte n, uint16_t actPeriod) {
	return activateDelayed(n, actPeriod, 0);
}

// sets a solenoid state, defining switch delay:
// solenoid state will switch after the specified delay.
inline uint32_t SolenoidSet::activateDelayed(byte n, uint16_t swDelay) {
	return activateDelayed(n, 0, swDelay);
}

// sets a solenoid state, defining switch delay and active period:
// solenoid state will switch after the specified delay.
uint32_t SolenoidSet::activateDelayed(byte n, uint16_t actPeriod, uint16_t swDelay) {
	if (n >= count) return;
	solen[n].activateDelayed(swDelay);
	return solenBits;
}

// resets all solenoids
void SolenoidSet::resetAll() {
	for (byte n = 1; n <= 9; n++) solen[n].set(false);
	solenBits = 0;
}

// returns true on changes
bool SolenoidSet::check(uint32_t t) {
	bool changes = false;

	solenBits = 0;
	for (int i = 1; i <= 9; i++) {
		if (solen[i].check(t) && solen[i].effective) { // changed?
			changes = true;
			if (solen[i].isActive()) solenBits |= bit(i);
			else solenBits &= ~bit(i);
			lastChanged = i;
		}
	}
	return changes;
}

uint32_t SolenoidSet::getBits() {
	return solenBits;
}

inline bool SolenoidSet::isActive(byte n) {
	return solen[n].isActive();
}

inline byte SolenoidSet::getLastChanged() {
	return lastChanged;
}
