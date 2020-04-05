/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
*/

#ifndef _TimerTask_h_
#define _TimerTask_h_

#include "Arduino.h"

// Abstract class to be implemented by any
// object with a timer task function implementation.
class TimerTaskExecutor {
public:
    //TimerTaskExecutor() {
    //    Serial.println("TimerTaskExecutor init...");
    //    delay(100);
    //};
    virtual ~TimerTaskExecutor() {};
    virtual void timerRoutine(int taskId, uint32_t& ms) = 0;
};

// TODO: rivedere descrizione
// A TimerTask is an arbitrary function implementation that takes a unit32_t
// integer as unique parameter, which should be the current milliseconds
// time as from millis() Arduino function.
// Create a new instance for each task and add it to a TimerSet instance
// in order to get it scheduled for execution.
class TimerTask {
public:
    int id;
    TimerTaskExecutor *execObj;
    //void* auxObj;
    bool oneShot; // not periodic
    const char* tag; // optional name
    TimerTask* _next; // optional next timer in a ordered set
private:
    uint32_t _startTime; // when timer has enabled
    uint32_t _period; // time after activation before trigger
    bool _enabled;

public:
    TimerTask();
    TimerTask(TimerTaskExecutor* obj, uint32_t ms, bool periodic = true, bool en = false);
    TimerTask(TimerTaskExecutor* obj, uint32_t ms, const char* name, bool en = false);
    void setPeriod(uint32_t ms);
	void setPeriod(uint32_t ms, bool periodic);
    void setPeriodic(bool periodic);
	void enable();
	void disable();
    bool isEnabled();
    uint32_t update(uint32_t& chkTime);
    uint32_t getRemainingTime(uint32_t chkTime = 0);
    void print();

};
#endif
