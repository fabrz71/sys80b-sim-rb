/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* GAME FUNDAMENTALS OBJECT
* ---
* Fundamental properties and logic for any game.
* Defines the minimal setPeriod of functions that any SYS880/B game have to implement.
*/

#include "Game.h"

#include "nvData.h"
#include "LightGroup.h"
#include "uiMenu.h"
#include "TimerTask.h"
#include "TimerSet.h"
#include "msgOutput.h"

Game::Game() {
	init();
}

// pinball generic initialization procedures
void Game::init() {
	byte i;

	// resets sound and display
	BoardIO::writeDisplayAndSoundReset(true);

	// setting up general purpose timer
	gpTimer = new TimerTask(NULL, 1000, "GPtmr");
	gpTimer->disable();
	tmr->add(gpTimer);

	_currentLightSet = lights;

	// setting up standard light stage
	//lightSet = new LightSet(light, Sys80b::LAMPS_COUNT);
	//lightSet = new LightSet(Sys80b::LAMPS_COUNT);
	//setActiveLightStage(lightStage);
	//lightStageEnabled = true;
	display = new DisplayWithFX(displ);

	// power-on stats update
	outp_clr("Power on counter: ");
	outpln(incrementPowerOnCounter());

	// loads settings and NV values
	//loadStats();
	loadCoinsPerCredits();
	loadAwardScoreLevels();
	loadHighScores();
	defaultBallsPerPlay = (getSettingSwitch(25) == 1) ? 3 : 5;
	
	// variables init
	credits = 0;
	players = 0;
	gameOver = true;
	for (i = 0; i < 3; i++) coins[i] = 0; // coins reset
	for (i = 0; i < MAX_PLAYERS; i++) player[i].score = 0; // scores reset

	//setPeriod(RELAY_Q_SOL, true); // game over
	//setPinMode(ATTRACT_MODE);
}

void Game::initGame_generic() {
	setSolenoid(RELAY_T_SOL, false); // NO tilt
	setSolenoid(RELAY_Q_SOL, false); // NO game over
	setPlayerOn(0); // 1st player
	player[playerOn].ballOnPlay = 1;
}

void Game::setPinballMode(byte mode) {
	String s;

	outp_clr(F("Switching mode: "));
	outpln(mode);
	pinballMode = mode;
	modeStep = 0;
	switch (pinballMode) {
	case START_MODE:
		setSolenoid(RELAY_Q_SOL, true); // game over
		display->setDisplayText(0, (const char *)F("SYS80/B PRB V1.0 BY"));
		display->setDisplayText(1, (const char *)F(" FABVOLPI@GMAIL.COM"));
		//setDelayedCall(startGame, 1000); // switches to another state
		tmr->print(); // test
		break;
	case ATTRACT_MODE:
		displ.clear();
		startAttractMode();
		// TODO...
		break;
	case GAME_MODE:
		initGame_generic();
		// TODO...
		break;
	case TEST_MODE:
		initMenu();
		// TODO...
		break;
	case BOOKKEEP_MODE:
		// TODO...
		break;
	}
}

void Game::switchModeStep(byte stp) {
	modeStep = stp;
	// ... msg output
	switch (modeStep) {
	case GAME_INIT:
		// TODO...
		break;
	case GAME_WAIT_BALLS_IN_HOLE:
		// TODO...
		break;
	case GAME_NEWBALL:
		// TODO...
		break;
	case GAME_TILT:
		setTrelay(true);
		switchOffAllLights();
		// TODO...
		break;
	case GAME_BONUS_AWARD:
		// TODO...
		break;
	case GAME_END:
		gameOver = true;
		// TODO...
		break;
	}
}

void Game::displayScoresAndCredits() {
	String st[4];
	int i, l;

	for (i = 0; i < 4; i++) {
		st[i] = String(F("0000000")) + String(player[i].score);
		l = st[i].length();
		if (l > 8) st[i] = st[i].substring(l - 8);
	}
	display->disableDisplayEffect(0);
	display->stopDisplayBlink(0);
	displ.clear();
	displ.setText(0, st[0]);
	displ.setText(0, 12, st[1]);
	displ.setText(1, st[2]);
	displ.setText(1, 12, st[3]);
	displ.setText(1, 9, String(credits));
}

void Game::loadHighScores() {
	byte i, j;

	for (i = 0; i < TOP_SCORES; i++) {
		if (i < 3) {
			j = 36 + i * 8;
			topScore[i].score = getStat32(j);
			topScore[i].initials[0] = getStat8(j + 4);
			topScore[i].initials[1] = getStat8(j + 5);
			topScore[i].initials[2] = getStat8(j + 6);
		}
		else {
			topScore[i].score = (TOP_SCORES - i) * 500000u;
			topScore[i].initials[0] = 'R';
			topScore[i].initials[1] = 'A';
			topScore[i].initials[2] = 'M';
		}
	}
}

void Game::loadAwardScoreLevels() {
	for (byte i = 0; i < 3; i++) awardLevelScore[i] = getStat32(22 + i * 4);
}

void Game::loadCoinsPerCredits() {
	byte i, v;

	for (i = 0; i < 3; i++) {
		v = getSettingByte(i);
		if (v <= 24) {
			ch_credits[i] = credits_ratio[v];
			ch_coins[i] = coins_ratio[v];
		}
		else { // unsupported "incentives" modes
			ch_credits[i] = 1;
			ch_coins[i] = 1;
		}
	}
}

void Game::onCoinInserted(byte cch) {
	if (cch > 2) return;
	coins[cch]++;
	if (coins[cch] == ch_coins[cch]) {
		incrementCredits(ch_credits[cch]);
		coins[cch] = 0;
	}
}

