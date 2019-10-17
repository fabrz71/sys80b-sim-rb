/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* SYSTEM 80/B I/O FUNCTIONS
* ---
* Defines a System 80/B architecture, keeping states of
* solenoids, lamps, sound buffer, display and switches.
* Implements higher level System 80/B I/O functions but does not
* directly act on hardware:
* All input/output commands are issued exclusively through
* BoardIO::setOuput() and BoardIO::getInput() functions, and would
* be effectively executed only when the class instance is pointed by
* BoardIO::connectedSystem.
*/

#ifndef _Sys80b_h_
#define _Sys80b_h_

//#include "Arduino.h"

#include "SoundBuff.h"
#include "LampSet.h"
#include "SolenoidSet.h"
#include "Display.h"
#include "SwitchGrid8x8.h"
#include "BoardIO.h"

class Sys80b : public TimerTarget {
public:
	static const int LAMPS_COUNT = 48;
	static const int SOLENOID_COUNT = 9;
	SolenoidSet solenoids; // real solenoids (0..9) (0 = unused)
	SolenoidSet lampSolenoids; // "lamp" solenoids (16..31)
	LampSet lamps;
	SoundBuff sound;
	Display displ;
	SwitchGrid8x8 switchGrid;
protected:
	byte updLampGroup = 0;
	//std::queue<uint16_t> soundCmdQueue;

public:
		Sys80b();

		void reset();
		void setSolenoid(byte n, bool active);
		void activateSolenoid(byte n, uint16_t actPeriod);
		void activateSolenoidDelayed(byte n, uint16_t swDelay);
		void activateSolenoidDelayed(byte n, uint16_t actPeriod, uint16_t swDelay);
		void checkSolenoids(uint32_t t);
		void setLamp(byte n, bool state);
		void setLamp(byte n, byte state);
		void set4Lamps(byte set, byte states);
		bool readLamp(byte n);
		void resetLamps();
		bool updateLights();
		void setSound(byte snd);
		void checkSoundCmd(uint32_t t);
		void setStrobe(byte n);
		bool readSwitch(byte swNum);
		byte getNextReturns(uint32_t t);
		void feedDisplay();
		bool isSpecialLamp(byte lamp);
		void setQrelay(bool s);
		void setTrelay(bool s);
		void setArelay(bool s);
		void timerFunction(uint32_t tm, int id); // implements TimerTarget interface
};

#endif

