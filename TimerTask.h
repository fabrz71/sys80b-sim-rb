/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMER TASK OBJECT DEFINITION
* ---
* "Timertask" helps to manage different tasks to be executed periodically
* from main execution cycle.
* Contains C++ code.
*/

#ifndef TimerTask_h
#define TimerTask_h

#define MAX_TASKS 20

typedef void (*func_t)(uint32_t);

class TimerSet;
//class TimerTask;

class TimerTask {
  public:
    func_t funct; // function to execute when timer triggers
    uint32_t start_time; // when timer has enabled
    uint32_t period; // time after activation before trigger
    bool oneShot; // not periodic
    bool enabled;
    char *tag; // optional tag
    TimerTask *next; // optional other related timer

    TimerTask(func_t sub, uint32_t ms, bool periodic);
    inline void set(uint32_t ms, bool periodic) { period = ms; oneShot = !periodic; }
    inline void enable() { start_time = millis(); enabled = true; }
    inline void disable() { enabled = false; }
    uint32_t check(uint32_t chkTime);
    uint32_t getRemainingTime(uint32_t chkTime = 0);
};

TimerTask::TimerTask(func_t sub, uint32_t ms, bool periodic = true) {
  funct = sub;
  period = ms;
  oneShot = !periodic;
  enabled = false;
  next = NULL;
}

// returns: remaining time before trigger [ms] > 0; 0 if not enabled
uint32_t TimerTask::check(uint32_t chkTime = 0) {
  uint32_t elapsed;

  if (!enabled) return 0;
  if (chkTime == 0) chkTime = millis();
  if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
  else elapsed = chkTime - start_time;
  if (elapsed >= period) {
    funct(chkTime);
    if (oneShot) {
      enabled = false;
      return 0;
    }
    start_time = chkTime;
    return period;
  }
  return period - elapsed;
}

uint32_t TimerTask::getRemainingTime(uint32_t chkTime) {
  uint32_t elapsed;
    
  if (!enabled) return 0;
  if (chkTime == 0) chkTime = millis();
  if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
  else elapsed = chkTime - start_time;
  if (elapsed >= period) return 0;
  return period - elapsed;
}

class TimerSet {
  protected:
    TimerTask *first_timer;
    TimerTask *last_timer;
    byte t_counter = 0;
    uint32_t chkTime;
  public:
    bool add(TimerTask *tt);
    bool remove(TimerTask *tt);
    bool contains(TimerTask *tt);
    uint32_t checkTimerTasks();
    inline byte getLength() { return t_counter; }
    void enableAllTimers();
    void disableAllTimers();
    //uint32_t getNextEventRemainingTime();
};

bool TimerSet::add(TimerTask *tt) {
  if (t_counter >= MAX_TASKS-1) return false;
  if (t_counter++ == 0) first_timer = tt;
  if (last_timer != NULL) last_timer->next = tt;
  last_timer = tt;
  tt->next = NULL;
  return true;
}

bool TimerSet::remove(TimerTask *ttask) {
  TimerTask *prevtt, *tt;

  if (t_counter == 0) return false; // empty list
  prevtt = NULL;
  tt = first_timer;
  while(tt != NULL) {
    if (tt == ttask) break;
    else prevtt = tt;
    tt = tt->next;
  }
  if (tt == NULL) return false;// not in list!
  if (prevtt == NULL) first_timer = tt->next; // first in list
  else prevtt->next = tt->next; // could be NULL
  if (tt->next == NULL) last_timer = prevtt; // last in list
  tt->enabled = false; // disable removed TimerTask
  t_counter--;
  return true;
}

bool TimerSet::contains(TimerTask *ttask) {
  TimerTask *tt;

  if (t_counter == 0) return false; // empty list
  tt = first_timer;
  while(tt != NULL) {
    if (tt == ttask) return true;
    tt = tt->next;
  }
  return false;
}

uint32_t TimerSet::checkTimerTasks() {
  TimerTask *tt;
  uint32_t rt, nextEventT;
  
  nextEventT = 60000;
  if (t_counter == 0) return 0;
  chkTime = millis();
  for (tt = first_timer; tt != NULL; tt = tt->next) {
    rt = tt->check(chkTime);
    if (rt < nextEventT) nextEventT = rt;
  }
  return nextEventT;
}

void TimerSet::enableAllTimers() {
  TimerTask *tt;

  if (t_counter == 0) return;
  for (tt = first_timer; tt != NULL; tt = tt->next) tt->enable();
}

void TimerSet::disableAllTimers() {
  TimerTask *tt;

  if (t_counter == 0) return;
  for (tt = first_timer; tt != NULL; tt = tt->next) tt->disable();
}

/*
uint32_t TimerSet::getNextEventRemainingTime() {
  TimerTask *tt;
  uint32_t now, dt, smallest;
  
  if (t_counter == 0) return 1000;
  now = millis();
  for (tt = first_timer; tt != NULL; tt = tt->next) {
    dt = tt->getRemainingTime(now);
    if (tt == first_timer || dt < smallest) smallest = dt;
  }
  return smallest;
}
*/

#endif // TimerTask_h