uint16_t Game::incrementPowerOnCounter() {
	uint16_t pot;
	byte i;

	pot = getStat16(0);
	if (pot == 0) { // on first bootup only
		for (i = 0; i < 4; i++) saveSettingByte(i, 0xff, factorySwSettings[i]);
		for (i = 0; i < 76; i++) saveStat8(i, factoryStatsBytes[i]);
	}
	saveStat16(0, ++pot);
	return pot;
}

void Game::incrementCredits(byte cr) {
	byte prevcr, maxcr;
	maxcr = max_credits[(getSettingByte(1) >> 6) & 3];
	prevcr = credits;
	credits += cr;
	if (credits > maxcr) credits = maxcr;
	if (credits > prevcr && player[playerOn].ballOnPlay <= 1 && players < MAX_PLAYERS) {
		incrementPlayers();
		displayScoresAndCredits();
		//setSound(_);
	}
}

void Game::incrementPlayers() {
	if (players < MAX_PLAYERS) {
		player[players].score = 0;
		player[players].bonus = 0;
		player[players].totalGameBalls = defaultBallsPerPlay;
		player[players].gameOver = false;
		player[players].awardLevelReached = 0;
		player[players].topScorer = false;
		players++;
	}
	displayScoresAndCredits();
	//setSound(_);
}

void Game::extraBall() {
	player[playerOn].totalGameBalls++;
	setSolenoid(8, true); // kicker
}

void Game::addScore(uint32_t sc) {
	byte lev;

	player[playerOn].score += sc;
	// check for Score Level award
	lev = player[playerOn].awardLevelReached;
	if (lev < 3 && player[playerOn].score >= awardLevelScore[lev]) {
		player[playerOn].awardLevelReached++;
		if (getSettingSwitch(29) == 1) extraBall();
		else {
			incrementCredits(1);
			setSolenoid(8, true); // kicker
		}
	}
	// check for top score
	if (!player[playerOn].topScorer && player[playerOn].score >= topScore[0].score) {
		player[playerOn].topScorer = true;
		incrementCredits(getSettingSwitch(23) * 2 + getSettingSwitch(24));
		setSolenoid(8, true); // kicker
	}
}

void Game::addBonus(uint32_t sc) {
	player[playerOn].bonus += sc;
}

// sets new player on game, updating display
void Game::setPlayerOn(byte newPlayer) {
	playerOn = newPlayer;
	if (playerOn < 4) {
		int r, c;
		r = dPlayerRow[playerOn];
		c = dPlayerCol[playerOn];
		display->setDisplayBlinkInterval(r, c, c + 7);
		display->startDisplayBlink(r, 250, 2000);
	}
}

bool Game::releaseBall() {
	bool ballReady = readSwitch(36) ? true : false;
	if (ballReady) activateSolenoid(BRELEASE_SOL, 250); // ball release
	return ballReady;
}

//// to call periodically for real lamps update
//void Game::updateLights(uint32_t ms) {
//	if (lightStageEnabled) lightStage->update(ms, true);
//	//renderLamps();
//}

// defines the function sub to call after a defined time delay
void Game::setDelayedCall(func_t sub, uint32_t dlay) {
	gpTimer->disable();
	Serial.print("Setting timer ");
	gpTimer->print();
	gpTimer->setPeriod(dlay, false);
	gpTimer->setFunction(sub);
	gpTimer->enable();
	Serial.print("->");
	gpTimer->print();
	Serial.println();
}

// defines the function sub to call periodically with specified period
void Game::setPeriodicCall(func_t sub, uint32_t period) {
	gpTimer->disable();
	gpTimer->setPeriod(period, true);
	gpTimer->setFunction(sub);
	gpTimer->enable();
}

void Game::startGame(uint32_t t) {
	//Serial.println("startGame()");
	setPinballMode(GAME_MODE);
}

// gives the next player on play (0..MAX_PLAYERS-1)
// returns MAX_PLAYERS when all players are over
byte Game::getNextPlayerOn() {
	byte p = playerOn++;

	if (p > MAX_PLAYERS) p = 0;
	while (p <= MAX_PLAYERS) {
		if (!player[p].gameOver) break;
		p++;
	}
	return p;
}

void Game::setCurrentLightSet(Light *ls) {
	_currentLightSet = ls;
	LightGroup::refLightSet = ls;
}

void Game::switchOffAllLights() {
	//for (byte i = 3; i < _size; i++) if (!isSpecialLamp(i)) light[i].state = OFF_L;
	for (byte i = 0; i < Sys80b::LAMPS_COUNT; i++) _currentLightSet[i].set(OFF_L);
}

// update all low-level "lamps" according to higher-level "lights" state
// of active light setPeriod <_currentLightSet>
// tm should contain actual time (ms)
void Game::renderLamps() {
	Light *l;

	for (int n = 0; n < Sys80b::LAMPS_COUNT; n++) {
		l = &(_currentLightSet[n]);
		if (!l->effective) lamps.setLamp(n, l->on);
	}
}

// updates all non-steady mode lights' state
// of active light setPeriod <_currentLightSet>
void Game::updateLights(uint32_t ms) {
	for (byte i = 0; i < Sys80b::LAMPS_COUNT; i++) _currentLightSet[i].update(ms);
}

// updates all non-steady mode lights state;
// the given lamp setPeriod will also be updated.
inline void Game::updateAndRenderLamps(uint32_t ms) {
	updateLights(ms);
	renderLamps();
}

// Called whenever a switch opens or closes.
// Game specific code should ovveride this function calling this base method,
// in order to correctly update <switches> variable.
// sw = 0..63
void Game::onEvent(byte sw, bool st) {
	sw = (sw >> 3) * 10 + (sw & 7); // format conversion ??? TODO
	if (st) swCount++;
}

