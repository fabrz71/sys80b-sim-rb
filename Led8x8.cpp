/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* ON-board 8x8 LED array programmer interface
* ---
* Functions for on-board LED array.
* (Static Class)
*/

#include "Led8x8.h"

void Led8x8::init(SPISettings ss, int pin) {
	_spis = ss;
	_ss_pin = pin;
	_writeCmd(CMD_DECODE, 0); // Decode state off
	_writeCmd(CMD_DIGITS, 7); // 8 rows ("digits")
	shutDownMode(false); // Shutdown mode off
	enabled = true;
	clearLGgrid();
	for (byte i = 0; i <8; i++) setLedRow(i, 1 << i); // test "slash"
}

// MSB on left
void Led8x8::setLedRowReverse(byte row, byte data) {
	if (enabled) _writeCmd((row & 0x7) + 1, revByte[data]);
}

// MSB on right
void Led8x8::setLedRow(byte row, byte data) {
	if (enabled) _writeCmd((row & 0x7) + 1, data);
}

void Led8x8::clearLGgrid() {
	if (enabled) {
		byte i = 0;
		while (i < 8) _writeCmd(++i, 0);
	}
}

inline void Led8x8::shutDownMode(bool b) {
	_writeCmd(CMD_SHUTDWN, b ? 0 : 1);
}

inline void Led8x8::setLGintens(byte i) {
	_writeCmd(CMD_INTNSTY, i & 0xf);
}

void Led8x8::_writeCmd(byte adr, byte data) {
	SPI.beginTransaction(_spis);
	digitalWrite(_ss_pin, LOW);
	SPI.transfer(adr & 0xf);
	SPI.transfer(data);
	digitalWrite(_ss_pin, HIGH);
	SPI.endTransaction();
}