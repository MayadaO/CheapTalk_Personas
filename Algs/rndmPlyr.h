#ifndef __Solver__rndmPlyr__
#define __Solver__rndmPlyr__

#include "defs.h"
#include "Player.h"
#include "LEGOEngine.h"

class rndmPlyr : public Player {
public:
    rndmPlyr();
    rndmPlyr(int _me);
    ~rndmPlyr();

    int Move(GameEngine *ge);
	int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);//GameEngine *ge, bool _roundDone = false);
    int roundUpdate();
    
	int convertIt(int accion, bool validActions[NUMACTIONS]);
    
    int me;
};

#endif /* defined(__Solver__rndmPlyr__) */
