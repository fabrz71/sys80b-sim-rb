/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
*/

//rev: jan/20

#ifndef _LightGroup_h
#define _LightGroup_h

#include "Arduino.h"
//#include "LightSet.h"
#include "Light.h"
//#include "msg.h"

class LightSet;

// Selection of Lights that belongs to an arbitrary group,
// referred by their sequential number [0..47].
// The group has its own dimension and name.
// LightGroup offers functions for common effects on all
// the lights of the group, as: setAll(), rotate...(), shift...().
// It operates on the lights of the active LightSet referred
// by public static pointer <activeLightSet>.
class LightGroup {
  public:
	//static const int MAX_SIZE = 16;
	static LightSet* activeLightSet; // unique reference LightSet for any instance
    String nameStr;
    byte *lightNum; // array of indexes (light numbers)

protected:
	byte _size; // light group size
	Light* _tmpLight; // temporary light object for copy operations

public:
    LightGroup(const char *setName, int32_t lNumber ...);
    LightGroup(String setName, int32_t lNumber ...);

	Light* light(byte n);
    void set(byte n, lightState st);
    void blink(byte n, uint16_t blinkP = Light::DEF_BLNK_PERIOD, byte ticks = 0);
    void invert(byte n);
    void pulse(byte n, uint16_t tm);
    lightState getState(byte n);
    bool isActive(byte n);
    void setAll(lightState st);
    inline void setAll(bool st) { setAll(st ? ON_L : OFF_L); }
    byte switchTheFirst(lightState st);
    byte switchTheFirst(lightState st, uint16_t maxP);
    void setActivePeriod(byte n, uint16_t maxP);
    bool areAll(lightState st);
    void shiftRight();
    void rotateRight();
    void shiftLeft();
    void rotateLeft();
	byte getSize();
    String toString();

    inline static void setActiveLightSet(LightSet* ls) { LightGroup::activeLightSet = ls; }
};

#endif
