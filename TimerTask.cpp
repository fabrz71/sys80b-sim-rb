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
	funct = NULL;
	period = 1000;
	oneShot = true;
	enabled = false;
	tag = "";
	_next = NULL;
}

TimerTask::TimerTask(func_t sub, uint32_t ms, bool periodic = true) {
	//Serial.println("Creating TimerTask... ");
	funct = sub;
	period = ms;
	oneShot = !periodic;
	enabled = false;
	tag = "";
	_next = NULL;
}

TimerTask::TimerTask(func_t sub, uint32_t ms, const char *name) {
	//Serial.println("Creating TimerTask... ");
	funct = sub;
	period = ms;
	oneShot = false;
	enabled = false;
	tag = (name == NULL) ? "" : name;
	_next = NULL;
}

// returns: remaining time before trigger [ms] > 0; 0 if not enabled
uint32_t TimerTask::check(uint32_t chkTime = 0) {
	uint32_t elapsed;

	if (!enabled) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
	else elapsed = chkTime - start_time;
	if (elapsed >= period) {
		if (funct != NULL) funct(chkTime);
		if (oneShot) {
			enabled = false;
			return 0;
		}
		start_time = chkTime;
		return period;
	}
	return period - elapsed;
}

uint32_t TimerTask::getRemainingTime(uint32_t chkTime) {
	uint32_t elapsed;

	if (!enabled) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
	else elapsed = chkTime - start_time;
	if (elapsed >= period) return 0;
	return period - elapsed;
}

void TimerTask::print() {
	Serial.print("['");
	if (tag != NULL) Serial.print(tag);
	Serial.print("',");
	Serial.print(period);
	if (enabled) Serial.print(",1]"); else Serial.print(",0]");
}

inline void TimerTask::setPeriod(uint32_t ms, bool periodic) { 
	period = ms; 
	oneShot = !periodic; 
}

inline void TimerTask::setFunction(func_t sub) { 
	funct = sub; 
}

inline void TimerTask::enable() { 
	start_time = millis(); 
	enabled = true;
}

inline void TimerTask::disable() { 
	enabled = false; 
}
