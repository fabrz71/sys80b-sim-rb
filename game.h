/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* GAME FUNDAMENTALS OBJECT
* ---
* Fundamental properties and logic for any Sys80/B game.
* Defines the minimal setPeriod of functions that any game should implement.
*/

#ifndef _Game_h_
#define _Game_h_

#include "Sys80b.h"
#include "DisplayWithFX.h"
#include "Player.h"
#include "Timer.h"

const int MAX_PLAYERS = 4;
const int TOP_SCORES = 5;
const int dPlayerRow[] = { 0, 0, 1, 1 };
const int dPlayerCol[] = { 0, 10, 0, 10 };

// pinball settings
#define GAME_BALLS() ((getSettingSwitch(25) == 1) ? 3 : 5)

// base functions
#define BALL_IN_OUTHOLE() getSwitch(66)

// standard "lamp" solenoids (controlled as lamps)
const int RELAY_Q_SOL = 16;
const int RELAY_T_SOL = 17;
const int BRELEASE_SOL = 18;
const int SND16_SOL = 20;

enum pinballMode { START_MODE, ATTRACT_MODE, GAME_MODE, TEST_MODE, BOOKKEEP_MODE };
enum coinChutes { LEFT_CHUTE, CENTER_CHUTE, RIGHT_CHUTE };

// GAME_INIT - new game initialization and remainig ball expulsion
// GAME_NEWBALL - new ball out
// ...TODO
enum gameModeSteps { GAME_INIT, GAME_WAIT_BALLS_IN_HOLE, GAME_NEWBALL, GAME_BONUS_AWARD, GAME_TILT, GAME_END };

// top score entry
struct topPlay {
	uint32_t score;
	byte initials[3];
};

class Game : public Sys80b {
public:
	topPlay topScore[TOP_SCORES];
	Player player[MAX_PLAYERS];
	byte credits;
	byte ch_coins[3]; // coins for credits (left, center, right) (settings)
	byte ch_credits[3]; // credits by coins (left, center, right) (settings)
	byte coins[3]; // uncredited coins inserted (left, center, right)
	byte pinballMode; // see enum pinball_mode
	byte defaultBallsPerPlay;
	byte modeStep; // see enum gameModeSteps
	byte playerOn; // 0..3
	bool gameOver; // no players on game
	byte players; // 1..4
	//bool allBallsInHole;
	byte ballsOnField;
	uint32_t awardLevelScore[3]; // next award score
	//byte lastSw;
	uint16_t swCount; // switchGrid changes counter
	//bool tiltEnabled;
	Light lights[LAMPS_COUNT];
	Light altLights[LAMPS_COUNT];
	TimerTask *gpTimer;// general purpose timer
	DisplayWithFX *display;
protected:
	Light *_currentLightSet;

public:
	Game();

	void init();
	void initGame_generic();
	void setPinballMode(byte mode);
	void displayScoresAndCredits();
	//void onButtonPressed(byte bt);
	void loadHighScores();
	void loadAwardScoreLevels();
	void loadCoinsPerCredits();
	void onCoinInserted(byte cch);
	uint16_t incrementPowerOnCounter();
	void incrementCredits(byte cr);
	void incrementPlayers();
	void extraBall();
	void addScore(uint32_t sc);
	void addBonus(uint32_t sc);
	void setPlayerOn(byte newPlayer);
	//void updateLights(uint32_t ms);
	bool releaseBall();
	void setDelayedCall(func_t sub, uint32_t dlay);
	void setPeriodicCall(func_t sub, uint32_t period);
	void startGame(uint32_t t);
	void switchModeStep(byte stp);
	byte getNextPlayerOn();
	//void onPRBButtonPressed(BoardIO::buttonID bt);
	void setCurrentLightSet(Light *ls);
	void switchOffAllLights();
	void renderLamps();
	void updateLights(uint32_t ms);
	void updateAndRenderLamps(uint32_t ms);
	virtual void onEvent(byte sw, bool st);

	// virtual functions to be implemented by Game subclass
	virtual String getGameName() = 0; // returns game name
	virtual int getGameNumber() = 0; //  returns Gottileb game id number
	virtual void initPinball() = 0; // initializes the machine
	virtual void startAttractMode() = 0; // starts idle mode
	virtual void initGame() = 0; // initialize a new play
	virtual bool isSpecialLamp(byte lamp) = 0; // return true if specified lamp is a solenoid
	//virtual void onEvent(byte sw, bool st) = 0;
	virtual void onOutHole() = 0;
	virtual void onPRBButtonPressed(BoardIO::buttonID bt) = 0;
};

#endif