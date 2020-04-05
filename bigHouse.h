/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
*/

//#ifdef _BigHouse_h_
#ifndef _BigHouse_h_
#define _BigHouse_h_

#include "Arduino.h"
#include "PinGame.h"
#include "msg.h"
//#include "TimerTask.h"
//#include "LightGroup.h"

#define _GAME_IMPLEM_ BigHouse

class LightGroup;
//extern Msg msg;

// Implements game specific rules and behavior.
// Valid only for the game specifed below.
//*** #713 "BIG HOUSE" ***
class BigHouse : public PinGame {
public:
	// pinball game-specific identification data
	const char* CLASSNAME = "BigHouse";
	const int GAME_NUMBER = 713;
	const char *GAME_NAME = "Big House";
	//const byte SND16_LMP = 4;

	bool multiballMode;
	bool ballTrap[3]; // left, right, upper right, lower right
	byte capturedBalls; // 0..3
	bool tableEnabled;
	bool extraBallGiven;
	byte cellBlockSeq, wallSeq, jailSeq, breakSeq, dropTargetHit;
	byte multiSeq;
	uint16_t bonusAdvanceSwitches;
	byte multiplier[4] = { 1, 2, 4, 8 };
	byte displayShowStep;
	uint16_t displayShowDelay;
	uint16_t lightShowDelay;
	uint16_t lightShowArg;
	bool lightShowOrder;
	byte lightShowType;
	uint16_t lightShowStep;
	TimerTask* dropt_tmr, * lshow_tmr, * brel_tmr, * dshow_tmr;
	int dropTargets_tt, lightShow_tt, ballRelease_tt, display_tt;

	// special lamps: not exactly real lamps but solenoids
	//static const byte SPECIAL_LAMPS_COUNT = 8;
	//const byte special_lamp[SPECIAL_LAMPS_COUNT] = { 0, 1, 2, 4, 12, 13, 14 };
	enum SpecialLamp { SND16_LMP = 4, RAMP_LMP = 12, BALLGATE_LMP = 13, AUGER_LMP = 14 };

	LightGroup *extraball_ls, *lastchance_ls, *special_ls, *wall_ls, *break_ls;
	LightGroup *jail_ls, *multi_ls, *cellblock_ls, *lightshow_ls, *droptarget_ls;
	LightGroup *show1_ls, *show2_ls, *show3_ls, *show4_ls, *show5_ls, *show6_ls;
	LightGroup* lShow_set[8];
	//LightGroup **lShow_set;

