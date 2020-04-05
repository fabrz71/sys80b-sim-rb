/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
*/

#include "BitMappedSet.h"
#include "LightSet.h"
#include "Light.h"

//LightSet::LightSet() {
//	_size = 0;
//}

LightSet::LightSet(int ls_size) {
	//Serial.println("Lightset init...");
	//delay(100);
	if (ls_size > MAX_COUNT) ls_size = MAX_COUNT;
	light = new Light[ls_size];
	//lightChanges = new BitMappedSet(ls_size);
	_size = ls_size;
}

LightSet::LightSet(Light *larray, int ls_size) {	
	light = larray;
	//lightChanges = new BitMappedSet(ls_size);
	_size = ls_size;
}

LightSet::~LightSet() {
	delete[] light;
	//delete lightChanges;
}

//void LightSet::setPeriod(Light *ls_array, int ls_size) {
//	if (ls_array = nullptr || ls_size < 1) return;
//	_init(ls_size); // missing ls_size max update!
//	lightArray = ls_array;
//	_size = ls_size;
//}

 int LightSet::getSize() {
	return _size;
}

 //returns nullptr if <light> is too big
 Light* LightSet::getLight(int n) {
	 if (n < 0 || n >= _size) {
		 Serial.printf(F("WARNING: LightSet::getLight(): invalid light number %d\n"), n);
		 return nullptr;
	 }
	 return &(light[n]);
}

 void LightSet::resetAll() {
	for (byte i = 0; i < _size; i++) light[i].reset();
}

 void LightSet::switchOffAllLights() {
	for (byte i = 0; i < _size; i++) light[i].set(OFF_L);
}

 void LightSet::switchOnAllLights() {
	 for (byte i = 0; i < _size; i++) light[i].set(ON_L);
 }

// Updates all low-level "lamps" according to corresponding "lights" state.
// Protected lamps won't be updated.
void LightSet::renderToLamps(BitMappedSet& ls, byte from, byte to) {
	byte top;
	
	if (from > to) return;
	if (to == 0) top = _size; else top = to + 1;
	if (top > _size) top = _size;
	for (byte i = from; i < top; i++) ls.setState(i, light[i].on);
}

// updates all non-steady mode lights state
 bool LightSet::update(uint32_t ms) {
	bool changes = false;
	for (byte i = 0; i < _size; i++) {
		if (light[i].update(ms) && !changes) changes = true;
	}
	return changes;
}

 //void LightSet::print() {
	// if (lightArray == nullptr || _size == 0) {
	//	 if (_size == 0) Serial.println(F("LightSet::print: no lights defined!"));
	//	 else Serial.println(F("LightSet::print: void light array!"));
	//	 return;
	// }
	// Serial.print("L:[");
	// for (int i = 0; i < _size; i++) Serial.print(lightArray[i].on ? "1" : "0");
	// Serial.println("]");
 //}

 //String LightSet::toString() {
	// String s = String("L:[");
	// if (lightArray == nullptr || _size == 0) s += "<nullPtr>!";
	// else {
	//	 for (int i = 0; i < _size; i++) s.append(lightArray[i].on ? "1" : "0");
	// }
	// s.append("]");
	// return s;
 //}

  // Returns a string that represents the state of all the lamps as a sequence of '0's and '1's.
 String LightSet::toString() {
	 Light* l;
	 String ret = "[";
	 for (int i = 0; i < _size; i++) {
		 l = &(light[i]);
		 if (l->impulse) ret += 'P';
		 else {
			 switch (l->state) {
			 case ON_L:
				 ret += '1';
				 break;
			 case OFF_L:
				 ret += '0';
				 break;
			 case BLINK_L:
				 ret += (l->on) ? 'B' : '0';
				 break;
			 }
		 }
	 }
	 ret += "]";
	 return ret;
 }

//// updates all non-steady mode lights state;
//// the given lamp setPeriod will also be updated.
//void LightSet::updateAndRender(BitMappedSet *ls, uint32_t ms) {
//	if (_size == 0) return;
//	update(ms);
//	renderToLamps(ls);
//}
