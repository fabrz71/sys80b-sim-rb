#include "Solenoid.h"

Solenoid::Solenoid()
{
	set(false);
	maxActivePeriod = 0;
	effective = true;
}

Solenoid::Solenoid(uint16_t maxActivePeriod) {
	set(false);
	this->maxActivePeriod = maxActivePeriod;
	effective = true;
}

inline bool Solenoid::isActive() {
	return active;
}

// sets solenoid immediate state, updating internal variables
void Solenoid::set(bool state) {
	active = state;
	setTime = millis();
	if (state) {
		swOnTime = setTime;
		swOnDelay = 0;
	}
	else swOffTime = setTime;
}

// sets a solenoid state, defining active period.
// actPeriod ignored when active = false
void Solenoid::activate(uint16_t actPeriod) {
	set(true);
	activePeriod = actPeriod;
}

// sets a solenoid state, defining switch-on delay:
// swDelay ignored when active = false.
void Solenoid::activateDelayed(uint16_t swDelay) {
	set(true);
	swOnDelay = swDelay;
}

// sets a solenoid state, defining switch delay:
// solenoid state will be setPeriod after the specified delay.
void Solenoid::activateDelayed(uint16_t actPeriod, uint16_t swDelay) {
	set(true);
	activePeriod = actPeriod;
	swOnDelay = swDelay;
}

// at given process current time t, checks whether:
// - delay activation time has elapsed (when delayedSwitch = true)
// - safe maximum active time has elapsed
// returns: true if solenoid state has been changed
bool Solenoid::check(uint32_t t) {
	if (active) { // solenoid activated ?
		if (maxActivePeriod > 0 && t - swOnTime > maxActivePeriod) {
			active = false;
			swOffTime = t;
			return true;
		}
		if (activePeriod > 0 && t - swOnTime > activePeriod) {
			active = false;
			swOffTime = t;
			return true;
		}
	}
	else if (swOnDelay > 0) { // delayed switch (active == false)
		if (t - setTime >= swOnDelay) {
			active = true;
			swOnTime = t;
			return true;
		}
	}
	return false;
}