#ifndef _msgOutput_h
#define _msgOutput_h

//#include "Arduino.h"
//#include "lcdOutput.h"

#define OUTPDELAY 10

bool serialOutputEnabled = true;
bool lcdOutputEnabled = false;

//#define lcdprn(s, l) lcd.setCursor(0, l); lcd.print(s)

#define outp(m) if (lcdOutputEnabled) lcd.print(m); \
                if (serialOutputEnabled) Serial.print(m)

#define outpln(m) if (lcdOutputEnabled) lcd.println(m); \
                  if (serialOutputEnabled) Serial.println(m)
                     
#define outp_clr(m) if (lcdOutputEnabled) { lcd.clear(); lcd.print(m); } \
                    if (serialOutputEnabled) Serial.print(m)

#define outpln_clr(m) if (lcdOutputEnabled) { lcd.clear(); lcd.println(m); } \
                      if (serialOutputEnabled) Serial.println(m)
                     
//void outp(const char *txt);
//void outp(const __FlashStringHelper *txt);
//void outp(uint32_t n);
//void outpln(const char *txt);
//void outpln(const __FlashStringHelper *txt);
//void outpln(uint32_t n);
//void lcdprn(char *st, byte line);
//void lcdprn(String st, byte line);
//void teststep(int i);

#endif
