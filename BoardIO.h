/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* BOARD LOW-LEVEL SYS80/B I/O FUNCTIONS
* ---
* Basic low-level System 80/B's I/O functions implementation for the PRB. 
* write*() and read*() functions are designed for direct I/O; while
* setOutput() and getInput() function should be used by any Sys80b
* object and will be executed only when conntectedSystem points
* to that object.
*/

#ifndef _BoardIO_h_
#define _BoardIO_h_

PROGMEM const byte mux8[8] = // 8 bit mux outputs
	{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

PROGMEM const uint16_t mux16[16] = // 16 bit mux outputs
	{ 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
	0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 };

class Sys80b;

namespace BoardIO {
	enum inputType { RETURNS, SLAM_SW };
	enum outputType { STROBES, SOLENOIDS, SOUND, LAMPS, DISPL_DATA, DISPL_ADDR, DS_RESET }; // DS_RESET: display and sound reset
	enum buttonID { BACK_BUTT, ENTER_BUTT, NEXT_BUTT };

	Sys80b *connectedSystem = NULL;

	void setOutput(Sys80b *sender, outputType typ, uint16_t state);
	uint16_t getInput(Sys80b *sender, inputType typ);

	void writeSound(byte b);
	void write4Lamps(byte adr, byte states);
	void writeDisplayAndSoundReset(bool b);
	void writeDisplayCtrl(byte ctrl); 
	void writeDisplayAddr(byte adr);
	void writeDisplayData(byte b);
	void writeDisplayLD1(byte b);
	void writeDisplayLD2(byte b);
	void writeStrobes(byte b);
	void writeSolenoids(uint16_t states);
	byte readReturns();
	bool readSlamSwitch();
};

#endif
