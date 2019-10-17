#include "TimerClient.h"

TimerClient::TimerClient() {
	period = 1000;
	oneShot = true;
	_enabled = false;
	tag = "";
	next = NULL;
}

//TimerClient::TimerClient(int function) {
//	//target = obj;
//	//fun = f;
//	//fid = 0;
//	period = 1000;
//	oneShot = true;
//	_enabled = false;
//	tag = "";
//	next = NULL;
//}
//
//TimerClient::TimerClient(uint32_t ms, bool periodic = true) {
//	period = ms;
//	oneShot = !periodic;
//	_enabled = false;
//	tag = "";
//	next = NULL;
//}
//
//TimerClient::TimerClient(uint32_t ms, const char *name, bool enable = false) {
//	period = ms;
//	oneShot = false;
//	_enabled = false;
//	tag = (name == NULL) ? "" : name;
//	next = NULL;
//}

// returns: remaining time before trigger [ms] > 0; 0 if not enabled
uint32_t TimerClient::check(uint32_t chkTime = 0) {
	uint32_t elapsed;

	if (!_enabled /* || target == NULL */) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
	else elapsed = chkTime - start_time;
	if (elapsed >= period) {
		//target->timerFunction(chkTime);
		//(this->*fun)(chkTime);
		timerFunction(chkTime);
		if (oneShot) {
			_enabled = false;
			return 0;
		}
		start_time = chkTime;
		return period;
	}
	return period - elapsed;
}

uint32_t TimerClient::getRemainingTime(uint32_t chkTime) {
	uint32_t elapsed;

	if (!_enabled) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
	else elapsed = chkTime - start_time;
	if (elapsed >= period) return 0;
	return period - elapsed;
}

void TimerClient::print() {
	Serial.print("['");
	if (tag != NULL) Serial.print(tag);
	Serial.print("',");
	Serial.print(period);
	if (_enabled) Serial.print(",1]"); else Serial.print(",0]");
}

//inline void TimerClient::setTarget(TimerTarget *tt) {
//	target = tt;
//}

inline void TimerClient::setPeriod(uint32_t ms, bool periodic) {
	period = ms;
	oneShot = !periodic;
}

//inline void TimerClient::setFunctionId(int id) {
//	fid = id;
//}

inline void TimerClient::enable() {
	start_time = millis();
	_enabled = true;
}

inline void TimerClient::disable() {
	_enabled = false;
}

inline bool TimerClient::isEnabled() {
	return _enabled;
}
