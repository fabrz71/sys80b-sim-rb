#ifndef _SYS80_B_DRIVER_
#define _SYS80_B_DRIVER_

class Sys80b;

namespace Driver {
	enum inputType { RETURNS, SLAM_SW };
	enum outputType { STROBES, SOLENOIDS, SOUND, LAMPS, DISPL };

	Sys80b *connectedSystem = NULL;

	void onOutputChange(outputType typ, uint16_t state);
	byte onInputRequest(inputType typ);
}

#endif