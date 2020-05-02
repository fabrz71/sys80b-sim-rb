#ifndef _TestModule_h_
#define _TestModule_h_

#include "Arduino.h"
#include "PinGame.h"
#include "msg.h"

//class GameTimerTask;

#define _GAME_IMPLEM_ TestModule

const bool OUTPUTS_TEST = false;

class TestModule : public PinGame {
public:
	// pinball game-specific identification data
	const char* CLASSNAME = "TestModule";
	const int GAME_NUMBER = 999;
	const char* GAME_NAME = "Test Module";

	TimerTask* freqTask;
	int freqTaskId;
	byte step;
	uint16_t counter;

	TestModule(Board_Sys80b& board);
	void begin();
	void onPinballModeChange(PinballMode mode);
	void onPlayerChange(byte n) {};
	void onSwitchClosed(byte sw);
	void onKeyPressed(UserKey key);
	void snd16Update(uint32_t& t);
	void millisRoutine(uint32_t& ms);

protected:
	//void msgRoutine(uint32_t& ms);
	void timerRoutine(int taskIdentifer, uint32_t& ms);
};

#endif
