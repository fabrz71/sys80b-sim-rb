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

#ifndef _BigHouse_h_
#define _BigHouse_h_

/********************
*                   *
*   GOTTLIEB GAME   *
*        #713       *
*    "BIG HOUSE"    *
*  SOFTWARE MODULE  *
*                   *
*********************/

//#include "Arduino.h"
//#include "BaseLogic.h"
#include "Game.h"
#include "LightGroup.h"

//class TimerTask;

#define _GAME_CLASS_ BigHouse

#ifndef byte
typedef uint8_t byte;
#endif

class BigHouse : public Game
{
public:
	// pinball game-specific identification data
	const int GAME_NUMBER = 713;
	const char *GAME_NAME = "Big House";
	
	bool multiballMode;
	bool ballTrap[3]; // left, right, upper right, lower right
	byte capturedBalls; // 0..3
	bool tableEnabled;
	bool extraBallGiven;
	byte cellBlockSeq, wallSeq, jailSeq, breakSeq, dropTargetSeq;
	byte multiSeq;
	uint16_t bonusAdvanceSwitches;
	TimerTask *dropt_tmr, *lshow_tmr;
	byte multiplier[4] = { 1, 2, 4, 8 };
	byte lightShowSeq;
	uint16_t lightShowDelay;
	bool lightShowOrder;

	LightGroup extraball_ls, lastchance_ls, special_ls, wall_ls, break_ls;
	LightGroup jail_ls, multi_ls, cellblock_ls, lightshow_ls, droptarget_ls;
	LightGroup show1_ls, show2_ls, show3_ls, show4_ls, show5_ls, show6_ls;
	LightGroup *lShow_set;

	// special lamps: not exactly real lamps but solenoids
	static const byte SPECIAL_LAMPS_COUNT = 8;
	PROGMEM const byte special_lamp[SPECIAL_LAMPS_COUNT] = { 0, 1, 2, 4, 12, 13, 14, 15 };
	

	// lamps description
	PROGMEM const String lightInfo[LAMPS_COUNT] = {
		F("_Relay 'Q'"),       // #0: not a lamp: GAME OVER 'Q' relay
		F("_Relay 'T'"),       // #1: not a lamp: TILT 'T' relay
		F("_ball release"),    // #2: not a lamp: BALL RELEASE solenoid
		F("Shoot Again"),     // #3
		F("_Sound 16"),        // #4: not a lamp: SOUND16 signal
		F("#1 'extra ball'"),  // #5
		F("#2 'extra ball'"),  // #6
		F("#3 'extra ball'"),  // #7
		F("Left 'last chance'"), // #8
		F("Right 'last chance'"), // #9
		F("Left 'Special'"),     // #10
		F("Right 'Special'"),  // #11
		F("_ramp"),            // #12: not a lamp: RAMP solenoid
		F("_ball gate relay"), // #13: not a lamp: BALL GATE relay
		F("_auger motor relay 'A'"), // #14: not a lamp: AUGER MOTOR ('A' relay)
		F("Ramp 'lock'"), // #15
		F("center dome (#67)"), // #16
		F("bottom left & right light show"), // #17
		F("100,000x multiplier"),    // #18 glass top lights
		F("Special"),                // #19 glass top lights
		F("Right shooter 'escape'"), // #20
		F("Left shooter 'lock'"),
		F("Brick wall #4"),
		F("Brick wall #3"),
		F("Brick wall #2"),
		F("Brick wall #1"),
		F("10,000x multiplier"),
		F("100,000x multiplier"),
		F("Lights special"),
		F("#1 'BREAK'"),
		F("#2 'BREAK'"), // #30
		F("#3 'BREAK'"),
		F("#4 'BREAK'"),
		F("#5 'BREAK'"),
		F("#1 'JAIL'"),
		F("#2 'JAIL'"),
		F("#3 'JAIL'"),
		F("#4 'JAIL'"),
		F("1x multiplier"),
		F("2x multiplier"),
		F("4x multiplier"), // #40
		F("8x multiplier"),
		F("Cell block #1"),
		F("Cell block #2"),
		F("Cell block #3"),
		F("Cell block #4"),
		F("Top light show"),
		F("Spot/Search light") // #47
	};

