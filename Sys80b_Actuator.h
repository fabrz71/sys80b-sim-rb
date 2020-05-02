#ifndef _Sys80b_Actuator_h_
#define _Sys80b_Actuator_h_

#include "Arduino.h"

//#include "Sys80bIO.h"
#include "Sys80b.h"

//using namespace Sys80bIO;

class Sys80b_Actuator {
	Sys80b* _sys;
	OutputType _type;
public:
	byte num;
	String descr;

	Sys80b_Actuator(Sys80b* refSystem, OutputType t, byte n, const char* descrText);
	Sys80b_Actuator(Sys80b* refSystem, OutputType t, byte n, const __FlashStringHelper* descrText);
	bool state();
	void set(bool state);
	void activate(uint16_t actPeriod = 0, uint16_t swDelay = 0);
	byte Number();
	String getDescription();
};

#endif
