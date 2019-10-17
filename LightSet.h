/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT SET OBJECT
* ---
* A "Light Set" is the setPeriod of all pinball lights
* with their own state. Only one stage may be active (current),
* although more than one LightSet objects could be declared.
*/

#ifndef _LightSet_h_
#define _LightSet_h_

#include "Light.h"
//#include "LampSet.h"

//#define setActiveLightStage(ls) LightSet::currentStage = ls;

//class Light;

class LightSet {
  protected:
	Light *lightArray; // pointer to array of Light objects
	////LampSet *lampSet = NULL;
	int _size; // size of light array (of Light objects)

  public:
	//LightSet();
    LightSet(int ls_size);
    LightSet(Light *larray, int ls_size);
	~LightSet();

	//void setPeriod(Light *larray, int ls_size);
	int getSize();
	Light* getLight(int n);
    void switchOffAllLights();
    void renderToLamps(LampSet *ls, uint32_t tm);
	void update(uint32_t ms);
    void updateAndRender(LampSet *ls, uint32_t ms);
    //void toString();
};

#endif