/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMERSET OBJECT DEFINITION
* ---
* "TimerSet" is a collection of TimerTask objects that helps to manage different tasks to be executed periodically
* from main execution cycle.
* Contains C++ code.
*/

#include "TimerSet.h"

TimerSet::TimerSet() {
	_first_timer = nullptr;
	_last_timer = nullptr;
	_count = 0;
	_nextTaskId = 0;
}

//TimerSet::~TimerSet()
//{
//}

//bool TimerSet::add(TimerTask& tt) {
//	Serial.print("Adding timer... ");
//	if (&tt == nullptr) {
//		Serial.println("abort: can't add null timer!");
//		return false;
//	}
//	//tt->print();
//	if (_count >= MAX_TASKS - 1) {
//		Serial.println("abort: max number of TimerTasks reached.");
//		return false;
//	}
//	if (_count++ == 0) _first_timer = &tt;
//	if (_last_timer != nullptr) _last_timer->_next = &tt;
//	_last_timer = &tt;
//	tt._next = nullptr;
//	Serial.print(" (_count: ");
//	Serial.print(_count);
//	Serial.println(")");
//	return true;
//}

int TimerSet::add(TimerTask& tt) {
	//Serial.print("Adding timer... ");
	if (&tt == nullptr) {
		Serial.println(F("! Can't add null timertask!"));
		return -1;
	}
	if (contains(tt)) {
		Serial.println(F("! Can't add same timertask!"));
		return -2;
	}
	//tt->print();
	if (_count >= MAX_TASKS - 1) {
		Serial.println(F("! Max number of TimerTasks reached."));
		return -3;
	}
	if (_count++ == 0) _first_timer = &tt;
	if (_last_timer != nullptr) _last_timer->_next = &tt;
	_last_timer = &tt;
	tt._next = nullptr;
	tt.id = _nextTaskId;
	//Serial.print(" (_count: ");
	//Serial.print(_count);
	//Serial.print(", id: ");
	//Serial.print(_nextTaskId);
	//Serial.println(")");
	return nextTaskId();
}

int TimerSet::add(TimerTask* tt) {
	return add(*tt);
}

//TimerTask* TimerSet::addNewTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic, byte taskId) {
//	TimerTask* tt = new TimerTask(obj, ms, periodic, taskId);
//	return add(*tt) ? tt : nullptr;
//}
//
//TimerTask* TimerSet::addNewTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, byte taskId) {
//	TimerTask* tt = new TimerTask(obj, ms, name, taskId);
//	return add(*tt) ? tt : nullptr;
//}

int TimerSet::addNewTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic, bool enabled) {
	TimerTask* tt = new TimerTask(obj, ms, periodic, enabled); 
	return add(*tt);
}

int TimerSet::addNewTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, bool enabled) {
	TimerTask* tt = new TimerTask(obj, ms, name, enabled);
	return add(*tt);
}

// removes a timeTask object in the setPeriod
bool TimerSet::remove(TimerTask& ttask) {
	TimerTask *prevtt, *tt;

	if (_count == 0) return false; // empty list
	prevtt = nullptr;
	tt = _first_timer;
	while (tt != nullptr) {
		if (tt == &ttask) break;
		else prevtt = tt;
		tt = tt->_next;
	}
	if (tt == nullptr) return false;// not in list!
	if (prevtt == nullptr) _first_timer = tt->_next; // first in list
	else prevtt->_next = tt->_next; // could be nullptr
	if (tt->_next == nullptr) _last_timer = prevtt; // last in list
	!tt->isEnabled(); // disable removed TimerTask
	_count--;
	return true;
}

bool TimerSet::contains(TimerTask& ttask) {
	TimerTask *tt;

	if (_count == 0) return false; // empty list
	tt = _first_timer;
	while (tt != nullptr) {
		if (tt == &ttask) return true;
		tt = tt->_next;
	}
	return false;
}

TimerTask* TimerSet::getTask(int id) {
	if (_count == 0) return nullptr; // empty list
	TimerTask* tt = _first_timer;
	while (tt != nullptr) {
		if (tt->id == id) return tt;
		tt = tt->_next;
	}
	return nullptr;
}

// checks all the registered Timertask
// returns: nearest next event time (ms)
uint32_t TimerSet::update(uint32_t& ms) {
	TimerTask *tt;
	uint32_t rt, nextEventT;

	nextEventT = UINT32_MAX;
	if (_count > 0) {
		_chkTime = ms;
		for (tt = _first_timer; tt != nullptr; tt = tt->_next) {
			rt = tt->update(_chkTime);
			if (rt < nextEventT) nextEventT = rt;
		}
	}
	return nextEventT;
}

bool TimerSet::enableTask(int id) {
	TimerTask* tt = getTask(id);
	if (tt == nullptr) return false;
	tt->enable();
	return true;
}

bool TimerSet::disableTask(int id) {
	TimerTask* tt = getTask(id);
	if (tt == nullptr) return false;
	tt->disable();
	return true;
}

void TimerSet::enableAllTasks() {
	TimerTask *tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != nullptr; tt = tt->_next) tt->enable();
}

void TimerSet::disableAllTasks() {
	TimerTask *tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != nullptr; tt = tt->_next) tt->disable();
}

/*
uint32_t TimerSet::getNextEventRemainingTime() {
TimerTask *tt;
uint32_t now, dt, smallest;

if (_count == 0) return 1000;
now = millis();
for (tt = _first_timer; tt != nullptr; tt = tt->next) {
dt = tt->getRemainingTime(now);
if (tt == _first_timer || dt < smallest) smallest = dt;
}
return smallest;
}
*/

void TimerSet::print() {
	TimerTask *tt;
	//uint32_t nextEventT;

	Serial.print("(");
	Serial.print(_count);
	Serial.print("):");
	if (_count == 0) {
		Serial.println("<no timers!>");
		return;
	}
	_chkTime = millis();
	for (tt = _first_timer; tt != nullptr; tt = tt->_next) {
		tt->print();
	}
	Serial.println("_");
}

int TimerSet::nextTaskId() {
	if (_nextTaskId == INT32_MAX) {
		_nextTaskId = 0;
		return INT32_MAX;
	}
	return _nextTaskId++;
}

inline byte TimerSet::getLength() { 
	return _count; 
}