	// lamps description
	const String lightInfo[LAMPS_COUNT] = {
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
		F("Ramp 'lock'"),		// #15
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
#define liftRamp(v) setLamp(12, v)
#define openGate(v) setLamp(13, v)
#define switchAuger(v) setLamp(14, v)

// solenoids description
	 const String solInfo[SOLENOID_COUNT+1]  = {
		F("(none)"),           // #0 (void)
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

	// soundBuffer effects - TODO: to complete
	enum Sfx {
		SND_ALLRIGHT = 1,
		SND2,
		SND3,
		SND4,
		SND5,
		SND6,
		SND_FADING, // fades soundBuffer/music volume to 0
		SND_MAXVOL, // turns volume to max level
		SND_LOWERVOL, // turns volume to a lower level
		SND10,
		SND_MUSIC_SPECIAL, // special music (continuos)
		SND12,
		SND_MUSIC_GRAND_FINAL,
		SND14,
		SND_MUSIC, // standard music (continuos)
		SND16,
		SND17,
		SND18,
		SND_COIN_DROP,
		SND_MUSIC2, // (continuos)
		SND_MUSIC3, // (continuos)
		SND22,
		SND23,
		SND24,
		SND25,
		SND26,
		SND_SIREN_FAST, // fast siren
		SND_SIREN_SLOW, // slow siren (continuos)
		SND29,
		SND30,
		SND_SHUT_UP
	};

	// void string for unused switches in switch matrix
#define NOSW (const char*)nullptr

// switch matrix: inputs description
	class Sw {
	protected:
		Sys80b* _sys;
	public:
		SENSOR(ballTarget, 26, "ball on chain");
		SENSOR(leftSideRollover, 30, "left side rollover");
		SENSOR(break1, 31, "'BREAK' target #1");
		SENSOR(kickRubbers, 32, "kicking rubbers");
		SENSOR(topPopBumper, 33, "top pop bumper");
		SENSOR(spotTarget1, 34, "cell block spot Target #1");
		SENSOR(leftFlipper, 35, "left flipper");
		SENSOR(rightFlipper, 36, "right flipper");
		SENSOR(dropTarget1, 40, "drop target #1");
		SENSOR(break2, 41, "'BREAK' target #2");
		SENSOR(jail1, 42, "'JAIL' target #1");
		SENSOR(midPopBumper, 43, "center pop bumper");
		SENSOR(spotTarget2, 44, "cell block spot Target #2");
		SENSOR(leftOutside, 45, "left outside rollover");
		SENSOR(leftShooter, 46, "left shooter");
		SENSOR(dropTarget2, 50, "drop target #2");
		SENSOR(break3, 51, "'BREAK' target #3");
		SENSOR(jail2, 52, "'JAIL' target #2");
		SENSOR(bottomPopBumper, 53, "bottom pop bumper");
		SENSOR(topRollover1, 54, "top rollover #1");
		SENSOR(leftReturn, 55, "left return rollover");
		SENSOR(rightShooter, 56, "right shooter");
		SENSOR(dropTarget3, 60, "drop target #3");
		SENSOR(break4, 61, "'BREAK' target #4");
		SENSOR(jail3, 62, "'JAIL' target #3");
		SENSOR(spotTarget3, 63, "cell block spot Target #3");
		SENSOR(topRollover2, 64, "top rollover #2");
		SENSOR(rightReturn, 65, "right return rollover");
		SENSOR(outHole, 66, "out hole");
		SENSOR(dropTarget4, 70, "drop target #4");
		SENSOR(break5, 71, "'BREAK' target #5");
		SENSOR(jail4, 72, "'JAIL' target #4");
		SENSOR(spotTarget4, 73, "cell block spot Target #4");
		SENSOR(topRollover3, 74, "top rollover #3");
		SENSOR(rightOutside, 75, "right outside rollover");
		SENSOR(hole, 76, "hole");
		// constructor
		Sw(Sys80b* sys) : _sys(sys) {};
	};
	Sw sw = Sw(this);
	
	// specific actuators
	class Act {
	protected:
		Sys80b* _sys;
	public:
		ACTUATOR(leftShooter, SOLENOIDS, 2, "left shooter");
		ACTUATOR(leftDomeLight, SOLENOIDS, 3, "left top dome light");
		ACTUATOR(rightDomeLight, SOLENOIDS, 4, "right top dome light");
		ACTUATOR(rightShooter, SOLENOIDS, 5, "right shooter");
		ACTUATOR(bankReset, SOLENOIDS, 6, "bank reset");
		ACTUATOR(midDomeLight, SOLENOIDS, 7, "right side dome light");
		ACTUATOR(ballRelease, LAMPS, 2, "ball release");
		ACTUATOR(ramp, LAMPS, 12, "ramp");
		ACTUATOR(ballGate, LAMPS, 13, "ball gate");
		ACTUATOR(auger, LAMPS, 14, "auger motor");
		// constructor
		Act(Sys80b* sys) : _sys(sys) {};
	};
	Act act = Act(this);

// switches "relative lamps"
	 const byte swRelLamp[64]  = {
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

	BigHouse(Board_Sys80b& board);
	~BigHouse();

	void begin();
	void startNewGame();
	//bool isSpecialLamp(byte lamp);
	void onSwitchEvent(byte sw, bool st);
	void onButtonPressed(UserKey butt);
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
	void onBreakTarget(byte n);
	void onJailTarget(byte n);
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
	void dropTimerRoutine(uint32_t &ms);
	void startLightShow();
	void switchToNextLightShow();
	void startDisplayShow();
	void stopLightShow();
	void stopDisplayShow();
	void lightShowCycle(uint32_t &ms);
	void displayShowCycle(uint32_t& ms);
	//void onPRBButtonPressed(byte bt);
	String getGameName();
	int getGameNumber();
	String getTopScoreStr(byte n);
	bool releaseBall();
	void snd16Update(uint32_t& t);
	void timerRoutine(int taskIdentifer, uint32_t& ms);
	void millisRoutine(uint32_t& ms);
};

#endif
