#include "tableItems.h"
#include "ledGrid.h"
#include "baseAPI.h"
#include "flashData.h"
#include "displFX.h"
#include "Light.h"
#include "LightStage.h"
#include "ui_menu.h"

#define MAX_PLAYERS 4
#define TOP_SCORES  5
#define LAMPS_COUNT 48

// on-board buttons IDs
#define BACK_BUTT  0
#define ENTER_BUTT 1
#define NEXT_BUTT  2

// pinball settings
#define GAME_BALLS() ((getSettingSwitch(25) == 1) ? 3 : 5)

// special lamps
#define setQrelay(s) setLamp(0,s)
#define setTrelay(s) setLamp(1,s)
#define setArelay(s) setLamp(14,s)

enum pinball_mode { START_MODE, ATTRACT_MODE, GAME_MODE, TEST_MODE, BOOKKEEP_MODE };
enum coinChutes { LEFT_CHUTE, CENTER_CHUTE, RIGHT_CHUTE };

// players score position on display
PROGMEM const byte dPlayerRow[] = { 0, 0, 1, 1 };
PROGMEM const byte dPlayerCol[] = { 0, 12, 0, 12 };
const char *zeroStr = "0000000";

struct topPlay {
  uint32_t score;
  byte initials[3];
};
topPlay topScore[TOP_SCORES];

struct Player{
  uint32_t score;
  uint32_t bonus;
  byte ballOnPlay;
  byte totalGameBalls; // balls of player game
  byte ballsInHole;
  byte awardLevelReached;
  bool topScorer;
  bool gameOver;
};
Player player[MAX_PLAYERS];

byte credits;
byte ch_coins[3];
byte ch_credits[3];
byte coins[3];
byte pinballMode;
byte defaultBallsPerPlay;
byte modeStep;
byte playerOn; // 0..3
byte players; // 1..4
bool allBallsInHole;
byte ballsOnField;
uint32_t awardLevelScore[3];
byte lastSw;
uint16_t switches;
bool tiltEnabled;
Light light[LAMPS_COUNT];
LightStage *lightStage;

extern bool lightsModeEnabled; // lightFX.h

void initPinball_generic();
void initGame_generic();
void initPlayer_generic(); // TODO
void setPinMode(byte mode);
void displayScoresAndCredits();
void onButtonPressed(byte bt);
void loadHighScores();
void loadAwardScoreLevels();
void loadCoinsPerCredits();
void onCoinInserted(byte cch);
void incrementPowerOnCounterStat();
void incrementCredits(byte cr);
void incrementPlayers();
void incrementBalls();
void addScore(uint32_t sc);
void addBonus(uint32_t sc);
bool switchToNextPlayer();
void tilt();
void resetTilt();
bool releaseBall();

extern void onEvent(byte sw);

void initPinball_generic() {
  byte i;

  lightStage = new LightStage(light, LAMPS_COUNT);
  setActiveLightStage(lightStage);
  incrementPowerOnCounterStat();
  //loadStats();
  loadCoinsPerCredits();
  loadAwardScoreLevels();
  loadHighScores();
  credits = 0;
  players = 0;
  defaultBallsPerPlay = (getSettingSwitch(25) == 1) ? 3 : 5;
  for (i=0; i<3; i++) coins[i] = 0; // coins reset
  for (i=0; i<MAX_PLAYERS; i++) player[i].score = 0; // scores reset
  lightsModeEnabled = true;
  setPinMode(START_MODE);
  setSolenoid(RELAY_Q_SOL, true); // game over
}

void initGame_generic() {
  playerOn = 0; // 1st player
  player[playerOn].ballOnPlay = 1;
  setSolenoid(RELAY_T_SOL, false); // NO tilt
  setSolenoid(RELAY_Q_SOL, false); // NO game over
}

void setPinMode(byte mode) {
  pinballMode = mode;
  modeStep = 0;
  switch (pinballMode) {
    case START_MODE:
      setDisplayText(0, (const char *) F("SYS80/B PRB 1.0 BY FABVOLPI@GMAIL.COM"));
      String s = new String(F("PRESS START FOR *") + String(GAMENAME) + "*");
      setDisplayText(1, s.c_str());
      break;
    case ATTRACT_MODE:
      displayScoresAndCredits();
      // ...
      break;
    case GAME_MODE:
      initGame_generic();
      // ...
      break;
    case TEST_MODE:
      initMenu();
      // ...
      break;
    case BOOKKEEP_MODE:
      // ...
      break;
  }
}

