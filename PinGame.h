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

#ifndef _PinGame_h_
#define _PinGame_h_

#include "Arduino.h"
#include "Sys80b.h"
#include "LightSet.h"
#include "Light.h" // for lightState enum definition
#include "TimerSet.h"
#include "TimerTask.h"
#include "LightGroup.h"
#include "Sys80b_Sensor.h"
#include "Sys80b_Actuator.h"
#include "ExtTextDisplay.h"

#define DISPATCH(f,a) case a: f; return

enum PinballMode { SHOW_MODE, GAME_MODE, TEST_MODE, BOOKKEEP_MODE, SETTINGS_MODE };

//struct Mode {
//	static const byte ATTRACT = 0;
//	static const byte PLAY = 1;
//	static const byte TEST = 2;
//	static const byte BOOKKEEP = 3;
//	static const byte SETTINGS = 4;
//};

// top score entry
struct TopPlay {
	uint32_t score;
	String scoreString;
	String playerName;
};

struct Player {
	uint32_t score = 0; // total play score
	uint32_t bonus = 0; // ball bonus scor
	byte ballsLeft = 0; // balls to play; 0 -> game over
	byte awardLevelReached = 0;
	bool topScorer = false;
	//bool gameOver = false;
};

class PinGame : public Sys80b, public TimerTaskExecutor {
public:
	static const byte MAX_PLAYERS = 4;
	static const byte TOP_SCORES = 5;
	static const byte SCORE_DIGITS = 8;

	static const byte dPlayerRow[];
	static const byte dPlayerCol[];

