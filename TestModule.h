#ifndef _TestModule_h_
#define _TestModule_h_

#include "Arduino.h"
#include "PinGame.h"

//class GameTimerTask;

#define _GAME_IMPLEM_ TestModule

class TestModule : public PinGame {
public:
	//GameTimerTask* msgTask;
	int tid;
	byte step;

	TestModule(Board_Sys80b& board);
	void onSwitchEvent(byte sw, bool st);
	void onButtonPressed(UserKey key);

protected:
	//void msgRoutine(uint32_t& ms);
	void timerRoutine(int taskIdentifer, uint32_t& ms);
};

#endif
