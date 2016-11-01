/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* version 0.16.10
* ---
* MAIN SOURCE FILE
* ---
* Teensy board setup and main loop code.
* ---
* This software is part of "Sys80b SIM PRB" project which aims to design
* a programmable replacement control board for Gottlieb pinballs based on
* System 80/B.
* For more informations see project overview document.
*/

#include <Bounce.h> // contacts unbounce library
#include "pinSpecific.h" // pinball generic functions code

#define DEBOUNCE_T 200      // keys debounce threshold time
#define LMP_TMR_PERIOD 50   // lamps update timer
#define SOL_TMR_PERIOD 100  // solenoid update timer
#define SND_TMR_PERIOD 50   // sound update timer period
#define BAUDRATE 57600      // serial communication through USB
#define BATCH_DELAY 1000    // batch loop duration (ms)

// general pinball identification data
#define PIN_BRAND "Gottlieb"
#define PIN_SYSTEM "System 80/B"

// Teensy I/O pins definition
const byte SLAM_PIN = 2; // INPUT, active low - slam switch
const byte NEXTB_PIN = 3; // INPUT PULLUP, active high - "Next" control button
const byte ENTRB_PIN = 4; // INPUT PULLUP, active high - "Enter" control button
const byte BACKB_PIN = 5; // INPUT PULLUP, active high - "Back" control button
const byte SD_SS_PIN = 10; // OUTPUT, active low - SPI SD select
const byte GPIO_SS_PIN = 14; // OUTPUT, active low - baseAPI.h - SPI GPIO select
const byte LG_SS_PIN = 15; // OUTPUT, active low - ledGrid.h - SPI Led Grid select
const byte D_LD1_PIN = 16; // OUTPUT, active high - pinball display 1 select
const byte D_LD2_PIN = 17; // OUTPUT, active high - pinball display 2 select
const byte D_RES_PIN = 18; // OUTPUT, active high - pinball displays reset
const byte LCD_D4_PIN = 23; // OUTPUT - LCD display data bits..
const byte LCD_D5_PIN = 22;
const byte LCD_D6_PIN = 21;
const byte LCD_D7_PIN = 20;
const byte LCD_RW_PIN = 6; // OUTPUT - LCD display R/W
const byte LCD_RS_PIN = 8; // OUTPUT - LCD display reset
const byte LCD_EN_PIN = 9; // OUTPUT - LCD display enable

byte lcd_rows = 2;
byte lcd_cols = 16;

// functions declaration
void pinsSetup();
void millisRoutine(uint32_t ms);
//void _outptest();

// buttons debounce objects
Bounce nextButton = Bounce(NEXTB_PIN, DEBOUNCE_T);
Bounce enterButton = Bounce(ENTRB_PIN, DEBOUNCE_T);
Bounce backButton = Bounce(BACKB_PIN, DEBOUNCE_T);

// timers declaration
TimerTask *millis_tmr, *lamps_tmr, *sols_tmr, *snd_tmr;
TimerSet *tset;

void setup() {

  // Teensy I/O pins setup
  pinsSetup();

  // SPI init
  SPI.begin();

  // MCP init
  MCP_init(GPIO_SS_PIN);

  // Serial COM init
  Serial.begin(BAUDRATE);

  // LCD setup
  lcd.begin(lcd_cols, lcd_rows);
  lcd.clear();

  // messages output initialization
  lcdOutputEnabled = true;
  serialOutputEnabled = true;

  // LED grid init
  outpln_clr(F("Led Grid init..."));
  ledGridEnabled = true;
  ledGridMode = LG_LIGHTS;
  initLedGrid(LG_SS_PIN);
  setLGintens(3);
  //writeSound(1); // LED init view using SOUND output

  // API setup
  outpln_clr(F("API init..."));
  initAPI();
  //writeSound(2);

  //return;

  // timers
  tset = new TimerSet();
  outpln(F("Timers init..."));
  //millis_tmr = new TimerTask(millisRoutine, 1);
  //millis_tmr->tag = (const char *)F("Milliseconds tasks");
  lamps_tmr = new TimerTask(updateLamps, LMP_TMR_PERIOD, "Lamps");
  sols_tmr = new TimerTask(checkSolenoids, SOL_TMR_PERIOD, "Solenoids");
  snd_tmr = new TimerTask(checkSoundCmd, SND_TMR_PERIOD, "Sound");
  //writeSound(3);
  //tset->add(millis_tmr);
  tset->add(lamps_tmr);
  tset->add(sols_tmr);
  tset->add(snd_tmr);
  //writeSound(4);
  lamps_tmr->enable();
  sols_tmr->enable();
  snd_tmr->enable();

  // pinball init
  outpln_clr(F("Pinball init..."));
  lcd.setCursor(0, 1);
  lcd.print(freeMemory());
  lcd.print(" bytes free");
  initPinball();
  //writeSound(5);

  outpln(F("Go..."));
  setPinMode(START_MODE);

}

// top-most Teensy loop
void loop() {
  uint32_t t, loopStartT, millisRoutineT; // [ms]
  uint32_t cumWorkTime, batchPeriod, t1, t2, dt; // [us]
  byte busy_perc; // [%]
  uint32_t loops;

  while(1) {
    //_outptest();
    //return;

    loops = 0;
    cumWorkTime = 0;
    batchPeriod = BATCH_DELAY * 1000;
    loopStartT = millis();
    millisRoutineT = 0;
    do { // Batch loop
      t1 = micros();

      t = millis();
      if (t != millisRoutineT) {
        millisRoutineT = t;
        millisRoutine(t);
      }

      //millisRoutine(0);

      tset->checkTimerTasks();
      t2 = micros();
      dt = t2 - t1;
      //delayMicroseconds(998-dt);
      cumWorkTime += dt;
      loops++;
    } while (millis() - loopStartT < BATCH_DELAY);
    busy_perc = (byte)(((cumWorkTime * 100) / batchPeriod) & 0xff);
    /*
    Serial.print(busy_perc);
    Serial.print("% - ");
    Serial.print(loops);
    Serial.println(" loops/sec");
    if (lcdOutputEnabled) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(busy_perc);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print(loops);
      lcd.print(" loops/sec");
    }
    */
    outp_clr(busy_perc);
    outp("% - ");
    outp(loops);
    outpln(" loops/s");
  };
}

// Teensy starting I/O pins setup
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

// system routines to call every 1 millisecond
void millisRoutine(uint32_t ms) {
  bool b;
  byte ret;

  if (ms == 0) ms = millis();

  // SWITCH GRID READING
  ret = getNextReturns(ms);

  // LED grid update
  if (ledGridMode == LG_SWITCHES) setLedRowReverse(strobe, ret);
  else if (ledGridMode == LG_LIGHTS) {
    byte i = (strobe & 7)<<1;
    setLedRowReverse(strobe, lamps[i] | (lamps[i+1] << 4));
  }

  // DISPLAY UPDATE
  pushByteOnDisplayRows();

  // Debounced on-board buttons update
  b = nextButton.update();
  if (b && nextButton.fallingEdge()) onPRBButtonPressed(NEXT_BUTT);
  b = enterButton.update();
  if (b && enterButton.fallingEdge()) onPRBButtonPressed(ENTER_BUTT);
  b = backButton.update();
  if (b && backButton.fallingEdge()) onPRBButtonPressed(BACK_BUTT);
}

/*
// (temporary) test routine for Teensy outputs
void _outptest() {
  int i;

  for (i=0; i<9; i++) {
    setSolenoid(i, true);
    delay(100);
  }
  for (i=0; i<9; i++) {
    setSolenoid(i, false);
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
