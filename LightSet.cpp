/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT STAGE OBJECT
* ---
* A "Light Set" is the setPeriod of all pinball lights
* with their own state. Only one stage may be active (current),
* although more than one LightSet objects could be declared.
*/

#include "LightSet.h"

//LightSet::LightSet() {
//	_size = 0;
//}

LightSet::LightSet(int ls_size) {
	if (ls_size > LampSet::MAX_COUNT) ls_size = LampSet::MAX_COUNT;
	lightArray = new Light[ls_size];
	_size = ls_size;
}

LightSet::LightSet(Light *larray, int ls_size) {	
	lightArray = larray;
	_size = ls_size;
}

LightSet::~LightSet() {
	delete[] lightArray;
}

//void LightSet::setPeriod(Light *ls_array, int ls_size) {
//	if (ls_array = NULL || ls_size < 1) return;
//	_init(ls_size); // missing ls_size max check!
//	lightArray = ls_array;
//	_size = ls_size;
//}

inline int LightSet::getSize() {
	return _size;
}

inline Light* LightSet::getLight(int n) {
	return &lightArray[n];
}

void LightSet::switchOffAllLights() {
	//for (byte i = 3; i < _size; i++) if (!isSpecialLamp(i)) light[i].state = OFF_L;
	for (byte i = 0; i < _size; i++) lightArray[i].state = OFF_L;
}

// update all low-level "lamps" according to higher-level "lights" state.
// tm should contain actual time (ms)
void LightSet::renderToLamps(LampSet *ls, uint32_t tm) {
	Light *l;

	for (int n = 0; n < _size; n++) {
		l = &(lightArray[n]);
		if (!l->effective) ls->setLamp(n, l->on);
	}
}

// updates all non-steady mode lights state
inline void LightSet::update(uint32_t ms) {
	for (byte i = 0; i < _size; i++) lightArray[i].update(ms);
}

// updates all non-steady mode lights state;
// the given lamp setPeriod will also be updated.
void LightSet::updateAndRender(LampSet *ls, uint32_t ms) {
	if (_size == 0) return;
	update(ms);
	renderToLamps(ls, ms);
}
