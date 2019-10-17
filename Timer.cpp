#include "Timer.h"

Timer::Timer() {
	enabled = false;
	_first_timer = NULL;
	_last_timer = NULL;
	_count = 0;
}

Timer::~Timer() {
	TimerClient *tc, *next;

	enabled = false;
	if (_count == 0) return;
	tc = _first_timer;
	do {
		next = tc->next;
		delete tc;
		tc = next;
	} while (next != NULL);
	_first_timer = NULL;
	_last_timer = NULL;
	_count = 0;
}

bool Timer::add(TimerTarget *tt) {
	TimerClient *tc;

	Serial.print("Adding timer task... ");
	if (tt == NULL) {
		Serial.println("abort: can't add null timer!");
		return false;
	}
	//tt->print();
	if (_count >= MAX_TASKS - 1) {
		Serial.println("abort: max number of TimerTasks reached.");
		return false;
	}
	tc = new TimerClient(tt); 
	if (_count++ == 0) _first_timer = tc;
	if (_last_timer != NULL) _last_timer->next = tc;
	_last_timer = tc;
	tc->next = NULL;
	Serial.print(" (_count: ");
	Serial.print(_count);
	Serial.println(")");
	return true;
}

// removes a timeTask object in the setPeriod
bool Timer::remove(TimerTarget *ttask) {
	//TimerTask *prevtt, *tt;
	TimerClient *prevtc, *tc;

	if (_count == 0) return false; // empty list

	// find the TimerClient that refers to TimerTarget <tt>
	prevtc = NULL;
	tc = _first_timer;
	while (tc != NULL) {
		if (tc->target == ttask) break;
		else prevtc = tc;
		tc = tc->next;
	}
	if (tc == NULL) return false; // not in list! (or list empty)
	
	if (prevtc == NULL) _first_timer = tc->next; // first in list
	else prevtc->next = tc->next; // could be NULL
	if (tc->next == NULL) _last_timer = prevtc; // last in list
	tc->disable(); // disable removed TimerTask
	delete tc;
	_count--;
	if (_count == 0) _first_timer = NULL;
	return true;
}

bool Timer::contains(TimerTarget *ttask) {
	TimerClient *tc;

	if (_count == 0) return false; // empty list
	tc = _first_timer;
	while (tc != NULL) {
		if (tc->target == ttask) break;
		tc = tc->next;
	}
	return (tc != NULL);
}

uint32_t Timer::checkTimerTasks() {
	//TimerTarget *tt;
	TimerClient *tc;
	uint32_t rt, nextEventT;

	nextEventT = 60000;
	if (!enabled) return nextEventT;
	if (_count == 0) return 0;
	_lastChkTime = millis();
	for (tc = _first_timer; tc != NULL; tc = tc->next) {
		rt = tc->check(_lastChkTime);
		if (rt < nextEventT) nextEventT = rt;
	}
	return nextEventT;
}

void Timer::enableAllTimers() {
	TimerClient *tc;

	if (_count == 0) return;
	for (tc = _first_timer; tc != NULL; tc = tc->next) tc->enable();
}

void Timer::disableAllTimers() {
	TimerClient *tc;

	if (_count == 0) return;
	for (tc = _first_timer; tc != NULL; tc = tc->next) tc->disable();
}

void Timer::print() {
	TimerClient *tt;
	//uint32_t nextEventT;

	Serial.print("(");
	Serial.print(_count);
	Serial.print("):");
	if (_count == 0) {
		Serial.println("<no timers!>");
		return;
	}
	_lastChkTime = millis();
	for (tt = _first_timer; tt != NULL; tt = tt->next) {
		tt->print();
	}
	Serial.println("_");
}

inline byte Timer::getLength() {
	return _count;
}
