/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* PINBALL SPECIFIC CODE
* ---
* Implements game specific properties, rules and behaviour.
* Valid only for the game specifed below.
*/

#include "pinGeneric.h"  // specific game code

/********************
*                   *
*   GOTTLIEB GAME   *
*        #713       *
*    "BIG HOUSE"    *
*  SOFTWARE MODULE  *
*                   *
*********************/

// pinball game-specific identification data
#define PIN_GAME_NAME "Big House"
#define PIN_GAME_NUMBER 713

// special lamps: not exactly real lamps but solenoids
const byte special_lamp[] = { 0, 1, 2, 4, 12, 13, 14, 15 };
const byte special_lamp_count = 8;

// lamps description
PROGMEM const String lightInfo[] = {
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

// solenoids description
PROGMEM const String solInfo[] = {
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
const uint16_t solMaxOnTime[] =
  { 500, 500, 0, 0, 500, 500, 0, 500, 500 };

// convenience solenoids constants
enum solenoid_ID {
  HOLE_SOL=1, LSHOOTER_SOL, LTOPDOME_SOL, RTOPDOME_SOL,
  RSHOOTER_SOL, BRESET_SOL, RDOME_SOL, KNOCKER_SOL, OUTHOLE_SOL,
  RAMP_SOL=28, BGATE_SOL, AUGER_SOL };

// sound effects ##TODO##
enum sound_ID { SND1=1, SND2, SND3, SND4, SND5, SND6, SND7, SND8,
                SND9, SND10, SND11, SND12, SND13, SND14, SND15, SND16,
                SND17, SND18, SND19, COIN_SND, SND21, SND22, SND23, SND24,
                SND25, SND26, SND27, SND28, SND29, SND30, SND31, SND32 };

// void string for unused switches in switch matrix
#define NOSW (const char*)NULL

// switch matrix: switches description
const String swInfo[] = {
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
const byte swRelLamp[] = {
   0,  0,  0,  0,  0,  0,  0,  0, // 0x
   0,  0,  0,  0,  0,  0,  0,  0, // 1x
   0,  0,  0,  0,  0,  0, 16,  0, // 2x
   0, 29,  0,  0, 42,  0,  0,  0, // 3x
   0, 30, 34,  0, 43,  0, 21,  0, // 4x
   0, 31, 35,  0,  5,  0,  0,  0, // 5x
   0, 32, 36, 44,  6,  0,  0,  0, // 6x
   0, 33, 37, 45,  7,  0, 15,  0, // 7x
};

// -------------------------------------

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

bool multiballMode;
bool ballTrap[3]; // left, right, upper right, lower right
byte capturedBalls; // 0..3
bool tableEnabled;
bool extraBallGiven;
byte cellBlockSeq, wallSeq, jailSeq, breakSeq, dropTargetSeq;
byte multiSeq;
uint16_t bonusAdvanceSwitches;
TimerTask *dropt_tmr, *lshow_tmr;
byte multiplier[] = { 1, 2, 4, 8 };
byte lightShowSeq;
uint16_t lightShowDelay;
bool lightShowOrder;

//extern TimerSet *tset;

// light sets - every set of light numbers MUST end with 0
LightSet extraball_ls("extraball",    (const byte[]){ 5, 6, 7, 0 });
LightSet lastchance_ls("lastchance",  (const byte[]){ 8, 9, 0 });
LightSet special_ls("special",        (const byte[]){ 10, 11, 0 });
LightSet wall_ls("wall",              (const byte[]){ 22, 23, 24, 25, 0 });
LightSet break_ls("break",            (const byte[]){ 29, 30, 31, 32, 33, 0 });
LightSet jail_ls("jail",              (const byte[]){ 34, 35, 36, 37, 0 });
LightSet multi_ls("multi",            (const byte[]){ 38, 39, 40, 41, 0 });
LightSet cellblock_ls("cellblock",    (const byte[]){ 42, 43, 44, 45, 0 });
LightSet lightshow_ls("lightshow",    (const byte[]){ 17, 46, 0 });
LightSet droptarget_ls("droptarget",  (const byte[]){ 26, 27, 28, 0 });
LightSet show1_ls("show1",            (const byte[]){ 26, 27, 28, 21, 0 });
LightSet show2_ls("show2",            (const byte[]){ 8, 10, 11, 9, 0 });
LightSet show3_ls("show3",            (const byte[]){ 15, 20, 44, 45, 0 });
LightSet show4_ls("show4",            (const byte[]){ 29, 30, 31, 32, 0 });
LightSet show5_ls("show5",            (const byte[]){ 5, 6, 7, 43, 0 });
LightSet show6_ls("show6",            (const byte[]){ 3, 17, 42, 33, 0 });

LightSet *lShow_ls[] = { &jail_ls, &wall_ls, &show1_ls, &show2_ls, &show3_ls, &show4_ls, &show5_ls, &show6_ls };

void initPinball();
void startAttractMode();
void initGame();
void initPlayer(); // TODO
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
void onPRBButtonPressed(byte bt);
String getGameName();

void initPinball() {
  initPinball_generic();
  dropt_tmr = new TimerTask(dropTimerRoutine, DROPT_LIGHT_TMR1, "Drop targets");
  lshow_tmr = new TimerTask(lightShowCycle, 200, "Light show");
  tset->add(dropt_tmr);
  tset->add(lshow_tmr);
}

void startAttractMode() {
  displayScoresAndCredits();
  startLightShow();
  //setTimerCall(updateAttractMode, 3000);
}

void initGame() {
  initGame_generic();
  multiballMode = false;
  bonusAdvanceSwitches = 100;
  cellBlockSeq = 0;
  multiSeq = 0;
  extraBallGiven = false;
  resetDropTargets();
  resetCapturedBalls();
}

// EVENT DISPATCHER ROUTINE
// sw = 0..63
void onEvent(byte sw, bool st) {
  sw = (sw>>3)*10 + (sw & 7); // format conversion
  switches++;
  switch (pinballMode) {
    case ATTRACT_MODE:
      if (sw == REPLAY_SW && st && credits > 0) {
        credits--;
        setPinMode(GAME_MODE);
      }
      // ..
      break;
    case GAME_MODE:
      if (switches >= bonusAdvanceSwitches) {
        switches -= bonusAdvanceSwitches;
        advanceMultiplier();
      }
      switch(sw) {
        DISPATCH1(onBallOnChain(), 26);
        DISPATCH1(onLeftRollover(), 30);
        DISPATCH5(onDropTarget((sw-31)/10), 31, 41, 51, 61, 71);
        DISPATCH1(onRubber(), 32);
        DISPATCH3(onPopBumper(), 33, 43, 53);
        DISPATCH2(onCellTarget((sw-34)/10), 34, 44);
        DISPATCH2(onCellTarget((sw-43)/10), 63, 73);
        DISPATCH4(onDropTarget((sw-40)/10), 40, 50, 60, 70);
        DISPATCH4(onJailTarget((sw-42)/10), 42, 52, 62, 72);
        DISPATCH2(onOutRollover((sw == 45) ? 0 : 1), 45, 75);
        DISPATCH1(onLeftKicker(), 46);
        DISPATCH3(onTopRollover((sw-54)/10), 54, 64, 74);
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

void resetCapturedBalls() {
  for (byte i=0; i<3; i++) ballTrap[i] = false;
  capturedBalls = 0;
}

void resetJailBreak() {
  jail_ls.setAll(OFF_L);
  break_ls.setAll(OFF_L);
  jail_ls.setL(0, BLINK_L);
  break_ls.setL(0, BLINK_L);
  jailSeq = 0;
  breakSeq = 0;
}

void resetDropTargets() {
  dropt_tmr->disable();
  dropt_tmr->period = DROPT_LIGHT_TMR1;
  dropTargetSeq = 0;
  droptarget_ls.setAll(OFF_L);
  setSolenoid(BRESET_SOL, true, 500);
}

void resetCellSpots() {
  cellblock_ls.setAll(ON_L);
}

void resetMultiplier() {
  multi_ls.setAll(OFF_L);
  multi_ls.setL(0, ON_L);
  multiSeq = 1;
}

void resetWall() {
  wall_ls.setAll(ON_L);
  wallSeq = 0;
}

bool hitWall() {
  bool wallDown = false;
  wall_ls.setL(wallSeq, OFF_L);
  if (++wallSeq >= 4) {
    advanceMultiplier();
    wallSeq = 0;
    wallDown = true;
  }
  return wallDown;
}

void advanceJail() {
  jail_ls.setL(jailSeq, ON_L);
  if (jailSeq < 4) jail_ls.setL(jailSeq++, BLINK_L);
}

void advanceBreak() {
  break_ls.setL(breakSeq, ON_L);
  if (breakSeq < 5) break_ls.setL(breakSeq++, BLINK_L);
}

void advanceMultiplier() {
  if (multiSeq < 3) multi_ls.setL(multiSeq++, ON_L);
  // .. sound
}

void awardExtraBall() {
  player[playerOn].totalGameBalls++;
  extraBallGiven = true;
  // .. sound
}

void onBallInHole() {
  //awardBonus();
  //..
}

String getGameName() {
  return String(PIN_GAME_NAME);
}

// GAME RULES ======================================================================

void onDropTarget(byte n) {
  if (_3BALLS) addScore(5000); else addScore(3000);
  addBonus(1000);

  if (dropTargetSeq == 0) {
    droptarget_ls.setL(0, BLINK_L);
    dropt_tmr->period = DROPT_LIGHT_TMR1;
    dropt_tmr->enable();
  }
  dropTargetSeq |= 1<<n;
  //if (allDropTargetsDown()) {
  if (dropTargetSeq == 0xf) { // all targets down
    if (droptarget_ls.isActive(2)) { // lights special
      special_ls.switchOne(BLINK_L, SPECIAL_TIME);
      droptarget_ls.setL(2, OFF_L);
    }
    else if (droptarget_ls.isActive(1)) { // 100000x
      addScore(100000);
      droptarget_ls.setL(1, OFF_L);
    }
    else if (droptarget_ls.isActive(0)) { // 10000x
      addScore(10000);
      droptarget_ls.setL(0, OFF_L);
    }
    resetDropTargets();
    // .. sounds
  }
}

void onJailTarget(byte n) {
  if (_3BALLS) addScore(3000); else addScore(1000);
  addBonus(1000);
  if (jail_ls.getLState(n) == BLINK_L) {
    advanceJail();
    addScore(10000);
  }
  else jail_ls.pulse(n, 100);
  if (jailSeq == 4 && breakSeq == 5) {
    extraball_ls.setL(1, ON_L);
    // .. sound
  }
}

void onBreakTarget(byte n) {
  if (_3BALLS) addScore(3000); else addScore(1000);
  addBonus(1000);
  if (break_ls.getLState(n) == BLINK_L) {
    advanceJail();
    addScore(10000);
  }
  else jail_ls.pulse(n, 100);
  if (jailSeq == 4 && breakSeq == 5) {
    extraball_ls.setL(1, ON_L);
    // .. sound
  }
}

void onCellTarget(byte n) {
  lightState ls;
  byte gln;

  if (_3BALLS) addScore(5000); else addScore(3000);
  addBonus(1000);
  ls = cellblock_ls.getLState(n);
  if (ls == ON_L) cellblock_ls.setL(n, BLINK_L);
  else if(ls == BLINK_L) {
    cellblock_ls.setL(n, OFF_L);
    cellBlockSeq |= bit(n);
    if (cellBlockSeq == 0b1111) {
      gln = extraball_ls.switchOne(ON_L);
      if (extraBallGiven || gln == 0xff) addScore(100000); // ?
      // .. sound
      cellBlockSeq = 0;
    }
  }
  else cellblock_ls.setL(n, ON_L); // when light is OFF
}

void onTopRollover(byte n) {
  if (_3BALLS) addScore(3000); else addScore(1000);
  addBonus(1000);
  if (n == 1) { // center rollover
    if (light[18].isActive()) addScore(100000);
    if (light[19].isActive()) advanceMultiplier();
  }
  if (extraball_ls.isActive(n)) {
    extraball_ls.setL(n, OFF_L);
    lastchance_ls.setAll(OFF_L);
    awardExtraBall();
  }
  // .. sound
}

void onLeftKicker() {
  // score
  if (_3BALLS) addScore(5000); else addScore(3000);
  if (!ballTrap[0]) { // ball already trapped
    extraball_ls.setL(1, ON_L);
  }
  else {
    light[15].set(BLINK_L);
    ballTrap[0] = true;
    // ... sound
  }
  //startTrapSequence(0);
}

void onRightKicker() {
  addScore(25000);
  //if (getLight(20) == BLINK_L) startMultiball(); // ???
  //startTrapSequence(2);
}

void onTopHole() { // hole
  addScore(25000);
  if (light[15].state == BLINK_L) ballTrap[2] = true;
  //else //..
  //startTrapSequence(1);
}

void onLeftRollover() {
  addScore(25000);
  addBonus(1000);
}

void onReturnRollover(byte n) {
  if (_3BALLS) addScore(5000);
  else addScore(3000);
  addBonus(1000);
  //..
}

void onOutRollover(byte n) {
  if (_3BALLS) addScore(5000);
  else addScore(3000);
  addBonus(1000);
  //..
}

void onPopBumper() {
  addScore(1000);
  special_ls.rotateRight();
  extraball_ls.rotateRight();
}

void onRubber() {
  addScore(30);
  special_ls.rotateRight();
}

void onBallOnChain() {
  addScore(5000);
  switches += 4;
}

void onOutHole() {
  if (tiltEnabled) resetTilt();
  // ..
}

// ================================================================================

void dropTimerRoutine(uint32_t ms) { // "light special" -> "100000x"
  if (droptarget_ls.isActive(2)) {
    droptarget_ls.setL(2, OFF_L);
    droptarget_ls.setL(1, BLINK_L);
  }
  else if (droptarget_ls.isActive(1)) { // "100000x" -> "10000x"
    droptarget_ls.setL(1, OFF_L);
    droptarget_ls.setL(0, BLINK_L);
    dropt_tmr->period = DROPT_LIGHT_TMR2;
  }
  else if (droptarget_ls.isActive(0)) { // "10000x" -> RESET
    droptarget_ls.setL(0, OFF_L);
    resetDropTargets();
  }
}

void startLightShow() {
  lightShowSeq = 0;
  lightShowDelay = 200; // ms
  lightShowOrder = false;
  for (byte i=0; i<8; i++) {
    lShow_ls[i]->setAll(OFF_L);
    lShow_ls[i]->setL(0, ON_L);
  }
  lshow_tmr->period = lightShowDelay;
  lshow_tmr->enable();
}

void stopLightShow() {
  lshow_tmr->disable();
}

void lightShowCycle(uint32_t ms) {
  Serial.print("lightShowCycle: (");
  Serial.print(ms);
  Serial.println(") ");
  //Serial.println(lightShowDelay);
  for (byte i=0; i<8; i++) {
    if (lightShowOrder) lShow_ls[i]->rotateLeft();
    else lShow_ls[i]->rotateRight();
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
void onPRBButtonPressed(byte bt) {
  switch (bt) {
    case BACK_BUTT:
      onEvent(LEFTADV_SW, true);
      break;
    case ENTER_BUTT:
      onEvent(REPLAY_SW, true);
      break;
    case NEXT_BUTT:
      onEvent(RIGHTADV_SW, true);
      break;
  }
}
