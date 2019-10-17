/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMER OBJECT
* ---
*
*
*/

#ifndef _Timer_h_
#define _Timer_h_

#include "TimerClient.h"

//class TimerTarget {
//public:
//	virtual void timerFunction(uint32_t tm) = 0;
//};

class Timer {
public:
	bool enabled;
protected:
	TimerClient *_first_timer; // pointer to first TimerTarget in a list
	TimerClient *_last_timer;
	byte _count = 0;
	uint32_t _lastChkTime;
public:
	Timer();
	~Timer();
	bool add(TimerClient *tt);
	bool remove(TimerClient *tt);
	bool contains(TimerClient *tt);
	uint32_t checkTimerTasks();
	//inline byte getLength() { return _count; }
	byte getLength();
	void enableAllTimers();
	void disableAllTimers();
	//uint32_t getNextEventRemainingTime();
	void print();
};

#endif