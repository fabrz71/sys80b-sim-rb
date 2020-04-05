/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

//rev: jan/20

#ifndef _Sys80b_h_
#define _Sys80b_h_

#include "Arduino.h"

#include "Board_Sys80b.h"
#include "BitMappedSet.h"
#include "Sys80b_Display.h"
#include "SwitchGrid8x8.h"
#include "Buffer.h"
#include "SolenoidSet.h"

//class Msg;

const byte credits_ratio[25] PROGMEM = { 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,1,3,1 };
const byte coins_ratio[25] PROGMEM = { 1,1,1,1,1,1,1,1,1, 1,2,2,2,2,2,2,2,2,2, 2,3,3,4,4,5 };
const byte max_credits[] PROGMEM = { 8, 10, 15, 20 };

enum StdSwitch { LEFTADV_SW = 6, TEST_SW = 7, RIGHTADV_SW = 16, LEFTCOIN_SW = 17, 
	RIGHTCOIN_SW = 27, CENTERCOIN_SW = 37, REPLAY_SW = 47, TILT_SW = 57 };

enum InputType { RETURNS, SLAM_SW };
enum OutputType { STROBES, SOLENOIDS, SOUND, LAMPS, DISPL };

enum StdSolenoid { HOLE_SOL = 1, KNOCKER_SOL = 8, OUTHOLE_SOL = 9 };

enum StdLamp { Q_LAMP = 0, T_LAMP = 1 };

enum UserKey { NO_KEY, REPLAY_KEY, LEFTADV_KEY, RIGHTADV_KEY };

// System 80/B architecture, keeping states of
// solenoids, lamps, sound buffer, display and switches.
// Implements System 80/B I/O main functions.
class Sys80b {

public:
	const __FlashStringHelper* CLASS_NAME = F("Sys80b");
	static const byte LAMPS_COUNT = 48;
	static const byte SOLENOID_COUNT = 9;
	static const uint16_t SOL_DEF_TIME = 500;
	static const byte SND_BUFF_SIZE = 10;

	static const uint16_t KEY_REPEAT_TIMER = 1000u;
	static const uint16_t KEY_REPEAT_PERIOD = 200u;

	//BitMappedSet* solenoids; // real solenoids (0..9) (0 = unused)
	BitMappedSet* lamps; // low-level lamps state
	SolenoidSet* solenoids; // standard solenoids
	//SolenoidSet* lampSolenoids; // "lamp" solenoids (16..31)
	Sys80b_Display display;
	SwitchGrid8x8 switchGrid;
	Buffer<uint16_t, SND_BUFF_SIZE> soundBuffer;
	byte currentLampGroup;
	bool slamSwitchNormalState; // true = Normally Open; false = Normally Closed
	bool slamSwitchLastState;

protected:
	byte _returnsInput;
	bool _soundPendingCmd;
	//byte _changedSwitchNum;
	//bool _changedSwitchState;
	//bool _slamSwitchAlarm;
	UserKey _keyPressed;
	bool _keyRepeat;
	uint32_t _keyStartTime;
	Board_Sys80b* hw;
	Msg* msg;

public:
	Sys80b(Board_Sys80b& board);
	virtual ~Sys80b();

	//inline void connectHardware(Board_Sys80b& board) { hw = &board; msg = &(board.msg); }
	void reset();
	//void setSlamSwitchNormalState(bool normalState);
	byte getBallsCount();
	void setSolenoid(byte n, bool active);
	inline bool getSolenoidState(byte n) { return solenoids->getState(n); }
	inline void setTiltRelay(bool st) { setLamp(T_LAMP, st); }
	inline void setOnGameRelay(bool st) { setLamp(Q_LAMP, st); }
	void activateSolenoid(byte n, uint16_t actPeriod = SOL_DEF_TIME, uint16_t swDelay = 0);
	void checkSolenoids(uint32_t& t);
	// setPeriod a lamp on or off, with status memory
	inline void setLamp(byte n, bool state, bool forceProtection = false) { 
		lamps->setState(n, state, forceProtection); 
	}
	// setPeriod a lamp on or off, with status memory
	inline void setLamp(byte n, byte state, bool forceProtection = false) { 
		lamps->setState(n, state > 0, forceProtection); 
	}
	inline bool readLamp(byte n) {	return lamps->getState(n); }
	inline void switchOffAllLamps() { lamps->setAllStates(false); }
	void setSound(byte snd);
	inline bool soundCmdPending() {	return _soundPendingCmd; }
	inline bool readSwitch(byte swNum) { return switchGrid.getSwitchState(swNum); }
	inline bool readSlamSwitch() { return hw->readSlamSw(); }
	byte incrementStrobe();
	bool acquireReturns(uint32_t& t);
	// returns last changed switch number, 0xff in case of no changes
	inline byte getChangedSwitchNum() { return switchGrid.getLastChangedSwitch(); };
	// returns last changed switch new state
	inline byte getChangedSwitchState() { return switchGrid.getLastChangedSwitchState(); }
	inline byte getCurrentReturns() { return _returnsInput; }
	void feedDisplay();
	bool renderNextLampsGroup();
	bool renderLampsGroup(byte lg);
	bool renderLamp(byte l) { return renderLampsGroup(l >> 2); }
	inline void setQrelay(bool s) {	setLamp(Q_LAMP, s);	}
	inline void setTrelay(bool s) {	setLamp(T_LAMP, s);	}
	inline byte getStrobeOutput() {	return switchGrid.getCurrentStrobe(); }
	inline byte get4LampGroupOutput() {	return currentLampGroup; }
	inline byte get4LampStatesOutput() { return lamps->getStates4(currentLampGroup); }
	inline uint32_t getSolenoidsOutput() { return solenoids->getStates(); }
	virtual void updateSound();
	inline uint16_t getDisplayOutput() { return display.LastData();	}
	void updUserKeyState(UserKey key, uint32_t& ms);
	void _millisRoutine(uint32_t& ms);
	virtual void onSwitchEvent(byte sw, bool st) {};
	virtual void onSlamSwitchEvent(bool active) {};
	virtual void onButtonPressed(UserKey key) { 
		Serial.print("onButtonPressed: "); 
		Serial.println(key);
	}
	virtual void onTestButtonPressed() {}

protected:
	void _onSwitchEvent(byte sw, bool st, uint32_t& ms); // calls onSwitchEvent()
	void _checkPressedKey(uint32_t& ms);
};

#endif