	// specific special lamps
#define liftRamp(v) setLamp(12, v);
#define openGate(v) setLamp(13, v);
#define switchAuger(v) setLamp(14, v)

// solenBits description
	PROGMEM const String solInfo[SOLENOID_COUNT] = {
		F("hole"),             // #1
		F("left shooter"),     // #2
		F("left top dome light"),   // #3
		F("right top dome light"),  // #4
		F("right shooter"),    // #5
		F("bank reset"),       // #6 (R relay)
		F("right side dome light"), // #7
		F("knocker"),          // #8
		F("outhole"),          // #9
	};

	// maximum active time for each solenoid [ms]
	const uint16_t solMaxOnTime[SOLENOID_COUNT] =
	{ 500, 500, 0, 0, 500, 500, 0, 500, 500 };

	// solenoid grouping by fuse number
	const byte solFuse[SOLENOID_COUNT] = { 11, 12, 0, 0, 11, 12, 0, 4, 11 };

	// convenience solenBits constants
	enum Sol {
		HOLE = 1, LSHOOTER, LTOPDOME, RTOPDOME, RSHOOTER, BRESET, RDOME, KNOCKER, OUTHOLE
	};
	// RAMP_SOL=28, BGATE_SOL, AUGER_SOL };

	// sound effects - TODO: to complete
	enum Sfx {
		ALLRIGHT = 1,
		SND2,
		SND3,
		SND4,
		SND5,
		SND6,
		FADING, // fades sound/music volume to 0
		MAXVOL, // turns volume to max level
		LOWERVOL, // turns volume to a lower level
		SND10,
		MUSIC_SPECIAL, // special music (continuos)
		SND12,
		MUSIC_GRAND_FINAL,
		SND14,
		MUSIC, // standard music (continuos)
		SND16,
		SND17,
		SND18,
		COIN_DROP,
		MUSIC2, // (continuos)
		MUSIC3, // (continuos)
		SND22,
		SND23,
		SND24,
		SND25,
		SND26,
		SIREN_FAST, // fast siren
		SIREN_SLOW, // slow siren (continuos)
		SND29,
		SND30,
		SHUT_UP
	};

	// void string for unused switches in switch matrix
#define NOSW (const char*)NULL

// switch matrix: switches description
	const String swInfo[64] = {
		/* 00..05 */ NOSW,  NOSW,  NOSW,  NOSW,  NOSW,  NOSW,
		/* 06 */ F("Left advance button"),
		/* 07 */ F("Play/Test button"),

		/* 10..15 */ NOSW,  NOSW,  NOSW,  NOSW,  NOSW,  NOSW,
		/* 16 */ F("Right advance button"),
		/* 17 */ F("Left Coin Chute"),

		/* 20..25 */ NOSW,  NOSW,  NOSW,  NOSW,  NOSW,  NOSW,
		/* 26 */ F("ball on chain"),
		/* 27 */ F("Right Coin Chute"),

		/* 30 */ F("left side rollover"),
		/* 31 */ F("#1 target 'BREAK'"),
		/* 32 */ F("kicking rubbers"),
		/* 33 */ F("top pop bumper"),
		/* 34 */ F("#1 cell block spot target"),
		/* 35 */ F("left flipper"),
		/* 36 */ F("right flipper"),
		/* 37 */ F("Center Coin Chute"),

		/* 40 */ F("#1 drop target"),
		/* 41 */ F("#2 target 'BREAK'"),
		/* 42 */ F("#1 target 'JAIL'"),
		/* 43 */ F("right pop bumper"),
		/* 44 */ F("#2 cell block spot target"),
		/* 45 */ F("left outside rollover"),
		/* 46 */ F("left shooter"),
		/* 47 */ F("Replay button"),

		/* 50 */ F("#2 drop target"),
		/* 51 */ F("#3 target 'BREAK'"),
		/* 52 */ F("#2 target 'JAIL'"),
		/* 53 */ F("bottom pop bumper"),
		/* 54 */ F("#1 top rollover"),
		/* 55 */ F("left return rollover"),
		/* 56 */ F("right shooter"),
		/* 57 */ F("tilt"),

		/* 60 */ F("#3 drop target"),
		/* 61 */ F("#4 target 'BREAK'"),
		/* 62 */ F("#3 target 'JAIL'"),
		/* 63 */ F("#3 cell block spot target"),
		/* 64 */ F("#2 top rollover"),
		/* 65 */ F("right return rollover"),
		/* 66 */ F("outhole"),
		/* 67 */  NOSW,

		/* 70 */ F("#4 drop target"),
		/* 71 */ F("#5 target 'BREAK'"),
		/* 72 */ F("#4 target 'JAIL'"),
		/* 73 */ F("#4 cell block spot target"),
		/* 74 */ F("#3 top rollover"),
		/* 75 */ F("right outside rollover"),
		/* 76 */ F("hole"),
		/* 77 */ NOSW
	};

