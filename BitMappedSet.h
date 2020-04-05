#ifndef _BitMappedSet_h_
#define _BitMappedSet_h_

// A set of binary states conveniently stored in a byte array.
// Access to the states are provided by setState() and getState().
// All che changes are tracked and available through hasChanges() and
// getChanges*() functions.
// rev: nov/19

//typedef unsigned short byte;
//typedef unsigned int uint16_t;
#include "Arduino.h"

class BitMappedSet {

public:
	static const unsigned int MAX_SIZE = 1024;

protected:
	byte* _state; // array of bytes containing the binary states
	byte* _protection;
	byte* _change;
	uint16_t _size;
	byte _arraySize;

public:
	BitMappedSet();
	BitMappedSet(uint16_t size);
	~BitMappedSet();

	void reset();
	bool setState(uint16_t n, bool state, bool forceProtection = false);
	void setProtection(uint16_t n, bool protection);
	bool getState(uint16_t n);
	bool isProtected(uint16_t n);
	bool hasChanged(uint16_t n);
	void clearChange(uint16_t n);
	inline uint16_t getSize() const { return _size;	}
	byte getStates4(uint16_t group);
	byte getStates8(uint16_t group);
	byte getChanges4(uint16_t group);
	byte getChanges8(uint16_t group);
	void clearChanges4(uint16_t group);
	void clearChanges8(uint16_t group);
	void setAllStates(bool state);
	void resetProtections();
	void clearChanges();
	bool pendingChanges();
	String toString(bool states = true, bool changes = false, bool protections = false);
};

#endif
