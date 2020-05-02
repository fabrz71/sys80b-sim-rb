/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.2 board, developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* MAIN "ARDUINO" SOURCE FILE
* ---
* Teensy board setup and main loop code.
* ---
* This software is part of "Sys80b SIM PRB" project which aims to offer
* a programmable replacement control board for Gottlieb pinballs based on
* System 80/B.
* For more informations see project overview document.
*/

//#include <Arduino.h>
//#include <SPI.h>
//#include <Bounce.h>
//#include <SD_t3.h>
////#include <SD.h>
//#include <EEPROM.h>
//#include <LiquidCrystalFast.h>
#include <MemoryFree.h>
#include "msg.h"
#include "Board_Sys80b.h"
#include "CmdExecutor.h"
//#include "msg.h"

// *** GAME SPECIFIC CODE ***
#include "BigHouse.h"
//#include "TestModule.h"
// **************************

#define BATCH_DELAY 1000    // statistic loop duration (ms)
//#define LG_MODE LG_LIGHTS
#define LG_MODE LG_LAMPS

uint32_t millisRoutineT; // system time of last routine call [ms]
Board_Sys80b board;
Msg& msg = board.msg;
Sys80b* sys;
uint16_t loopCounter;
CmdExecutor* executor;
PinGame* game;
void millisRoutine(uint32_t& ms);
void checkBoardButtons(uint32_t& ms);
void updateLedGrid(LG_mode mode);
bool readSerialInput();

void setup() { 
    loopCounter = 0;
    board.begin();
    msg.disableLCDOutput();
    msg.setPostDelay(0);
    board.ledGridMode = LG_MODE;
    msg.prFreeMem();
    msg.outln(F("Game instantiation..."));
    //game = new _GAME_IMPLEM_();
    game = new _GAME_IMPLEM_(board);
    sys = game;
    msg.outln(F("Connecting hardware..."));
    //delay(100);
    //sys->connectHardware(board);
    executor = new CmdExecutor(&board, game);
    //sys->reset();
    msg.outln(F("Starting..."));
    //msg.setPostDelay(50);
    game->begin();

    millisRoutineT = 0;
    msg.prFreeMem();
}

// top-most Teensy loop
void loop() {
    //msg.setPostDelay(0);
    msg.out("Loop #");
    msg.out(loopCounter);
    msg.outln("...");
    //msg.setPostDelay(50);
    //prFreeMem();
    readSerialInput();

    uint32_t t, loopStartT; // [ms]
    uint32_t cumWorkTime, batchPeriod, t1, dt; // [us]
    byte busy_perc; // [%]
    uint32_t loops;

    loops = 0;
    cumWorkTime = 0;
    batchPeriod = BATCH_DELAY * 1000u;
    loopStartT = millis();
    dt = 0;

    do { // Batch loop
      delayMicroseconds(BATCH_DELAY - dt);
      t = millis();

      if (t != millisRoutineT) {
        t1 = micros();
        millisRoutineT = t;
		millisRoutine(t);
        dt = micros() - t1;
        cumWorkTime += dt;
      }

      loops++;
    } while (t - loopStartT < BATCH_DELAY);

    busy_perc = (byte)((cumWorkTime * 100) / batchPeriod);

    //msg.clr();
    //String s = game->display.toString();
    //msg.outln(s);
    
    //msg.prFreeMem();

    //String st;
    //ls = "Running.. (";
    //ls += busy_perc;
    //ls += "%)";
    //board.lcdprn(1, ls);
    //processSerialInput();

    msg.out(busy_perc);
    msg.out("% - ");
    msg.out(loops);
    msg.out(" loops/s ");
    msg.out("(");
    msg.out(cumWorkTime);
    msg.outln("us work time)\n");
    loopCounter++;
}

void millisRoutine(uint32_t& ms) {
    //msg.out(" I");
    updateLedGrid(board.ledGridMode); // LED grid update
    //msg.out(" II");
    checkBoardButtons(ms);
    //msg.out(" III ");
    game->millisRoutine(ms);
}

// PRB buttons check
void checkBoardButtons(uint32_t& ms) {
    if ((ms & 0x1f) == 0) { // every 32 ms
        if (board.readButtonsChange()) { // ONLY ON CHANGE EVENT...
            Button b = board.readButtons(); // Debounced on-board buttons update
            UserKey k = NO_KEY;
            switch (b) {
            case NEXT_BUTT:
                msg.outln("NEXT button pressed");
                k = RIGHTADV_KEY;
                break;
            case BACK_BUTT:
                msg.outln("BACK button pressed");
                k = LEFTADV_KEY;
                break;
            case ENTER_BUTT:
                msg.outln("ENTER button pressed");
                k = REPLAY_KEY;
                break;
            case NONE_BUTT:
                msg.outln("button released");
                k = NO_KEY;
                break;
            }
            game->setKeyPressed(k, ms);
        }
    }
}

void updateLedGrid(LG_mode mode) {
    byte b = 0;
    static byte r = 0;
    //static bool show = true;

    //r = game->switchGrid.getCurrentStrobe();
    switch (mode) {
    case LG_SWITCHES:
        b = sys->switchGrid.getRow(r);
        if (r == 7) { // slam switch rendered as switch 77 (strobe #7, return #7)
            if (sys->readSlamSwitch()) b |= 0x01;
            else b &= 0xfe;
        }
        //if (show) Serial.printf("s%d -> %d\n", r, b);
        //if (r == 7) show = false;
        break;
    case LG_LAMPS:
        if (r < 6) b = sys->lamps->getStates8(r);
        else { // r = 6, 7
            b = 0;
            for (int i = (r-6)*8; i < (8 + (r-6)*4); i++) {
                b <<= 1;
                if (sys->lamps->getChanges4(i) > 0) b |= 0x1; 
            }
        }
        break;
    case LG_OFF:
        break;
        b = 0;
    default:
        return;
    }
    board.ledGrid->setRowByte(r++, b);
    r &= 0x7;
}

bool readSerialInput() {
    bool ret = false;
	if (Serial.available() > 0) {
		String st = Serial.readString();
		ret = executor->execCmd(st);
        //delete st;
	}
	return ret;
}
