#ifndef _Solenoid_h_
#define _Solenoid_h_

class Solenoid
{
	protected:
		bool active;
		uint32_t setTime; // process time when solenoid has been setPeriod
		uint32_t swOnTime; // switch-on event time [ms]
		uint32_t swOffTime; // switch-off event time [ms]
		uint16_t activePeriod; // standard active state period, infinite when = 0 [ms]
		uint16_t maxActivePeriod; // maximum safe active period, 0 = not setPeriod [ms]
		//uint16_t restPeriod; // rest period after max active period [ms]
		uint16_t swOnDelay; // switch delay to new state, ignored when = 0 [ms]

	public:
		bool effective; // rendering flag

		Solenoid();
		Solenoid(uint16_t maxActivePeriod);

		bool isActive();
		void set(bool state);
		void activate(uint16_t actPeriod);
		void activateDelayed(uint16_t swDelay);
		void activateDelayed(uint16_t actPeriod, uint16_t swDelay);
		bool check(uint32_t t);
};

#endif
