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
#include "TimerTask.h"

TimerSet::TimerSet() {
	_first_timer = NULL;
	_last_timer = NULL;
	_count = 0;
}

//TimerSet::~TimerSet()
//{
//}

bool TimerSet::add(TimerTask *tt) {
	Serial.print("Adding timer... ");
	if (tt == NULL) {
		Serial.println("abort: can't add null timer!");
		return false;
	}
	//tt->print();
	if (_count >= MAX_TASKS - 1) {
		Serial.println("abort: max number of TimerTasks reached.");
		return false;
	}
	if (_count++ == 0) _first_timer = tt;
	if (_last_timer != NULL) _last_timer->_next = tt;
	_last_timer = tt;
	tt->_next = NULL;
	Serial.print(" (_count: ");
	Serial.print(_count);
	Serial.println(")");
	return true;
}

// removes a timeTask object in the setPeriod
bool TimerSet::rmv(TimerTask *ttask) {
	TimerTask *prevtt, *tt;

	if (_count == 0) return false; // empty list
	prevtt = NULL;
	tt = _first_timer;
	while (tt != NULL) {
		if (tt == ttask) break;
		else prevtt = tt;
		tt = tt->_next;
	}
	if (tt == NULL) return false;// not in list!
	if (prevtt == NULL) _first_timer = tt->_next; // first in list
	else prevtt->_next = tt->_next; // could be NULL
	if (tt->_next == NULL) _last_timer = prevtt; // last in list
	tt->enabled = false; // disable removed TimerTask
	_count--;
	return true;
}

bool TimerSet::contains(TimerTask *ttask) {
	TimerTask *tt;

	if (_count == 0) return false; // empty list
	tt = _first_timer;
	while (tt != NULL) {
		if (tt == ttask) return true;
		tt = tt->_next;
	}
	return false;
}

uint32_t TimerSet::checkTimerTasks() {
	TimerTask *tt;
	uint32_t rt, nextEventT;

	nextEventT = 60000;
	if (_count == 0) return 0;
	_chkTime = millis();
	for (tt = _first_timer; tt != NULL; tt = tt->_next) {
		rt = tt->check(_chkTime);
		if (rt < nextEventT) nextEventT = rt;
	}
	return nextEventT;
}

void TimerSet::enableAllTimers() {
	TimerTask *tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != NULL; tt = tt->_next) tt->enable();
}

void TimerSet::disableAllTimers() {
	TimerTask *tt;

	if (_count == 0) return;
	for (tt = _first_timer; tt != NULL; tt = tt->_next) tt->disable();
}

/*
uint32_t TimerSet::getNextEventRemainingTime() {
TimerTask *tt;
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
	for (tt = _first_timer; tt != NULL; tt = tt->_next) {
		tt->print();
	}
	Serial.println("_");
}

inline byte TimerSet::getLength() { 
	return _count; 
}