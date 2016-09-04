// fundamental Programmer Interface

//#include <LiquidCrystal.h>
#include <LiquidCrystalFast.h>
#include "mcp_io.h"
#include "display.h"
//#include "sdio.h"

#define DEF_BLINK_TIME 500
#define SW_ONOFF_MIN_TIME 50
#define SNDBUFF_LEN 10 
#define SNDCMD_TIME 100

#define writeSound(b)  mcpWritePB(3, b)
#define write4Lamps(a, b)  mcpWrite(1, ( mux16[a]<<4) | ((b) & 0xf) )
#define writeDisplayData(b)  mcpWritePB(3, b)
#define writeDisplayLD1(b)  digitalWrite(LD1_PIN, ((b) ? HIGH : LOW)
#define writeDisplayLD2(b)  digitalWrite(LD2_PIN, ((b) ? HIGH : LOW)
#define writeDisplayReset(b)  digitalWrite(DRES_PIN, ((b) ? LOW : HIGH)
#define writeStrobes(b)  mcpWritePA(2, (b & 0xff))
#define writeSolenoids(a)  mcpWrite(0, a)
#define RETURNS()  mcpReadPB(2)
#define SLAM_SW()  (digitalRead(SLAM_PIN) == HIGH)
#define getSwitch(n) ( ( returns_latch[(n)>>3] & (1<<((n)&7)) ) > 0 ? 1 : 0 )

//PROGMEM const char *inpName[] = { "RETURNS" , "SLAM SWITCH" };
//PROGMEM const char *outpName[] = { "STROBES", "SOLENOIDS", "SOUND", "LAMPS", "DISPLAY" };

PROGMEM byte displayInitSeq[] = { 
  CD_DIGIT_CNT | DISPLAY_COLS, 
  CD_DUTY_CYCLE | 0x20, 
  CD_NORMAL_MODE, 
  CD_START_SCAN, 
  0 };

PROGMEM const uint16_t mux16[16] = // 16 bit mux outputs
  { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 };
    
enum inputType { RETURNS, SLAM_SW };
enum outputType { STROBES, SOLENOIDS, SOUND, LAMPS, DISPL };

extern const byte D_LD1_PIN;
extern const byte D_LD2_PIN;
extern const byte D_RES_PIN;
extern const byte SLAM_PIN;

// single solenoid state
struct Solenoid {
  bool state = false;
  bool newState = false;
  bool delayedSwitch = false;
  uint32_t activationTime; // time when solenoid has activated
  uint16_t activePeriod = 0; // ms
  uint16_t switchDelay = 0; // ms
};

Solenoid solen[32];

byte sound;
uint16_t solenoids;
byte strobe;
byte lastSwitchClosed;
uint32_t lastSwitchClosedTime[8];
byte lamps[12]; // lamp states nybbles (groups of 4 lamps)
bool lampsUpdate[12]; // hardware lamp-group update-request flags (nybbles)
byte returns_latch[8];
uint16_t staticRetCount[8];
byte sndCmdBuf[SNDBUFF_LEN];
byte sndBufIdx = 0;
byte sndBufLen = 0;
bool sndCmdOut = false;

//extern const byte special_lamp[]; // special lamps
//extern const byte special_lamp_count;
//extern const uint16_t solMaxOnTime[];

void initAPI();
void setSolenoid(byte n, bool state);
void setSolenoid(byte n, bool state, uint16_t actPeriod);
void setSolenoidDelayed(byte n, bool state, uint16_t swDelay);
void resetSolenoids();
void setLamp(byte n, bool state);
void set4Lamps(byte set, byte states);
bool readLamp(byte n);
void resetLamps();
void renderLamps();
void setSound(byte snd);
void checkSoundCmd(uint32_t t);
void setStrobe(byte n);
bool readMatrixSw(byte col, byte row);
void clearDispl(byte line);
void getNextReturns(uint32_t t);
void checkSolenoids(uint32_t t);
//void setSolenoidsActiveTime(uint16_t *solTimes);
bool isSpecialLamp(byte lamp);

// base I/O functions
void writeDisplayCtrl(byte ctrl);

extern void onEvent(byte n, bool s); // pinSpecific.h

