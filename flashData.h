/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* FLASH MEMORY DATA I/O
* ---
* Functions for pinball non-volatile data retrieve/store.
*/

#include <EEPROM.h>
#include "sdio.h"

#define _getFVal(a) EEPROM.read(a)
#define _wrtFVal(a, v) EEPROM.write(a, v)
#define getSettingSwitch(n) getSettingBit((n)-1)
#define setSettingSwitch(n, s) saveSettingBit((n)-1, (s) ? true : false)
//#define saveSystemPowerOns(n) saveStat16(0, n)
//#define loadSystemPowerOns() getStat16(0)

PROGMEM const byte factorySwSettings[] = { // Factory "Op-Adj" switch settings values
  0b00000011, 0b00000111, 0b00000001, 0b11000010
};

PROGMEM const byte factoryStatsBytes[] = { // Factory stats values
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0,00, 0x40, 0x4b, 0x4c, 0x00, 0x80, 0x96, 0x98, 0x00, 0x00, 0x2d,
  0x31, 0x01, 0x00, 0x00, 0x60, 0xe3, 0x16, 0x00, 0x52, 0x41, 0x4d, 0x00, 0x80, 0x4f, 0x12, 0x00,
  0x52, 0x41, 0x4d, 0x00, 0x40, 0x42, 0x0f, 0x00, 0x52, 0x41, 0x4d, 0x00, 0x00, 0x35, 0x0c, 0x00,
  0x52, 0x41, 0x4d, 0x00, 0x20, 0xa1, 0x07, 0x00, 0x52, 0x41, 0x4d, 0x00
};

const byte STATBASEADR = 8;

byte getSettingByte(byte addr);
byte getSettingBit(byte bitPos);
bool saveSettingByte(byte addr, byte byteMask, byte value);
bool saveSettingBit(byte bitPos, bool state);
void saveStat8(byte addr, byte value);
void saveStat16(byte addr, uint16_t value);
void saveStat32(byte addr, uint32_t value);
byte getStat8(byte addr);
uint16_t getStat16(byte addr);
uint32_t getStat32(byte addr);

byte getSettingByte(byte addr) {
  if (addr > 3) return 0;
  return _getFVal(addr);
}

byte getSettingBit(byte bitPos) {
  byte _bit, _byte;
  if (bitPos > 31) return false;
  _byte = bitPos >> 3;
  _bit = bitPos % 8;
  return (_getFVal(_byte) & (1<<_bit)) ? 1 : 0;
}

bool saveSettingByte(byte addr, byte byteMask, byte value) {
  if (addr > 3) return false;
  byte v = _getFVal(addr);
  v = (v & (~byteMask)) | (value & byteMask);
  if (v == value) return false;
  _wrtFVal(addr, v);
  return true;
}

bool saveSettingBit(byte bitPos, bool state) {
  byte v, _bit, addr;
  if (bitPos > 31) return false;
  addr = bitPos >> 3;
  _bit = bitPos % 8;
  v = _getFVal(addr);
  if (state) v = v | (1<<_bit);
  else v = v & (~(1<<_bit));
  _wrtFVal(addr, v);
  return true;
}

void saveStat8(byte addr, byte value) {
  _wrtFVal(STATBASEADR + addr, value);
}

void saveStat16(byte addr, uint16_t value) {
  addr += STATBASEADR;
  _wrtFVal(addr, (byte)((value & 0xff00)>>8));
  _wrtFVal(addr+1, (byte)(value & 0xff));
}

void saveStat32(byte addr, uint32_t value) {
  addr += STATBASEADR;
  _wrtFVal(addr, (byte)((value & 0xff000000)>>24));
  _wrtFVal(addr+1, (byte)((value & 0xff0000)>>16));
  _wrtFVal(addr+2, (byte)((value & 0xff00)>>8));
  _wrtFVal(addr+3, (byte)(value & 0xff));
}

byte getStat8(byte addr) {
  return _getFVal(STATBASEADR + addr);
}

uint16_t getStat16(byte addr) {
  uint16_t v;
  addr += STATBASEADR;
  v = _getFVal(addr);
  v = v << 8;
  v |= _getFVal(addr+1);
  return v;
}

uint32_t getStat32(byte addr) {
  uint32_t v;
  addr += STATBASEADR;
  v = _getFVal(addr);
  v = v << 8;
  v |= _getFVal(addr+1);
  v = v << 8;
  v |= _getFVal(addr+2);
  v = v << 8;
  v |= _getFVal(addr+3);
  return v;
}
