/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* PINBALL GAME SPECIFIC CODE
* ---
* Implements game specific rules and behavior.
* Valid only for the game specifed below.
*/

#include "BigHouse.h"
#include "LightGroup.h"

//using namespace msg;

BigHouse::BigHouse(Board_Sys80b& board) : PinGame(board) {
	msg->outln(F("BigHouse init..."));
	delay(100);
	_gameName = String(GAME_NAME);
	_gameNum = GAME_NUMBER;
	slamSwitchNormalState = false;
	//msg->outln(F("- light groups init..."));
	//delay(100);
	class LGroups {
	public:
		LightGroup* extraBall = new LightGroup("extraball", 5, 6, 7, -1);
		LightGroup* lastChance = new LightGroup("lastchance", 8, 9, -1);
		LightGroup* special = new LightGroup("special", 10, 11, -1);
		LightGroup* wall = new LightGroup("wall", 22, 23, 24, 25, -1);
		LightGroup* break_ = new LightGroup("break", 29, 30, 31, 32, 33, -1);
		LightGroup* jail = new LightGroup("jail", 34, 35, 36, 37, -1);
		LightGroup* multi = new LightGroup("multi", 38, 39, 40, 41, -1);
		LightGroup* cellBlock = new LightGroup("cellblock", 42, 43, 44, 45, -1);
		LightGroup* lightShow = new LightGroup("lightshow", 17, 46, -1);
		LightGroup* dropTarget = new LightGroup("droptarget", 26, 27, 28, -1);
		//LightGroup* show1 = new LightGroup("show1", 26, 27, 28, 21, -1);
		//LightGroup* show2 = new LightGroup("show2", 8, 10, 11, 9, -1);
		//LightGroup* show3 = new LightGroup("show3", 15, 20, 44, 45, -1);
		//LightGroup* show4 = new LightGroup("show4", 29, 30, 31, 32, -1);
		//LightGroup* show5 = new LightGroup("show5", 5, 6, 7, 43, -1);
		//LightGroup* show6 = new LightGroup("show6", 3, 17, 42, 33, -1);
	};
	LGroups lGroup;
	//extraball_ls = new LightGroup("extraball", 5, 6, 7, -1);
	//lastchance_ls = new LightGroup("lastchance", 8, 9, -1);
	//special_ls = new LightGroup("special", 10, 11, -1);
	//wall_ls = new LightGroup("wall", 22, 23, 24, 25, -1);
	//break_ls = new LightGroup("break", 29, 30, 31, 32, 33, -1);
	//jail_ls = new LightGroup("jail", 34, 35, 36, 37, -1);
	//multi_ls = new LightGroup("multi", 38, 39, 40, 41, -1);
	//cellblock_ls = new LightGroup("cellblock", 42, 43, 44, 45, -1);
	//lightshow_ls = new LightGroup("lightshow", 17, 46, -1);
	//droptarget_ls = new LightGroup("droptarget", 26, 27, 28, -1);
	//show1_ls = new LightGroup("show1", 26, 27, 28, 21, -1);
	//show2_ls = new LightGroup("show2", 8, 10, 11, 9, -1);
	//show3_ls = new LightGroup("show3", 15, 20, 44, 45, -1);
	//show4_ls = new LightGroup("show4", 29, 30, 31, 32, -1);
	//show5_ls = new LightGroup("show5", 5, 6, 7, 43, -1);
	//show6_ls = new LightGroup("show6", 3, 17, 42, 33, -1);
	//lShow_set[0] = jail_ls;
	//lShow_set[1] = wall_ls;
	//lShow_set[2] = show1_ls;
	//lShow_set[3] = show2_ls;
	//lShow_set[4] = show3_ls;
	//lShow_set[5] = show4_ls;
	//lShow_set[6] = show5_ls;
	//lShow_set[7] = show6_ls;
	lShow_set[0] = lGroup.jail;
	lShow_set[1] = lGroup.wall;
	lShow_set[2] = new LightGroup("show1", 26, 27, 28, 21, -1);
	lShow_set[3] = new LightGroup("show2", 8, 10, 11, 9, -1);
	lShow_set[4] = new LightGroup("show3", 15, 20, 44, 45, -1);
	lShow_set[5] = new LightGroup("show4", 29, 30, 31, 32, -1);
	lShow_set[6] = new LightGroup("show5", 5, 6, 7, 43, -1);
	lShow_set[7] = new LightGroup("show6", 3, 17, 42, 33, -1);
	//lShow_set[] = { jail_ls, wall_ls, show1_ls, show2_ls, show3_ls , show4_ls , show5_ls , show6_ls };
	//for (int i = 0; i < 8; i++) {
	//	Serial.print("Light Show group: ");
	//	Serial.println(lShow_set[i]->nameStr);
	//}
	//msg->outln(F("- timers init..."));
	dropt_tmr = new TimerTask(this, DROPT_LIGHT_TMR1, "Drop targets", false);
	lshow_tmr = new TimerTask(this, 200, "Light show", true);
	dshow_tmr = new TimerTask(this, 1000, "Display show", false);
	dropTargets_tt = tmr.add(dropt_tmr);
	lightShow_tt = tmr.add(lshow_tmr);
	display_tt = tmr.add(dshow_tmr);
}

