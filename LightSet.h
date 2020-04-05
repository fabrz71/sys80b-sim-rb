/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
*/

// rev: feb/20

#ifndef _LightSet_h_
#define _LightSet_h_

#include "Arduino.h"
#include "msg.h"
//#include "Light.h"
//#include "BitMappedSet.h"
//#include "LampSet.h"

//#define setActiveLightStage(ls) LightSet::currentStage = ls;

class Light;
class BitMappedSet;

// The set of the states of all pinball lights.
// Only one LightSet may be pointed as the active one (rendered to hardware),
// although more than one LightSet objects could be created.
class LightSet {
public:
	static const int MAX_COUNT = 256;

 protected:
	Light* light; // pointer to array of Light objects
	//BitMappedSet* lightChanges; // lights change flags
	//BitMappedSet* lampStates; // lamps state bits (1 bit = 1 lamp)
	////LampSet *lampSet = nullptr;
	int _size; // size of light array (of Light objects)

 public:
	//LightSet();
    LightSet(int ls_size);
    LightSet(Light* larray, int ls_size);
	~LightSet();

	//void setPeriod(Light *larray, int ls_size);
	int getSize();
	Light* getLight(int n);
	void resetAll();
    void switchOffAllLights();
	void switchOnAllLights();
    void renderToLamps(BitMappedSet& ls, byte from = 0, byte to = 0);
	bool update(uint32_t ms);
	//void print();
	String toString();
};

#endif
