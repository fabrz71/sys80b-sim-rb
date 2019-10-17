#ifndef _PinHW_h_
#define _PinHW_h_

//#include "Arduino.h"

#include "SoundBuff.h"
#include "LampSet.h"
#include "SolenoidSet.h"
#include "Display.h"
#include "SwitchGrid8x8.h"

class PinHW
{
public:
	SolenoidSet solenoids; // real solenoids (0..9) (0 = unused)
	LampSet lamps;
	Display displ;
	SwitchGrid8x8 switchGrid;

public:
	//PinHW();

	void reset();
	void setSolenoid(byte n, bool active);
	void activateSolenoid(byte n, uint16_t actPeriod);
	void activateSolenoidDelayed(byte n, uint16_t swDelay);
	void activateSolenoidDelayed(byte n, uint16_t actPeriod, uint16_t swDelay);
	void updateSolenoids(uint32_t t);

	void setLamp(byte n, bool state);
	void setLamp(byte n, byte state);
	bool readLamp(byte n);
	void resetLamps();
	bool updateLights();

	void setSound(byte snd);
	void checkSoundCmd(uint32_t t);

	void setStrobe(byte n);
	byte getReturns(uint32_t t);
	bool readSwitch(byte swNum);
};

#endif
