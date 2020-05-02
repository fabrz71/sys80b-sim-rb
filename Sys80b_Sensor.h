#ifndef _Sys80b_Sensor_h_
#define _Sys80b_Sensor_h_

#include "Arduino.h"
#include "Sys80b.h"
//class Sys80b;

class Sys80b_Sensor {
	Sys80b* _sys;
public:
	byte num;
	String descr;
	Sys80b_Sensor(Sys80b* refSystem, byte n, const char* descrText) : _sys(refSystem), num(n) {
		descr = String(descrText);
	};
	Sys80b_Sensor(Sys80b* refSystem, byte n, const __FlashStringHelper* descrText) :
		_sys(refSystem), num(n) {
		descr = String(descrText);
	};
	inline bool isClosed() { return _sys->readSwitch(num); }
	inline bool isOpen() { return !_sys->readSwitch(num); }
	inline byte Number() { return num; }
	inline String getDescription() { return descr; }
};

#endif
