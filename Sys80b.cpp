#include "Sys80b.h"

//#include "SolenoidSet.h"

Sys80b::Sys80b(Board_Sys80b& board) {
	Serial.println("Sys80b init...");
	//delay(100);
	hw = &board;
	msg = &(board.msg);
	solenoids = new SolenoidSet(SOLENOID_COUNT); // solenoids (numbers: 1..9)
	//lampSolenoids = new SolenoidSet(16); // "lamp" solenoids (16..31)
	lamps = new BitMappedSet(LAMPS_COUNT);
	slamSwitchNormalState = true;
	slamSwitchLastState = slamSwitchNormalState;
	//_soundValueOut = false;
	_soundOutTime = 0;
	_soundOutputStage = 0;
	//hw = nullptr;
	//reset();
}

Sys80b::~Sys80b() {
	delete solenoids;
	delete lamps;
}

// reset all outputs to 0
void Sys80b::reset() {
	msg->outln(F("Sys80b reset..."));
	delay(100);

	hw->outputReset();
	lamps->reset();
	solenoids->resetAll();
	soundBuffer.Clear();
	setSound(15); // shout
	currentLampGroup = 0;
	display.reset();
	_soundPendingCmd = false;
	//_changedSwitchNum = 0xff;
	slamSwitchLastState = slamSwitchNormalState;
}

byte Sys80b::getBallsPerPlayCount() {
	return (hw->getSettingSwitch(25) == 1) ? 3 : 5;
	//return 3; // TODO
}

// sets a solenoid state, updating internal variables
// n = [1..9]
void Sys80b::setSolenoid(byte n, bool active) {
	if (n < 1 || n > 9) return;
	solenoids->setState(n-1, active);
	hw->writeSolenoids(solenoids->getStates());
}

// sets a solenoid state, defining switch delay and active _period:
// solenoid state will switch after the specified delay.
// n = [1..9]
 void Sys80b::activateSolenoid(byte n, uint16_t actPeriod, uint16_t swDelay) {
	//Serial.printf("activateSolenoid(%d, %d, %d)\n",n, actPeriod, swDelay);
	if (n < 1 || n > 9) return;
	if (actPeriod == 0) actPeriod = SOL_DEF_TIME;
	solenoids->activate(n-1, actPeriod, swDelay);
	hw->writeSolenoids(solenoids->getStates());
}

// checks whether:
// - delay activation time elapsed (when delayedSwitch = true)
// - maximum active time elapsed for safe switch-off
 void Sys80b::checkSolenoids(uint32_t& t) {
	if (solenoids->update(t)) hw->writeSolenoids(solenoids->getStates());
}

 void Sys80b::setLampImmediate(byte n, bool state, bool forceProtection) {
	 setLamp(n, state, forceProtection);
	 renderLamp(n);
	 currentLampGroup = n >> 2;
 }

 // snd = 0..31
void Sys80b::setSound(byte snd) {
	soundBuffer.put(snd);
	_soundPendingCmd = true;
}

// uint16_t Sys80b::pullSoundCmd() {
//	return (soundBuffer.isEmpty()) ? 0 : soundBuffer.get();
//}

byte Sys80b::incrementStrobe() {
	byte strb = switchGrid.incrementStrobe();
	hw->writeStrobes(~(byte)(1u<<strb)); // strobe advance (inverted)
	return strb;
}

// Stores returns states in <_returnsInput> variable
// updating internal SwitchGrid8x8 object attributes.
// Returned value is also stored internally and readable
// through getChangedSwitch() function.
// returns: true in case of changes detected on current return  line
bool Sys80b::acquireReturns(uint32_t& t) {
	_returnsInput = (byte)~(hw->readReturns()); // read actual switch returns on current strobe line (inverted)
	return (switchGrid.setCurrentRow(_returnsInput, t) > 0);
}