BigHouse::~BigHouse() {
	delete extraball_ls;
	delete lastchance_ls;
	delete special_ls;
	delete wall_ls;
	delete break_ls;
	delete jail_ls;
	delete multi_ls;
	delete cellblock_ls;
	delete lightshow_ls;
	delete droptarget_ls;
	delete show1_ls;
	delete show2_ls;
	delete show3_ls;
	delete show4_ls;
	delete show5_ls;
	delete show6_ls;
	delete[] lShow_set;
}

void BigHouse::begin() {
	msg->outln(F("BigHouse begin..."));
	reset();
	lamps->setProtection(SND16_LMP, true);
	lamps->setProtection(RAMP_LMP, true);
	lamps->setProtection(BALLGATE_LMP, true);
	lamps->setProtection(AUGER_LMP, true);
	setSound(SND_SHUT_UP);
	//msg->outln(F("bank reset..."));
	act.bankReset.activate(250);
	//msg->outln(F("starting shows..."));
	setPinballMode(SHOW_MODE);
	//startLightShow();
	//startDisplayShow();
}

void BigHouse::onPinballModeChange(PinballMode m) {
	switch (m) {
	case SHOW_MODE:
		startLightShow();
		startDisplayShow();
		break;
	case GAME_MODE:
		multiballMode = false;
		bonusAdvanceSwitches = 100;
		cellBlockSeq = 0;
		multiSeq = 0;
		extraBallGiven = false;
		setSound(SND_MUSIC);
		resetDropTargets();
		resetCapturedBalls();
		liftRamp(false);
		openGate(true);
		switchAuger(false);
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

//void BigHouse::initGame() {
//	PinGame::initGame();
//	setPinballMode(GAME_MODE);
//	multiballMode = false;
//	bonusAdvanceSwitches = 100;
//	cellBlockSeq = 0;
//	multiSeq = 0;
//	extraBallGiven = false;
//	setSound(SND_MUSIC);
//	resetDropTargets();
//	resetCapturedBalls();
//	liftRamp(false);
//	openGate(true);
//	switchAuger(false);
//}

//bool BigHouse::isSpecialLamp(byte lamp) {
//	if (lamp > 47) return false;
//	byte i = 0;
//	while (i < SPECIAL_LAMPS_COUNT) if (lamp == special_lamp[i++]) return true;
//	return false;
//}

// EVENT DISPATCHER ROUTINE
// swtch = 0..63
// called whenever a switch opens or closes.
// handles table events depending on pinball mode
void BigHouse::onSwitchClosed(byte swtch) {
	//swtch = (swtch >> 3) * 10 + (swtch & 7); // format conversion
	PinGame::onSwitchClosed(swtch);
	switch (_mode) {
	case SHOW_MODE:
		if (swtch == REPLAY_SW && credits > 0) {
			credits--;
			setPinballMode(GAME_MODE);
		}
		// ..
		break;
	case GAME_MODE:
		if (tiltState && swtch == sw.holeThrough.Number()) {
			setTilt(false);
			act.ballGate.set(false);
			act.auger.set(false);
			return;
		}
		if (swCount >= bonusAdvanceSwitches) {
			swCount -= bonusAdvanceSwitches;
			advanceMultiplier();
		}
		switch (swtch) {
			DISPATCH(onBallOnChain(), 26);
			DISPATCH(onLeftRollover(), 30);
			DISPATCH5(onBreakTarget((swtch - 31) / 10), 31, 41, 51, 61, 71); // 0, 1, 2, 3, 4
			DISPATCH(onRubber(), 32);
			DISPATCH3(onPopBumper(), 33, 43, 53);
			DISPATCH2(onCellTarget((swtch - 34) / 10), 34, 44); // 0, 1
			DISPATCH2(onCellTarget((swtch - 43) / 10), 63, 73); // 2, 3
			DISPATCH4(onDropTarget((swtch - 40) / 10), 40, 50, 60, 70); // side targets 0, 1, 2, 3
			DISPATCH4(onJailTarget((swtch - 42) / 10), 42, 52, 62, 72); // 0, 1, 2, 3
			DISPATCH2(onOutRollover((swtch == 45) ? 0 : 1), 45, 75);
			DISPATCH(onLeftKicker(), 46);
			DISPATCH3(onTopRollover((swtch - 54) / 10), 54, 64, 74); // 0, 1, 2
			DISPATCH2(onReturnRollover((swtch == 55) ? 0 : 1), 55, 65); //0, 1
			DISPATCH(onRightKicker(), 56);
			DISPATCH(onTopHole(), 76);
		}
		break;
	case TEST_MODE:
		// TODO...
		break;
	case BOOKKEEP_MODE:
		// TODO...
		break;
	}
}

//void BigHouse::onKeyPressed(UserKey butt) {
//	PinGame::onKeyPressed(butt);
//}

void BigHouse::resetCapturedBalls() {
	for (byte i = 0; i<3; i++) ballTrap[i] = false;
	capturedBalls = 0;
}

void BigHouse::resetJailBreak() {
	jail_ls->setAll(OFF_L);
	break_ls->setAll(OFF_L);
	jail_ls->set(0, BLINK_L);
	break_ls->set(0, BLINK_L);
	jailSeq = 0;
	breakSeq = 0;
}

void BigHouse::resetDropTargets() {
	dropt_tmr->disable();
	dropt_tmr->setPeriod(DROPT_LIGHT_TMR1);
	dropTargetHit = 0;
	droptarget_ls->setAll(OFF_L);
	activateSolenoid(BRESET, 250);
}

void BigHouse::resetCellSpots() {
	cellblock_ls->setAll(ON_L);
}

void BigHouse::resetMultiplier() {
	multi_ls->setAll(OFF_L);
	multi_ls->set(0, ON_L);
	multiSeq = 1;
}

void BigHouse::resetWall() {
	wall_ls->setAll(ON_L);
	wallSeq = 0;
}

bool BigHouse::hitWall() {
	bool wallDown = false;
	wall_ls->set(wallSeq, OFF_L);
	if (++wallSeq >= 4) {
		advanceMultiplier();
		wallSeq = 0;
		wallDown = true;
	}
	return wallDown;
}

void BigHouse::advanceJail() {
	jail_ls->set(jailSeq, ON_L);
	if (jailSeq < 4) jail_ls->set(jailSeq++, BLINK_L);
}

void BigHouse::advanceBreak() {
	break_ls->set(breakSeq, ON_L);
	if (breakSeq < 5) break_ls->set(breakSeq++, BLINK_L);
}

void BigHouse::advanceMultiplier() {
	if (multiSeq < 3) multi_ls->set(multiSeq++, ON_L);
	// .. soundBuffer
}

void BigHouse::awardExtraBall() {
	player[playerOn].ballsLeft++;
	extraBallGiven = true;
	// .. soundBuffer
}

inline String BigHouse::getGameName() {
	return String(GAME_NAME);
}

inline int BigHouse::getGameNumber() {
	return GAME_NUMBER;
}

//void BigHouse::setSound(byte snd) {
//	sound16((snd & 0x10) == 0); // sound16 signal update (active low)
//	'renderLamp(SND16_LMP); // immediate lamp update
//	Serial.printf("%d: sound16: %d\n", millis(), (snd & 0x10) ? 0 : 1);
//	Sys80b::setSound(snd);
//}

void BigHouse::setSnd16(bool s) {
	setLamp(SND16_LMP, s); 
	renderLamp(SND16_LMP);
}

bool BigHouse::releaseBall() {
	bool ballReady = sw.outHole.isClosed();
	//if (ballReady) _activeLightSet->getLight(BALLRELEASE_LMP)->pulse(500); // ball release
	if (ballReady) act.ballRelease.activate(1000);
	return ballReady;
}

void BigHouse::timerRoutine(int taskId, uint32_t& ms) {
	//Serial.printf("timerRoutine(%i, %u): ", taskId, ms);
	if (taskId == dropTargets_tt) {
		dropTimerRoutine(ms);
		return;
	};
	if (taskId == lightShow_tt) {
		//msg->outln(F("* light show timer"));
		lightShowCycle(ms);
		return;
		}
	if (taskId == ballRelease_tt) {
		//msg->outln(F("* ball release timer"));
		act.ballRelease.set(false);
		return;
	}
	if (taskId == display_tt) {
		displayShowCycle(ms);
		return;
	}
	msg->warn(F("unknown taskId"), CLASSNAME, "timerRoutine");
}

//void BigHouse::millisRoutine(uint32_t& ms) {
//	snd16Update(ms); // "Sound16" bit preliminary update
//	PinGame::millisRoutine(ms);
//}

// GAME RULES ======================================================================

void BigHouse::onDropTarget(byte n) { // n = 0, 1, 2, 3
	msg->outln("onDropTarget");
	if (_3balls) addScore(5000); else addScore(3000);
	addBonus(1000);

	if (dropTargetHit == 0) {
		droptarget_ls->set(0, BLINK_L);
		dropt_tmr->setPeriod(DROPT_LIGHT_TMR1);
		dropt_tmr->enable();
	}
	dropTargetHit |= 1 << n;
	//if (allDropTargetsDown()) {
	if (dropTargetHit == 0xf) { // all targets down
		if (droptarget_ls->isActive(2)) { // lights special
			special_ls->switchTheFirst(BLINK_L, SPECIAL_TIME);
			droptarget_ls->set(2, OFF_L);
		}
		else if (droptarget_ls->isActive(1)) { // 100000x
			addScore(100000);
			droptarget_ls->set(1, OFF_L);
		}
		else if (droptarget_ls->isActive(0)) { // 10000x
			addScore(10000);
			droptarget_ls->set(0, OFF_L);
		}
		resetDropTargets();
		// .. sounds
	}
}

void BigHouse::onJailTarget(byte n) {
	msg->outln("onJailTarget");
	if (_3balls) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (jail_ls->getState(n) == BLINK_L) {
		advanceJail();
		addScore(10000);
	}
	else jail_ls->pulse(n, 100);
	if (jailSeq == 4 && breakSeq == 5) {
		extraball_ls->set(1, ON_L);
		// .. soundBuffer
	}
}

void BigHouse::onBreakTarget(byte n) {
	msg->outln("onBreakTarget");
	if (_3balls) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (break_ls->getState(n) == BLINK_L) {
		advanceJail();
		addScore(10000);
	}
	else jail_ls->pulse(n, 100);
	if (jailSeq == 4 && breakSeq == 5) {
		extraball_ls->set(1, ON_L);
		// .. soundBuffer
	}
}

void BigHouse::onCellTarget(byte n) {
	lightState ls;
	byte gln;

	msg->outln("onCellTarget");
	if (_3balls) addScore(5000); else addScore(3000);
	addBonus(1000);
	ls = cellblock_ls->getState(n);
	if (ls == ON_L) cellblock_ls->set(n, BLINK_L);
	else if (ls == BLINK_L) {
		cellblock_ls->set(n, OFF_L);
		cellBlockSeq |= bit(n);
		if (cellBlockSeq == 0b1111) {
			gln = extraball_ls->switchTheFirst(ON_L);
			if (extraBallGiven || gln == 0xff) addScore(100000); // ?
																 // .. soundBuffer
			cellBlockSeq = 0;
		}
	}
	else cellblock_ls->set(n, ON_L); // when light is STATIC_TEXT
}

void BigHouse::onTopRollover(byte n) {
	msg->outln("onTopRollover");
	if (_3balls) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (n == 1) { // center rollover
		if (isLightActive(18)) addScore(100000);
		if (isLightActive(19)) advanceMultiplier();
	}
	if (extraball_ls->isActive(n)) {
		extraball_ls->set(n, OFF_L);
		lastchance_ls->setAll(OFF_L);
		awardExtraBall();
	}
	// .. soundBuffer
}

void BigHouse::onLeftKicker() {
	msg->outln("onLeftKicker");
	// score
	if (_3balls) addScore(5000); else addScore(3000);
	if (!ballTrap[0]) { // ball already trapped
		extraball_ls->set(1, ON_L);
	}
	else {
		setLight(15, BLINK_L);
		ballTrap[0] = true;
		// ... soundBuffer
	}
	//startTrapSequence(0);
}

void BigHouse::onRightKicker() {
	msg->outln("onRightKicker");
	addScore(25000);
	//if (getLight(20) == BLINK_L) startMultiball(); // ???
	//startTrapSequence(2);
}

void BigHouse::onTopHole() { // hole
	msg->outln("onTopHole");
	addScore(25000);
	if (isLightActive(15)) ballTrap[2] = true;
	//else //..
	//startTrapSequence(1);
}

void BigHouse::onLeftRollover() {
	msg->outln("onLeftRollover");
	addScore(25000);
	addBonus(1000);
}

void BigHouse::onReturnRollover(byte n) {
	msg->outln("onReturnRollover");
	if (_3balls) addScore(5000);
	else addScore(3000);
	addBonus(1000);
	//..
}

void BigHouse::onOutRollover(byte n) {
	msg->outln("onOutTarget");
	if (_3balls) addScore(5000);
	else addScore(3000);
	addBonus(1000);
	//..
}

void BigHouse::onPopBumper() {
	msg->outln("onPopBumper");
	addScore(1000);
	special_ls->rotateRight();
	extraball_ls->rotateRight();
}

void BigHouse::onRubber() {
	addScore(30);
	special_ls->rotateRight();
}

void BigHouse::onBallOnChain() {
	msg->outln("onBallOnChain");
	addScore(5000);
	//switches += 4;
}

void BigHouse::onHole() {
	//if (modeStep == 0) {
	//	modeStep = GAME_NEWBALL;
	//	switchAuger(OFF);
	//	releaseBall();
	//}
	//else if (modeStep == GAME_TILT) switchModeStep(GAME_NEWBALL);

	// ..TODO
}

void BigHouse::onTilt() {
	if (sw.holeThrough.isOpen()) { // missing balls in hole
		if (sw.leftShooter.isClosed()) {
			act.ballGate.set(true);
			act.leftShooter.activate(250, 500);
		}
		act.auger.set(true); // auger
	}
	if (sw.topHole.isClosed()) act.holeKicker.activate(250);
}

// ================================================================================

void BigHouse::dropTimerRoutine(uint32_t &ms) { 
	msg->outln("dropTimerRoutine()");
	if (droptarget_ls->isActive(2)) { // "light special" -> "100000x"
		droptarget_ls->set(2, OFF_L);
		droptarget_ls->set(1, BLINK_L);
	}
	else if (droptarget_ls->isActive(1)) { // "100000x" -> "10000x"
		droptarget_ls->set(1, OFF_L);
		droptarget_ls->set(0, BLINK_L);
		dropt_tmr->setPeriod(DROPT_LIGHT_TMR2);
	}
	else if (droptarget_ls->isActive(0)) { // "10000x" -> RESET
		droptarget_ls->set(0, OFF_L);
		resetDropTargets();
	}
	else {
		msg->warn("No special lit.", CLASSNAME, "dropTimerRoutine");
	}
}

void BigHouse::startLightShow() {
	msg->outln(F("startLightShow..."));
	lightShowType = 10;
	switchToNextLightShow();
	lshow_tmr->enable();

}

void BigHouse::stopLightShow() {
	lshow_tmr->disable();
}

void BigHouse::switchToNextLightShow() {
	switchOffAllLights();
	if (++lightShowType > 7) lightShowType = 0;
	switch (lightShowType) {
	case 0: // original light show
	case 2:
	case 4:
		Serial.println("Original lightshow");
		for (byte i = 0; i < 8; i++) {
			//lShow_set[i]->setAll(OFF_L);
			lShow_set[i]->set(0, ON_L);
		}
		lightShowDelay = 200; // ms
		lightShowOrder = false;
		break;
	case 1: // linear on -> linear off
		Serial.println("Linear lightshow");
		lightShowDelay = 15; // ms
		break;
	case 3: // groups flashing
		Serial.println("Groups flash lightshow");
		lightShowDelay = 100;
		break;
	case 5: // random on -> random off
		Serial.println("random lightshow");
		lightShowDelay = 20;
		lightShowOrder = true;
		break;
	case 6: // PCM
		Serial.println("PCM lightshow");
		lightShowOrder = true;
		lightShowArg = 1;
		break;
	case 7: // light wave
		Serial.println("Wave lightshow");
		lightShowDelay = 20;
	}
	lightShowStep = 0;
	lshow_tmr->setPeriod(lightShowDelay);
}

void BigHouse::lightShowCycle(uint32_t &ms) {
	byte i;

	switch (lightShowType) {
	case 0: // original light show
	case 2:
	case 4:
		if (lightShowOrder) for (i = 0; i < 8; i++) lShow_set[i]->rotateLeft();
		else for (i = 0; i < 8; i++) lShow_set[i]->rotateRight();
		if (lightShowOrder) lightShowDelay += 5; else lightShowDelay -= 5;
		lshow_tmr->setPeriod((uint32_t)lightShowDelay);
		if (lightShowDelay <= 100 || lightShowDelay >= 300) lightShowOrder = !lightShowOrder;
		//lights->print();
		if (lightShowStep >= 80) switchToNextLightShow();
		break;
	case 1:
		if (lightShowStep < 48) invertLight(lightShowStep);
		else invertLight(lightShowStep - 48);
		if (lightShowStep >= 95) switchToNextLightShow();
		break;
	case 3:
		i = lightShowStep & 0x07; // i = 0..7
		if (i > 0) lShow_set[i-1]->setAll(OFF_L); 
		else lShow_set[7]->setAll(OFF_L);
		lShow_set[i]->setAll(ON_L);
		if (lightShowStep >= 48) switchToNextLightShow();
		break;
	case 5:
		getActiveLightSet()->getLight(micros() % 48)->set(lightShowOrder); // random lamp
		if (lightShowStep == 100) lightShowOrder = false;
		if (lightShowStep >= 200) switchToNextLightShow();
		break;
	case 6: 
		if (!getActiveLightSet()->getLight(0)->isActive()) { // switch ON cycle
			getActiveLightSet()->switchOnAllLights();
			if (lightShowOrder) {
				lightShowArg += 2;
				if (lightShowArg >= 25) lightShowOrder = false;
			}
			else {
				lightShowArg -= 2;
				if (lightShowArg <= 1) lightShowOrder = true;
			}
			lightShowDelay = lightShowArg;
		}
		else { // switch OFF cycle
			getActiveLightSet()->switchOffAllLights();
			lightShowDelay = 26 - lightShowArg;
		}
		lshow_tmr->setPeriod((uint32_t)lightShowDelay);
		if (lightShowStep == 150) switchToNextLightShow();
		break;
	case 7:
		if (lightShowStep < 48) pulseLight(lightShowStep, 1000);
		if (lightShowStep == 100) switchToNextLightShow();
		break;
	}
	lightShowStep++;
}

String BigHouse::getTopScoreStr(byte n) {
	String st = " (" + String(n) + ") ";
	st += topScore[n].playerName + "- ";
	st += topScore[n].scoreString + "  ";
	return st;
}

void BigHouse::startDisplayShow() {
	msg->outln(F("startDisplayShow..."));
	displayShowStep = 0;
	dshow_tmr->enable();
}

void BigHouse::stopDisplayShow() {
	dshow_tmr->disable();
}

void BigHouse::displayShowCycle(uint32_t& ms) {
	//Serial.print("displayShowCycle(..) step #");
	//Serial.println(displayShowStep);
	if (displayShowStep == 0) {
		displayScoresAndCredits();
		dshow_tmr->setPeriod(1000u);
	}
	else if (displayShowStep == 3) {
		extDisplay->clear();
		extDisplay->setDynamicFX(0, F("** ESCAPE ARTISTS **"), DFX_type::LEFT_INS);
		dshow_tmr->setPeriod(2500u);
	}
	else if (displayShowStep >= 4) {
		if (displayShowStep == 4) 
			extDisplay->setDynamicFX(0, F("*ARTISTI DELLA FUGA*"), DFX_type::RIGHT_INS);
		String st = getTopScoreStr(displayShowStep - 4);
		extDisplay->setDynamicFX(1, st , DFX_type::LEFT_INS);
		if (displayShowStep == 3 + TOP_SCORES) {
			displayShowStep = 0;
			return;
		}
	}
	displayShowStep++;
}

//void BigHouse::snd16Update(uint32_t& ms) {
//	//if (!soundBuffer.isEmpty()) { // set S16 before S1-2-4-8
//	//	uint16_t snd = soundBuffer.peek();
//	//	// light #4 sound16 update (active low)
//	//	setLamp(SND16_LMP, (snd & 0x10) == 0, true);
//	//	Serial.printf("%d: sound16: %d\n", millis(), ((snd & 0x10) == 0) ? 1 : 0);
//	//	renderLamp(SND16_LMP); // immediate update
//	//}
//
//	if (_soundPendingCmd && !_soundValueOut) { // set S16 before S1-2-4-8
//		uint16_t snd = soundBuffer.peek();
//		setLamp(SND16_LMP, (snd & 0x10) == 0, true); // light #4 sound16 update (active low)
//		Serial.printf("%d: sound16: %d\n", millis(), ((snd & 0x10) == 0) ? 1 : 0);
//		renderLamp(SND16_LMP); // immediate update
//	}
//}