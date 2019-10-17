/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* SD CARD I/O
* ---
* Basic I/O functions for SD card.
*/

#include "sdio.h"

boolean sdOk;

boolean initSD(int sd_pin) {
	pinMode(sd_pin, OUTPUT);
	digitalWrite(sd_pin, HIGH);
	sdOk = SD.begin(sd_pin);
	if (sdOk) Serial.println(F("SD initialized."));
	else Serial.println(F("SD init failed!"));
	return sdOk;
}

// reads only the last 256 bytes of specified file
boolean loadRAM(byte m[], char *fileName) {
	File f;
	long i, fsz;

	Serial.print(F("Loading RAM '"));
	Serial.print(fileName);
	Serial.print("'... ");
	if (!SD.exists(fileName)) {
		Serial.println(F("Warning: File does not exist."));
		return false;
	}
	f = SD.open(fileName);
	if (!f) {
		Serial.println(F("Troubles opening file!"));
		return false;
	}
	fsz = f.size();
	if ((fsz & 0xff) != 0) {
		Serial.print(F("Warning: RAM file '"));
		Serial.print(fileName);
		Serial.print(F("' size ("));
		Serial.print(fsz);
		Serial.println(F(") is not multiple of 256."));
	}
	if (fsz > 256) f.seek(fsz - 256 - 1);
	for (i = 0; i < 256 && i < fsz; i++) m[i] = (char)f.read();
	f.close();
	Serial.println("ok.");
	return true;
}

// 256-bytes cumulative writing (file append)
boolean updateRAM(byte m[], char *fileName) {
	File f;
	long r;

	Serial.print(F("Updating RAM file... "));
	f = SD.open(fileName, FILE_WRITE);
	if (!f) {
		Serial.println(F("Troubles opening file!"));
		return false;
	}
	r = f.write(m, 256);
	f.close();
	if (r < 256) {
		Serial.println(F("Troubles writing file!"));
		return false;
	}
	else Serial.println("ok.");
	return true;
}
