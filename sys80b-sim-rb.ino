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

#include <SPI.h>
#include <SD.h>
#include <LiquidCrystalFast.h>
#include <Bounce.h>
//#include <MemoryFree.h>

#include "ioPins.h"
#include "PIO.h"
#include "Led8x8.h"
#include "msgOutput.h"
#include "TimerTask.h"
#include "TimerSet.h"
//#include "Timer.h"

// *** GAME SPECIFIC CODE ***
#include "BigHouse.h"
// **************************

#define BAUDRATE 115200      // serial communication through USB
#define BATCH_DELAY 1000    // batch loop duration (ms)
#define LMP_TMR_PERIOD 50   // lamps update timer
#define SOL_TMR_PERIOD 100  // solenoid update timer
#define SND_TMR_PERIOD 50   // sound update timer period

SPISettings spiSettings(SPI_CLK, MSBFIRST, SPI_MODE0);

LiquidCrystalFast lcd(LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, 
	LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
//const byte lcd_rows = lcd.numlines;
//const byte lcd_cols = lcd.numcols;

Bounce nextButton(NEXTB_PIN, DEBOUNCE_T);
Bounce enterButton(ENTRB_PIN, DEBOUNCE_T);
Bounce backButton(BACKB_PIN, DEBOUNCE_T);

// timers declaration
TimerTask *millis_tmr, *lamps_tmr, *sols_tmr, *snd_tmr;
TimerSet *tmr;
//Timer *tmr;

Game *game;
enum lg_mode_enum ledGridMode;
uint32_t millisRoutineT; // [ms]

// functions declaration
void millisRoutine(uint32_t ms);
void updateLedGrid(lg_mode_enum mode);
void pinsSetup();
//void _outptest();

void setup() {

  _GAME_CLASS_ sys80_game;
  game = &sys80_game;

  // I/O init
  PIO::init(spiSettings, GPIO_SS_PIN);
  Led8x8::init(spiSettings, LG_SS_PIN);
  Led8x8::setLGintens(3);
  Led8x8::enabled = true;
  ledGridMode = LG_LIGHTS;

  // Serial COM init
  Serial.begin(BAUDRATE);

  // LCD setup
  //lcd.begin(lcd_cols, lcd_rows);
  lcd.begin(lcd.numcols, lcd.numlines);
  lcd.clear();
  lcdOutputEnabled = true;
  
  // messages output initialization
  serialOutputEnabled = true;
  outpln_clr(F("System init..."));
   
   //writeSound(1); // LED init view using SOUND output

  // API setup
  //outpln_clr(F("API init..."));
  //initAPI();
  //writeSound(2);
  //return;

  // timers setup
  outpln(F("Timers init..."));
  //tmr = new Timer();
  tmr = new TimerSet();
  //millis_tmr = new TimerTask(millisRoutine, 1, "Milliseconds tasks");
  lamps_tmr = new TimerTask(game->updateLights, LMP_TMR_PERIOD, "Lamps");
  sols_tmr = new TimerTask(game->checkSolenoids, SOL_TMR_PERIOD, "Solenoids");
  snd_tmr = new TimerTask(game->checkSoundCmd, SND_TMR_PERIOD, "Sound");
  //tset->add(millis_tmr);
  lamps_tmr->enable();
  sols_tmr->enable();
  snd_tmr->enable();
  //millis_tmr->enable();tmr->add(lamps_tmr);
  tmr->add(sols_tmr);
  tmr->add(snd_tmr);
  millisRoutineT = 0;

  // pinball init
  outpln_clr(F("Pinball init..."));
  //lcd.setCursor(0, 1);
  //lcd.print(freeMemory());
  //lcd.print(" bytes free");
  //initPinball();
  game->init();
  //writeSound(5);

  outpln(F("Go..."));
  game->setPinballMode(ATTRACT_MODE);

}

// top-most Teensy loop
void loop() {
  uint32_t t, loopStartT; // [ms]
  uint32_t cumWorkTime, batchPeriod, t1, dt; // [us]
  byte busy_perc; // [%]
  uint32_t loops;

    loops = 0;
    cumWorkTime = 0;
    batchPeriod = BATCH_DELAY * 1000;
    loopStartT = millis();
    //millisRoutineT = 0;

    do { // Batch loop
      t1 = micros();

      t = millis();
      if (t != millisRoutineT) {
        millisRoutineT = t;
        millisRoutine(t);
      }

      tmr->checkTimerTasks();
      dt = micros() - t1;
      cumWorkTime += dt;
      loops++;
	  if (dt < 100) delayMicroseconds(100 - dt);
    } while (millis() - loopStartT < BATCH_DELAY);
    busy_perc = (byte)(((cumWorkTime * 100) / batchPeriod) & 0xff);
    outp_clr(busy_perc);
    outp("% - ");
    outp(loops);
    outpln(" loops/s");
}

// system routines to call every 1 millisecond
void millisRoutine(uint32_t ms) {
  bool b;

  if (ms == 0) ms = millis();

  // SWITCH GRID READING AND EVENTS
  game->getNextReturns(ms);

  // LED grid update
  updateLedGrid(ledGridMode);

  // DISPLAY UPDATE
  game->feedDisplay();

  // Debounced on-board buttons update
  b = nextButton.update();
  if (b && nextButton.fallingEdge()) game->onPRBButtonPressed(BoardIO::NEXT_BUTT);
  b = enterButton.update();
  if (b && enterButton.fallingEdge()) game->onPRBButtonPressed(BoardIO::ENTER_BUTT);
  b = backButton.update();
  if (b && backButton.fallingEdge()) game->onPRBButtonPressed(BoardIO::BACK_BUTT);
}

void updateLedGrid(lg_mode_enum mode) {
	byte b = 0;

	switch (mode) {
	case LG_SWITCHES:
		b = game->switchGrid.getSequentialRow();
		break;
	case LG_LIGHTS:
		byte i = game->switchGrid.strobe << 1;
		b = game->lamps.getGroup[i] | (game->lamps.getGroup[i + 1] << 4);
		break;
	case LG_OFF:
		break;
		b = 0;
	default:
		return;
	}
	Led8x8::setLedRowReverse(game->switchGrid.strobe, b);
}

void pinsSetup() {
	pinMode(SLAM_PIN, INPUT_PULLUP);
	pinMode(NEXTB_PIN, INPUT_PULLUP);
	pinMode(ENTRB_PIN, INPUT_PULLUP);
	pinMode(BACKB_PIN, INPUT_PULLUP);
	pinMode(GPIO_SS_PIN, OUTPUT);
	pinMode(LG_SS_PIN, OUTPUT);
	pinMode(SD_SS_PIN, OUTPUT);
	pinMode(D_LD1_PIN, OUTPUT);
	pinMode(D_LD2_PIN, OUTPUT);
	pinMode(D_RES_PIN, OUTPUT);
	digitalWrite(GPIO_SS_PIN, HIGH);
	digitalWrite(LG_SS_PIN, HIGH);
	digitalWrite(SD_SS_PIN, HIGH);
	digitalWrite(D_LD1_PIN, LOW);
	digitalWrite(D_LD2_PIN, LOW);
	digitalWrite(D_RES_PIN, LOW);
}

/*
// (temporary) test routine for Teensy outputs
void _outptest() {
  int i;

  for (i=0; i<9; i++) {
    setPeriod(i, true);
    delay(100);
  }
  for (i=0; i<9; i++) {
    setPeriod(i, false);
    delay(100);
  }
  for (i=0; i<LAMPS_COUNT; i++) {
    setLamp(i, true);
    renderLamps();
    delay(100);
  }
  for (i=0; i<LAMPS_COUNT; i++) {
    setLamp(i, false);
    renderLamps();
    delay(100);
  }
  for (i=0; i<32; i++) {
    writeSound(i);
    delay(100);
  }
  writeSound(0);
  for (i=0; i<8; i++) {
    setLedRow(i,1<<i);
    delay(100);
  }
    for (i=0; i<8; i++) {
    setLedRow(i,0);
    delay(100);
  }
  clearLGgrid();
}
*/