void initAPI() {
  int i;

  // machine setup
  writeSound(0);
  writeStrobes(0xff);
  resetLamps();
  resetSolenoids();
  strobe = 0;
  setStrobe(strobe);
  for (i=0; i<2; i++) clearDisplayRow(i);
  initDisplay();
  for (i=0; displayInitSeq[i] != 0; i++) dPushCmd(3, displayInitSeq[i]);
}

// note: 
// n = [1..9] standard solenoid
// n = [16..32] "lamp" solenoid 0..15
void setSolenoid(byte n, bool active) {
  if (n < 1 || n > 32) return;
  solen[n].state = active;
  solen[n].newState = active;
  if (active) solen[n].activationTime = millis();
  if (n >= 1 && n <= 9) {
    if (active) solenoids |= bit(n-1);
    else solenoids &= 0x1ff - bit(n-1);
    writeSolenoids(solenoids);
  }
  else if (n >= 16 && n <= 32) setLamp(n & 0xf, active); // lamps 0..15
  solen[n].activePeriod = 0;
  solen[n].delayedSwitch = false;
}

void setSolenoid(byte n, bool state, uint16_t actPeriod) {
  setSolenoid(n, state);
  solen[n].activePeriod = actPeriod;
}

void setSolenoidDelayed(byte n, bool state, uint16_t swDelay) {
  if (swDelay == 0) {
    setSolenoid(n, state);
    return;
  }
  solen[n].delayedSwitch = true;
  solen[n].newState = state;
  solen[n].switchDelay = swDelay;
  solen[n].activationTime = millis();
}

void setSolenoidActiveTime(byte n, uint16_t actPeriod) {
  if (n < 1 || n > 32) return;
  solen[n].activePeriod = actPeriod;
}

void resetSolenoids() {
  for (byte i=0; i<32; i++) setSolenoid(i, false);
}

// set a lamp on or off, with status memory
void setLamp(byte n, bool state) {
  byte b, set;
  
  if (n > 47) return;
  set = n / 4;
  b = 1 << (n % 4);
  if (state) lamps[set] |= b;
  else lamps[set] &= (~b);
  lampsUpdate[set] = true;
}

void set4Lamps(byte set, byte states) {
  if (set > 11) return;
  states &= 0x0f;
  if (lamps[set] == states) return;
  lamps[set] = states;
  lampsUpdate[set] = true;
}

bool readLamp(byte n) {
  if (n > 47) return false;
  return ((lamps[n/4] & bit(n%4)) != 0);
}

// switch immediately all lamps off
void resetLamps() {
  for (int i=0; i<12; i += 2) {
    lamps[i] = 0;
    write4Lamps(i, 0);
    lampsUpdate[i] = false;
  }
}

// to call periodically for real lamps update
void renderLamps() {
  byte i;

  for (i=0; i<12; i++) {
    if (lampsUpdate[i]) {
      write4Lamps(i, lamps[i]);
      lampsUpdate[i] = false;
    }
  }
}

/*
// snd = 0..31
void setSound(byte snd) {
  sound = snd &= 0x1f;
  writeSound(~sound);
  // sound16 light #4 update
  if (snd & 0x10) lamps[0] |= 0x10;
  else lamps[0] &= 0xef;
  write4Lamps(1, (lamps[0] & 0xf0) >> 4); // immediate light #4 update
}
*/

// snd = 0..31
void setSound(byte snd) {
  byte i;

  if (sndBufLen >= SNDBUFF_LEN) return; // return on full buffer
  i = (sndBufIdx + sndBufLen) % SNDBUFF_LEN; // cursor
  sndCmdBuf[i] = snd & 0x1f;
  sndBufLen++;
}

void checkSoundCmd(uint32_t t) {
  byte snd;

  if (sndCmdOut) {
    writeSound(0xff);
    sndCmdOut = false;
  }
  if (sndBufLen == 0) return;
  snd = sndCmdBuf[sndBufIdx];
  if (++sndBufIdx >= SNDBUFF_LEN) sndBufIdx = 0;
  sndBufLen--;
  sndCmdOut = true;
  writeSound(~snd);
  // sound16 light #4 update
  if (snd & 0x10) lamps[0] |= 0x10;
  else lamps[0] &= 0xef;
  write4Lamps(1, (lamps[0] & 0xf0) >> 4); // immediate light #4 update
}

