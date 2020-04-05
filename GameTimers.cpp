/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* GameTimers OBJECT DEFINITION
* ---
* "GameTimers" is a collection of GameTimerTask objects that helps to manage different tasks to be executed periodically
* from main execution cycle.
* Contains C++ code.
*/

#include "GameTimers.h"

#include "GameTimerTask.h"

GameTimers::GameTimers() {
	_first_timer = NULL;
	_last_timer = NULL;
	_count = 0;
}

GameTimers* GameTimers::add(GameTimerTask& tt) {
	Serial.print("Adding timer... ");
	if (&tt == NULL) {
		Serial.println("abort: can't add null GameTimerTask!");
		return this;
	}
	if (contains(tt)) {
		Serial.println("abort: can't add same GameTimerTask!");
		return this;
	}
	//tt->print();
	if (_count >= MAX_TASKS - 1) {
		Serial.println("abort: max number of TimerTasks reached.");
		return this;
	}
	if (_count++ == 0) _first_timer = &tt;
	if (_last_timer != NULL) _last_timer->_next = &tt;
	_last_timer = &tt;
	tt._next = NULL;
	Serial.print(" (_count: ");
	Serial.print(_count);
	Serial.print(", id: ");
	Serial.print(_nextTaskId);
	Serial.println(")");
	return this;
}

GameTimers* GameTimers::add(GameTimerTask* tt) {
	return add(*tt);
}

//GameTimerTask* GameTimers::addNewTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic, byte taskId) {
//	GameTimerTask* tt = new GameTimerTask(obj, ms, periodic, taskId);
//	return add(*tt) ? tt : NULL;
//}
//
//GameTimerTask* GameTimers::addNewTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, byte taskId) {
//	GameTimerTask* tt = new GameTimerTask(obj, ms, name, taskId);
//	return add(*tt) ? tt : NULL;
//}

GameTimerTask* GameTimers::addNewTask(Game* g, GameFunct f, uint32_t ms, bool periodic, bool enabled) {
	GameTimerTask* tt = new GameTimerTask(g, f, ms, periodic, enabled);
	add(*tt);
	return tt;
}

GameTimerTask* GameTimers::addNewTask(Game* g, GameFunct f, uint32_t ms, const char* name, bool enabled) {
	GameTimerTask* tt = new GameTimerTask(g, f, ms, name, enabled);
	add(*tt);
	return tt;
}

// removes a timeTask object in the setPeriod
bool GameTimers::remove(GameTimerTask& ttask) {
	GameTimerTask* prevtt, *tt;

	if (_count == 0) return false; // empty list
	prevtt = NULL;
	tt = _first_timer;
	while (tt != NULL) {
		if (tt == &ttask) break;
		else prevtt = tt;
		tt = tt->_next;
	}
	if (tt == NULL) return false;// not in list!
	if (prevtt == NULL) _first_timer = tt->_next; // first in list
	else prevtt->_next = tt->_next; // could be NULL
	if (tt->_next == NULL) _last_timer = prevtt; // last in list
	tt->disable(); // disable removed GameTimerTask
	_count--;
	return true;
}

bool GameTimers::contains(GameTimerTask& ttask) {
	GameTimerTask* tt;

	if (_count == 0) return false; // empty list
	tt = _first_timer;
	while (tt != NULL) {
		if (tt == &ttask) return true;
		tt = tt->_next;
	}
	return false;
}

uint32_t GameTimers::update() {
	GameTimerTask* tt;
	uint32_t rt, nextEventT;

	nextEventT = 60000;
	if (_count == 0) return 0;
	_chkTime = millis();
	for (tt = _first_timer; tt != NULL; tt = tt->_next) {
		rt = tt->update(_chkTime);
		if (rt < nextEventT) nextEventT = rt;
	}
	return nextEventT;
}

void GameTimers::enableAllTasks() {
	GameTimerTask* tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != NULL; tt = tt->_next) tt->enable();
}

void GameTimers::disableAllTasks() {
	GameTimerTask* tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != NULL; tt = tt->_next) tt->disable();
}

/*
uint32_t GameTimers::getNextEventRemainingTime() {
GameTimerTask *tt;
uint32_t now, dt, smallest;

if (_count == 0) return 1000;
now = millis();
for (tt = _first_timer; tt != NULL; tt = tt->next) {
dt = tt->getRemainingTime(now);
if (tt == _first_timer || dt < smallest) smallest = dt;
}
return smallest;
}
*/

void GameTimers::print() {
	GameTimerTask* tt;
	//uint32_t nextEventT;

	Serial.print("(");
	Serial.print(_count);
	Serial.print("):");
	if (_count == 0) {
		Serial.println("<no timers!>");
		return;
	}
	_chkTime = millis();
	for (tt = _first_timer; tt != NULL; tt = tt->_next) {
		tt->print();
	}
	Serial.println("_");
}

int GameTimers::nextTaskId() {
	if (_nextTaskId == INT32_MAX) {
		_nextTaskId = 0;
		return INT32_MAX;
	}
	return _nextTaskId++;
}

inline byte GameTimers::getLength() {
	return _count;
}
