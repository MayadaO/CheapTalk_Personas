#ifndef RL_H
#define RL_H

#include "defs.h"
#include "Player.h"
#include "MarkovGame.h"
#include "GameEngine.h"
#include "State.h"
#include "stageGame.h"

class RL : public Player {
public:
    RL();
    RL(int _me, MarkovGame *_mg, int _tipo);
    ~RL();
    
    
	//void Reset();
	int Move(GameEngine *ge);
	int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);
    int roundUpdate();
    void vIteration(int iter);
    
    MarkovGame *mg;
    int me;
    int tipo;
    int currentState;    
    stageGame **stages;
    
    bool lastValidActs[2][NUMACTIONS*2];
};

#endif