// set one of eight strobes bit to 0 (active low)
// n = 0..7
void setStrobe(byte n) {
  writeStrobes( ~(((byte)1) << (n & 0b111)) );
}

bool readMatrixSw(byte col, byte row) {
  byte ret;
  if (col < 0 || col > 7) return false;
  if (row < 0 || row > 7) return false;
  setStrobe(col);
  ret = ~RETURNS();
  returns_latch[col] = ret;
  return ((ret & bit(row)) != 0);
}

// 1. reads returns on current strobe line 
// 2. call special subroutine on any open/closed-switch event (with debounce effect)
// 3. updates returns LED grid
// 4. increments strobe line for next read
void getNextReturns(uint32_t t) {
  byte i, ret, diff, b, ss, sw;
  byte stbx8;
  
  ret = RETURNS();
  diff = ret ^ returns_latch[strobe]; // return byte changed bits
  if (diff) { // some change occurred
    stbx8 = strobe << 3;
    for (i=0; i<8; i++) { // for every bit...
      b = 1<<i; // bit balue
      ss = ret & b; // single return bit (switch state)
      if (diff & b) { // switch has changed ?
        sw = stbx8 | i; // strobe * 8 + return
        if (ss) { // switch has closed
          lastSwitchClosed = sw;
          lastSwitchClosedTime[i] = t;
          onEvent(sw, true);
          returns_latch[strobe] = ret; 
        }
        else { // switch has opened
          if (lastSwitchClosed == i && 
              t - lastSwitchClosedTime[i] >= SW_ONOFF_MIN_TIME) { // debounce condition
            onEvent(sw, false);
            returns_latch[strobe] = ret;
          }
        }
      }
    }
    setLedRow(strobe, ret);
  }
  if (++strobe > 7) strobe = 0;
  setStrobe(strobe);
}

void checkSolenoids(uint32_t t) {
  uint16_t maxTime;
  Solenoid sol;
  
  for (byte i=1; i<32; i++) {
    if (i >= 10 && i < 16) continue;
    sol = solen[i];
    if (sol.delayedSwitch) { // delayed switch
      if (t - sol.activationTime >= sol.switchDelay) // delay elapsed
        setSolenoid(i, sol.newState);
    }
    if (sol.state) { // solenoid activated ?
      maxTime = 0;
      if (solMaxOnTime[i] > 0) maxTime = solMaxOnTime[i];
      if (sol.activePeriod > 0 && sol.activePeriod < maxTime) maxTime = sol.activePeriod;
      if (maxTime > 0) { // limited active-period ?
        if (t - sol.activationTime > maxTime) setSolenoid(i, false);
      }
    }
  }
}

/*
void checkSolenoids(uint32_t t) {
  uint16_t stdTime, maxTime;
  uint32_t dt;
  
  for (byte i=1; i<32; i++) {
    if (i >= 10 && i < 16) continue;
    if (solen[i].active) { // solenoid activated?
      stdTime = solen[i].activePeriod;
      maxTime = solMaxOnTime[i];
      dt = t - solen[i].activationTime;
      if ( (maxTime > 0 && dt > maxTime) || (stdTime > 0 && dt > stdTime) )
          setSolenoid(i, false);
    }
  }
}
*/

bool isSpecialLamp(byte lamp) {
  byte i;

  if (lamp > 47) return false;
  i = 0;
  while (i < special_lamp_count) if (lamp == special_lamp[i++]) return true;
  return false;
}

// base I/O functions ----------------------------------------------------------------

// bit 0: LD1 (active high)
// bit 1: LD2 (active high)
// bit 2: RESET (active low)
void writeDisplayCtrl(byte ctrl) {
  digitalWrite(D_LD1_PIN, ((ctrl & 1) != 0) ? HIGH : LOW);
  digitalWrite(D_LD2_PIN, ((ctrl & 2) != 0) ? HIGH : LOW);
  digitalWrite(D_RES_PIN, ((ctrl & 4) != 0) ? HIGH : LOW);
}


