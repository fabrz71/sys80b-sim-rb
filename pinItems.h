/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* PINBALL GAME SPECIFIC IREMS
* ---
* Implements game specific items.
* Valid only for the game specifed below.
*/

/********************
*                   *
*   GOTTLIEB GAME   *
*        #713       *
*    "BIG HOUSE"    *
*       ITEMS       *
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

// specific special lamps
#define liftRamp(v) setLamp(12, v);
#define openGate(v) setLamp(13, v);
#define switchAuger(v) setLamp(14, v)

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

// solenoid grouping by fuse number
const byte solFuse[] = { 11, 12, 0, 0, 11, 12, 0, 4, 11 };

// convenience solenoids constants
enum solenoid_ID {
  HOLE_SOL=1, LSHOOTER_SOL, LTOPDOME_SOL, RTOPDOME_SOL,
  RSHOOTER_SOL, BRESET_SOL, RDOME_SOL, KNOCKER_SOL, OUTHOLE_SOL };
// RAMP_SOL=28, BGATE_SOL, AUGER_SOL };

// sound effects *TODO*
enum sound_ID { SND_ALLRIGHT=1,
                SND2,
                SND3,
                SND4,
                SND5,
                SND6,
                SND_FADING, // fades sound/music volume to 0
                SND_MAXVOL, // turns volume to max level
                SND_LOWERVOL, // turns volume to a lower level
                SND10,
                SND_MUSIC_SPECIAL, // special music (continuos)
                SND12,
                SND_MUSIC_FINAL,
                SND14,
                SND_MUSIC, // standard music (continuos)
                SND16,
                SND17,
                SND18,
                SND_COIN,
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
                SND_SILENCED };

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
