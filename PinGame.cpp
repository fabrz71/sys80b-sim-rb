/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
*/

#include "PinGame.h"

//#include "nvData.h"
#include "TimerSet.h"
#include "LightSet.h"

const byte PinGame::dPlayerRow[] = { 0, 0, 1, 1 };
const byte PinGame::dPlayerCol[] = { 0, 12, 0, 12 };

PinGame::PinGame(Board_Sys80b& board) : Sys80b(board) {
	msg->outln(F("Game init..."));
	delay(100);
	extDisplay = new ExtTextDisplay(display);
	lights = new LightSet(LAMPS_COUNT);
	_gameName = String("<no_name>");
	_gameNum = 0;
	tmr.name = F("Pinball timer");
	for (int i = 0; i < 3; i++)	lamps->setProtection(i, true); // special lamps
	setActiveLightSet(lights);
	//reset();
}

PinGame::~PinGame() {
	delete lights;
}

// pinball generic initialization procedures
void PinGame::reset() {
	byte i;

	msg->outln(F("Game reset..."));
	//delay(100);
	Sys80b::reset();
	// variables init
	setActiveLightSet(lights);
	_keyPressed = NO_KEY;
	//_keyRepeat = false;
	//_mode = NORMAL_MODE;
	//_gameOver = true;
	_players = 0;
	credits = 0; // TODO
	tiltState = false;

	//hw->lcdclr();
	String st = F("Game: ");
	st += getGameName();
	hw->lcdprn(0, st);
	//delay(50);

	for (i = 0; i < 3; i++) coins[i] = 0; // coins reset
	for (i = 0; i < MAX_PLAYERS; i++) player[i].score = 0; // scores reset
	for (i = 0; i < TOP_SCORES; i++) {
		topScore[i].playerName = String("FAB");
		topScore[i].score = 1000;
		topScore[i].scoreString = getScoreStr(topScore[i].score, 8, true, true);
	}
	for (i = 0; i < 3; i++)	lamps->setProtection(i, true); // special lamps
														   
	//_3balls = (getBallsPerPlayCount() == 3) ? true : false;
	_3balls = true; // TODO
	loadCoinsPerCredits();
	loadAwardScoreLevels();
	loadHighScores();
	//defaultBallsPerPlay = (getSettingSwitch(25) == 1) ? 3 : 5;
	
	//setPinballMode(SHOW_MODE);
}

// Tries to add one more player.
// Conditions: credits available AND players < MAX_PLAYERS
// Note: does not automatically switch to the new player.
// returns true when succesfull
bool PinGame::addNewPlayer() {
	if (credits > 0 && _players < MAX_PLAYERS) {
		if (_gameOver) setPinballMode(GAME_MODE);
		initPlayer(_players++);
		if (_players == 1) playerOn = 0; // 1st player on
		credits--;
		return true;
	}
	return false;
}

// switches to next active player 
// updates <playerOn> (0..MAX_PLAYERS-1)
// returns false when no more active players available (game over)
bool PinGame::switchToNextPlayer() {
	if (_players > 1) {
		byte p = playerOn;
		for (int i = 1; i < MAX_PLAYERS; i++) {
			if (++p >= MAX_PLAYERS) p = 0;
			if (player[p].ballsLeft > 0) break;
		}
		playerOn = p;
	}
	else playerOn = 0;
	if (player[playerOn].ballsLeft == 0) return false; // game over
	displayScore(playerOn, true);
	return true;
}

//// gives the next player on play 
//// returns MAX_PLAYERS when all players are over
//byte PinGame::getNextPlayerOn() {
//	byte p = playerOn++;
//
//	if (p > MAX_PLAYERS) p = 0;
//	while (p <= MAX_PLAYERS) {
//		if (player[p].ballsLeft == 0) break;
//		p++;
//	}
//	return p;
//}

//// sets new active player, updating display
//void PinGame::setActivePlayer(byte nextPlayer) {
//	if (nextPlayer == playerOn) return;
//	playerOn = nextPlayer;
//	if (playerOn < MAX_PLAYERS) {
//		int r, c;
//		r = dPlayerRow[playerOn];
//		c = dPlayerCol[playerOn];
//		String scStr = String(player[nextPlayer].score);
//		extDisplay->setBlinkParams(r, 500);
//		extDisplay->putText(r, scStr, c, true);
//		//delete &scStr;
//	}
//}