void displayScoresAndCredits() {
  String st[4];
  int i, l;
  
  for (i=0; i<4; i++) {
    st[i] = String(zeroStr) + String(player[i].score);
    l = st[i].length();
    if (l>8) st[i] = st[i].substring(l-8);
  }
  disableDisplayEffect(0);
  stopDisplayBlink(0);
  clearDispl();
  writeDisplayText(0, st[0]);
  writeDisplayText(0, 12, st[1]);
  writeDisplayText(1, st[2]);
  writeDisplayText(1, 12, st[3]);
  writeDisplayText(1, 9, credits);
}

void loadHighScores() {
  byte i, j;
  
  for (i=0; i<TOP_SCORES; i++) {
    if (i < 3) {
      j = 36 + i*8;
      topScore[i].score = getStat32(j);
      topScore[i].initials[0] = getStat8(j+4);
      topScore[i].initials[1] = getStat8(j+5);
      topScore[i].initials[2] = getStat8(j+6);
    }
    else {
      topScore[i].score = (TOP_SCORES-i) * 500000u;
      topScore[i].initials[0] = 'R';
      topScore[i].initials[1] = 'R';
      topScore[i].initials[2] = 'R';
    }
  }
}

void loadAwardScoreLevels() {
  for (byte i=0; i<3; i++) awardLevelScore[i] = getStat32(22 + i*4);
}

void loadCoinsPerCredits() {
  byte i, v;
  
  for (i=0; i<3; i++) {
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

void onCoinInserted(byte cch) {
  if (cch > 2) return;
  coins[cch]++;
  if (coins[cch] == ch_coins[cch]) {
    incrementCredits(ch_credits[cch]);
    coins[cch] = 0;
  }
  // setSound( );
}

void incrementPowerOnCounterStat() {
  uint16_t pot;
  byte i;
  
  pot = getStat16(0);
  if (pot == 0) { // on first bootup only
    for (i=0; i<4; i++) saveSettingByte(i, 0xff, factorySwSettings[i]);
    for (i=0; i<76; i++) saveStat8(i, factoryStatsBytes[i]);
  } 
  saveStat16(0, ++pot);
}

void incrementCredits(byte cr) {
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

void incrementPlayers() {
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

void extraBall() {
  player[playerOn].totalGameBalls++;
  setSolenoid(8, true); // kicker
}

void addScore(uint32_t sc) {
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
    incrementCredits(getSettingSwitch(23)*2 + getSettingSwitch(24));
    setSolenoid(8, true); // kicker
  }
}

void addBonus(uint32_t sc) {
  player[playerOn].bonus += sc;
}

bool switchToNextPlayer() {
  byte prevPlayer;
  bool res;
  
  prevPlayer = playerOn;
  if (++playerOn >= MAX_PLAYERS) playerOn = 0;
  while (player[playerOn].gameOver) {
    if (++playerOn >= MAX_PLAYERS) playerOn = 0;
    if (playerOn == prevPlayer) break;
  }
  res = !player[playerOn].gameOver; // another player available ?
  if (res) {
    int r, c;
    r = dPlayerRow[playerOn];
    c = dPlayerCol[playerOn];
    startDisplayBlink(playerOn/2, r);
    setDisplayBlinkInterval(r, c, c+7);
  }
  //if (!res) setPinMode(ATTRACT_MODE);
  return res;
}

void tilt() {
  tiltEnabled = true;
  setTrelay(true);
  // ...
}

void resetTilt() {
  tiltEnabled = false;
  setTrelay(false);
  // ...
}

bool releaseBall() {
  bool ballReady;
  
  ballReady = (getSwitch(36) > 0);
  if (ballReady) setSolenoid(BRELEASE_SOL, true, 500); // ball release
  return ballReady;
}

// to call periodically for real lamps update
void checkLights(uint32_t ms) {
  lstage->updateLights(ms);
  renderLamps();
}
