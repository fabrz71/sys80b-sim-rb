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

#ifndef _TimerSet_h
#define _TimerSet_h

#include "Arduino.h"
#include "TimerTask.h"
//#define MAX_TASKS 20

class TimerSet {
public: 
	static const int MAX_TASKS = 20;
	String name;
protected:
	TimerTask* _first_timer;
	TimerTask* _last_timer;
	byte _count;
	int _nextTaskId;
	uint32_t _chkTime;
public:
	TimerSet();
	//~TimerSet();
	//bool add(TimerTask& tt);
	int add(TimerTask& tt);
	int add(TimerTask* tt);
	//TimerTask* addNewTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic, byte taskId = 0);
	//TimerTask* addNewTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, byte taskId = 0);
	int addNewTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic = true, bool enabled = false);
	int addNewTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, bool enabled = false);
	bool remove(TimerTask& tt);
	bool contains(TimerTask& tt);
	uint32_t update(uint32_t& tm);
	//inline byte getLength() { return _count; }
	TimerTask* getTask(int id);
	byte getLength();
	bool enableTask(int id);
	bool disableTask(int id);
	void enableAllTasks();
	void disableAllTasks();
	//uint32_t getNextEventRemainingTime();
	void print();
protected:
	int nextTaskId();
};

#endif
