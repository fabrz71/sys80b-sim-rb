/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board developed with Arduino IDE
* under GNU General Public Livence v3
* by Fabrizio Volpi (fabvolpi@gmail.com)
* ---
* SYSTEM 80/B I/O FUNCTIONS
* ---
* Defines a System 80/B architecture, keeping states of
* solenoids, lamps, sound buffer, display and switches.
* Implements higher level System 80/B I/O functions but does not
* directly act on hardware:
* All input/output commands are issued exclusively through
* BoardIO::setOuput() and BoardIO::getInput() functions, and would
* be effectively executed only when the class instance is pointed by
* BoardIO::connectedSystem.
*/

#include "Sys80b.h"

using namespace BoardIO;

Sys80b::Sys80b() {
	// virtual objects instantiation
	solenoids = SolenoidSet(10); // real solenoids (0..9) (0 = unused)
	lampSolenoids = SolenoidSet(16); // "lamp" solenoids (16..31)
	lamps = LampSet(LAMPS_COUNT);
	sound = SoundBuff(8);

}

// reset all outputs to 0
void Sys80b::reset() {

	// sound and display reset
	setOutput(this, DS_RESET, 0); 
	delay(100);
	setOutput(this, DS_RESET, 1);
	sound.Empty();
	setOutput(this, SOUND, 0xff); // reset sound output (active low)
	displ.clear();
	displ.reset();

	// other machine setup
	switchGrid.strobe = 0;
	setOutput(this, STROBES, 0xff); // reset strobes output (active low)
	solenoids.resetAll();
	setOutput(this, SOLENOIDS, 0);
	lamps.resetAll();
}

// sets a solenoid state, updating internal variables
// note:
// n = [1..9] standard solenoid
// n = [10..15] no effect
// n = [16..31] "lamp" solenoid 0..15
void Sys80b::setSolenoid(byte n, bool active) {
	if (n < 1 || n > 32 || (n > 9 && n < 16)) return;
	if (n <= 9) { // real solenoid
		uint16_t sol = solenoids.set(n, active);
		setOutput(this, SOLENOIDS, sol);
	}
	else { // "lamp" solenoid
		lampSolenoids.set(n & 0xf, active);
		lamps.setLamp(n & 0xf, active);
	}
}

// sets a solenoid state, defining active period.
// actPeriod updates solenoid attribute also when active = false
inline void Sys80b::activateSolenoid(byte n, uint16_t actPeriod) {
	activateSolenoidDelayed(n, actPeriod, 0);
}

// sets a solenoid state, defining switch delay:
// solenoid state will switch after the specified delay.
inline void Sys80b::activateSolenoidDelayed(byte n, uint16_t swDelay) {
	activateSolenoidDelayed(n, 0, swDelay);
}

// sets a solenoid state, defining switch delay and active period:
// solenoid state will switch after the specified delay.
inline void Sys80b::activateSolenoidDelayed(byte n, uint16_t actPeriod, uint16_t swDelay) {
	if (n < 1 || n > 32 || (n > 9 && n < 16)) return;
	if (n <= 9) solenoids.activateDelayed(n, actPeriod, swDelay);
	else lampSolenoids.activateDelayed(n, actPeriod, swDelay);
}

// checks whether:
// - delay activation time elapsed (when delayedSwitch = true)
// - maximum active time elapsed for safe switch-off
inline void Sys80b::checkSolenoids(uint32_t t) {
	if (solenoids.check(t)) setOutput(this, SOLENOIDS, solenoids.getBits());
	if (lampSolenoids.check(t)) {
		byte sol = lampSolenoids.getLastChanged();
		lamps.setLamp(sol + 16, lampSolenoids.isActive(sol));
	}
}

// setPeriod a lamp on or off, with status memory
inline void Sys80b::setLamp(byte n, bool state) {
	lamps.setLamp(n, state);
}

// setPeriod a lamp on or off, with status memory
inline void Sys80b::setLamp(byte n, byte state) {
	lamps.setLamp(n, state > 0);
}

inline void Sys80b::set4Lamps(byte set, byte states) {
	lamps.setGroup(set, states);
}

inline bool Sys80b::readLamp(byte n) {
	return lamps.getLamp(n);
}

// switch all lamps off
inline void Sys80b::resetLamps() {
	lamps.resetAll();
}

