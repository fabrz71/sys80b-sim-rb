#ifndef _SoundBuff_h_
#define _SoundBuff_h_

#define SNDCMD_TIME 100

class SoundBuff
{
protected:
	byte *sndCmdBuf; // array
	byte idx;
	byte count;
	byte _size;
	bool cmdOutput;

public:
	static const byte DEF_LENGTH = 8;

	SoundBuff();
	SoundBuff(byte lenght);

	void sendCmd(byte snd);
	byte getNextSndOutput(uint32_t t);
	void Empty();
};

#endif

