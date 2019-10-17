#ifndef _Player_h_
#define _Player_h_

//#include "Arduino.h"

// player game status
class Player
{
public:
	Player();
		
	uint32_t score; // total play score
	uint32_t bonus; // ball bonus score
	byte ballOnPlay;
	byte totalGameBalls; // balls of player game
							//byte ballsInHole;
	byte awardLevelReached;
	bool topScorer;
	bool gameOver;
};

#endif
