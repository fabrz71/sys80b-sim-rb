#ifndef _GameTimerTask_h_
#define _GameTimerTask_h_

#include "Arduino.h"

class Game;

typedef void (Game::*GameFunct)(uint32_t&);

class GameTimerTask {
public:
    bool oneShot; // not periodic
    const char* name; // optional name
    GameTimerTask* _next; // optional next timer in a ordered set
private:
    uint32_t _startTime; // when timer has enabled
    uint32_t _period; // time after activation before trigger
    bool _enabled;
    Game* _game;
    GameFunct _gameFunct;
    //void* _fun;

public:
    GameTimerTask();
    GameTimerTask(Game* game, GameFunct fn, uint32_t ms, bool periodic = false, bool en = false);
    GameTimerTask(Game* game, GameFunct fn, uint32_t ms, const char* name, bool en = false);
    void setPeriod(uint32_t ms, bool periodic);
    void enable();
    void disable();
    bool isEnabled();
    uint32_t update(uint32_t& chkTime);
    uint32_t getRemainingTime(uint32_t chkTime = 0);
    void print();

};

#endif
