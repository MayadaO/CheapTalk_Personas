#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "defs.h"
#include "State.h"
#include "MarkovGame.h"

class GameEngine {
public:
    GameEngine();
	virtual ~GameEngine();
    
    virtual State *getState(MarkovGame *mg, bool *validActs1, bool *validActs2);
	virtual void MoveAgents();
	virtual void initRound();
    
	virtual bool OnGoal(int index);
	virtual void getValidActions(bool *validActs, int index);
	virtual double getCurrentReward(int index);
    
	virtual int convertIt(int accion, bool *validActions, int numActs);
    virtual void deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]);
    
    
	int numAgents;
    
	int *actions;
	int **Pos;
	double *rewardMove;
    
	bool oneGoalNeeded;
};

#endif
