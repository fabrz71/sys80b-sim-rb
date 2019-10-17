#include "SoundBuff.h"

SoundBuff::SoundBuff() {
	sndCmdBuf = new byte[DEF_LENGTH];
	_size = DEF_LENGTH;
}

SoundBuff::SoundBuff(byte length) {
	sndCmdBuf = new byte[length];
	_size = length;
}

// snd = 0..31
void SoundBuff::sendCmd(byte snd) {
	if (count >= _size) return; // return on full buffer
	sndCmdBuf[(idx + count) % _size] = snd;
	count++;
}

byte SoundBuff::getNextSndOutput(uint32_t t) {
	byte snd;

	if (!cmdOutput) {
		if (count == 0) return 0;
		snd = sndCmdBuf[idx];
		if (++idx >= count) idx = 0;
		count--;
		cmdOutput = true;
	}
	else {
		snd = 0; // resets previous sound cmd output byte
		cmdOutput = false;
	}
	return snd;
}

inline void SoundBuff::Empty() {
	idx = 0;
	count = 0;
}
