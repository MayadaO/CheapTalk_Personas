#ifndef HUMAN_H
#define HUMAN_H

#include "defs.h"
#include "Player.h"
#include "GameEngine.h"
#include "MazeEngine.h"
#include "LEGOEngine.h"

class Human : public Player {
public:
	Human();
	Human(int _me, bool _thePD);
	~Human();
	
	int Move(GameEngine *ge);//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose);
	int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);//GameEngine *ge, bool _roundDone = false);
    int roundUpdate();

	int convertIt(int accion, bool validActions[NUMACTIONS]);
    int convertIt2(int accion, bool validActions[2*NUMACTIONS]);
	
	int me;
    bool thePD;
};


#endif