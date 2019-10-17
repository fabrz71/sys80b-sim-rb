/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT OBJECT DEFINITION
* ---
* "Lights" are an abstraction of pinball real hardware "lamps".
* In order to make a light blink of pulse you have to
* call steadily update() function with a convenient time interval.
* Contains C++ code.
*/

#include "Light.h"

// constructor with default property values
Light::Light() {
	on = false;
	impulse = false;
	state = OFF_L;
	blinkPeriod = 500;
	blinkDutyCycle = 50;
	activePeriod = 0;
	effective = true;
}

// convenient funcion for copying a Light state into another
void Light::copy(Light &from) {
	on = from.on;
	state = from.state;
	//printf("%d %d\n", blinkPeriod, from.blinkPeriod);
	blinkPeriod = from.blinkPeriod;
	blinkDutyCycle = from.blinkDutyCycle;
	activationTime = from.activationTime;
	activePeriod = from.activePeriod;
}

// convenient funcion for copying a Light state into another
void Light::copy(Light *from) {
	on = from->on;
	state = from->state;
	//printf("%d %d\n", blinkPeriod, from.blinkPeriod);
	blinkPeriod = from->blinkPeriod;
	blinkDutyCycle = from->blinkDutyCycle;
	activationTime = from->activationTime;
	activePeriod = from->activePeriod;
}

// switch off light and reset its properties
void Light::reset() {
	on = false;
	state = OFF_L;
	blinkPeriod = 500; // ON+OFF (ms)
	blinkDutyCycle = 50; // 0-100 % ON time
	activePeriod = 0; // (ms)
}

// define the state of the light
inline void Light::set(lightState st) {
	set(st, (st == BLINK_L) ? DEF_BLNK_PERIOD : 0, 50);
}

// define the state of the light
// st: light state
// blinkP: blink period (on + off) [milliseconds]
inline void Light::set(lightState st, uint16_t blinkP) {
	set(st, blinkP, 50);
}

void Light::set(lightState st, uint16_t blinkP, byte blinkDutyC) {
	switch (st) {
	case OFF_L:
		on = false;
		break;
	case ON_L:
		on = true;
		break;
	case BLINK_L:
		if (blinkP == 0) state = ON_L;
		on = true;
		break;
	default:
		return;
	}
	state = st;
	if (on) activationTime = millis();
	blinkPeriod = blinkP;
	if (blinkDutyC == 0) blinkDutyCycle = 50;
	else blinkDutyCycle = blinkDutyC;
	activePeriod = 0; // unlimited state time (default)
}

inline void Light::blink(uint16_t blinkP, byte ticks) {
	set(BLINK_L, blinkP, 50);
	activePeriod = blinkP * ticks;
}

// inverts light ON/OFF state
void Light::invert() {
	if (state == ON_L) state = OFF_L;
	else if (state == OFF_L) state = ON_L;
}

void Light::pulse(uint16_t pulseP) {
	pulsePeriod = pulseP;
	impulse = true;
	pulseTime = millis();
	on = !on;
}

void Light::update(uint32_t ms) {
	uint32_t t; // blink ON time

				// blink mode
	if (state == BLINK_L) {
		t = (ms - activationTime) % (uint32_t)blinkPeriod;
		on = ((t * 100 / blinkPeriod) < blinkDutyCycle);
	}

	// pulse state
	if (impulse) {
		if ((ms - pulseTime) > pulsePeriod) {
			impulse = false;
			on = !on;
		}
	}

	// checks light active time
	if (on) {
		if (activePeriod > 0 && (ms - activationTime) >= (uint32_t)activePeriod) {
			activePeriod = 0; // resets previous active-time setting
			state = OFF_L;
		}
	}
}

inline bool Light::isActive() {
    return (state != OFF_L); 
}