void PinGame::initPlayer(byte n) {
	Player& p = player[n];
	p.score = 0;
	p.bonus = 0;
	p.ballsLeft = getBallsPerPlayCount();
	p.awardLevelReached = 0;
	p.topScorer = false;
	//p.gameOver = false;
}

void PinGame::hole() {
	actStd.holeKicker.activate(250, 1000);
	player[playerOn].ballsLeft--;
	onHole();
	setTilt(false);
	if (switchToNextPlayer()) actStd.outhole.activate(1000);
}

void PinGame::setTilt(bool st) {
	if (st == tiltState) return;
	tiltState = st;
	actStd.relay_Q.set(st); // game controls & reactors
	actStd.relay_T.set(st); // lights
	if (st) {
		switchOffAllLamps();
		display.clear();
		extDisplay->putText(1, "TILT", 8, true);
		onTilt();
	}
}

void PinGame::setPinballMode(PinballMode m) {
	String s;

	msg->clr();
	msg->out(F("Switching mode: "));
	msg->outln(m);
	_mode = m;
	//modeStep = 0;
	switch (m) {
	case SHOW_MODE:
		setOnGameRelay(false); // game over
		extDisplay->clear();
		_gameOver = true;
		break;
	case GAME_MODE:
		setTiltRelay(false); // NO setTilt
		setOnGameRelay(true); // NO game over
		setOnGameRelay(true); // game over
		extDisplay->clear();
		_gameOver = false;
		// missing specific code: out of hole ball kick
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
	onPinballModeChange(m);
}

//void PinGame::switchModeStep(byte stp) {
//	modeStep = stp;
//	// ... _text output
//	switch (modeStep) {
//	case GAME_INIT:
//		// TODO...
//		break;
//	case GAME_WAIT_BALLS_IN_HOLE:
//		// TODO...
//		break;
//	case GAME_NEWBALL:
//		// TODO...
//		break;
//	case GAME_TILT:
//		setTrelay(true);
//		switchOffAllLights();
//		// TODO...
//		break;
//	case GAME_BONUS_AWARD:
//		// TODO...
//		break;
//	case GAME_END:
//		gameOver = true;
//		// TODO...
//		break;
//	}
//}

//void PinGame::displayCurrentScore() {
//	String st = String(F("0000000")) + String(player[playerOn].score);
//	int l = st.length();
//	if (l > 8) st = st.substring(l - 8);
//	extDisplay->clear();
//	extDisplay->putText(dPlayerRow[playerOn], st, dPlayerCol[playerOn], true);
//	//delete st;
//}

void PinGame::displayCredits() {
	//extDisplay->stopDynamicFX(1);
	String crStr = String(credits);
	//extDisplay->putText(1, crStr, 9);
	display.setText(1, 9, crStr);
}

void PinGame::displayScore(byte pl, bool blink) {
	if (pl >= MAX_PLAYERS) return;
	byte r = dPlayerRow[pl];
	_scoreStr = getScoreStr(player[pl].score, SCORE_DIGITS, false);
	if (blink) extDisplay->setBlinkParams(r, 1000, 333);
	extDisplay->putText(r, _scoreStr, dPlayerCol[playerOn], blink);
}

// TODO
void PinGame::displayScoresAndCredits() {
	//String st[4];
	int i;

	extDisplay->clear();
	displayCredits();

	//for (i = 0; i < 4; i++) st[i] = getScoreStr(player[i].score, SCORE_DIGITS, false);
	//extDisplay->putText(0, st[0] ,0);
	//extDisplay->putText(0, st[1], 12);
	//extDisplay->putText(1, st[2], 0);
	//extDisplay->putText(1, st[3], 12);
	for (i = 0; i < 4; i++) displayScore(i, false);

	//String crStr = String(credits);
	//extDisplay->putText(1, crStr, 10 - crStr.length());
}

String PinGame::getScoreStr(uint32_t scr, byte digits, bool leadingZeros, bool dots) {
	byte ch, i, len;
	String st = F("000000000") + String(scr);
	if (digits > 10) digits = 10;
	st = st.substring(st.length() - digits);
	len = st.length();

	// zero clearing
	if (!leadingZeros && len > 2) { // cut leading zeros
		for (i = 0; i < (len - 2) && st.charAt(i) == '0'; i++) st.setCharAt(i, ' ');
	}

	// dots
	//len = st.length();
	if (dots && len > 3) {
		for (i = 3; i < len; i += 3) {
			ch = st.charAt(len - i - 1);
			if (ch == ' ') break;
			st.setCharAt(len - i - 1, (char)(ch | 0x80));
		}
	}

	return st;
}

void PinGame::loadHighScores() {
	int i, j;
	uint32_t sc;
	char inits[4];
	//String scStr;

	//msg->outln(F("- loadHighScores()..."));
	inits[3] = 0;
	for (i = 0; i < TOP_SCORES; i++) {
		j = 36 + i * 8;
		sc = hw->NVData->readDWord(j);
		topScore[i].score = sc;
		topScore[i].scoreString = getScoreStr(sc, SCORE_DIGITS, false, true);
		inits[0] = (char)(hw->NVData->readByte(j + 4));
		inits[1] = (char)(hw->NVData->readByte(j + 5));
		inits[2] = (char)(hw->NVData->readByte(j + 6));
		topScore[i].playerName = String(inits);
		//Serial.printf("<%d>  ", i+1);
		//Serial.print(topScore[i].playerName);
		//Serial.print(" -  ");		
		//Serial.println(topScore[i].scoreString);
		//delay(100);
	}
}

void PinGame::loadAwardScoreLevels() {
	//msg->outln(F("- loadAwardScoreLevels()..."));
	for (byte i = 0; i < 3; i++) {
		awardLevelScore[i] = hw->NVData->readDWord(22 + i * 4);
		//Serial.printf("level %d: ", i + 1);
		//Serial.println(awardLevelScore[i]);
	}
}

void PinGame::loadCoinsPerCredits() {
	byte i, v;

	//msg->outln(F("- loadCoinsPerCredits()..."));
	for (i = 0; i < 3; i++) { // i: coin chute
		v = hw->getSettingByte(i) & 0x1f;
		if (v <= 24) {
			ch_credits[i] = credits_ratio[v];
			ch_coins[i] = coins_ratio[v];
			//Serial.printf("chute %d: %d credit(s) for %d coin(s)\n", 
			//	i+1 , ch_credits[i], ch_coins[i]);
		}
		else { // unsupported "incentives" modes
			ch_credits[i] = 1;
			ch_coins[i] = 1;
		}
	}
}

// adds credits upon "coins-per-credit" setting
void PinGame::onCoinInserted(byte cch) {
	if (cch > 2) return;
	coins[cch]++;
	if (coins[cch] == ch_coins[cch]) {
		addCredits(ch_credits[cch]);
		coins[cch] = 0;
	}
}

void PinGame::addCredits(byte increment) {
	//byte prevcr;
	byte  maxcr = max_credits[(hw->getSettingByte(1) >> 6) & 3];
	//prevcr = credits;
	credits += increment;
	if (credits > maxcr) credits = maxcr;
	//if (credits > prevcr && player[playerOn].ballOnPlay <= 1) {
	//	//incrementPlayers();
	//	addNewPlayer();
	//	displayScoresAndCredits();
	//	//setSound(_);
	//}
}

//void PinGame::incrementPlayers() {
//	if (players < MAX_PLAYERS) {
//		player[players].score = 0;
//		player[players].bonus = 0;
//		player[players].totalGameBalls = defaultBallsPerPlay;
//		player[players].gameOver = false;
//		player[players].awardLevelReached = 0;
//		player[players].topScorer = false;
//		players++;
//	}
//	displayScoresAndCredits();
//	//setSound(_);
//}

void PinGame::addExtraBall() {
	player[playerOn].ballsLeft++;
	activateSolenoid(KNOCKER_SOL, 250); // knocker
	//actStd.knocker.activate(250);
}

void PinGame::addScore(uint32_t sc) {
	byte lev;

	player[playerOn].score += sc;
	// update for Score Level award
	lev = player[playerOn].awardLevelReached;
	if (lev < 3 && player[playerOn].score >= awardLevelScore[lev]) {
		player[playerOn].awardLevelReached++;
		if (hw->getSettingSwitch(29) == 1) addExtraBall();
		else {
			addCredits(1);
			setSolenoid(KNOCKER_SOL, true); // kicker
		}
	}
	// update for top score
	if (!player[playerOn].topScorer && player[playerOn].score >= topScore[0].score) {
		player[playerOn].topScorer = true;
		addCredits(hw->getSettingSwitch(23) * 2 + hw->getSettingSwitch(24));
		setSolenoid(KNOCKER_SOL, true); // kicker
	}
	displayScore(playerOn, false);
}

void PinGame::addBonus(uint32_t sc) {
	player[playerOn].bonus += sc;
}

String PinGame::getTopPlayerString(byte playerNum) {
	String str = "<" + topScore[playerNum].playerName + ">";
	str += " -  " + topScore[playerNum].scoreString;
	return str;
}

// void PinGame::setDefaultLightSet() {
//	setActiveLightSet(lights);
//}

// Checks for keys command, implementing hold&repeat feature and calling upper-level event handler
// onKeyPressed(..) function.
// Must be called at regular times, with period <= KEY_REPEAT_PERIOD
void PinGame::_checkPressedKey(uint32_t& ms) {
	static UserKey prevKey = NO_KEY;
	static bool keyRepeat = false;

	if (_keyPressed != NO_KEY) {
		if (_keyPressed != prevKey) {
			onKeyPressed(_keyPressed);
			prevKey = _keyPressed;
		}
		else {
			if (!keyRepeat) {
				if (ms - _keyUpdateTime >= KEY_REPEAT_DELAY) keyRepeat = true;
			}
			else { // repeating key
				if (ms - _keyUpdateTime >= KEY_REPEAT_PERIOD) {
					//Serial.print("key repeating...");
					_keyUpdateTime = ms;
					onKeyPressed(_keyPressed);
				}
			}
		}
	}
	else keyRepeat = false;
}

void PinGame::onKeyPressed(UserKey key)	{
	//msg->out("onKeyPressed: ");
	//msg->outln(key);
	Serial.println("onKeyPressed()");

	switch (key) {
	case REPLAY_KEY:
		break;
	case LEFTADV_KEY: // rotate ledgrid mode
		if (_mode == SHOW_MODE) {
			switch (hw->ledGridMode) {
			case LG_OFF:
				hw->ledGridMode = LG_SWITCHES;
				break;
			case LG_SWITCHES:
				hw->ledGridMode = LG_LAMPS;
				break;
			case LG_LAMPS:
				hw->ledGridMode = LG_OFF;
				break;
			}
			msg->outln("Ledgrid mode changed.");
		}
		break;
	case RIGHTADV_KEY:
		break;
	case TEST_KEY:
		//onTestButtonPressed();
		if (_gameOver) setPinballMode(TEST_MODE);
		break;
	case NO_KEY:
		break;
	}
}

void PinGame::onSwitchClosed(byte sw) {
	
	switch (_mode) {
	case SHOW_MODE:
		switch (sw) {
		case REPLAY_SW:
			addNewPlayer();
			playerOn = 0; // 1st player on
			break;
		// TODO...
		}
		break;
	case GAME_MODE:			
		if (sw == HOLE_SW) {
			hole();
			return;
		}
		if (tiltState) {
			return;
		}
		switch (sw) {
			DISPATCH(addNewPlayer(), REPLAY_SW);
			DISPATCH(setTilt(true), TILT_SW);
			//DISPATCH(hole(), HOLE_SW);
			// TODO...
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

void PinGame::millisRoutine(uint32_t& ms) {

	tmr.update(ms);
	_checkPressedKey(ms);

	// updates all non-steady mode lights state
	/* if (currentLampGroup == 0) */ LightGroup::activeLightSet->update(ms);
	
	// PERCHE' NON FUNZIONA ?!
	//byte cl = currentLampGroup << 2;
	//LightGroup::activeLightSet->renderToLamps(lamps, cl, cl + 3);
	LightGroup::activeLightSet->renderToLamps(*lamps);
	
	extDisplay->update(ms);
	
	Sys80b::_millisRoutine(ms); // may call onSwitchEvent(..)
}
