/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* LIGHT OBJECT
* ---
* "Lights" are an abstraction of pinball real hardware "lamps"
* with their own state: on, off or blinking (see: enum lightState)
* In order to make a light blink of pulse you have to
* steadily call update() function with a convenient time interval.
*/

#ifndef _Light_h_
#define _Light_h_

#include "Arduino.h"

//#define DEF_BLNK_PERIOD 200 // default light blink rate
//#define DEF_PULSE_PERIOD 200 // default light pulse rate
//#define LSTAGE_SIZE 48 // stage total lights _size

// defines different possible light states
enum lightState {
  OFF_L,    // light off
  ON_L,     // light on
  BLINK_L,  // alternate on/off ligth state (with duty cycle)
};

// software mid-level light object, with its own state
// regular periodic calls to update() function is required for
// not-constant states (other than OFF_L and ON_L).
class Light {
  public:
	  static const uint32_t DEF_BLNK_PERIOD = 200; // default light blink rate
	  static const uint32_t DEF_PULSE_PERIOD = 200; // default light pulse rate
	  static const uint32_t LSTAGE_SIZE = 48; // stage total lights _size
    bool on; // true when light switched on
    lightState state; // defines light behaviour along time
    uint16_t blinkPeriod; // ON+STATIC_TEXT time (ms) when in BLINK_L state
    byte blinkDutyCycle; // 0-100 % ON time when in BLINK_L state
    uint32_t activationTime; // time when lamp has activated (from STATIC_TEXT to another state)
    uint16_t activePeriod; // time limit for any state different than OFF_L (ms)
        // (0 = unlimited time)
    bool impulse; // temporary inverted on/off state
    uint32_t pulseTime; // time when lamp pulse has activated
    uint16_t pulsePeriod; // pulse state duration
    //byte brightness;

//public:
    Light();
    void copy(Light &from);
	void copy(Light *from);
    void reset();

    // define the state of the light
    inline void set(bool st) { set(st ? ON_L : OFF_L); }
    void blink(uint16_t blinkP = DEF_BLNK_PERIOD, byte blinkDutyC = 50, byte ticks = 0);
    void invert();
    void pulse(uint16_t pulseP = DEF_PULSE_PERIOD);
    bool update(uint32_t ms);
    inline bool isActive() const { return (state != OFF_L); }
    void set(lightState st, uint16_t blinkP = DEF_BLNK_PERIOD, byte blinkDutyC = 50, uint16_t activeT = 0);
};

#endif
