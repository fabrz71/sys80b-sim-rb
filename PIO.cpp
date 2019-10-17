/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LOW LEVEL I/O
* ---
* Low level I/O functions for MCP24S17 GPIO expanders.
* MCP24S17 IC offers two 8-bit I/O ports and it's controlled
* by Teensy through SPI interface.
*/

#include "PIO.h"

void PIO::init(SPISettings ss, int pin) {
	_spis = ss;
	_ss_pin = pin;
	// Enable address mode
	_byteWrite(0, IOCON, ADDR_ENABLE);
	_byteWrite(1, IOCON, ADDR_ENABLE);
	// Ports data direction
	_wordWrite(0, IODIRA, 0x0000); // all 16 pins setPeriod as output
	mcpWrite(0, 0); // resets MCP0 outputs
	_wordWrite(1, IODIRA, 0x0000); // all 16 pins setPeriod as output
	mcpWrite(1, 0); // resets MCP1 outputs
	_wordWrite(2, IODIRA, 0x00ff); // 8 bits output + 8 bits input
	mcpWritePA(2, 0); // resets MCP2 outputs
	_wordWrite(3, IODIRA, 0x0000); // all 16 pins setPeriod as output
	mcpWrite(3, 0); // resets MCP3 outputs
					/*
					// Outputs reset
					mcpWrite(0, 0); // resets MCP0 outputs
					mcpWrite(1, 0); // resets MCP1 outputs
					mcpWritePA(2, 0); // resets MCP2 outputs
					mcpWrite(3, 0); // resets MCP3 outputs
					*/
}

// GENERIC BYTE WRITE - will write a byte to a register, arguments are register address and the value to write
void PIO::_byteWrite(uint8_t adr, uint8_t reg, uint8_t value) {      // Accept the register and byte
	_SPIstart();                                 // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(reg);                                   // Send the register we want to write
	SPI.transfer(value);                                 // Send the byte
	_SPIstop();                                 // Slave Select HIGH after SPI action
}

// GENERIC WORD WRITE - will write a word to a register pair, LSB to first register, MSB to next higher value register
void PIO::_wordWrite(uint8_t adr, uint8_t reg, uint16_t value) {  // Accept the start register and word
	_SPIstart();                               // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(reg);                                   // Send the register we want to write
	SPI.transfer((uint8_t)(value & 0xff));                      // Send the low byte (register address pointer will auto-increment after write)
	SPI.transfer((uint8_t)(value >> 8));                 // Shift the high byte down to the low byte location and send
	_SPIstop();                                 // Slave Select HIGH after SPI action
}

uint8_t PIO::_byteRead(uint8_t adr, uint8_t reg) {        // This function will read a single register, and return it
	uint8_t value = 0;                        // Initialize a variable to hold the read values to be returned
	_SPIstart();                     // Slave Select LOW before SPI action
	SPI.transfer(OPCODER | (adr << 1));  // Send the MCP23S17 opcode, chip address, and read bit
	SPI.transfer(reg);                        // Send the register we want to read
	value = SPI.transfer(0x00);               // Send any byte, the function will return the read value
	_SPIstop();                      // Slave Select HIGH after SPI action
	return value;                             // Return the constructed word, the format is 0x(register value)
}

void PIO::_pinDirection(uint8_t adr, uint16_t mode) {    // Accept the word…
	_wordWrite(adr, IODIRA, mode);                // Call the the generic word writer with start register and the mode cache
}

void PIO::_pullupMode(uint8_t adr, uint16_t mode) {
	_wordWrite(adr, GPPUA, mode);
}

void PIO::_inputInvert(uint8_t adr, uint16_t mode) {
	_wordWrite(adr, IPOLA, mode);
}

uint16_t PIO::mcpRead(uint8_t adr) {       // This function will read all 16 bits of I/O, and return them as a word in the format 0x(portB)(portA)
	uint16_t value = 0;                   // Initialize a variable to hold the read values to be returned
	_SPIstart();                      // Slave Select LOW before SPI action
	SPI.transfer(OPCODER | (adr << 1));  // Send the MCP23S17 opcode, chip address, and read bit
	SPI.transfer(GPIOA);                      // Send the register we want to read
	value = SPI.transfer(0x00);               // Send any byte, the function will return the read value (register address pointer will auto-increment after write)
	value |= (SPI.transfer(0x00) << 8);       // Read in the "high byte" (portB) and shift it up to the high location and merge with the "low byte"
	_SPIstop();                     // Slave Select HIGH after SPI action
	return value;                             // Return the constructed word, the format is 0x(portB)(portA)
}

byte PIO::mcpReadPA(uint8_t adr) {       // This function will read 8 bits of I/O
	byte value = 0;                   // Initialize a variable to hold the read values to be returned
	_SPIstart();                      // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));  // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(GPIOA);                      // Send the register we want to read
	value = SPI.transfer(0x00);               // Send any byte, the function will return the read value (register address pointer will auto-increment after write)
	_SPIstop();                     // Slave Select HIGH after SPI action
	return value;                             // Return the byte
}

byte PIO::mcpReadPB(uint8_t adr) {       // This function will read 8 bits of I/O
	byte value = 0;                   // Initialize a variable to hold the read values to be returned
	_SPIstart();                      // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));  // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(GPIOB);                      // Send the register we want to read
	value = SPI.transfer(0x00);               // Send any byte, the function will return the read value (register address pointer will auto-increment after write)
	_SPIstop();                     // Slave Select HIGH after SPI action
	return value;                             // Return the byte
}

void PIO::mcpWrite(uint8_t adr, uint16_t value) {
	_SPIstart();                               // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(GPIOA);                                   // Send the register we want to write
	SPI.transfer((uint8_t)(value & 0xff));                      // Send the low byte (register address pointer will auto-increment after write)
	SPI.transfer((uint8_t)(value >> 8));                 // Shift the high byte down to the low byte location and send
	_SPIstop();                                 // Slave Select HIGH after SPI action
}

void PIO::mcpWritePA(uint8_t adr, uint8_t value) {
	_SPIstart();                               // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(GPIOA);                                   // Send the register we want to write
	SPI.transfer(value);
	_SPIstop();                                 // Slave Select HIGH after SPI action
}

void PIO::mcpWritePB(uint8_t adr, uint8_t value) {
	_SPIstart();                               // Slave Select LOW before SPI action
	SPI.transfer(OPCODEW | (adr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(GPIOB);                                   // Send the register we want to write
	SPI.transfer(value);
	_SPIstop();                                 // Slave Select HIGH after SPI action
}

inline void PIO::_SPIstart() {
	SPI.beginTransaction(_spis); 
	digitalWrite(_ss_pin, LOW);
}

inline void PIO::_SPIstop() {
	digitalWrite(_ss_pin, HIGH); 
	SPI.endTransaction();
}
