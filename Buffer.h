#ifndef _Buffer_h_
#define _Buffer_h_

#include "Arduino.h"

// rev: mar/20
template <typename Type, int size>
class Buffer {
public:
	static const byte MIN_LENGTH = 2;

protected:
	Type* _buff;
	unsigned _firstPtr;
	unsigned _valuesCount;
	unsigned _size;

public:
	Buffer() {
		_size = (size < MIN_LENGTH) ? MIN_LENGTH : size;
		_buff = new Type[_size];
		Clear();
	}

	~Buffer() {
		delete[] _buff;
	}

	void Clear() {
		_firstPtr = 0;
		_valuesCount = 0;
	}

	// snd = 0..31
	bool put(Type value) {
		if (_valuesCount >= _size) return false; // return on full buffer
		unsigned p = _firstPtr + _valuesCount++;
		if (p >= _size) p -= _size;
		_buff[p] = value;
		return true;
	}

	Type get() {
		if (_valuesCount == 0) return (Type)0;
		Type value = _buff[_firstPtr++];
		if (_firstPtr == _size) _firstPtr = 0;
		_valuesCount--;
		return value;
	}

	inline Type peek() {
		return (_valuesCount == 0) ? (Type)0 : _buff[_firstPtr];
	}

	inline bool isEmpty() {
		return (_valuesCount == 0) ? true : false;
	}

	inline int getCount() {
		return _valuesCount;
	}

	inline int getSize() {
		return _size;
	}
};

#endif