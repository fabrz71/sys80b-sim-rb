#include "TestModule.h"
//#include "GameTimerTask.h"

TestModule::TestModule(Board_Sys80b& board) : PinGame(board) {
	Serial.println("TestModule init...");
	delay(100);
	_gameName = String("Test Module");
	_gameNum = 1;
	//msgTask = tmr.addNewTask(this, msgRoutine, 1000, true, true);
	//GameTimerTask* gtt = new GameTimerTask(this, &msgRoutine, true);
	step = 0;
}

void TestModule::onSwitchEvent(byte sw, bool st) {
}

void TestModule::onButtonPressed(UserKey key) {
}

//void TestModule::msgRoutine(uint32_t& ms) {
//	switch (_mode) {
//		case SHOW_MODE:
//		switch (step) {
//			case 1:
//				display.putText(0, F("*** TEST  MODULE ***"));
//				display.putText(1, F("  for  Sys80/b PRB  "));
//				break;
//		}
//		step++;
//	break;
//	}
//}

void TestModule::timerRoutine(int taskIdentifer, uint32_t& ms) {
	if (_mode == SHOW_MODE) {
		if (taskIdentifer == tid) {
			switch (step) {
			case 1:
				extDisplay->putText(0, F("*** TEST  MODULE ***"));
				extDisplay->putText(1, F("  for  Sys80/b PRB  "));
				break;
			}
			step++;
		}
	}
}
