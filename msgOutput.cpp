//#include <LiquidCrystalFast.h>

//#include "ioPins.h"
//#include "msgOutput.h"

//LiquidCrystalFast lcd(LCD_RS_PIN, LCD_RW_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
//LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
//bool serialOutputEnabled = true;
//byte msgOutputLevel = 0

/*
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