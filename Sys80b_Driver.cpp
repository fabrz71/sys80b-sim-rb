#include "Sys80b_Driver.h"
#include "BoardIO.h"

using namespace BoardIO;

void Driver::onOutputChange(outputType typ, uint16_t state) {
	switch (typ) {
	case STROBES:
		writeStrobes(state);
		break;
	case SOLENOIDS:
		writeSolenoids(state);
		break;
	case SOUND:
		writeSound(state);
		break;
	case LAMPS:
		write4Lamps((byte)(data & 0xf0), (byte)(data & 0x0f));
	case DISPL:

	}

};

byte Driver::onInputRequest(inputType typ) {
	switch (typ) {
	case RETURNS:
	case SLAM_SW:
	}
};