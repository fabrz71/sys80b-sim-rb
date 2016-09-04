// low level functions for MAX7219 LED matrix driver 
#include <SPI.h>

#define LGR_SPI_CLK 1000000

#define CMD_DECODE  0x9
#define CMD_INTNSTY 0xa
#define CMD_DIGITS  0xb
#define CMD_SHUTDWN 0xc

int lg_cs_pin; // SPI CS signal

SPISettings lgSpiSetup(LGR_SPI_CLK, MSBFIRST, SPI_MODE0);

void initLedGrid(int cspin);
void _writeCmd(byte adr, byte data);
void setLedRow(byte row, byte data);
void shutDownMode(bool shut);
void setLGintens(byte i);
void clearLGgrid();

void initLedGrid(int cspin) {
  lg_cs_pin = cspin;
  _writeCmd(CMD_DECODE, 0); // Decode state off
  _writeCmd(CMD_DIGITS, 7); // 8 rows ("digits")
  shutDownMode(false); // Shutdown mode off
}

void _writeCmd(byte adr, byte data) {
  SPI.beginTransaction(lgSpiSetup);
  digitalWrite(lg_cs_pin, LOW);
  SPI.transfer(adr & 0xf);
  SPI.transfer(data);
  digitalWrite(lg_cs_pin, HIGH);
  SPI.endTransaction();
}

void setLedRow(byte row, byte data) {
  _writeCmd((row & 0x7)+1, data);
}

void shutDownMode(bool shut) {
  _writeCmd(CMD_SHUTDWN, shut ? 0 : 1);
}

// i = 0..f
void setLGintens(byte i) {
  _writeCmd(CMD_INTNSTY, i & 0xf);
}

void clearLGgrid() {
  for (byte i=0; i<8; i++) setLedRow(i, 0);
}


