/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT SET OBJECT DEFINITION
* ---
* A "Light setPeriod" are a setPeriod of game lighs of a group.
* Contains C++ code.
*/

#include "LightGroup.h"

//#include "LightSet.h"

LightGroup::LightGroup() {
	nameStr = "";
	_size = 0;
}

//LightGroup::LightGroup(const char *setName, const byte *lightNumber, byte arrayLength) {
//	nameStr = setName;
//	lightNum = (byte *)lightNumber;
//	if (arrayLength > MAX_SIZE) arrayLength = MAX_SIZE;
//	_size = arrayLength;
//	//printf("* new LightGroup '%s' defined with %d lamps.\n", setName, _size);
//}

//LightGroup::LightGroup(const char *setName, const byte *lightNumber) {
//	for (_size = 0; _size < MAX_SIZE; _size++)
//		if (lightNumber[_size] == 0) break;
//	nameStr = setName;
//	lightNum = (byte *)lightNumber;
//	//printf("* new LightGroup '%s' defined with %d lamps.\n", setName, _size);
//}

// TODO: DA VERIFICARE!
LightGroup::LightGroup(const char *setName, byte lNumber ...) {
	byte *p = &lNumber;
	byte nargs = lNumber;

	//p++;
	if (nargs > MAX_SIZE) nargs = MAX_SIZE;
	lightNum = new byte[nargs];
	for (byte i = 0; i < nargs; i++) lightNum[i] = *(++p);
	_size = nargs;
}

//LightGroup::LightGroup(const char *setName, const byte l1, const byte l2) {
//	nameStr = setName;
//	_size = 2;
//	lightNum = new byte[_size];
//	lightNum[0] = l1;
//	lightNum[1] = l2;
//}
//
//LightGroup::LightGroup(const char *setName, const byte l1, const byte l2, const byte l3) {
//	nameStr = setName;
//	_size = 3;
//	lightNum = new byte[_size];
//	lightNum[0] = l1;
//	lightNum[1] = l2;
//	lightNum[2] = l3;
//}
//
//LightGroup::LightGroup(const char *setName, const byte l1, const byte l2, const byte l3, const byte l4) {
//	nameStr = setName;
//	_size = 4;
//	lightNum = new byte[_size];
//	lightNum[0] = l1;
//	lightNum[1] = l2;
//	lightNum[2] = l3;
//	lightNum[3] = l4;
//}
//
//LightGroup::LightGroup(const char *setName, const byte l1, const byte l2, const byte l3, const byte l4, const byte l5) {
//	nameStr = setName;
//	_size = 5;
//	lightNum = new byte[_size];
//	lightNum[0] = l1;
//	lightNum[1] = l2;
//	lightNum[2] = l3;
//	lightNum[3] = l4;
//	lightNum[4] = l5;
//}

//inline Light* LightGroup::Light(byte n) {
//	return refLightSet->getLight(lightNum[n]);
//}

// returns NULL if no light context is defined in <refLightSet>
inline Light* LightGroup::Light(byte n) {
	//if (refLightSet == NULL) return NULL;
	return &(refLightSet[lightNum[n]]);
}

void LightGroup::setLight(byte n, lightState st) {
	if (refLightSet == NULL || n >= _size) return;
	Light(n)->set(st);
}

void LightGroup::setLight(byte n, lightState st, uint16_t blinkP) {
	if (refLightSet == NULL || n >= _size) return;
	Light(n)->set(st, blinkP, 50);
}

void LightGroup::invert(byte n) {
	if (refLightSet == NULL || n >= _size) return;
	Light(n)->invert();
}

void LightGroup::pulse(byte n, uint16_t tm) {
	if (refLightSet == NULL || n >= _size) return;
	Light(n)->pulse(tm);
}

lightState LightGroup::getLState(byte n) {
	if (refLightSet == NULL || n >= _size) return OFF_L;
	return Light(n)->state;
}

inline bool LightGroup::isActive(byte n) {
	if (refLightSet == NULL || n >= _size) return false;
	return Light(n)->isActive();
}

void LightGroup::setAll(lightState st) {
	if (refLightSet == NULL) return;
	for (int i = 0; i < _size; i++) Light(i)->set(st);
}

void LightGroup::setAll(lightState st, uint16_t blinkP) {
	if (refLightSet == NULL) return;
	for (int i = 0; i < _size; i++) Light(i)->set(st, blinkP, 50);
}

// switch the first light of the group of a different state than st
// to the given state.
// returns: the number of group-light switched; 0xFF else
byte LightGroup::switchOne(lightState st) {
	byte i, gl;

	if (refLightSet == NULL) return 0xff;
	gl = 0xff;
	i = 0;
	while (i<_size) {
		if (Light(i)->state != st) {
			Light(i)->set(st);
			gl = i;
			break;
		}
	}
	return gl;
}

byte LightGroup::switchOne(lightState st, uint16_t maxP) {
	byte gl;
	gl = switchOne(st);
	if (maxP > 0 && gl != 0xff) Light(gl)->activePeriod = maxP;
	return gl;
}

void LightGroup::setActivePeriod(byte n, uint16_t maxP) {
	if (refLightSet == NULL || n >= _size) return;
	Light(n)->activePeriod = maxP;
}

bool LightGroup::areAll(lightState st) {
	byte n = 0;
	if (refLightSet == NULL) return false;
	while (n<_size && Light(n)->state == st) n++;
	return (n == _size);
}

void LightGroup::shiftRight() {
	if (refLightSet == NULL) return;
	if (_size < 1) return;
	if (_size > 1) {
		for (int i = _size - 1; i > 0; i--) Light(i + 1)->copy(Light(i));
	}
	Light(0)->reset();
}

void LightGroup::rotateRight() {
	if (refLightSet == NULL) return;
	if (_size <= 1) return;
	//printf("rotateRight on group ");
	//toString();
	_tmpLight.copy(Light(_size - 1));
	for (int i = _size - 2; i >= 0; i--) Light(i + 1)->copy(Light(i));
	Light(0)->copy(_tmpLight);
}

void LightGroup::shiftLeft() {
	if (refLightSet == NULL) return;
	if (_size < 1) return;
	for (int i = 0; i < _size - 1; i++)
		Light(i)->copy(Light(i + 1));
	Light(_size - 1)->reset();
}

void LightGroup::rotateLeft() {
	if (refLightSet == NULL) return;
	if (_size <= 1) return;
	//printf("rotateLeft on group ");
	//toString();
	_tmpLight.copy(Light(0));
	for (int i = 0; i < _size - 1; i++) Light(i)->copy(Light(i + 1));
	Light(_size - 1)->copy(_tmpLight);
}

inline byte LightGroup::getSize() { 
	return _size; 
}

void LightGroup::toString() {
	char c;

	printf("%s:[", nameStr);
	for (int i = 0; i<_size; i++) {
		switch (Light(i)->state) {
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
	for (int i = 1; i<_size; i++) printf(",%d", lightNum[i]);
	printf(")\n");
}
