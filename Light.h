/* high-level lights API
* --
* Contains "lights" definition and functions. Lights
* are an abstraction of pinball real hardware "lamps".
* In order to make a light blink of pulse you have to
* call steadily the function update() with a convenient
* period.
*/

#define DEF_BLNK_PERIOD 200 // default light blink rate
#define DEF_PULSE_PERIOD 200 // default light pulse rate
#define LSTAGE_SIZE 48 // stage total lights count

// defines different possible light states
enum lightState { 
  OFF_L,    // light off
  ON_L,     // light on
  BLINK_L,  // alternate on/off ligth state (with duty cycle)
  PULSE_L   // temporary light state inversion -> (then) ON or OFF
  };

// software mid-level light object, with its own state
// regular periodic calls to update() function is required for
// not-constant states (other than OFF_L and ON_L).
class Light {
  public:
    bool on; // true when light switched on
    lightState state; // defines light behaviour along time
    uint16_t blinkPeriod; // ON+OFF time (ms) when in BLINK_L state
    byte blinkDutyCycle; // 0-100 % ON time when in BLINK_L state
    uint32_t activationTime; // time when lamp has activated (from OFF to another state)
    uint16_t activePeriod; // time limit for any state different than OFF_L (ms)
        // (0 = unlimited time)
    
    Light();
    void copy(Light &from);
    void reset();
    inline void set(lightState st);
    inline void blink(lightState st, uint16_t blinkP);
    void blink(lightState st, uint16_t blinkP, byte blinkDutyC);
    void invert();
    void pulse(uint16_t pulseP);
    void blink(uint16_t blinkP, byte ticks);
    void update(uint32_t ms);
    inline bool isActive() { return (state != OFF_L); }
};

// constructor with default property values
Light::Light() { 
  on = false;
  state = OFF_L;
  blinkPeriod = 500;
  blinkDutyCycle = 50; 
  activePeriod = 0; 
}

// convenient funcion for copying a Light state into another
void Light::copy(Light &from) {
  this->on = from.on;
  this->state = from.state;
  //printf("%d %d\n", blinkPeriod, from.blinkPeriod);
  this->blinkPeriod = from.blinkPeriod;
  this->blinkDutyCycle = from.blinkDutyCycle;
  this->activationTime = from.activationTime;
  this->activePeriod = from.activePeriod;
}

// switch off light and reset its properties
void Light::reset() {
    on = false;
    state = OFF_L;
    blinkPeriod = 500; // ON+OFF (ms)
    blinkDutyCycle = 50; // 0-100 % ON time
    activePeriod = 0; // (ms)
}

// define the state of the light
inline void Light::set(lightState st) {
  set(st, (st == BLINK_L) ? DEF_BLNK_PERIOD : 0, 50);
}

// define the state of the light
// st: light state
// blinkP: blink period (on + off) [milliseconds]
inline void Light::blink(lightState st, uint16_t blinkP) {
  set(st, blinkP, 50);
}

void Light::blink(lightState st, uint16_t blinkP, byte blinkDutyC) {
  switch (st) {
    case OFF_L:
      on = false;
      break;
    case ON_L:
      on = true;
      break;
    case BLINK_L:
      if (blinkP == 0) state = ON_L;
      on = true;
      break;
    case PULSE_L:
      on = !on;
      break;
    default:
      return;
  }
  state = st;
  if (st != OFF_L) activationTime = millis();
  blinkPeriod = blinkP;
  if (blinkDutyC == 0) blinkDutyCycle = 50; 
  else blinkDutyCycle = blinkDutyC;
  activePeriod = 0; // unlimited state time (default)
}

// inverts light ON/OFF status
void Light::invert() {
  if (state == ON_L) state = OFF_L;
  else if (state == OFF_L) state = ON_L;
}

void Light::pulse(uint16_t pulseP) {
  set(PULSE_L, (pulseP == 0) ? DEF_PULSE_PERIOD : pulseP);
}

void Light::blink(uint16_t blinkP, byte ticks) {
  set(BLINK_L, blinkP, 50);
  activePeriod = blinkP * ticks;
}

void Light::update(uint32_t ms) {
  uint32_t bt; // blink ON time
    
  // checks light active time
  if (state != OFF_L) {
    if (activePeriod > 0 && 
      (ms - activationTime) >= (uint32_t)activePeriod) {
      activePeriod = 0; // resets previous active-time setting
      state = OFF_L;
    }
  }
  
  // implements special light modes
  if (state == BLINK_L) {
    bt = (ms - activationTime) % (uint32_t)blinkPeriod;
    on = ((bt*100 / blinkPeriod) < blinkDutyCycle);
  }
  else if (state == PULSE_L) {
    if ((ms - activationTime) >= blinkPeriod) {
      on = !on;
      if (state) state = ON_L; else state = OFF_L; // auto state switch
    }
  }
}

