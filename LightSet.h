/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT SET OBJECT DEFINITION
* ---
* A "Light set" are a set of game lighs of a group.
* Contains C++ code.
*/

#include "LightStage.h"

#define MAX_LSET_SIZE 32
#define S_LIGHT(n) LightStage::currentStage->light[lightNum[n]]

class LightSet {
  public:
    const char *nameStr;
    byte *lightNum; // array
    static Light _tmpLight; // temporary buffer

    LightSet(const char *setName, const byte *lightNumber, byte arrayLength);
    LightSet(const char *setName, const byte *lightNumber);
    void setL(byte n, lightState st);
    void setL(byte n, lightState st, uint16_t blinkP);
    void invert(byte n);
    void pulse(byte n, uint16_t tm);
    lightState getLState(byte n);
    inline bool isActive(byte n);
    void setAll(lightState st);
    void setAll(lightState st, uint16_t blinkP);
    byte switchOne(lightState st);
    byte switchOne(lightState st, uint16_t maxP);
    void setActivePeriod(byte n, uint16_t maxP);
    bool allLights(lightState st);
    void shiftRight();
    void rotateRight();
    void shiftLeft();
    void rotateLeft();
    inline byte length() { return _length; }
    void prLights();
  private:
    byte _length;
};

Light LightSet::_tmpLight; // class' static variable init

LightSet::LightSet(const char *setName, const byte *lightNumber, byte arrayLength) {
  nameStr = setName;
  lightNum = (byte *)lightNumber;
  if (arrayLength > MAX_LSET_SIZE) arrayLength = MAX_LSET_SIZE;
  _length = arrayLength;
  //printf("* new LightSet '%s' defined with %d lamps.\n", setName, _length);
}

LightSet::LightSet(const char *setName, const byte *lightNumber) {
  for (_length=0; _length < MAX_LSET_SIZE; _length++)
    if (lightNumber[_length] == 0) break;
  nameStr = setName;
  lightNum = (byte *)lightNumber;
  //printf("* new LightSet '%s' defined with %d lamps.\n", setName, _length);
}

void LightSet::setL(byte n, lightState st) {
  if (n >= _length) return;
  S_LIGHT(n).set(st);
}

void LightSet::setL(byte n, lightState st, uint16_t blinkP) {
  if (n >= _length) return;
  S_LIGHT(n).set(st, blinkP, 50);
}

void LightSet::invert(byte n) {
  if (n >= _length) return;
  S_LIGHT(n).invert();
}

void LightSet::pulse(byte n, uint16_t tm) {
  if (n >= _length) return;
  S_LIGHT(n).pulse(tm);
}

lightState LightSet::getLState(byte n) {
  if (n >= _length) return OFF_L;
  return S_LIGHT(n).state;
}

inline bool LightSet::isActive(byte n) {
  if (n >= _length) return false;
  return S_LIGHT(n).isActive();
}

void LightSet::setAll(lightState st) {
  for (int i=0; i < _length; i++) S_LIGHT(i).set(st);
}

void LightSet::setAll(lightState st, uint16_t blinkP) {
  for (int i=0; i < _length; i++) S_LIGHT(i).set(st, blinkP, 50);
}

// switch the first light of the group of a different state than st
// to the given state.
// returns: the number of group-light switched; 0xFF else
byte LightSet::switchOne(lightState st) {
  byte i, gl;

  gl = 0xff;
  i=0;
  while (i<_length) {
    if (S_LIGHT(i).state != st) {
      S_LIGHT(i).set(st);
      gl = i;
      break;
    }
  }
  return gl;
}

byte LightSet::switchOne(lightState st, uint16_t maxP) {
  byte gl;
  gl = switchOne(st);
  if (maxP > 0 && gl != 0xff) S_LIGHT(gl).activePeriod = maxP;
  return gl;
}

void LightSet::setActivePeriod(byte n, uint16_t maxP) {
  if (n >= _length) return;
  S_LIGHT(n).activePeriod = maxP;
}

bool LightSet::allLights(lightState st) {
  byte n = 0;
  while (n<_length && S_LIGHT(n).state == st) n++;
  return (n == _length);
}

void LightSet::shiftRight() {
  if (_length < 1) return;
  if (_length > 1) {
    for (int i = _length-1; i > 0; i--) S_LIGHT(i+1).copy(S_LIGHT(i));
  }
  S_LIGHT(0).reset();
}

void LightSet::rotateRight() {
  if (_length <= 1) return;
  //printf("rotateRight on group ");
  //prLights();
  LightSet::_tmpLight.copy(S_LIGHT(_length-1));
  for (int i = _length-2; i >= 0; i--) S_LIGHT(i+1).copy(S_LIGHT(i));
  S_LIGHT(0).copy(LightSet::_tmpLight);
}

void LightSet::shiftLeft() {
  if (_length < 1) return;
  for (int i = 0; i < _length-1; i++)
    S_LIGHT(i).copy(S_LIGHT(i+1));
  S_LIGHT(_length-1).reset();
}

void LightSet::rotateLeft() {
  if (_length <= 1) return;
  //printf("rotateLeft on group ");
  //prLights();
  _tmpLight.copy(S_LIGHT(0));
  for (int i = 0; i < _length-1; i++) S_LIGHT(i).copy(S_LIGHT(i+1));
  S_LIGHT(_length-1).copy(LightSet::_tmpLight);
}

void LightSet::prLights() {
  char c;

  printf("%s:[", nameStr);
  for (int i=0; i<_length; i++) {
    switch (S_LIGHT(i).state) {
      case OFF_L:
        c = ' '; break;
      case ON_L:
        c = '1'; break;
      case BLINK_L:
        c = 'B'; break;
      default:
        c = '?'; break;
    }
    printf("%c", c);
  }
  printf("] (%d", lightNum[0]);
  for (int i=1; i<_length; i++) printf(",%d", lightNum[i]);
  printf(")\n");
}