// Gets next sequencial data to send to display and forwards each byte to its 
// correspondig line.
// If data contains a byte routed to both rows, it will be forwarded only once.
void Sys80b::feedDisplay() {
	uint16_t dd; // display control + data
	byte dc; // display control code: 1 = row 1; 2 = row 2; 3 = both rows
	byte db; // display data (8-bit);
	uint16_t LD_time = 25; // us

	for (int i = 0; i < 2; i++) { // 1 iteration only in case dc == 3
		dd = display.getNextDisplayData();
		dc = (byte)((dd & 0x300) >> 8);
		db = (byte)(dd & 0xff);
		hw->writeDisplayData(db); // updates 8-bit data bus
		if (dc & 1u) hw->writeDisplayLD1(LOW); // LD on
		if (dc & 2u) hw->writeDisplayLD2(LOW);
		delayMicroseconds(LD_time);
		if (dc & 1u) hw->writeDisplayLD1(HIGH); // LD off
		if (dc & 2u) hw->writeDisplayLD2(HIGH);
		if (dc == 3u) break; // CMD for both rows has already been sent
		if (i == 0) delayMicroseconds(100u - LD_time); // only after first iteration
	}

}

// Real lamps update routine.
// Updates 1 consecutive group of 4 lamps for each call.
// Returns true in case of effective update forwarded.
bool Sys80b::renderNextLampsGroup() {
	byte i;

	for (i = 0; i < 12; i++) { // search next not-updated group
		if (lamps->getChanges4(currentLampGroup) > 0) break;
		if (++currentLampGroup >= 12) currentLampGroup = 0;
	}
	if (i < 12) { // pending update
		hw->write4Lamps(currentLampGroup, lamps->getStates4(currentLampGroup));
		lamps->clearChanges4(currentLampGroup);
		return true;
	}
	hw->clearLampsOutput();
	return false;
}

// Updates a group of 4 lamp.
// Returns true in case of effective update forwarded.
bool Sys80b::renderLampsGroup(byte lg) {
	if (lamps->getChanges4(lg) > 0) { // pending changes ?
		hw->write4Lamps(lg, lamps->getStates4(lg));
		lamps->clearChanges4(lg);
		return true;
	}
	return false;
}

// to call every 1 ms
void Sys80b::updateSound(uint32_t& tm) {
	static byte snd = 0; // only first 4 lower output bits (snd1, snd2, snd4, snd8)

	//if (_soundOutTime > 0) { // sound output value already set
	//	if (tm - _soundOutTime < SND_OUT_TIME) return; // introduces a delay before clearing output
	//	setSnd16(false);
	//	_soundOutTime = 0;
	//	snd = 0; // clear sound output
	//}
	//else { // ready for new sound output
	//	if (!_soundPendingCmd) return;
	//	snd = soundBuffer.get();
	//	_soundOutTime = tm;
	//	setSnd16((snd & 16) > 0);
	//	if (soundBuffer.isEmpty()) _soundPendingCmd = false;
	//}
	//hw->writeSound(~(snd & 0x7f)); // only bits 0..6, not the 7th bit
	//Serial.printf("%d: writeSound(", millis());
	//Serial.print((byte)(~(snd & 0x7f)), BIN);
	//Serial.println(")");

	switch (_soundOutputStage) {
		case 0: // check for pending command
			if (soundBuffer.isEmpty()) break; // no sound pending command
			snd = soundBuffer.get();
			break;
		case 1: // snd16 output: 1 more cycle (1 ms)
			setSnd16((snd & 16) > 0); // snd16 output
			break;
		case 2: // LSBs sound output (snd1, snd2, snd4, snd8)
			hw->writeSound(~(snd & 0x7f)); // set sound LSBs
			break;
		case 3: // sound-out wait state 
			if (tm - _soundOutTime < SND_OUT_TIME) return; // no stage auto-increment
			if ((snd & 16) == 0) _soundOutputStage++; // avoid redundant snd16 reset
			break;
		case 4: // (optional) snd16 reset
			setSnd16(false); // snd16 reset
			break;
		case 5: // reset LSBs
			hw->writeSound(0x7f); // reset sound LSBs
			break;
	}
	if (++_soundOutputStage > 5) _soundOutputStage = 0;
 }

