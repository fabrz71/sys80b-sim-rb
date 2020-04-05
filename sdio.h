/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* SD CARD I/O
* ---
* Basic I/O functions for SD card.
*/

#ifndef _sdio_h
#define _sdio_h

#include "Arduino.h"
//#include <SD.h>

// microSD R/W functions
bool initSD(int sd_pin);
bool loadRAM(byte m[], char *fileName);
bool updateRAM(byte m[], char *fileName);

#endif
