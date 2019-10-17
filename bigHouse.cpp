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

BigHouse::BigHouse() {
	LightGroup extraball_ls("extraball", 5, 6, 7);
	LightGroup lastchance_ls("lastchance", 8, 9);
	LightGroup special_ls("special", 10, 11);
	LightGroup wall_ls("wall", 22, 23, 24, 25);
	LightGroup break_ls("break", 29, 30, 31, 32, 33);
	LightGroup jail_ls("jail", 34, 35, 36, 37);
	LightGroup multi_ls("multi", 38, 39, 40, 41);
	LightGroup cellblock_ls("cellblock", 42, 43, 44, 45);
	LightGroup lightshow_ls("lightshow", 17, 46);
	LightGroup droptarget_ls("droptarget", 26, 27, 28);
	LightGroup show1_ls("show1", 26, 27, 28, 21);
	LightGroup show2_ls("show2", 8, 10, 11, 9);
	LightGroup show3_ls("show3", 15, 20, 44, 45);
	LightGroup show4_ls("show4", 29, 30, 31, 32);
	LightGroup show5_ls("show5", 5, 6, 7, 43);
	LightGroup show6_ls("show6", 3, 17, 42, 33);
	initPinball();
}

void BigHouse::initPinball() {
	init();
	//TimerTask(func_t sub, uint32_t ms, const char *name);
	dropt_tmr = new TimerTask(dropTimerRoutine, (uint32_t)DROPT_LIGHT_TMR1, (const char *)"Drop targets");
	tmr->add(dropt_tmr);
	lshow_tmr = new TimerTask(lightShowCycle, (uint32_t)200, (const char *)"Light show");
	tmr->add(lshow_tmr);
}

void BigHouse::startAttractMode() {
	displayScoresAndCredits();
	startLightShow();
	//setTimerCall(updateAttractMode, 3000);
}

void BigHouse::initGame() {
	initGame_generic();
	switchModeStep(GAME_INIT);
	multiballMode = false;
	bonusAdvanceSwitches = 100;
	cellBlockSeq = 0;
	multiSeq = 0;
	extraBallGiven = false;
	setSound(Sfx::MUSIC);
	resetDropTargets();
	resetCapturedBalls();
	liftRamp(false);
	openGate(true);
}

bool BigHouse::isSpecialLamp(byte lamp) {
	if (lamp > 47) return false;
	byte i = 0;
	while (i < SPECIAL_LAMPS_COUNT) if (lamp == special_lamp[i++]) return true;
	return false;
}

// EVENT DISPATCHER ROUTINE
// sw = 0..63
// called whenever a switch opens or closes.
// handles table events depending on pinball mode
void BigHouse::onEvent(byte sw, bool st) {
	Game::onEvent(sw, st); // superclass funciton
	sw = (sw >> 3) * 10 + (sw & 7); // format conversion
	switch (pinballMode) {
	case ATTRACT_MODE:
		if (sw == REPLAY_SW && st && credits > 0) {
			credits--;
			setPinballMode(GAME_MODE);
		}
		// ..
		break;
	case GAME_MODE:
		if (swCount >= bonusAdvanceSwitches) {
			swCount -= bonusAdvanceSwitches;
			advanceMultiplier();
		}
		switch (sw) {
			DISPATCH1(onBallOnChain(), 26);
			DISPATCH1(onLeftRollover(), 30);
			DISPATCH5(onDropTarget((sw - 31) / 10), 31, 41, 51, 61, 71);
			DISPATCH1(onRubber(), 32);
			DISPATCH3(onPopBumper(), 33, 43, 53);
			DISPATCH2(onCellTarget((sw - 34) / 10), 34, 44);
			DISPATCH2(onCellTarget((sw - 43) / 10), 63, 73);
			DISPATCH4(onDropTarget((sw - 40) / 10), 40, 50, 60, 70);
			DISPATCH4(onJailTarget((sw - 42) / 10), 42, 52, 62, 72);
			DISPATCH2(onOutRollover((sw == 45) ? 0 : 1), 45, 75);
			DISPATCH1(onLeftKicker(), 46);
			DISPATCH3(onTopRollover((sw - 54) / 10), 54, 64, 74);
			DISPATCH2(onReturnRollover((sw == 55) ? 0 : 1), 55, 65);
			DISPATCH1(onRightKicker(), 56);
			DISPATCH1(tilt(), 57);
			DISPATCH1(onOutHole(), 66);
			DISPATCH1(onTopHole(), 76);
		}
		break;
	case TEST_MODE:
		if (!st) break;
		if (sw == LEFTADV_SW) execMenuCmd(MENU_BACK);
		else if (sw == RIGHTADV_SW) execMenuCmd(MENU_NEXT);
		else if (sw == REPLAY_SW) execMenuCmd(MENU_ENTER);
		break;
	}
}

