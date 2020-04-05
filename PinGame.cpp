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
const byte PinGame::dPlayerCol[] = { 0, 10, 0, 10 };

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
	_keyRepeat = false;
	//_mode = NORMAL_MODE;
	//_gameOver = true;
	_players = 0;
	credits = 0; // TODO

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
														   
	//_3balls = (getBallsCount() == 3) ? true : false;
	_3balls = true; // TODO
	loadCoinsPerCredits();
	loadAwardScoreLevels();
	loadHighScores();
	//defaultBallsPerPlay = (getSettingSwitch(25) == 1) ? 3 : 5;
	
	//setPinballMode(SHOW_MODE);
}

void PinGame::startNewGame() {
	_gameOver = false;
	setTiltRelay(false); // NO tilt
	setOnGameRelay(false); // NO game over
	addPlayer();
	setActivePlayer(0); // 1st player
}

// returns true when succesfull
bool PinGame::addPlayer() {
	if (_players >= MAX_PLAYERS) return false;
	if (_gameOver) startNewGame();
	_initPlayer(_players++);
	displayScoresAndCredits();
	return true;
}

void PinGame::_initPlayer(byte n) {
	Player& p = player[n];
	p.score = 0;
	p.bonus = 0;
	p.ballsLeft = getBallsCount();
	p.awardLevelReached = 0;
	p.topScorer = false;
}

void PinGame::setPinballMode(pinballMode m) {
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
		setOnGameRelay(true); // game over
		extDisplay->clear();
		_gameOver = false;
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

void PinGame::displayCurrentScore() {
	String st = String(F("0000000")) + String(player[playerOn].score);
	int l = st.length();
	if (l > 8) st = st.substring(l - 8);
	extDisplay->clear();
	extDisplay->putText(dPlayerRow[playerOn], st, dPlayerCol[playerOn]);
	//delete st;
}

void PinGame::displayCredits() {
	String crStr = String(credits);
	extDisplay->putText(1, crStr, 9);
	//delete& crStr;
}

// TODO
void PinGame::displayScoresAndCredits() {
	String st[4];
	int i;

	//for (i = 0; i < 4; i++) {
	//	st[i] = String(F("0000000")) + String(player[i].score);
	//	l = st[i].length();
	//	if (l > 8) st[i] = st[i].substring(l - 8);
	//}
	//displayFx->clear();
	//displayFx->putText(0, st[0], 0);
	//displayFx->putText(0, st[1], 12);
	//displayFx->putText(1, st[2], 0);
	//displayFx->putText(1, st[3], 12);

	extDisplay->clear();
	for (i = 0; i < 4; i++) st[i] = getScoreStr(player[i].score, SCORE_DIGITS, false);
	extDisplay->putText(0, st[0] ,0);
	extDisplay->putText(0, st[1], 12);
	extDisplay->putText(1, st[2], 0);
	extDisplay->putText(1, st[3], 12);

	String crStr = String(credits);
	extDisplay->putText(1, crStr, 10 - crStr.length());
}

String PinGame::getScoreStr(uint32_t scr, byte digits, bool leadingZeros, bool dots) {
	byte ch, i, len;
	String st = F("0000000000") + String(scr);
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
	for (i = 0; i < 3; i++) {
		v = hw->getSettingByte(i) & 0b00011111;
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
	//	addPlayer();
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
	//stdAct.knocker.activate(250);
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
}

void PinGame::addBonus(uint32_t sc) {
	player[playerOn].bonus += sc;
}

// sets new player on game, updating display
void PinGame::setActivePlayer(byte nextPlayer) {
	if (nextPlayer == playerOn) return;
	playerOn = nextPlayer;
	if (playerOn < MAX_PLAYERS) {
		int r, c;
		r = dPlayerRow[playerOn];
		c = dPlayerCol[playerOn];
		String scStr = String(player[nextPlayer].score);
		extDisplay->putText(r, scStr, c, true, true);
		extDisplay->setBlinkParams(r, 1000);
		//delete &scStr;
	}
}

// gives the next player on play (0..MAX_PLAYERS-1)
// returns MAX_PLAYERS when all players are over
byte PinGame::getNextPlayerOn() {
	byte p = playerOn++;

	if (p > MAX_PLAYERS) p = 0;
	while (p <= MAX_PLAYERS) {
		if (player[p].ballsLeft == 0) break;
		p++;
	}
	return p;
}

String PinGame::getTopPlayerString(byte playerNum) {
	String str = "<" + topScore[playerNum].playerName + ">";
	str += " -  " + topScore[playerNum].scoreString;
	return str;
}

// void PinGame::setDefaultLightSet() {
//	setActiveLightSet(lights);
//}

void PinGame::millisRoutine(uint32_t& ms) {

	tmr.update(ms);
	
	// updates all non-steady mode lights state
	/* if (currentLampGroup == 0) */ LightGroup::activeLightSet->update(ms);
	
	// PERCHE' NON FUNZIONA ?!
	//byte cl = currentLampGroup << 2;
	//LightGroup::activeLightSet->renderToLamps(lamps, cl, cl + 3);
	LightGroup::activeLightSet->renderToLamps(*lamps);
	
	extDisplay->update(ms);

	Sys80b::_millisRoutine(ms); // may call onSwitchEvent(..)
}
