/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMER SET OBJECT DEFINITION
* ---
* "TimerSet" is a collection of TimerTask objects that helps to manage different tasks to be executed periodically
* from main execution cycle.
* Contains C++ code.
*/

//#pragma once

#ifndef _TimerSet_h
#define _TimerSet_h

//#include "Arduino.h"

#define MAX_TASKS 20

class TimerTask;

class TimerSet {
protected:
	TimerTask *_first_timer;
	TimerTask *_last_timer;
	byte _count = 0;
	uint32_t _chkTime;
public:
	TimerSet();
	//~TimerSet();
	bool add(TimerTask *tt);
	bool rmv(TimerTask *tt);
	bool contains(TimerTask *tt);
	uint32_t checkTimerTasks();
	//inline byte getLength() { return _count; }
	byte getLength();
	void enableAllTimers();
	void disableAllTimers();
	//uint32_t getNextEventRemainingTime();
	void print();
};

#endif
