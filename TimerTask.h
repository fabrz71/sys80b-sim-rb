/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* TIMER TASK OBJECT DEFINITION
* ---
* "TimerTask" helps to manage different tasks to be executed periodically
* from main execution cycle.
* A task is an arbitrary function implementation that takes a unit32_t
* integer as unique parameter, which should be the current milliseconds
* time as from millis() Arduino function.
* Create a new instance for each task and add it to a TimerSet 
* in order to get it scheduled for execution.
*/

#ifndef _TimerTask_h_
#define _TimerTask_h_

//#include "Arduino.h"

typedef void (*func_t)(uint32_t);

class TimerTask {
public:
    func_t funct; // (member) function to execute when timer triggers
	//void *context; // pointer to relative object instance
    uint32_t start_time; // when timer has enabled
    uint32_t period; // time after activation before trigger
    bool oneShot; // not periodic
    bool enabled;
    const char *tag; // optional tag
//protected:
    TimerTask *_next; // optional next timer in a ordered setPeriod

public:
	TimerTask();
    TimerTask(func_t sub, uint32_t ms, bool periodic);
    TimerTask(func_t sub, uint32_t ms, const char *name);
	void setPeriod(uint32_t ms, bool periodic);
	void setFunction(func_t sub);
	void enable();
	void disable();
    uint32_t check(uint32_t chkTime);
	uint32_t check(void *context, uint32_t chkTime);
    uint32_t getRemainingTime(uint32_t chkTime = 0);
    void print();
};

#endif