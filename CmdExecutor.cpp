#include "CmdExecutor.h"

#include <MemoryFree.h>
#include "Board_Sys80b.h"
#include "PinGame.h"

bool CmdExecutor::execCmd(String command) {
	String s;

	unsigned int len = command.length();
	if (len == 0) return false;
	if (_hw == nullptr || _sys == nullptr) {
		Serial.print(F("execCmd(..): WARNING: neither system or board undefined!"));
		return false;
	}
	if (len > MAX_LENGTH) command = command.substring(0, MAX_LENGTH-1);
	_getParts(command);

	//Serial.printf(F("execCmd(..): detected %d parts:\n"), _partsCount);
	//for (int i = 0; i < _partsCount; i++) {
	//	Serial.printf("#%d: '", i);
	//	Serial.print(_cmdPart[i]);
	//	Serial.println("'");
	//}
	//if (_partsCount > 0) Serial.println("---");

	if (_partsCount == 0) return false;
	String& cmd = _cmdPartStr[0];
	const char* clrStr = "clear";
	int arg1, arg2, arg3;
	bool ok = false;

	if (cmd == soundCmd) {
		arg1 = _cmdPartStr[1].toInt() & 0x1f;
		_sys->setSound(arg1);
		if (_partsCount > 2) {
			arg2 = _cmdPartStr[2].toInt() & 0x1f;
			_sys->setSound(arg2);
		}
		ok = true;
	}
	if (cmd == solenoidCmd) {
		if (_cmdPartStr[1] == clrStr) _sys->solenoids->resetAll();
		else {
			arg1 = _cmdPartStr[1].toInt();
			arg2 = _cmdPartStr[2].toInt();
			_sys->setSolenoid(arg1, arg2 > 0);
		}
		ok = true;
	}
	if (cmd == pulsesolenoidCmd) {
		arg1 = _cmdPartStr[1].toInt();
		arg2 = _cmdPartStr[2].toInt();
		if (_partsCount == 2) _sys->activateSolenoid(arg1, arg2);
		else {
			arg3 = _cmdPartStr[3].toInt();
			_sys->activateSolenoid(arg1, arg2, arg3);
		}
		ok = true;
	}
	if (cmd == lampCmd) {
		if (_cmdPartStr[1] == clrStr) _sys->switchOffAllLights();
		else {
			arg1 = _cmdPartStr[1].toInt();
			arg2 = _cmdPartStr[2].toInt();
			_sys->setLamp(arg1, arg2 > 0, true);
		}
		ok = true;
	}
	if (cmd == lightCmd) {
		if (_cmdPartStr[1] == clrStr) _sys->switchOffAllLights();
		else {
			arg1 = _cmdPartStr[1].toInt();
			arg2 = _cmdPartStr[2].toInt();
			_sys->setLight(arg1, arg2 > 0);
		}
		ok = true;
	}
	if (cmd == blinklightCmd) {
		arg1 = _cmdPartStr[1].toInt();
		if (_partsCount >= 3) arg2 = _cmdPartStr[2].toInt(); else arg2 = 250u;
		_sys->blinkLight(arg1, arg2);
		ok = true;
	}
	if (cmd == pulselightCmd) {
		arg1 = _cmdPartStr[1].toInt();
		_sys->pulseLight(arg1);
		ok = true;
	}
	if (cmd == ledgridCmd) {
		if (_cmdPartStr[1] == clrStr) _hw->ledGrid->clear();
		else {
			arg1 = _cmdPartStr[1].toInt() & 7;
			arg2 = _cmdPartStr[2].toInt() & 0xff;
			_hw->ledGrid->setRow(arg1, arg2);
		}
		ok = true;
	}
	if (cmd == ledgridmodeCmd) {
		if (_cmdPartStr[1] == "lamps") {
			_hw->ledGridMode = LG_LAMPS;
			ok = true;
		}
		if (_cmdPartStr[1] == "switches") {
			_hw->ledGridMode = LG_SWITCHES;
			ok = true;
		}
		if (_cmdPartStr[1] == "off") {
			_hw->ledGridMode = LG_OFF;
			ok = true;
		}
		ok = true;
	}
	if (cmd == lcdCmd) {
		if (_cmdPartStr[1] == clrStr) _hw->lcdclr();
		else {
			s = command.substring(_cmdPartIdx[2]);
			_hw->lcdprn(0, s, arg1);
		}
		ok = true;
	}
	if (cmd == displayCmd) {
		if (_cmdPartStr[1] == clrStr) _sys->display.clear();
		else {
			arg1 = _cmdPartStr[1].toInt() & 1;
			s = command.substring(_cmdPartIdx[2]);
			_sys->display.setText(arg1, s);
		}
		ok = true;
	}

	if (cmd == factorysettingsCmd) {
		_hw->restoreFactorySettings(true);
		_hw->lcdclr();
		_hw->lcdprn(0, F("Factory Settings restored."));
		ok = true;
	}
	if (cmd == freememCmd) {
		_hw->restoreFactorySettings(true);
		_hw->lcdclr();
		s = String(freeMemory());
		_hw->lcdprn(0, s);
		_hw->lcdprn(F(" bytes free"));
		ok = true;
	}
	if (cmd == revCmd) {
		_hw->lcdclr();
		_hw->lcdprn(0, F("PRB rev."));
		s = String(_hw->getRevision());
		_hw->lcdprn(s);
		_hw->lcdprn(F(" by"));
		_hw->lcdprn(1, F("F.Volpi - ITALY"));
		ok = true;
	}
	if (cmd == pocCmd) {
		if (_partsCount > 1 && _cmdPartStr[1] == clrStr) _hw->resetPowerOnCounter();
		_hw->lcdclr();
		_hw->lcdprn(0, F("Power-on counter"));
		s = String(_hw->getPowerOnCounter());
		_hw->lcdprn(1, s);
		ok = true;
	}
	if (cmd == resetCmd) {
		_sys->reset();
		_hw->lcdclr();
		_hw->lcdprn(0, F("Reset done."));
		ok = true;
	}
	if (cmd == helpCmd) {
		static const char* str1 = " <number> [<number>]";
		static const char* str2 = " clear | <number> <0|1>";
		static const char* str3 = " clear | <row> <text>";
		_hw->lcdclr();
		_hw->lcdprn(0, F("See serial comm"));
		_hw->lcdprn(1, F("for list of cmd."));
		Serial.println(F("AVAILABLE COMMANDS:"));
		Serial.print(soundCmd);	Serial.println(str1);
		Serial.print(solenoidCmd); Serial.println(str2);
		Serial.print(pulsesolenoidCmd);	Serial.println(F(" <number> [<milliseconds>]"));
		Serial.print(lampCmd); Serial.println(str2);
		Serial.print(lightCmd);	Serial.println(str2);
		Serial.print(blinklightCmd); Serial.println(F(" <number> [<period>]"));
		Serial.print(pulselightCmd); Serial.println(F(" <number>"));
		Serial.print(ledgridmodeCmd); Serial.println(F(" lights | switches | off"));
		Serial.print(lcdCmd); Serial.println(str3);
		Serial.print(displayCmd); Serial.println(str3);
		Serial.print(factorysettingsCmd);
		Serial.println(F(" : restore factory settings in NVRAM"));
		Serial.print(freememCmd);
		Serial.println(F(" : show Teensy's free memory"));
		Serial.print(revCmd);
		Serial.println(F(" : show board revision"));
		Serial.print(pocCmd);
		Serial.println(F(" [clear] : [clear and] show power-on counter"));
		Serial.print(resetCmd);	Serial.println(F(" : reset system 80/B"));
		Serial.println(helpCmd);
		Serial.println();
		ok = true;
	}
	if (!ok) {
		Serial.print(F("* Unknown or incorrect command: '"));
		Serial.print(cmd);
		Serial.println("'!");
	}
	return ok;
}

