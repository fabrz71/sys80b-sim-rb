#ifndef _TimerClient_h_
#define _TimerClient_h_

//class TimerTarget {
//public:
//	virtual void timerFunction(uint32_t tm, int id = 0) = 0;
//};

//typedef void (TimerClient::*Fun)(uint32_t); // pointer to arbitrary TimerClient member function

class TimerClient {
public:
	//TimerTarget *target;
	//Fun fun;
	int timerFunction; // optional parameter
	uint32_t start_time; // when timer has enabled
	uint32_t period; // time after activation before trigger
	bool oneShot; // not periodic
	const char *tag; // optional tag
	TimerClient *next; // optional next timer in a ordered setPeriod
protected:
	bool _enabled;

public:
	TimerClient();
	//TimerClient(int function);
	//TimerClient(uint32_t ms, bool periodic);
	//TimerClient(uint32_t ms, const char *name, bool enable);
	//void setTarget(TimerTarget *tt);
	void set(uint32_t ms, const char *name, bool enable);
	void setPeriod(uint32_t ms, bool periodic);
	//void setFunctionId(int fid);
	void enable();
	void disable();
	bool isEnabled();
	uint32_t check(uint32_t chkTime);
	uint32_t getRemainingTime(uint32_t chkTime = 0);
	void print();
	// function called by a Timer object 
	// it have to be overridden by any subclass
	virtual void timerFunction(uint32_t tm, int id = 0) = 0;
};

#endif