	// cabinet buttons' switches
#define LEFTADV_SW 6
#define RIGHTADV_SW 14
#define REPLAY_SW 39
#define TEST_SW 7

// switches "relative lamps"
	PROGMEM const byte swRelLamp[64] = {
		0,  0,  0,  0,  0,  0,  0,  0, // 0x
		0,  0,  0,  0,  0,  0,  0,  0, // 1x
		0,  0,  0,  0,  0,  0, 16,  0, // 2x
		0, 29,  0,  0, 42,  0,  0,  0, // 3x
		0, 30, 34,  0, 43,  0, 21,  0, // 4x
		0, 31, 35,  0,  5,  0,  0,  0, // 5x
		0, 32, 36, 44,  6,  0,  0,  0, // 6x
		0, 33, 37, 45,  7,  0, 15,  0, // 7x
	};

#define SPECIAL_TIME 60000
#define DROPT_LIGHT_TMR1 2000
#define DROPT_LIGHT_TMR2 5000

#define DISPATCH1(f,a) case a: f; break
#define DISPATCH2(f,a,b) case a: case b: f; break
#define DISPATCH3(f,a,b,c) case a: case b: case c: f; break
#define DISPATCH4(f,a,b,c,d) case a: case b: case c: case d: f; break
#define DISPATCH5(f,a,b,c,d,e) case a: case b: case c: case d: case e: f; break

#define _3BALLS (GAME_BALLS() == 3)
	//#define allDropTargetsDown() (getSwitch(40) == 1 && getSwitch(50) == 1 && getSwitch(60) == 1 && getSwitch(70) == 1)
#define PLAYER player[playerOn]

	BigHouse();
	void initPinball();
	void startAttractMode();
	void initGame();
	bool isSpecialLamp(byte lamp);
	void onEvent(byte sw, bool st);
	void resetCapturedBalls();
	void resetJailBreak();
	void resetDropTargets();
	void resetCellSpots();
	void resetMultiplier();
	void resetWall();
	bool hitWall();
	void advanceJail();
	void advanceBreak();
	void advanceMultiplier();
	void awardExtraBall();
	void onBallInHole();
	void onDropTarget(byte n);
	void onJailTarget(byte n);
	void onBreakTarget(byte n);
	void onCellTarget(byte n);
	void onTopRollover(byte n);
	void onLeftKicker();
	void onRightKicker();
	void onTopHole();
	void onLeftRollover();
	void onReturnRollover(byte n);
	void onOutRollover(byte n);
	void onPopBumper();
	void onRubber();
	void onBallOnChain();
	void onOutHole();
	void dropTimerRoutine(uint32_t);
	void startLightShow();
	void stopLightShow();
	void lightShowCycle(uint32_t ms);
	void onPRBButtonPressed(BoardIO::buttonID bt);
	String getGameName();
	int getGameNumber();

};

#endif