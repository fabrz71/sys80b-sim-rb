#include "LampSet.h"

LampSet::LampSet() {
	_lamps = NULL;
	_size = 0;
}

LampSet::LampSet(byte n) {
	if (n > MAX_COUNT) n = MAX_COUNT;
	_init(n);
}

LampSet::~LampSet() {
	delete [] _lamps;
	_size = 0;
	_groups4 = 0;
}

inline byte LampSet::getSize() {
	return _size;
}

// setPeriod a lamp on (true) or off (false)
// return true if lamp state has changed
bool LampSet::setLamp(byte n, bool state) {
	byte byt, set;
	bool prevState;

	if (n >= _size) return;
	set = n >> 2;
	byt = 1 << (n & 3);
	prevState = ((_lamps[set] & byt) > 0);
	if (state != prevState) {
		if (state) _lamps[set] |= byt;
		else _lamps[set] &= (~byt);
		_changed[set] |= byt;
		_lastChanged = n;
		return true;
	}
	return false;
}

bool LampSet::setGroup(byte set, byte states) {
	if (set >= _groups4) return false;
	states &= 0x0f;
	if (_lamps[set] == states) return false;
	_changed[set] |= _lamps[set] ^ states;
	_lamps[set] = states;
	return true;
}

void LampSet::setSequence(byte idx, uint32_t bits, byte cnt) {
	if (idx >= _size) return;
	if (idx + cnt >= _size) cnt = _size - idx;
	for (byte i=0; i < cnt; i++) setLamp(idx + i, ((bits & bit(i)) > 0));
}

inline byte LampSet::getGroup(byte set) {
	return _lamps[set % _groups4];
}

inline bool LampSet::getLamp(byte n) {
	n %= _size;
	return ((_lamps[n >> 2] & bit(n & 3)) > 0);
}

// switch all lamps off
void LampSet::resetAll() {
	for (int i = 0; i < _groups4; i++) {
		_lamps[i] = 0;
		_changed[i] |= 0xf;
	}
}

inline bool LampSet::isGroupChanged(byte set) {
	return (_changed[set % _groups4] > 0);
}

inline void LampSet::validateGroup(byte set) {
	_changed[set % _groups4] = 0;
}

inline void LampSet::invalidate() {
	for (int i = 0; i < _groups4; i++) _changed[i] = 0xf;
}

void LampSet::_init(byte n) {
	_size = n;
	_groups4 = (_size + 3) >> 2;
	if (n == 0) return;
	_lamps = new byte[_groups4];
	_changed = new byte[_groups4];
	resetAll();
}

