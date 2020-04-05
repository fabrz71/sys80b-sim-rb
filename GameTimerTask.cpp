#include "GameTimerTask.h"
GameTimerTask::GameTimerTask() {
	_period = 1000;
	oneShot = true;
	_enabled = false;
	name = "";
	_next = NULL;
}

GameTimerTask::GameTimerTask(Game* game, GameFunct fn, uint32_t ms, bool periodic, bool en) {
	_game = game;
	_gameFunct = fn;
	//_fun = fn;
	_period = ms;
	oneShot = !periodic;
	_enabled = en;
	name = "";
	_next = NULL;
}

GameTimerTask::GameTimerTask(Game* game, GameFunct fn, uint32_t ms, const char* name, bool en) {
	_game = game;
	_gameFunct = fn;
	//_fun = fn;
	_period = ms;
	oneShot = false;
	_enabled = en;
	name = (name == NULL) ? "" : name;
	_next = NULL;
}

// returns: remaining time before trigger [ms] > 0; 0 if not enabled
uint32_t GameTimerTask::update(uint32_t& chkTime) {
	uint32_t elapsed;
	GameFunct f;

	if (!_enabled) return 0;
	if (chkTime < _startTime) elapsed = ~_startTime + chkTime + 1;
	else elapsed = chkTime - _startTime;
	if (elapsed >= _period) {
		(_game->*_gameFunct)(chkTime);
		//f = (void (Game::*)(uint32_t&))_fun;
		//f = (GameFunct)_fun;
		//(_game->*f)(chkTime);
		if (oneShot) {
			_enabled = false;
			return 0;
		}
		_startTime = chkTime;
		return _period;
	}
	return _period - elapsed;
}

uint32_t GameTimerTask::getRemainingTime(uint32_t chkTime) {
	uint32_t elapsed;

	if (!_enabled) return 0;
	if (chkTime == 0) chkTime = millis();
	if (chkTime < _startTime) elapsed = ~_startTime + chkTime + 1;
	else elapsed = chkTime - _startTime;
	if (elapsed >= _period) return 0;
	return _period - elapsed;
}

void GameTimerTask::print() {
	Serial.print("['");
	if (name != NULL) Serial.print(name);
	Serial.print("',");
	Serial.print(_period);
	if (_enabled) Serial.print(",1]"); else Serial.print(",0]");
}

void GameTimerTask::setPeriod(uint32_t ms, bool periodic) {
	_period = ms;
	oneShot = !periodic;
}

void GameTimerTask::enable() {
	_startTime = millis();
	_enabled = true;
}

void GameTimerTask::disable() {
	_enabled = false;
}

bool GameTimerTask::isEnabled() {
	return _enabled;
}
