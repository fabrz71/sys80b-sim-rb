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

#ifndef _GameTimers_h
#define _GameTimers_h

#include "Arduino.h"
class GameTimerTask;

class GameTimers {
public:
	static const int MAX_TASKS = 20;
	String name;
protected:
	GameTimerTask* _first_timer;
	GameTimerTask* _last_timer;
	byte _count = 0;
	int _nextTaskId = 0;
	uint32_t _chkTime;
public:
	GameTimers();
	GameTimers* add(GameTimerTask& tt);
	GameTimers* add(GameTimerTask* tt);
	GameTimerTask* addNewTask(Game* g, GameFunct f, uint32_t ms, bool periodic = false, bool enabled = false);
	GameTimerTask* addNewTask(Game* g, GameFunct f, uint32_t ms, const char* name, bool enabled = false);
	//GameTimerTask* addNewTask(Game* g, void* f, uint32_t ms, bool periodic = false, bool enabled = false);
	//GameTimerTask* addNewTask(Game* g, void* f, uint32_t ms, const char* name, bool enabled = false);
	bool remove(GameTimerTask& tt);
	bool contains(GameTimerTask& tt);
	uint32_t update();
	byte getLength();
	void enableAllTasks();
	void disableAllTasks();
	//uint32_t getNextEventRemainingTime();
	void print();
protected:
	int nextTaskId();
};

#endif