// to call periodically for real lamps update
// updates a group of 4 lamps for each call
bool Sys80b::updateLights() {
	byte i = 0;
	bool changeHit = false;

	while (i++ < 12 && !changeHit) {
		if (++updLampGroup >= 12) updLampGroup = 0;
		changeHit = lamps.isGroupChanged(updLampGroup);
	}
	if (changeHit) {
		//BoardIO::write4Lamps(updLampGroup, lamps.getGroup[updLampGroup]);
		setOutput(this, LAMPS, updLampGroup << 4 | lamps.getGroup[updLampGroup]);
		lamps.validateGroup(updLampGroup);
		return true;
	}
	return false;
}

// snd = 0..31
inline void Sys80b::setSound(byte snd) {
	sound.sendCmd(snd);
	// light #4 sound16 update
	lamps.setLamp(4, (snd & 0x10) > 0);
}

inline void Sys80b::checkSoundCmd(uint32_t t) {
	byte snd = sound.getNextSndOutput(t);
	setOutput(this, SOUND, ~snd);
}

// setPeriod one of eight strobes bit to 0 (active low)
// n = 0..7
inline void Sys80b::setStrobe(byte n) {
	setOutput(this, STROBES, ~bit(n) & 0xff);
}

inline bool Sys80b::readSwitch(byte swNum) {
	return switchGrid.getSwitch(swNum);
}

// 1. reads returns on current strobe line
// 2. call special subroutine on any switch open/close event (with debounce function)
// 3. increments strobe line for next read
// returns: changed switches on strobe line (unbounced), packed in a byte
byte Sys80b::getNextReturns(uint32_t t) {
	byte i, ret, diff, bv, sw;
	byte stbx8;

	ret = (byte)~getInput(this, RETURNS); // read actual switch returns on current strobe line (inverted)
	diff = switchGrid.setRow(switchGrid.strobe, ret, t); // any changes ? (with unbounce effect)
	if (diff) { // changes detected
		stbx8 = switchGrid.strobe << 3; // strobe * 8
		bv = 0; // bit decimal balue
		for (i = 0; i<8; i++) { // for every bit...
			if (diff & bv) { // switch has changed ?
				sw = stbx8 | i; // switch number = strobe * 8 + return
				if (ret & bv) game->onEvent(sw, true); // switch has closed
				else game->onEvent(sw, false); // switch has opened
			}
			bv <<= 1;
		}
	}
	setOutput(this, STROBES, ~bit(switchGrid.incrementStrobe())); // strobe advance (inverted)
	return ret;
}

// Gets next sequencial data to send to display and send ("push") each byte to its 
// correspondigs row.
// If data contains a byte routed to both rows, it will be pushed only at once.
void Sys80b::feedDisplay() {
	uint16_t dd; // display control + data
	byte dc; // display control code only

	// FIRST OR BOTH DISPLAYS
	dd = displ.getNextDisplayData(); // row 0
	dc = (byte)((dd & 0x300) >> 8);
	setOutput(this, DISPL_DATA, dd & 0xff);
	setOutput(this, DISPL_ADDR, dc);
	delayMicroseconds(10);
	setOutput(this, DISPL_ADDR, dc);

	//if ((dd & 0x0300) == 0x0300) return; // CMD for both lines has already been sent
	if (dc == 3) return; // CMD for both rows has already been sent
	delayMicroseconds(100);

	// SECOND DISPLAY
	dd = displ.getNextDisplayData(); // row 1
	//BoardIO::writeDisplayData((byte)(dd & 0xff));
	//BoardIO::writeDisplayLD2(HIGH);
	//delayMicroseconds(10);
	//BoardIO::writeDisplayLD2(LOW);
	dc = (byte)((dd & 0x300) >> 8);
	setOutput(this, DISPL_DATA, dd & 0xff);
	setOutput(this, DISPL_ADDR, dc);
	delayMicroseconds(10);
	setOutput(this, DISPL_ADDR, dc);
}

inline bool Sys80b::isSpecialLamp(byte lamp) {
	return false;
}

inline void Sys80b::setQrelay(bool s) {
	setLamp(0, s);
}

inline void Sys80b::setTrelay(bool s) {
	setLamp(1, s);
}

inline void Sys80b::setArelay(bool s) {
	setLamp(14, s);
}

void Sys80b::timerFunction(uint32_t tm, int id) {
	// TODO
}