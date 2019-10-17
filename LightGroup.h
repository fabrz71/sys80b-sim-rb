/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT GROUP OBJECT
* ---
* A "Light group" is a selection of Lights.
* Lights in the group are simply referred by their sequential number.
* The group has its own dimension and name.
*/

#ifndef _LightGroup_h
#define _LightGroup_h

//#include "Arduino.h"
#include "Light.h"

//class LightSet;
//#define S_LIGHT(n) LightSet::currentStage->light[lightNum[n]]
//#define S_LIGHT(n) refLightSet.getLight(lightNum[n]);

class LightGroup {
  public:
	//static const int MAX_SIZE = 16;
	static Light *refLightSet; // unique reference LightSet for any instance
    const char *nameStr;
    byte *lightNum; // array

protected:
	byte _size;
	//LightSet* refLightSet; // underlying light setPeriod
	Light _tmpLight; // temporary buffer

public:
	LightGroup();
    //LightGroup(const char *setName, const byte *lightNumber, byte arrayLength);
    //LightGroup(const char *setName, const byte *lightNumber);
	LightGroup(const char *setName, byte lNumber ...);
	//LightGroup(const char *setName, const byte l1, const byte l2);
	//LightGroup(const char *setName, const byte l1, const byte l2, const byte l3);
	//LightGroup(const char *setName, const byte l1, const byte l2, const byte l3, const byte l4);
	//LightGroup(const char *setName, const byte l1, const byte l2, const byte l3, const byte l4, const byte l5);

	Light* Light(byte n);
    void setLight(byte n, lightState st);
    void setLight(byte n, lightState st, uint16_t blinkP);
    void invert(byte n);
    void pulse(byte n, uint16_t tm);
    lightState getLState(byte n);
    bool isActive(byte n);
    void setAll(lightState st);
    void setAll(lightState st, uint16_t blinkP);
    byte switchOne(lightState st);
    byte switchOne(lightState st, uint16_t maxP);
    void setActivePeriod(byte n, uint16_t maxP);
    bool areAll(lightState st);
    void shiftRight();
    void rotateRight();
    void shiftLeft();
    void rotateLeft();
	byte getSize();
    void toString();
};

#endif