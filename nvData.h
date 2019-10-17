/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* NON-VOLATILE DATA MEMORY I/O
* ---
* Functions for pinball non-volatile data retrieve/store.
*/

#ifndef _nvData_h
#define _nvData_h

//#include "Arduino.h"
//#include <EEPROM.h>
//#include "sdio.h"

#define _getFVal(a) EEPROM.read(a)
#define _wrtFVal(a, v) EEPROM.write(a, v)
//#define getSettingSwitch(n) getSettingBit((n)-1)
//#define setSettingSwitch(n, s) saveSettingBit((n)-1, (s) ? true : false)
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
byte getSettingSwitch(byte n);
void setSettingSwitch(byte n, bool s);

#endif