void BigHouse::resetCapturedBalls() {
	for (byte i = 0; i<3; i++) ballTrap[i] = false;
	capturedBalls = 0;
}

void BigHouse::resetJailBreak() {
	jail_ls.setAll(OFF_L);
	break_ls.setAll(OFF_L);
	jail_ls.setLight(0, BLINK_L);
	break_ls.setLight(0, BLINK_L);
	jailSeq = 0;
	breakSeq = 0;
}

void BigHouse::resetDropTargets() {
	dropt_tmr->disable();
	dropt_tmr->period = DROPT_LIGHT_TMR1;
	dropTargetSeq = 0;
	droptarget_ls.setAll(OFF_L);
	setSolenoid(Sol::BRESET, true, 250);
}

void BigHouse::resetCellSpots() {
	cellblock_ls.setAll(ON_L);
}

void BigHouse::resetMultiplier() {
	multi_ls.setAll(OFF_L);
	multi_ls.setLight(0, ON_L);
	multiSeq = 1;
}

void BigHouse::resetWall() {
	wall_ls.setAll(ON_L);
	wallSeq = 0;
}

bool BigHouse::hitWall() {
	bool wallDown = false;
	wall_ls.setLight(wallSeq, OFF_L);
	if (++wallSeq >= 4) {
		advanceMultiplier();
		wallSeq = 0;
		wallDown = true;
	}
	return wallDown;
}

void BigHouse::advanceJail() {
	jail_ls.setLight(jailSeq, ON_L);
	if (jailSeq < 4) jail_ls.setLight(jailSeq++, BLINK_L);
}

void BigHouse::advanceBreak() {
	break_ls.setLight(breakSeq, ON_L);
	if (breakSeq < 5) break_ls.setLight(breakSeq++, BLINK_L);
}

void BigHouse::advanceMultiplier() {
	if (multiSeq < 3) multi_ls.setLight(multiSeq++, ON_L);
	// .. sound
}

void BigHouse::awardExtraBall() {
	player[playerOn].totalGameBalls++;
	extraBallGiven = true;
	// .. sound
}

void BigHouse::onBallInHole() {
	//awardBonus();
	//..
}

inline String BigHouse::getGameName() {
	return String(GAME_NAME);
}

inline int BigHouse::getGameNumber() {
	return GAME_NUMBER;
}

// GAME RULES ======================================================================

void BigHouse::onDropTarget(byte n) {
	if (_3BALLS) addScore(5000); else addScore(3000);
	addBonus(1000);

	if (dropTargetSeq == 0) {
		droptarget_ls.setLight(0, BLINK_L);
		dropt_tmr->period = DROPT_LIGHT_TMR1;
		dropt_tmr->enable();
	}
	dropTargetSeq |= 1 << n;
	//if (allDropTargetsDown()) {
	if (dropTargetSeq == 0xf) { // all targets down
		if (droptarget_ls.isActive(2)) { // lights special
			special_ls.switchOne(BLINK_L, SPECIAL_TIME);
			droptarget_ls.setLight(2, OFF_L);
		}
		else if (droptarget_ls.isActive(1)) { // 100000x
			addScore(100000);
			droptarget_ls.setLight(1, OFF_L);
		}
		else if (droptarget_ls.isActive(0)) { // 10000x
			addScore(10000);
			droptarget_ls.setLight(0, OFF_L);
		}
		resetDropTargets();
		// .. sounds
	}
}

void BigHouse::onJailTarget(byte n) {
	if (_3BALLS) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (jail_ls.getLState(n) == BLINK_L) {
		advanceJail();
		addScore(10000);
	}
	else jail_ls.pulse(n, 100);
	if (jailSeq == 4 && breakSeq == 5) {
		extraball_ls.setLight(1, ON_L);
		// .. sound
	}
}

void BigHouse::onBreakTarget(byte n) {
	if (_3BALLS) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (break_ls.getLState(n) == BLINK_L) {
		advanceJail();
		addScore(10000);
	}
	else jail_ls.pulse(n, 100);
	if (jailSeq == 4 && breakSeq == 5) {
		extraball_ls.setLight(1, ON_L);
		// .. sound
	}
}

void BigHouse::onCellTarget(byte n) {
	lightState ls;
	byte gln;

	if (_3BALLS) addScore(5000); else addScore(3000);
	addBonus(1000);
	ls = cellblock_ls.getLState(n);
	if (ls == ON_L) cellblock_ls.setLight(n, BLINK_L);
	else if (ls == BLINK_L) {
		cellblock_ls.setLight(n, OFF_L);
		cellBlockSeq |= bit(n);
		if (cellBlockSeq == 0b1111) {
			gln = extraball_ls.switchOne(ON_L);
			if (extraBallGiven || gln == 0xff) addScore(100000); // ?
																 // .. sound
			cellBlockSeq = 0;
		}
	}
	else cellblock_ls.setLight(n, ON_L); // when light is OFF
}