void CmdExecutor::_getParts(String& cmd) {
	int i0, i1, len;
	i0 = 0;
	i1 = 0;
	
	//firstArgIdx = 0;
	_partsCount = 0;
	len = cmd.length();
	if (len == 0) return;
	//for (i0 = 0; i0 < len; i0++) if (cmd.charAt(i0) < 32) cmd.setCharAt(i0, 32);
	//for (i0 = 0; i0 < len; i0++) if (cmd.charAt(i0) == ',') cmd.setCharAt(i0, 32);
	for (_partsCount = 0; _partsCount < MAX_PARTS; _partsCount++) {
		while (cmd.charAt(i0) <= 32 && i0 < len) i0++; // search 1st non-space char
		if (i0 == len) break;
		i1 = i0;
		while (cmd.charAt(i1) > 32 && i1 < len) i1++; // search 1st space char
		_cmdPartStr[_partsCount] = cmd.substring(i0, i1).toLowerCase();
		_cmdPartIdx[_partsCount] = i0;
		//if (_partsCount == 1) firstArgIdx = i0;
		i0 = i1;
	}
	for (i1 = _partsCount; i1 < MAX_PARTS; i1++) {
		_cmdPartStr[i1] = "";
		_cmdPartIdx[i1] = i0;
	}
}
