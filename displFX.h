/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* DISPLAY SPECIAL EFFECTS
* ---
* Display Extention library for special effects.
*/

#include "TimerTask.h"
#include <string.h>

#define STD_DFX_DELAY 100
#define STD_BLK_DELAY 500
#define TXT_MAX_LEN 100

#define disableDisplayEffect(l) dLine[(l)&1].fx_timer->disable()

enum dispEffect { // single line text display effects:
  DISPL_NO_EFF, // efffects disabled
  DISPL_SHOW_EFF,  // for overwidth msg only, bounces msg from left to right and back
  DISPL_LSCRL_EFF, // left scroll
  DISPL_RSCRL_EFF, // right scroll
};

struct DisplayLine {
    char msg[TXT_MAX_LEN+1]; // line text
    byte msgLen; // display text length
    short int chOffset; // where to write the msg
    byte displayEffect; // line display effect (FX) id
    byte fxVar;
    TimerTask *fx_timer;
    bool blinkState; // cycle phase (on/off)
    uint32_t blinkEnableTime;
    uint16_t blinkActivePeriod;
    byte blinkStartCol; // first blinking char
    byte blinkEndCol; // last blinking char
    char blinkStr[TXT_MAX_LEN+1]; // alternate string for line blinking
    TimerTask *blink_timer;
};

DisplayLine dLine[2];
extern TimerSet *tset;

void initDisplayFx();
void setDisplayText(byte line, const char *text);
void setDisplayText(byte line, const char *text, byte effect);
void enableDisplayEffect(byte line, byte effectId);
void setDisplayEffectPeriod(byte line, uint16_t period);
void _stepDisplayEffect(byte line);
void startDisplayBlink(byte line, uint16_t bDelay, uint16_t bPeriod);
void startDisplayBlink(byte line, uint16_t bDelay);
void startDisplayBlink(byte line);
void stopDisplayBlink(byte line);
void setDisplayBlink(byte line, bool enabled);
void setDisplayBlinkInterval(byte line, byte firstCh, byte lastCh);
void setDisplayBlinkPeriod(byte line, uint16_t bPeriod);
void _stepFx0(uint32_t);
void _stepFx1(uint32_t);
void _dBlink0(uint32_t t);
void _dBlink1(uint32_t t);
void _dBlinkSwitch(byte line);
void _updBlinkedTxt(byte line, byte startCh, byte endCh);
void updateDisplayLine(byte line);

void initDisplayFx() {
  // effects timers
  dLine[0].fx_timer = new TimerTask(_stepFx0, STD_DFX_DELAY, "R0 displFX");
  dLine[1].fx_timer = new TimerTask(_stepFx1, STD_DFX_DELAY, "R1 displFX");
  // blink timers
  dLine[0].blink_timer = new TimerTask(_dBlink0, STD_DFX_DELAY, "R0 displBlink");
  dLine[1].blink_timer = new TimerTask(_dBlink1, STD_DFX_DELAY, "R1 displBlink");
  for (byte i=0; i<=1; i++) {
    tset->add(dLine[i].fx_timer);
    tset->add(dLine[i].blink_timer);
  }
}

void setDisplayText(byte line, const char *text) {
  disableDisplayEffect(line);
  stopDisplayBlink(line);
  strncpy(dLine[line].msg, text, TXT_MAX_LEN);
  dLine[line].msgLen = strlen(text);
  dLine[line].chOffset = 0;
}

void setDisplayText(byte line, const char *text, byte effect) {
  setDisplayText(line, text);
  enableDisplayEffect(line, effect);
}

void enableDisplayEffect(byte line, byte effectId) {
  byte fxv = 0; // effect variable
  short int off = 0; // string offset
  uint16_t fxd = STD_DFX_DELAY; // effect delay

  line &= 1;
  if (effectId == DISPL_SHOW_EFF && dLine[line].msgLen <= DISPLAY_COLS)
    effectId = DISPL_NO_EFF;
  disableDisplayEffect(line);
  switch (effectId) {
    case DISPL_NO_EFF:
      off = 0;
      break;
    case DISPL_SHOW_EFF:
      off = 0;
      fxd = 100;
      fxv = 0; // text direction: 0-3 = left; 4-7 = right
      break;
    case DISPL_LSCRL_EFF:
      char buff[TXT_MAX_LEN+DISPLAY_COLS+1];
      // concatenates string on display with msg
      strcpy(buff, dLine[line].msg);
      strcpy(dLine[line].msg, txtBuff[line]);
      strcat(dLine[line].msg, buff);
      dLine[line].msgLen += DISPLAY_COLS;
      strcpy(buff, dLine[line].blinkStr);
      strcpy(dLine[line].blinkStr, txtBuff[line]);
      strcat(dLine[line].blinkStr, buff);
      off = 0;
      fxd = 50;
      break;
    case DISPL_RSCRL_EFF:
      // concatenates msg with string on display
      strncat(dLine[line].msg, txtBuff[line], TXT_MAX_LEN);
      off = -((short int)dLine[line].msgLen);
      dLine[line].msgLen += DISPLAY_COLS;
      strncat(dLine[line].blinkStr, txtBuff[line], TXT_MAX_LEN);
      fxd = 50;
      break;
    default:
      return;
  }
  dLine[line].displayEffect = effectId;
  dLine[line].fxVar = fxv;
  dLine[line].chOffset = off;
  if (effectId == DISPL_NO_EFF) return;
  dLine[line].fx_timer->set(fxd, true);
  dLine[line].fx_timer->enable();
}

