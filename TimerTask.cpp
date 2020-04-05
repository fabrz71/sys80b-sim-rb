/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMER TASK OBJECT DEFINITION
* ---
* "TimerTask" helps to manage different tasks to be executed periodically
* from main execution cycle.
* A task is an arbitrary function implementation that takes a unit32_t 
* integer as unique parameter, which should be the current milliseconds 
* time as from millis() Arduino function.
* Create a new instance for each task and add it to a TimerSet
* in order to get it scheduled for execution.
*/

#include "TimerTask.h"

TimerTask::TimerTask() {
	//Serial.println("Timertask init...");
	execObj = NULL;
	id = 0;
	_period = 1000;
	oneShot = true;
	_enabled = false;
	tag = "";
	_next = NULL;
}

TimerTask::TimerTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic, bool en) {
	//Serial.println("Creating TimerTask... ");
	execObj = obj;
	//id = taskId;
	_period = ms;
	oneShot = !periodic;
	_enabled = en;
	tag = "";
	_next = NULL;
}

TimerTask::TimerTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, bool en) {
	//Serial.println("Creating TimerTask... ");
	execObj = obj;
	//id = taskId;
	_period = ms;
	oneShot = false;
	_enabled = en;
	tag = (name == NULL) ? "" : name;
	_next = NULL;
}

// Updates Timertask state, checking whether its wait period has expired
// returns: remaining time before trigger [ms] > 0; 0 if not enabled
uint32_t TimerTask::update(uint32_t& chkTime) {
	uint32_t elapsed;

	if (!_enabled) return UINT32_MAX;
	//if (chkTime == 0) chkTime = millis();
	if (chkTime < _startTime) elapsed = ~_startTime + chkTime + 1;
	else elapsed = chkTime - _startTime;
	if (elapsed >= _period) {
		execObj->timerRoutine(id, chkTime);
		//if (funct != NULL) (_game->*funct)(chkTime);
		if (oneShot) {
			_enabled = false;
			return 0;
		}
		_startTime = chkTime;
		return _period;
	}
	return _period - elapsed;
}

uint32_t TimerTask::getRemainingTime(uint32_t chkTime) {
	uint32_t elapsed;

	if (!_enabled) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < _startTime) elapsed = ~_startTime + chkTime + 1;
	else elapsed = chkTime - _startTime;
	if (elapsed >= _period) return 0;
	return _period - elapsed;
}

void TimerTask::print() {
	Serial.print("['");
	if (tag != NULL) Serial.print(tag);
	Serial.print("',");
	Serial.print(_period);
	if (_enabled) Serial.print(",1]"); else Serial.print(",0]");
}

void TimerTask::setPeriod(uint32_t ms) {
	_period = ms;
}

void TimerTask::setPeriod(uint32_t ms, bool periodic) {
	_period = ms;
	oneShot = !periodic;
}

void TimerTask::setPeriodic(bool periodic) {
	oneShot = !periodic;
}

void TimerTask::enable() {
	_startTime = millis();
	_enabled = true;
}

void TimerTask::disable() {
	_enabled = false;
}

bool TimerTask::isEnabled() {
	return _enabled;
}
