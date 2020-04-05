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
	//updated = true;
	//_changed = false;
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
	impulse = from->impulse;
	pulseTime = from->pulseTime;
	pulsePeriod = from->pulsePeriod;
}

// switch off light and reset its properties
void Light::reset() {
	on = false;
	state = OFF_L;
	blinkPeriod = 500; // ON+STATIC_TEXT (ms)
	blinkDutyCycle = 50; // 0-100 % ON time
	activePeriod = 0; // (ms)
}

void Light::set(lightState st, uint16_t blinkP, byte blinkDutyC, uint16_t activeT) {
	on = (st != OFF_L);
	if (st == ON_L && st != state) {
		activationTime = millis();
		activePeriod = activeT; // unlimited state time (default)
	}
	if (st == BLINK_L) {
		blinkPeriod = (blinkP == 0) ? DEF_BLNK_PERIOD : blinkP;
		blinkDutyCycle = (blinkDutyC == 0) ? 50 : blinkDutyC;
	}
	state = st;
}

void Light::blink(uint16_t blinkP, byte blinkDutyC, byte ticks) {
	set(BLINK_L, blinkP, blinkDutyC);
	activePeriod = blinkP * ticks;
}

// inverts light ON/OFF static state
void Light::invert() {
	if (state == ON_L) set(OFF_L);
	else if (state == OFF_L) set(ON_L);
}

void Light::pulse(uint16_t pulseP) {
	pulsePeriod = pulseP;
	impulse = true;
	pulseTime = millis();
	on = !on;
}

// return true when light changes its on/off state
bool Light::update(uint32_t ms) {
	bool prevState = on;

	// blink mode (ON/STATIC_TEXT iteration)
	if (state == BLINK_L) {
		uint32_t t; // blink ON time
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
			on = false;
		}
	}

	return (on != prevState);
}