	TopPlay topScore[TOP_SCORES];
	Player player[MAX_PLAYERS];
	byte credits;
	byte ch_coins[3]; // coins for credits (left, center, right) (settings)
	byte ch_credits[3]; // credits by coins (left, center, right) (settings)
	byte coins[3]; // uncredited coins inserted (left, center, right)
	//byte PinballMode; // see enum pinball_mode
	//byte defaultBallsPerPlay;
	byte modeStep; // see enum gameStage
	byte playerOn; // current player index: 0..MAX_PLAYER-1
	bool tiltState;
	//bool allBallsInHole;
	//byte ballsOnField;
	uint32_t awardLevelScore[3]; // next award score
	//byte lastSw;
	uint16_t swCount; // switchGrid changes counter
	//bool tiltEnabled;
	LightSet* lights; // default light set
	//LightSet* altLights;
	ExtTextDisplay* extDisplay;

#define SENSOR(name, n, descr) Sys80b_Sensor name = Sys80b_Sensor(_sys, n, F(descr))
	class StdSw {
	protected:
		Sys80b* _sys;
	public:
		SENSOR(leftAdvance, LEFTADV_SW, "Left advance button");
		SENSOR(test, TEST_SW, "Test button");
		SENSOR(rightAdvance, RIGHTADV_SW, "Right advance button");
		SENSOR(leftCoin, LEFTCOIN_SW, "Left Coin");
		SENSOR(rightCoin, RIGHTCOIN_SW, "Right coin");
		SENSOR(centerCoin, CENTERCOIN_SW, "Right coin");
		SENSOR(replay, REPLAY_SW, "Replay button");
		SENSOR(setTilt, REPLAY_SW, "Tilt (cabinet)");
		SENSOR(hole, HOLE_SW, "Hole");
		// constructor
		StdSw(Sys80b* sys) : _sys(sys) {
			//Serial.println("StdSw init...");
			//delay(100);
		};
	};
	StdSw swStd = StdSw(this);

#define ACTUATOR(name, type, n, descr) Sys80b_Actuator name = Sys80b_Actuator(_sys, type, n, F(descr))
	class StdAct {
	protected:
		Sys80b* _sys;
	public:
		ACTUATOR(holeKicker, SOLENOIDS, HOLE_SOL, "hole");
		ACTUATOR(knocker, SOLENOIDS, KNOCKER_SOL, "knocker");
		ACTUATOR(outhole, SOLENOIDS, OUTHOLE_SOL, "outhole");
		ACTUATOR(relay_Q, LAMPS, Q_RELAY_LAMP, "Q relay");
		ACTUATOR(relay_T, LAMPS, T_RELAY_LAMP, "T relay");
		// constructor
		StdAct(Sys80b* sys) : _sys(sys) {
			//Serial.println("StdAct init...");
			//delay(100);
		};
	};
	StdAct actStd = StdAct(this);

protected:
	//Sys80b _sys;
	//TimerTask* gpTimerTask;// general purpose timer
	TimerSet tmr; // general purpose timer
	//uint16_t specialLight_tid;
	//GameTimers tmr; // general purpose timer
	//LightSet* _activeLightSet;
	String _gameName;
	uint16_t _gameNum;
	PinballMode _mode;
	bool _gameOver; // no players on game
	byte _players; // 1..MAX_PLAYER
	bool _3balls;
	//String inits;
	String _scoreStr;

public:
	PinGame(Board_Sys80b& board);
	virtual ~PinGame();
	void reset();
	virtual void begin() {};
	inline bool gameIsOver() { return _gameOver; }
	bool addNewPlayer();
	bool switchToNextPlayer();
	virtual void onPlayerChange(byte n) {};
	virtual void initPlayer(byte n);
	void hole();
	void setTilt(bool st);
	void setPinballMode(PinballMode mode);
	//void displayCurrentScore();
	void displayCredits();
	void displayScore(byte player, bool blink);
	void displayScoresAndCredits();
	String getScoreStr(uint32_t scr, byte digits, bool leadingZeros = false, bool dots = false);
	//void onButtonPressed(byte bt);
	void loadHighScores();
	void loadAwardScoreLevels();
	void loadCoinsPerCredits();
	//uint16_t incrementPowerOnCounter();
	void addCredits(byte increment);
	//void incrementPlayers();
	void addExtraBall();
	void addScore(uint32_t sc);
	void addBonus(uint32_t sc);
	//void setActivePlayer(byte nextPlayer);
	//void updateLights(uint32_t ms);
	//void setDelayedCall(func_t sub, uint32_t dlay);
	//void setPeriodicCall(func_t sub, uint32_t _period);
	//void switchModeStep(byte stp);
	//byte getNextPlayerOn();
	String getTopPlayerString(byte playerNum);
	//void onPRBButtonPressed(Sys80bIO::buttonID bt);
	//void setDefaultLightSet();
	inline void setActiveLightSet(LightSet *ls) { LightGroup::activeLightSet = ls; }
	inline LightSet* getActiveLightSet() { return LightGroup::activeLightSet; }
	inline void setLight(byte n, lightState s) { 
		LightGroup::activeLightSet->getLight(n)->set(s);
	}
	inline void setLight(byte n, bool s) {
		LightGroup::activeLightSet->getLight(n)->set(s ? ON_L : OFF_L);
	}
	inline void blinkLight(byte n, 
		uint16_t blinkPeriod = 250, byte blinkDutyC = 50, byte ticks = 0) {
			LightGroup::activeLightSet->getLight(n)->blink(blinkPeriod, blinkDutyC, ticks);
	}
	inline void pulseLight(byte n, uint16_t pulsePeriod = 250) {
		LightGroup::activeLightSet->getLight(n)->pulse(pulsePeriod);
	}
	inline void invertLight(byte n) {
		LightGroup::activeLightSet->getLight(n)->invert();
	}
	inline bool isLightOn(byte n) {
		return (LightGroup::activeLightSet->getLight(n)->state == ON_L);
	}
	inline bool isLightActive(byte n) {
		return LightGroup::activeLightSet->getLight(n)->isActive();
	}
	inline void switchOffAllLights() { LightGroup::activeLightSet->switchOffAllLights(); }
	//inline void setTilt() { /* TODO.. */ }
	inline void onSlamSwitchEvent(bool active) { if (active && !_gameOver) onTilt(); }
	virtual void onPinballModeChange(PinballMode mode) {};
	virtual void onKeyPressed(UserKey key);
	virtual void onSwitchClosed(byte sw);
	virtual void onCoinInserted(byte cch);
	virtual void onTilt() {}
	virtual void onHole() {};
	virtual void onGameOver(byte n) {};
	virtual void onGameStart() {};
	virtual void millisRoutine(uint32_t& ms);
	inline String getGameName() { return _gameName; } // returns game name
	inline int getGameNumber() { return _gameNum; } //  returns Gottileb game id number
	virtual void timerRoutine(int taskIdentifer, uint32_t& tm) {}

protected:
	void _checkPressedKey(uint32_t& ms);
};


#endif
