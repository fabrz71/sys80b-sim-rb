/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT STAGE OBJECT DEFINITION
* ---
* A "Light Stage" is a configuration of all pinball lights
* with their own state. Only one stage may be activated.
*/

#include "Light.h"

#define setActiveLightStage(ls) LightStage::currentStage = ls;

extern bool isSpecialLamp(byte lamp);
extern void set4Lamps(byte set, byte states);

// A complete set of Light objects, with their own state.
// Note that more than one LightStage objects could be declared.
class LightStage {
  public:
    //Light light[LSTAGE_SIZE]; // arrray of Lights objects
    Light *light; // pointer to array of Lights objects
    int size = 0; // size of light array
    static LightStage *currentStage;

    LightStage(int ls_size);
    LightStage(Light *larray, int ls_size);
    inline static void setActiveStage(LightStage ls) {
      currentStage = &ls;
    }
    void switchOffAllLights();
    void lightsToLamps(uint32_t tm);
    void updateLights(uint32_t ms, bool lampsUpdateReq);
    //void prLights();
};

LightStage *LightStage::currentStage;

LightStage::LightStage(int ls_size) {
  light = new Light[ls_size];
  size = ls_size;
}

LightStage::LightStage(Light *larray, int ls_size) {
  light = larray;
  size = ls_size;
}

void LightStage::switchOffAllLights() {
  for (byte i=3; i < size; i++) if (!isSpecialLamp(i)) light[i].state = OFF_L;
}

// update all low-level "lamps" reading high-level "lights" state.
// tm should contain actual time (ms)
void LightStage::lightsToLamps(uint32_t tm) {
  byte b, i, j;

  for (i=0; i<12; i++) { // for every lamp group
    b = 0; // 4-lamps states nybble
    for (j=0; j<4; j++) {
      if (light[(i<<2)|j].on) b |= 1<<j;
    }
    set4Lamps(i, b);
  }
  //renderLamps();
}

// updates all non-steady mode lights state
void LightStage::updateLights(uint32_t ms, bool lampsUpdateReq) {
  for (byte i=0; i<LSTAGE_SIZE; i++) light[i].update(ms);
  if (lampsUpdateReq) lightsToLamps(ms);
}