void setDisplayEffectPeriod(byte line, uint16_t period) {
  dLine[line&1].fx_timer->period = period;
}

// private
void _stepDisplayEffect(byte line) {
  byte fxv; // effect variable (optional)
  bool fxEnd; // effect end reached
  byte len; // msg length
  short int off;

  fxv = dLine[line].fxVar;
  off = dLine[line].chOffset;
  fxEnd = false;
  len = dLine[line].msgLen;
  switch (dLine[line].displayEffect) {
    case DISPL_LSCRL_EFF:
      displayScrollLeft(line);
      if (--off < DISPLAY_COLS-len) fxEnd = true;
      updateDisplayLine(line);
      break;
    case DISPL_RSCRL_EFF:
      displayScrollRight(line);
      if (++off > 0) fxEnd = true;
      updateDisplayLine(line);
      break;
    case DISPL_SHOW_EFF:
      if (fxv < 4) {
        if (fxv == 0 && off > -(short int)(len-DISPLAY_COLS)) off--;
        else ++fxv;
      }
      else { // fxv >= 4
        if (fxv == 4 && off < 0) off++;
        else if (++fxv > 7) fxv = 0;
      }
      updateDisplayLine(line);
      break;
  }
  dLine[line].fxVar = fxv;
  dLine[line].chOffset = off;
  if (fxEnd) disableDisplayEffect(line);
}

void stopDisplayEffect(byte line) {
  dLine[line&1].fx_timer->disable();
}

void startDisplayBlink(byte line, uint16_t bDelay, uint16_t bPeriod) {
  line &= 1;
  dLine[line].blink_timer->set(bDelay, true);
  dLine[line].blink_timer->enable();
  dLine[line].blinkState = false;
  dLine[line].blinkActivePeriod = bPeriod;
  dLine[line].blinkEnableTime = millis();
}

void startDisplayBlink(byte line, uint16_t bDelay) {
  startDisplayBlink(line, bDelay, 0);
}

void startDisplayBlink(byte line) {
  startDisplayBlink(line, STD_BLK_DELAY, 0);
}

void stopDisplayBlink(byte line) {
  line &= 1;
  dLine[line].blink_timer->disable();
  dLine[line].blinkState = true;
  updateDisplayLine(line);
}

void setDisplayBlink(byte line, bool enabled) {
  if (enabled) startDisplayBlink(line);
  else stopDisplayBlink(line);
}

void setDisplayBlinkPeriod(byte line, uint16_t bPeriod){
  dLine[line&1].blinkActivePeriod = bPeriod;
}

void _stepFx0(uint32_t t) {
  _stepDisplayEffect(0);
}

void _stepFx1(uint32_t t) {
  _stepDisplayEffect(1);
}

void _dBlink0(uint32_t t) {
  _dBlinkSwitch(0);
}

void _dBlink1(uint32_t t) {
  _dBlinkSwitch(1);
}

void _dBlinkSwitch(byte line) {
  bool st;
  uint32_t dt;

  st = dLine[line].blinkState;
  st = !st;
  if (dLine[line].blinkActivePeriod > 0) { // limited time blink
    dt = millis() - dLine[line].blinkEnableTime;
    if (dt > dLine[line].blinkActivePeriod) { // blink period is over
      st = true;
      stopDisplayBlink(line);
    }
  }
  dLine[line].blinkState = st;
  updateDisplayLine(line);
}

void _updBlinkedTxt(byte line, byte startCh, byte endCh) {
  char *oc, *dc;
  byte i, l;

  l = dLine[line].msgLen;
  if (startCh >= l || endCh < startCh) return;
  if (endCh >= l) endCh = l-1;
  oc = dLine[line].msg;
  dc = dLine[line].blinkStr;
  strcpy(dc, oc);
  for (i=startCh; i<=endCh; i++) dc[i] = ' ';
}

void updateDisplayLine(byte line) {
  char *st;
  DisplayLine &dl = dLine[line];
  if (dl.blink_timer->enabled && dl.blinkState) st = dl.blinkStr;
  else st = dl.msg;
  writeDisplayText(line, st, dl.chOffset);
}
