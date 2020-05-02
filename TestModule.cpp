#include "TestModule.h"
//#include "GameTimerTask.h"

TestModule::TestModule(Board_Sys80b& board) : PinGame(board) {
	Serial.println(F("TestModule init..."));
	delay(100);
	_gameName = String(F("Test Module"));
	_gameNum = 1;
	freqTask = new TimerTask(this, 100);
	freqTaskId = tmr.add(freqTask);
	step = 0;
	counter = 0;
}

void TestModule::begin() {
	msg->outln(F("BigHouse begin..."));
	reset();
    display.clear();
	display.setText(0, F("*** TEST  MODULE ***"));
    if (OUTPUTS_TEST) {
        display.setText(1, F("--- OUTPUT TESTS ---"));
        freqTask->enable();
    }
}

void TestModule::onPinballModeChange(PinballMode m) {
    switch (m) {
    case SHOW_MODE:
        // TODO...
        break;
    case GAME_MODE:
        // TODO...
        break;
    case TEST_MODE:
        //initMenu();
        // TODO...
        break;
    case BOOKKEEP_MODE:
        // TODO...
        break;
    }
}

void TestModule::onSwitchClosed(byte sw) {
    String s = "switch closed: " + sw;
    display.setText(1, s, true);
}

void TestModule::onKeyPressed(UserKey key) {
    String s = "button: " + key;
    display.setText(1, s, true);
}

void TestModule::timerRoutine(int taskIdentifer, uint32_t& ms) {
    bool endOfStep = false;

    if (taskIdentifer == freqTaskId) {
        switch (step) {
        case 0:
            if (counter == 0) Serial.println("- solenoids");
            setSolenoid(counter, true);
            if (counter == 9) endOfStep = true;
            break;
        case 1:
            setSolenoid(counter, false);
            if (counter == 9) endOfStep = true;
            break;
        case 2:
            if (counter == 0) Serial.println("- lamps");
            setLamp(counter, true);
            if (counter == 47) endOfStep = true;
            break;
        case 3:
            setLamp(counter, true);
            if (counter == 47) endOfStep = true;
            break;
        case 4:
            if (counter == 0) Serial.println("- sound");
            setSound(counter);
            if (counter == 31) endOfStep = true;
            break;
        case 5:
            setSound(0);
            endOfStep = true;
            break;
        }
    }

    if (endOfStep) {
        counter = 0;
        if (++step > 5) step = 0;
    }
    else counter++;
}

void TestModule::snd16Update(uint32_t & ms) {
    if (!soundBuffer.isEmpty()) { // set S16 before S1-2-4-8
        uint16_t snd = soundBuffer.peek();
        // light #4 sound16 update (active low)
        setLamp(4, (snd & 0x10) == 0, true);
        //Serial.printf("%d: sound16: %d\n", millis(), ((snd & 0x10) == 0) ? 1 : 0);
        renderLamp(4); // immediate update
    }
}

void TestModule::millisRoutine(uint32_t& ms) {
    snd16Update(ms); // "Sound16" bit preliminary update
    PinGame::millisRoutine(ms);
}