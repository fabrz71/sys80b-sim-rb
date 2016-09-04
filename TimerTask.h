#ifndef TimerTask_h
#define TimerTask_h 
#define MAX_TASKS 20

typedef void (*func_t)(uint32_t);

class TimerSet;
class TimerTask;

class TimerTask {
  public:
    func_t funct; // function to execute when timer ticks
    uint32_t start_time; // when timer has enabled
    uint32_t period;
    bool oneShot; // not periodic
    bool enabled;
    char *tag; // optional tag
    TimerTask *next; // optional other related timer
    
    TimerTask(func_t sub, uint32_t ms, bool periodic);
    inline void set(uint32_t ms, bool periodic) { period = ms; oneShot = !periodic; }
    inline void enable() { start_time = millis(); enabled = true; }
    inline void disable() { enabled = false; }
    void check(uint32_t chkTime);
};

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
    void checkTimerTasks();
    inline byte getLength() { return t_counter; }
    void enableAllTimers();
    void disableAllTimers();
};

TimerTask::TimerTask(func_t sub, uint32_t ms, bool periodic = true) {
  funct = sub;
  period = ms;
  oneShot = !periodic;
  enabled = false;
  next = NULL;
}

/**
TimerTask::~TimerTask() {
  enabled = false;
  //tset.remove(this);
}
**/

void TimerTask::check(uint32_t chkTime = 0) {
  uint32_t elapsed;

  if (!enabled) return;
  if (chkTime == 0) chkTime = millis();
  if (chkTime < start_time) elapsed = ~start_time + chkTime + 1;
  else elapsed = chkTime - start_time;
  if (elapsed >= period) {
    funct(chkTime);
    if (oneShot) enabled = false;
    else start_time = chkTime;
  }
}

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

void TimerSet::checkTimerTasks() {
  TimerTask *tt;
  
  if (t_counter == 0) return;
  chkTime = millis();
  for (tt = first_timer; tt != NULL; tt = tt->next) tt->check(chkTime);
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

#endif // TimerTask_h

/** PREVIOUS  VERSION **
#ifndef timerTask_h
#define timerTask_h 

#define MAX_TASKS 20

typedef void (*func_t)(uint32_t);

struct timert {
  byte id;
  func_t funct;
  uint32_t start_time;
  uint32_t period;
  bool oneShot;
  bool disabled;
  String tag;
  timert *next;
};

timert *first_timer;
timert *last_timer;
byte t_counter = 0;
uint32_t chkTime;

byte newTimerTask(func_t sub, uint32_t ms);
byte newTimerTask(func_t sub, uint32_t ms, bool periodic);
bool startTimerTask(byte id);
bool stopTimerTask(byte id);
bool setTimerPeriod(byte id, uint32_t ms);
bool setTimerPeriod(byte id, uint32_t ms, bool periodic);
void checkTimerTasks();
timert *getTimerTask(byte tid);

byte newTimerTask(func_t sub, uint32_t ms) {
  return newTimerTask(sub, ms, true);
}

byte newTimerTask(func_t sub, uint32_t ms, bool periodic) {
  timert *tt;
  
  if (t_counter >= MAX_TASKS-1) return 255;
  tt = (timert*) malloc(sizeof(timert));
  if (t_counter == 0) first_timer = tt;
  tt->id = t_counter++;
  tt->funct = sub;
  tt->period = ms;
  tt->oneShot = !periodic;
  tt->disabled = true;
  tt->next = NULL;
  if (last_timer != NULL) last_timer->next = tt;
  last_timer = tt;
  tt->start_time = millis();
  return tt->id;
}

bool deleteTimerTask(byte tid) {
  bool res;
  timert *prevtt, *tt;

  if (t_counter == 0) return false;
  prevtt = NULL;
  tt = first_timer;
  while(tt != NULL) {
    if (tt->id == tid) break;
    else prevtt = tt;
    tt = tt->next;
  }
  if (tt == NULL) return false;
  if (prevtt == NULL) first_timer = tt->next;
  else prevtt->next = tt->next;
  tt->disabled = true;
  delete(tt);
  t_counter--;
  return true;
}

bool startTimerTask(byte id) {
  timert *tt;
  tt = getTimerTask(id);
  if (tt == NULL) return false;
  tt->disabled = false;
  tt->start_time = millis();
  return true;
}

bool stopTimerTask(byte id) {
  timert *tt;
  tt = getTimerTask(id);
  if (tt == NULL) return false;
  tt->disabled = true;
  return true;
}

void checkTimerTasks() {
  timert *tt;
  uint32_t elapsed;
  
  if (t_counter == 0) return;
  tt = NULL;
  chkTime = millis();
  do {
    if (tt == NULL) tt = first_timer;
    else tt = tt->next;
    if (chkTime < tt->start_time) elapsed = ~(tt->start_time) + chkTime + 1;
    else elapsed = chkTime - tt->start_time;
    if ( !(tt->disabled) && (elapsed >= tt->period) ) {
      (tt->funct)(chkTime);
      if (tt->oneShot) tt->disabled = true;
    }
  } while (tt != last_timer);
}

bool setTimerPeriod(byte id, uint32_t ms) {
  timert *tt;
  tt = getTimerTask(id);
  if (tt == NULL) return false;
  tt->period = ms;
  tt->oneShot = false;
  return true;
}

bool setTimerPeriod(byte id, uint32_t ms, bool periodic) {
  if (!setTimerPeriod(id, ms)) return false;
  getTimerTask(id)->oneShot = !periodic;
  return true;
}

timert *getTimerTask(byte tid) {
  timert *tt;
  if (t_counter == 0) return NULL;
  for (tt = first_timer; tt != NULL; tt = tt->next) if (tt->id == tid) break;
  return tt;
}

#endif

**/