void BigHouse::onTopRollover(byte n) {
	if (_3BALLS) addScore(3000); else addScore(1000);
	addBonus(1000);
	if (n == 1) { // center rollover
		if (lights[18].isActive()) addScore(100000);
		if (lights[19].isActive()) advanceMultiplier();
	}
	if (extraball_ls.isActive(n)) {
		extraball_ls.setLight(n, OFF_L);
		lastchance_ls.setAll(OFF_L);
		awardExtraBall();
	}
	// .. sound
}

void BigHouse::onLeftKicker() {
	// score
	if (_3BALLS) addScore(5000); else addScore(3000);
	if (!ballTrap[0]) { // ball already trapped
		extraball_ls.setLight(1, ON_L);
	}
	else {
		lights[15].set(BLINK_L);
		ballTrap[0] = true;
		// ... sound
	}
	//startTrapSequence(0);
}

void BigHouse::onRightKicker() {
	addScore(25000);
	//if (getLight(20) == BLINK_L) startMultiball(); // ???
	//startTrapSequence(2);
}

void BigHouse::onTopHole() { // hole
	addScore(25000);
	if (lights[15].state == BLINK_L) ballTrap[2] = true;
	//else //..
	//startTrapSequence(1);
}

void BigHouse::onLeftRollover() {
	addScore(25000);
	addBonus(1000);
}

void BigHouse::onReturnRollover(byte n) {
	if (_3BALLS) addScore(5000);
	else addScore(3000);
	addBonus(1000);
	//..
}

void BigHouse::onOutRollover(byte n) {
	if (_3BALLS) addScore(5000);
	else addScore(3000);
	addBonus(1000);
	//..
}

void BigHouse::onPopBumper() {
	addScore(1000);
	special_ls.rotateRight();
	extraball_ls.rotateRight();
}

void BigHouse::onRubber() {
	addScore(30);
	special_ls.rotateRight();
}

void BigHouse::onBallOnChain() {
	addScore(5000);
	switches += 4;
}

void BigHouse::onOutHole() {
	if (modeStep == 0) {
		modeStep = GAME_NEWBALL;
		switchAuger(OFF);
		releaseBall();
	}
	else if (modeStep == GAME_TILT) switchModeStep(GAME_NEWBALL);
	// ..TODO
}

// ================================================================================

void BigHouse::dropTimerRoutine(uint32_t ms) { // "light special" -> "100000x"
	if (droptarget_ls.isActive(2)) {
		droptarget_ls.setLight(2, OFF_L);
		droptarget_ls.setLight(1, BLINK_L);
	}
	else if (droptarget_ls.isActive(1)) { // "100000x" -> "10000x"
		droptarget_ls.setLight(1, OFF_L);
		droptarget_ls.setLight(0, BLINK_L);
		dropt_tmr->period = DROPT_LIGHT_TMR2;
	}
	else if (droptarget_ls.isActive(0)) { // "10000x" -> RESET
		droptarget_ls.setLight(0, OFF_L);
		resetDropTargets();
	}
}

void BigHouse::startLightShow() {
	lightShowSeq = 0;
	lightShowDelay = 200; // ms
	lightShowOrder = false;
	for (byte i = 0; i<8; i++) {
		lShow_set[i]->setAll(OFF_L);
		lShow_set[i]->setLight(0, ON_L);
	}
	lshow_tmr->period = lightShowDelay;
	lshow_tmr->enable();
}

void BigHouse::stopLightShow() {
	lshow_tmr->disable();
}

void BigHouse::lightShowCycle(uint32_t ms) {
	Serial.print("lightShowCycle: (");
	Serial.print(ms);
	Serial.println(") ");
	//Serial.println(lightShowDelay);
	for (byte i = 0; i<8; i++) {
		if (lightShowOrder) lShow_set[i]->rotateLeft();
		else lShow_set[i]->rotateRight();
	}
	if (++lightShowSeq >= 4) lightShowSeq = 0;
	if (lightShowSeq == 0) { // new sequence
		if (lightShowOrder) { // reverse direction
			if (lightShowDelay <= 100) lightShowOrder = !lightShowOrder;
			else lightShowDelay -= 20;
		}
		else { // forward direction
			if (lightShowDelay >= 300) lightShowOrder = !lightShowOrder;
			else lightShowDelay += 20;
		}
	}
	lshow_tmr->period = (uint32_t)lightShowDelay;
}

// Called by Teensy Bounce extern instance
// Simulates corresponding swich closure
void BigHouse::onPRBButtonPressed(BoardIO::buttonID bt) {
	switch (bt) {
	case BoardIO::BACK_BUTT:
		onEvent(LEFTADV_SW, true);
		break;
	case BoardIO::ENTER_BUTT:
		onEvent(REPLAY_SW, true);
		break;
	case BoardIO::NEXT_BUTT:
		onEvent(RIGHTADV_SW, true);
		break;
	}
}
