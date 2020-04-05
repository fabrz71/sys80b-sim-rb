#include "SolenoidSet.h"
#include "msg.h"

SolenoidSet::SolenoidSet() {
	//Serial.println("SolenoidSet() init...");
	//delay(100);
	_solenoid = nullptr;
	_size = 0;
}

SolenoidSet::SolenoidSet(byte n) {
	//Serial.println("SolenoidSet(n) init...");
	//delay(100);
	if (n == 0 || n >= MAX_COUNT) {
		_solenoid = nullptr;
		_size = 0;
		return;
	}
	_solenoid = new Solenoid[n];
	_state = new BitMappedSet(n);
	_size = n;
}

SolenoidSet::~SolenoidSet() {
	delete[] _solenoid;
	delete _state;
}

byte SolenoidSet::getSize() {
	return _size;
}

// sets a solenoid state, updating internal variables
bool SolenoidSet::_switch(byte n, bool state, uint32_t& t) {
	Solenoid& sol = _solenoid[n];
	if (sol.active == state) return false;
	sol.active = state;
	if (state) sol.swOnTime = t;
	else sol.swOffTime = t;
	_state->setState(n, state);
	_lastChanged = n;
	return true;
}

// Sets a solenoid state immediately.
// If state == true, solenoid is switched on for an undefined _period.
bool SolenoidSet::setState(byte n, bool state) {
	if (n >= _size) return false;
	Solenoid& sol = _solenoid[n];
	sol.settingTime = millis();
	sol.swOnDelay = 0;
	sol.activePeriod = 0;
	if (sol.active == state) return false;
	uint32_t t = millis();
	_switch(n, state, t);
	return true;
}

bool SolenoidSet::getState(byte n) {
	if (n >= _size) return false;
	return _solenoid[n].active;
}

// sets a solenoid state, defining switch delay:
// solenoid state will switch after the specified delay.
 bool SolenoidSet::activateDelayed(byte n, uint16_t delay) {
	return activate(n, 0, delay);
}

// sets a solenoid state, defining switch delay and active _period:
// solenoid state will switch after the specified delay.
bool SolenoidSet::activate(byte n, uint16_t actPeriod, uint16_t delay) {
	//Serial.printf("SolenoidSet::activate(%d, %d, %d)\n", n, actPeriod, delay);
	if (n >= _size) return false;
	Solenoid& sol = _solenoid[n];
	sol.settingTime = millis();
	if (delay == 0) {
		uint32_t t = millis();
		_switch(n, true, t);
	}
	sol.activePeriod = (actPeriod == 0) ? Solenoid::DEF_ACTIVE_TIME : actPeriod;
	sol.swOnDelay = delay;
	return true;
}

// resets all solenoids
 void SolenoidSet::resetAll() {
	for (byte n = 0; n < _size; n++) setState(n, false);
}

// returns true on changes
bool SolenoidSet::update(uint32_t& t) {
	bool changes = false;
	bool changedState;
	Solenoid* sol;

	for (int i = 0; i < _size; i++) {
		sol = &_solenoid[i];
		changedState = false;
		if (sol->active) { // solenoid activated ?
			// check if solenoid is active for too long time
			if (sol->maxActivePeriod > 0 && (t - sol->swOnTime) > sol->maxActivePeriod) {
				_switch(i, false, t);
				changedState = true;
			}
			// check if solenoid activation time is over
			if (sol->activePeriod > 0 && (t - sol->swOnTime) > sol->activePeriod) {
				_switch(i, false, t);
				changedState = true;
			}
		}
		// check if solenoid should switch on after a delay (on active == false)
		else if ((sol->swOnDelay > 0) && (t - sol->settingTime) >= sol->swOnDelay) {
			_switch(i, true, t);
			sol->swOnDelay = 0;
			changedState = true;
		}

		if (changedState) {
			_lastChanged = i;
			changes = true;
		}
	}

	//for (int i = 0; i < _size; i++) {
	//	if (_checkSolenoid(i, t)) { // changed?
	//		_state->setState(i, solenoid[i].active); // updates solenoids bits
	//		_lastChanged = i;
	//		changes = true;
	//	}
	//}

	return changes;
}

// Returns a 32-bit word with actual solenoid states coded in its bits:
// bit n contains state of solenoid n.
uint32_t SolenoidSet::getStates() {
	uint32_t bits = 0;
	for (byte i = 0; i*8 < _size; i++) bits |= ((uint32_t)_state->getStates8(i) << (8 * i));
	return bits;
}

// Returns a 32-bit word with actual solenoid states coded in its bits:
// bit n contains state of solenoid n.
uint32_t SolenoidSet::getChangesBits() {
	uint32_t bits = 0;
	for (byte i = 0; i * 8 < _size; i++) bits |= ((uint32_t)_state->getChanges8(i) << (8 * i));
	return bits;
}

 void SolenoidSet::clearChanges() {
	_state->clearChanges();
}

 bool SolenoidSet::isActive(byte n) {
	return _solenoid[n].active;
}

 byte SolenoidSet::getLastChanged() {
	return _lastChanged;
}

 Solenoid& SolenoidSet::get(byte n) {
	 if (n >= _size) {
		 Serial.printf("SolenoidSet::get: Invalid solenoid #%d - using solenoid #0\n", n);
		 n = 0;
	 }
	 return _solenoid[n];
 }

 // Returns a string that represents the state of all solenoids.
 String SolenoidSet::toString() {
	 Solenoid* s;
	 String ret = "[";
	 for (int i = 0; i < _size; i++) {
		 s = &(_solenoid[i]);
		 if (s->active) {
			 if (s->activePeriod == 0) ret += "1";
			 else ret += "l";
		 }
		 else ret += "0";
	 }
	 ret += "]";
	 return ret;
 }