// declare a key state change
// shoud always be called also after a key release with key = NO_KEY
void Sys80b::setKeyPressed(UserKey key, uint32_t& ms) {
	if (key != _keyPressed) { // key has changed
		_keyPressed = key;
		_keyUpdateTime = ms;
	}
}

// Low-level switches state change handler routine.
void Sys80b::_onSwitchEvent(byte sw, bool st, uint32_t& ms) {
	String str;

	str = "switch ";
	str += sw;
	str += (st ? " closed" : " opened");
	msg->outln(str);

	// user buttons pressed
	if (sw == LEFTADV_SW || sw == RIGHTADV_SW || sw == REPLAY_SW || sw == TEST_SW) {
		UserKey key;
		switch (sw) {
		case LEFTADV_SW:
			key = LEFTADV_KEY;
			str = "LEFT";
			break;
		case RIGHTADV_SW:
			key = RIGHTADV_KEY;
			str = "RIGHT";
			break;
		case REPLAY_SW:
			key = REPLAY_KEY;
			str = "REPLAY";
			break;
		case TEST_SW:
			key = TEST_KEY;
			str = "TEST";
			break;
		default:
			key = NO_KEY;
			str = "<unknown>";
		}
		if (st) {
			str = "Key " + str + " pressed";
			msg->outln(str);
		}
		else {
			msg->outln(F("key released."));
			key = NO_KEY;
		}
		setKeyPressed(key, ms);
		return;
	}
	//onSwitchEvent(sw, st);
	if (st) onSwitchClosed(sw); else onSwitchOpened(sw);
}

//// Checks for keys command, implementing hold&repeat feature and calling upper-level event handler
//// onKeyPressed(..) function.
//// Must be called at regular times, with period <= KEY_REPEAT_PERIOD
//void Sys80b::_checkPressedKey(uint32_t& ms) {
//	if (_keyPressed != NO_KEY) {
//		if (!_keyRepeat) {
//			if (ms - _keyUpdateTime >= KEY_REPEAT_DELAY) _keyRepeat = true;
//		}
//		else { // repeating key
//			if (ms - _keyUpdateTime >= KEY_REPEAT_PERIOD) {
//				//Serial.print("key repeating...");
//				_keyUpdateTime = ms;
//				onKeyPressed(_keyPressed);
//			}
//		}
//	}
//	//else _keyRepeat = false;
//}

// Periodic update and output routine, to be
// called every millisecond.
// Returns true when an input change has detected:
// either a switch from grid or the slam switch.
// In case of change detection, use getChangesSwitch()
// to get the event.
void Sys80b::_millisRoutine(uint32_t& ms) {
	
	renderNextLampsGroup();
	updateSound(ms); // may change a lamp - put it after renderNextLampsGroup()
	checkSolenoids(ms);
	feedDisplay();
	// /* if (display.isLastPosition()) */ display.update(ms);
	
	bool slsw = hw->readSlamSw();
	if (slsw != slamSwitchLastState) {
		onSlamSwitchEvent(slamSwitchNormalState ? !slsw : slsw); // code from superclass
		slamSwitchLastState = slsw;
	}

	if (acquireReturns(ms)) { // any changes?
		byte sw = switchGrid.getLastChangedSwitch(); // surely sw != 0xff
		bool st = switchGrid.getLastChangedSwitchState();
		switchGrid.clearLastSwitchChanged();
		_onSwitchEvent(sw, st, ms);
		//onSwitchEvent(sw, st); // virtual function: code provided by superclass
	}
	incrementStrobe();

	//_checkPressedKey(ms);
}
