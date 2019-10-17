/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* BOARD LOW-LEVEL SYS80/B I/O FUNCTIONS
* ---
* Implements basic low-level System 80/B I/O functions.
* write*() and read*() functions are designed for direct I/O; while
* setOutput() and getInput() function should be used by every Sys80b
* object and will be executed only when conntectedSystem points
* to that object.
*/

#include "BoardIO.h"
#include "ioPins.h"
#include "PIO.h"

void BoardIO::setOutput(Sys80b *sender, BoardIO::outputType typ, uint16_t data) {
	if (connectedSystem != sender) return;
	switch (typ) {
		case STROBES:
			writeStrobes((byte)(data & 0xff));
			break;
		case SOLENOIDS:
			writeSolenoids(data);
			break;
		case SOUND:
			writeSound((byte)(data & 0xff));
			break;
		case LAMPS:
			write4Lamps((byte)(data & 0xf0), (byte)(data & 0x0f));
			break;
		case DISPL_DATA:
			writeDisplayData(data);
			break;
		case DISPL_ADDR:
			writeDisplayAddr(data);
			break;
		case DS_RESET:
			writeDisplayAndSoundReset((data != 0) ? true : false);
			break;
	}
};

uint16_t BoardIO::getInput(Sys80b *sender, inputType typ) {
	if (connectedSystem != sender) return 0;
	switch (typ) {
	case RETURNS:
		return readReturns();
	case SLAM_SW:
		return (uint16_t)readSlamSwitch();
	}
};

// bit 0: LD1 (active high)
// bit 1: LD2 (active high)
// bit 2: RESET (active low)
inline void BoardIO::writeDisplayCtrl(byte ctrl) {
	digitalWrite(D_LD1_PIN, ((ctrl & 1) != 0) ? HIGH : LOW);
	digitalWrite(D_LD2_PIN, ((ctrl & 2) != 0) ? HIGH : LOW);
	digitalWrite(D_RES_PIN, ((ctrl & 4) != 0) ? HIGH : LOW);
}

inline void BoardIO::writeDisplayAddr(byte adr) {
	digitalWrite(D_LD1_PIN, ((adr & 1) != 0) ? HIGH : LOW);
	digitalWrite(D_LD2_PIN, ((adr & 2) != 0) ? HIGH : LOW);
}

inline void BoardIO::writeDisplayData(byte b) {
	PIO::mcpWritePA(3, b);
}

inline void BoardIO::writeDisplayLD1(byte b) {
	digitalWrite(D_LD1_PIN, (b ? HIGH : LOW));
}

inline void BoardIO::writeDisplayLD2(byte b) {
	digitalWrite(D_LD2_PIN, (b ? HIGH : LOW));
}

inline void BoardIO::writeSound(byte b) {
	PIO::mcpWritePB(3, b);
}

inline void BoardIO::write4Lamps(byte adr, byte states) {
	PIO::mcpWrite(1, (mux16[adr] << 4) | ((states) & 0xf));
}

inline void BoardIO::writeDisplayAndSoundReset(bool b) {
	digitalWrite(D_RES_PIN, (b) ? LOW : HIGH);
}

inline void BoardIO::writeStrobes(byte b) {
	PIO::mcpWritePA(2, (b & 0xff));
}

inline void BoardIO::writeSolenoids(uint16_t states) {
	PIO::mcpWrite(0, states);
}

//inline void BoardIO::writeLedGridRow(byte row, byte data) {
//	Led8x8::setLedRowReverse(row, data);
//}

inline byte BoardIO::readReturns() {
	return PIO::mcpReadPB(2);
}

inline bool BoardIO::readSlamSwitch() {
	return (digitalRead(SLAM_PIN) == HIGH);
}
