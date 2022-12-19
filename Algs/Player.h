#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "GameEngine.h"
#include "CommAgent.h"
//#include "delayer.h"

class Player {
public:
	Player();
	virtual ~Player();

	virtual void Reset();
    virtual double processCheapTalk(char buf[10000]);
	virtual int Move(GameEngine *ge);//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose);
	virtual int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);//GameEngine *ge, bool _roundDone = false);
    virtual int roundUpdate();
	virtual void print(int config, int x1, int y1, int x2, int y2);
    
    CommAgent *comms;
    
    char nickname[1024];
    //delayer *dlyr;
    
    double none_rate, some_rate;
};

#endif