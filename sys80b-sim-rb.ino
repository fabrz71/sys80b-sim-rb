/* Gottileb System 80/B SIM PRB (SIMulation Pinball Replacement control Board)
* ---
* This software was developed under Arduino IDE for _Teensy 3.x_ boards. 
* This software is part of "Sys80b SIM PRB" project which aims to design 
* a programmable replacement control board for Gottlieb pinballs based on 
* System 80/B. 
* For more informations see project overview document.
*/

#include <SPI.h>
#include <LiquidCrystalFast.h>
#include <Bounce.h>
#include <SD.h>
#include <EEPROM.h>
#include "pinSpecific.h" // all pinball stuff starts from here

#define DEBOUNCE_T 200      // keys debounce threshold time
#define LMP_TMR_PERIOD 40   // lamps update timer
#define SOL_TMR_PERIOD 100  // solenoid update timer
#define SND_TMR_PERIOD 50   // sound update timer period
#define BAUDRATE 57600      // serial communication through USB

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
const byte LCD_D4_PIN = 20; // OUTPUT - display data bits..
const byte LCD_D5_PIN = 21;
const byte LCD_D6_PIN = 22;
const byte LCD_D7_PIN = 23;
const byte LCD_RS_PIN = 8; // OUTPUT - display reset
const byte LCD_EN_PIN = 9; // OUTPUT - display enable
const byte D_LD1_PIN = 16; // OUTPUT, active high - pinball display 1 select
const byte D_LD2_PIN = 17; // OUTPUT, active high - pinball display 2 select
const byte D_RES_PIN = 18; // OUTPUT, active high - pinball displays reset

// functions declaration
void pinsSetup();
void lcdprn(char *st, byte line);
void lcdprn(String st, byte line);
void millisRoutine(uint32_t ms);
void _outptest();

// buttons debounce objects
Bounce nextButton = Bounce(NEXTB_PIN, DEBOUNCE_T);
Bounce enterButton = Bounce(ENTRB_PIN, DEBOUNCE_T);
Bounce backButton = Bounce(BACKB_PIN, DEBOUNCE_T);

//LCD display pins setup
LiquidCrystalFast lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

// timers declaration
TimerTask *millis_tmr, *lamps_tmr, *sols_tmr, *snd_tmr;
TimerSet tset;

void setup() {
  
  // Teensy I/O pins setup
  pinsSetup();
  
  // Serial COM init
  Serial.begin(BAUDRATE);
  delay(1000);
  
  // SPI init
  Serial.println(F("Starting SPI..."));
  SPI.begin();
  delay(1000);

  // MCP init
  Serial.println(F("MCP init..."));
  MCP_init(GPIO_SS_PIN);
  delay(1000);
  
  // LED grid init
  Serial.println(F("Led Grid init..."));
  initLedGrid(LG_SS_PIN);
  setLGintens(2);
  delay(1000);

  // API setup
  Serial.println(F("API init..."));
  initAPI();
  delay(1000);
    
  Serial.println(F("OUTPUT tests..."));
  /* while (1) */ _outptest();
  delay(1000);

  // pinball init
  Serial.println(F("Pinball init..."));
  initPinball();
  delay(1000);
  
  // timers setup and function associations
  Serial.println(F("Timers init..."));
  millis_tmr = new TimerTask(millisRoutine, 1);
  lamps_tmr = new TimerTask(checkLights, LMP_TMR_PERIOD);
  sols_tmr = new TimerTask(checkSolenoids, SOL_TMR_PERIOD);
  snd_tmr = new TimerTask(checkSoundCmd, SND_TMR_PERIOD);
  TimerSet tset;
  tset.add(millis_tmr);
  tset.add(lamps_tmr);
  tset.add(sols_tmr);
  tset.add(snd_tmr);
  
  // Go!
  Serial.println(F("Go..."));
  lcd.clear();
  lcdprn("Running...", 0);
  tset.enableAllTimers();
}

// top-most Teensy loop
void loop() {
  tset.checkTimerTasks();
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
  digitalWrite(D_LD1_PIN, HIGH);
  digitalWrite(D_LD2_PIN, HIGH);
  digitalWrite(D_RES_PIN, HIGH);
}

// system routines to call every 1 millisecond
void millisRoutine(uint32_t ms) {
  uint16_t dd;
  bool b;
  
  // SWITCH GRID READING
  getNextReturns(ms);
  
  // DISPLAY UPDATE
  dd = getNextDisplayData();
  writeDisplayData((byte)(dd & 0xff));
  writeDisplayCtrl((byte)((dd & 0x700)>>8));
  delayMicroseconds(10);
  writeDisplayCtrl(0); // resets LD1, LD2

  // Debounced on-board buttons update
  b = nextButton.update();
  if (b && nextButton.fallingEdge()) onPRBButtonPressed(NEXT_BUTT);
  b = enterButton.update();
  if (b && enterButton.fallingEdge()) onPRBButtonPressed(ENTER_BUTT);
  b = backButton.update();
  if (b && backButton.fallingEdge()) onPRBButtonPressed(BACK_BUTT);
}


void lcdprn(const char *st, byte line) {
  lcd.setCursor(0, line);
  lcd.print(st);
}

void lcdprn(String st, byte line) {
  lcd.setCursor(0, line);
  lcd.print(st);
}

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
    setSound(i);
    delay(100);
  }
  setSound(0);
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

