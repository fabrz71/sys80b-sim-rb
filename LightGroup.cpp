#include "LightGroup.h"
#include "LightSet.h"

LightSet* LightGroup::activeLightSet = nullptr;

//LightGroup::LightGroup() {
//	nameStr = "";
//	_tmpLight = new Light();
//	_size = 0;
//}

LightGroup::LightGroup(const char* setName, int32_t lNumber ...) {
	_size = 0; 
	_tmpLight = new Light();
	int32_t* ip = &lNumber;
	while (*(ip++) != -1) _size++;
	lightNum = new byte[_size];
	ip = &lNumber;
	for (byte i = 0; i < _size; i++) lightNum[i] = (byte)*(ip+i);
	nameStr = String(setName);

	//Serial.printf("new light group (size:%i) '", _size);
	//Serial.print(nameStr);
	//Serial.print("':[");
	//for (int i = 0; i < _size; i++) Serial.printf("%i,",lightNum[i]);
	//Serial.println("]");
}

LightGroup::LightGroup(String setName, int32_t lNumber ...) {
	_size = 0;
	int32_t* ip = &lNumber;
	while (*(ip++) != -1) _size++;
	lightNum = new byte[_size];
	ip = &lNumber;
	for (byte i = 0; i < _size; i++) lightNum[i] = (byte) * (ip + i);
	nameStr = setName;

	//Serial.printf("created light group (size:%i) '", _size);
	//Serial.print(nameStr);
	//Serial.print("':[");
	//for (int i = 0; i < _size; i++) Serial.printf("%i,", lightNum[i]);
	//Serial.println("]");
}

// returns nullptr if no light context is defined in <activeLightSet>
 Light* LightGroup::light(byte n) {
	 if (activeLightSet == nullptr) {
		 Serial.println(F("LightGroup::light: undefined light set"));
		 delay(100);
		 return nullptr;
	 }
	 Light* l = activeLightSet->getLight(lightNum[n]);
	 if (l == nullptr) {
		 Serial.println(F("LightGroup::light: null light!"));
		 delay(100);
	 }
	 return l;
}

void LightGroup::set(byte n, lightState st) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::set: undefined light set!"));
		delay(100);
		return;
	}
	if (n >= _size) {
		Serial.println(F("LightGroup::set: illegal light number"));
		delay(100);
		return;
	}
	light(n)->set(st);
}

void LightGroup::blink(byte n, uint16_t blinkP, byte ticks) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::set: undefined light set!"));
		delay(100);
		return;
	}
	if (n >= _size) {
		Serial.println(F("LightGroup::set: illegal light number"));
		delay(100);
		return;
	}
	light(n)->blink(blinkP, 50, ticks);
}

void LightGroup::invert(byte n) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::invert: undefined light set!"));
		delay(100);
		return;
	}
	light(n)->invert();
}

void LightGroup::pulse(byte n, uint16_t tm) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::pulse: undefined light set!"));
		delay(100);
		return;
	}
	light(n)->pulse(tm);
}

lightState LightGroup::getState(byte n) {
	if (activeLightSet == nullptr || n >= _size) return OFF_L;
	return light(n)->state;
}

bool LightGroup::isActive(byte n) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::isActive: undefined light set!"));
		delay(100);
		return false;
	}
	if (n >= _size) {
		Serial.println(F("LightGroup::isActive: illegal light number"));
		delay(100);
		return false;
	}
	Light* l = light(n);
	if (l == nullptr) {
		Serial.printf(F("LightGroup::isActive: undefined light #%d\n"), n);
		delay(100);
		return false;
	}
	return l->isActive();
}

void LightGroup::setAll(lightState st) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::setAll: undefined light set!"));
		delay(100);
		return;
	}
	for (int i = 0; i < _size; i++) light(i)->set(st);
}

// switch the first light of the group of a different state than st
// to the given state.
// returns: the order number of light switched; 0xFF else
byte LightGroup::switchTheFirst(lightState st) {
	byte i;

	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::switchTheFirst: undefined light set!"));
		delay(100);
		return 0xff;
	}
	for (i = 0; i < _size; i++) {
		if (light(i)->state != st) {
			light(i)->set(st);
			break;
		}
	}
	return i;
}

byte LightGroup::switchTheFirst(lightState st, uint16_t maxP) {
	byte gl;
	gl = switchTheFirst(st);
	if (maxP > 0 && gl != 0xff) light(gl)->activePeriod = maxP;
	return gl;
}

void LightGroup::setActivePeriod(byte n, uint16_t maxP) {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::setActivePeriod: undefined light set!"));
		delay(100);
		return;
	}
	light(n)->activePeriod = maxP;
}

bool LightGroup::areAll(lightState st) {
	byte n = 0;
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::areAll: undefined light set!"));
		delay(100);
		return false;
	}
	while (n<_size && light(n)->state == st) n++;
	return (n == _size);
}

void LightGroup::shiftRight() {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::shiftRight: undefined light set!"));
		delay(100);
		return;
	}
	if (_size < 1) return;
	if (_size > 1) {
		for (int i = _size - 1; i > 0; i--) light(i + 1)->copy(light(i));
	}
	light(0)->reset();
}

void LightGroup::rotateRight() {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::rotateRight: undefined light set!"));
		delay(100);
		return;
	}
	if (_size <= 1) {
		Serial.println(F("LightGroup::rotateRight: group size <= 1"));
		delay(100);
		return;
	}
	//Serial.print("rotateRight on group ");
	//Serial.print(toString());
	_tmpLight->copy(light(_size - 1));
	for (int i = _size - 2; i >= 0; i--) light(i + 1)->copy(light(i));
	light(0)->copy(_tmpLight);
	//Serial.print(" -> ");
	//Serial.println(toString());
}

void LightGroup::shiftLeft() {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::shiftLeft: undefined light set!"));
		delay(100);
		return;
	}
	if (_size <= 1) {
		Serial.println(F("LightGroup::rotateLeft: group size <= 1"));
		delay(100);
		return;
	}
	for (int i = 0; i < _size - 1; i++)
		light(i)->copy(light(i + 1));
	light(_size - 1)->reset();
}

void LightGroup::rotateLeft() {
	if (activeLightSet == nullptr) {
		Serial.println(F("LightGroup::rotateLeft: undefined light set!"));
		delay(100);
		return;
	}
	if (_size <= 1) return;
	//Serial.print("rotateLeft on group ");
	//Serial.print(toString());
	_tmpLight->copy(light(0));
	for (int i = 0; i < _size - 1; i++) light(i)->copy(light(i + 1));
	light(_size - 1)->copy(_tmpLight);
	//Serial.print(" -> ");
	//Serial.println(toString());
}

byte LightGroup::getSize() { 
	return _size; 
}

String LightGroup::toString() {
	String str;
	char c;

	str = nameStr + ":[";
	//printf("%s:[", nameStr.c_str);
	for (int i = 0; i<_size; i++) {
		switch (light(i)->state) {
		case OFF_L:
			c = ' '; break;
		case ON_L:
			c = '1'; break;
		case BLINK_L:
			c = 'B'; break;
		default:
			c = '?'; break;
		}
		str += c;
	}
	str += "] (";
	for (int i = 0; i < _size; i++) {
		str += (int)lightNum[i];
		if (i < _size-1) str += ",";
	}
	str += ")";
	return str;
}

