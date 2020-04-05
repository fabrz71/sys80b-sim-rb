/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
*/

#include "BitMappedSet.h"

BitMappedSet::BitMappedSet() {
	BitMappedSet(32u);
}

BitMappedSet::BitMappedSet(uint16_t size) {
	//Serial.println("BitMappedSet(n) init...");
	delay(100);
	if (size > MAX_SIZE) size = MAX_SIZE;
	_size = size;
	_arraySize = (byte)(((size - 1) >> 3) + 1);
	//Serial.print("BitMappedSet(");
	//Serial.println(size);
	//Serial.println("): array size : ");
	//Serial.println(_arraySize);
	_state = new byte[_arraySize];
	_change = new byte[_arraySize];
	_protection = new byte[_arraySize];
	reset();
}

BitMappedSet::~BitMappedSet() {
	delete[] _state;
	delete[] _change;
	delete[] _protection;
}

void BitMappedSet::reset() {
	//Serial.println("BitMappedSet reset...");
	delay(100);
	for (uint16_t i = 0; i < _arraySize; i++) {
		_state[i] = 0;
		_change[i] = 0;
		_protection[i] = 0;
	}
}

// returns true when state changed
bool BitMappedSet::setState(uint16_t n, bool state, bool forceProtection) {
	if (n >= _size) return false;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7);
	if (!forceProtection && ((_protection[byteNum] & bitVal) > 0)) return false; // protected!
	if (state) {
		if ((_state[byteNum] & bitVal) == 0) {
			_state[byteNum] |= bitVal;
			_change[byteNum] |= bitVal;
			return true;
		}
	}
	else {
		if ((_state[byteNum] & bitVal) > 0) {
			_state[byteNum] &= ~bitVal;
			_change[byteNum] |= bitVal;
			return true;
		}
	}
	return false;
}

void BitMappedSet::setProtection(uint16_t n, bool protection) {
	if (n >= _size) return;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7u);
	if (protection) _protection[byteNum] |= bitVal;
	else _protection[byteNum] &= ~bitVal;
}

bool BitMappedSet::getState(uint16_t n) {
	if (n >= _size) return false;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7u);
	return ((_state[byteNum] & bitVal) > 0);
}

bool BitMappedSet::isProtected(uint16_t n) {
	if (n >= _size) return false;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7u);
	return ((_protection[byteNum] & bitVal) > 0);
}

bool BitMappedSet::hasChanged(uint16_t n) {
	if (n >= _size) return false;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7u);
	return ((_change[byteNum] & bitVal) > 0);
}

void BitMappedSet::clearChange(uint16_t n) {
	if (n >= _size) return;
	uint16_t byteNum = n >> 3;
	byte bitVal = (byte)1 << (n & 7u);
	_change[byteNum] &= ~bitVal;
}

byte BitMappedSet::getStates4(uint16_t group) {
	uint16_t byteNum = group >> 1;
	if (byteNum >= _arraySize) return 0;
	return ((group & 1u) == 0) ? _state[byteNum] & 0x0f : (_state[byteNum] & 0xf0) >> 4;
}

byte BitMappedSet::getStates8(uint16_t group) {
	if (group >= _arraySize) return 0;
	return _state[group];
}

byte BitMappedSet::getChanges4(uint16_t group) {
	uint16_t byteNum = group >> 1;
	if (byteNum >= _arraySize) return 0;
	return ((group & 1u) == 0) ? _change[byteNum] & 0x0f : (_change[byteNum] & 0xf0) >> 4;
}

byte BitMappedSet::getChanges8(uint16_t group) {
	if (group >= _arraySize) return 0;
	return _change[group];
}

void BitMappedSet::clearChanges4(uint16_t group) {
	uint16_t byteNum = group >> 1;
	if (byteNum >= _arraySize) return;
	if ((group & 1u) == 0) _change[byteNum] &= 0xf0;
	else _change[byteNum] &= 0x0f;
}

void BitMappedSet::clearChanges8(uint16_t group) {
	if (group >= _arraySize) return;
	_change[group] = 0;
}

// note: updates changes bits
void BitMappedSet::setAllStates(bool state) {
	uint16_t i;
	if (state) {
		for (i = 0; i < _arraySize; i++) {
			_change[i] = _state[i] ^ 0xff;
			_state[i] = 0xff;
		}
	}
	else {
		for (i = 0; i < _arraySize; i++) {
			_change[i] = _state[i] ^ 0;
			_state[i] = 0;
		}
	}
}

void BitMappedSet::resetProtections() {
	for (uint16_t i = 0; i < _arraySize; i++) _protection[i] = 0;
}

void BitMappedSet::clearChanges() {
	for (uint16_t i = 0; i < _arraySize; i++) _change[i] = 0;
}

bool BitMappedSet::pendingChanges() {
	for (uint16_t i = 0; i < _arraySize; i++) if (_change[i] > 0) return true;
	return false;
}

// Returns a string that represents the state of all the lamps as a sequence of '0's and '1's.
String BitMappedSet::toString(bool states, bool changes, bool protections) {
	String ret;
	if (states) {
		ret += "St:[";
		for (uint16_t i = 0; i < _size; i++) ret += getState(i) ? "1" : "0";
		ret += "]\n";
	}
	if (changes) {
		ret += "Ch:[";
		for (uint16_t i = 0; i < _size; i++) ret += hasChanged(i) ? "*" : " ";
		ret += "]\n";
	}
	if (protections) {
		ret += "Pr:[";
		for (uint16_t i = 0; i < _size; i++) ret += isProtected(i) ? "!" : " ";
		ret += "]\n";
	}
	return ret;
}