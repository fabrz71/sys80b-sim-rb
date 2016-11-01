#ifndef MsgOutput_h
#define MsgOutput_h

#define OUTPDELAY 10

//#include <LiquidCrystalFast.h> // Teensy 3.x optimized library for LCD
#include <LiquidCrystal.h>

//#define lcdprn(s, l) lcd.setCursor(0, l); lcd.print(s)

#define outp(m) if (lcdOutputEnabled) lcd.print(m); \
                if (serialOutputEnabled) Serial.print(m)

#define outpln(m) if (lcdOutputEnabled) lcd.println(m); \
                  if (serialOutputEnabled) Serial.println(m)
                     
#define outp_clr(m) if (lcdOutputEnabled) { lcd.clear(); lcd.print(m); } \
                    if (serialOutputEnabled) Serial.print(m)

#define outpln_clr(m) if (lcdOutputEnabled) { lcd.clear(); lcd.println(m); } \
                      if (serialOutputEnabled) Serial.println(m)
                     
extern const byte LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN, LCD_RW_PIN, LCD_RS_PIN, LCD_EN_PIN; // pins definition

//LiquidCrystalFast lcd(LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

bool lcdOutputEnabled = false;
bool serialOutputEnabled = true;
//byte msgOutputLevel = 0;

/*
void outp(const char *txt);
void outp(const __FlashStringHelper *txt);
void outp(uint32_t n);
void outpln(const char *txt);
void outpln(const __FlashStringHelper *txt);
void outpln(uint32_t n);
void lcdprn(char *st, byte line);
void lcdprn(String st, byte line);
//void teststep(int i);

void outp(const char *txt) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcdprn(txt, 0);
  }
  if (serialOutputEnabled) Serial.print(txt);
  delay(OUTPDELAY);
}

void outp(const __FlashStringHelper *txt) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcdprn(txt, 0);
  }
  if (serialOutputEnabled) Serial.print(txt);
  delay(OUTPDELAY);
}

void outp(uint32_t n) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(n);
  }
  if (serialOutputEnabled) Serial.print(n);
  delay(OUTPDELAY);
}

void outpln(const char *txt) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcdprn(txt, 0);
  }
  if (serialOutputEnabled) Serial.println(txt);
  delay(OUTPDELAY);
}

void outpln(const __FlashStringHelper *txt) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcdprn(txt, 0);
    delay(OUTPDELAY);
  }
  if (serialOutputEnabled) Serial.println(txt);
}

void outpln(uint32_t n) {
  if (lcdOutputEnabled) {
    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(n);
  }
  if (serialOutputEnabled) Serial.println(n);
  delay(OUTPDELAY);
}

void lcdprn(const char *st, byte line) {
  lcd.setCursor(0, line);
  lcd.print(st);
}

void lcdprn(String st, byte line) {
  lcd.setCursor(0, line);
  lcd.print(st);
}
*/

/*
void teststep(int i) {
  Serial.print(F("test point: "));
  Serial.println(i);
  lcd.setCursor(0, 1);
  lcd.print(i);
  delay(OUTPDELAY);
}
*/

#endif
