#include "Sys80b_Actuator.h"

Sys80b_Actuator::Sys80b_Actuator(Sys80b* refSystem, OutputType t, byte n, const char* descrText) {
	_sys = refSystem;
	_type = t;
	num = n;
	descr = String(descrText);
};

Sys80b_Actuator::Sys80b_Actuator(Sys80b* refSystem, OutputType t, byte n, 
	const __FlashStringHelper* descrText) {
	_sys = refSystem;
	_type = t;
	num = n;
	descr = String(descrText);
};

bool Sys80b_Actuator::state() {
	switch (_type) {
	case SOLENOIDS:
		return _sys->getSolenoidState(num);
	case LAMPS:
		return _sys->readLamp(num);
	default:
		return false;
	}
};

void Sys80b_Actuator::set(bool state) {
	switch (_type) {
	case SOLENOIDS:
		_sys->setSolenoid(num, state);
		break;
	case LAMPS:
		_sys->setLamp(num, state);
		break;
	default:
		return;
	}
};

void Sys80b_Actuator::activate(uint16_t actPeriod, uint16_t swDelay) {
	if (_type == SOLENOIDS)	_sys->activateSolenoid(num, actPeriod, swDelay);
}

byte Sys80b_Actuator::getNumber() {
	return num;
}

String Sys80b_Actuator::getDescription() {
	return descr;
}