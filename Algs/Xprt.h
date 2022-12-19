#ifndef XPRT_H
#define XPRT_H

#include "defs.h"
#include "Player.h"
#include "MarkovGame.h"
#include "mega.h"
#include "State.h"
#include "GridWarEngine.h"
#include "LEGOEngine.h"

class Xprt : public Player {
public:
	Xprt();
	Xprt(int _me, MarkovGame *_mg, char tipito[1024], bool _utilitarian = false);
	~Xprt();
	
	int Move(GameEngine *ge);
    int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);
    int roundUpdate();
	
	int me;
    int expertID;
    mega *ab;
    
    State *lastState;
    bool lastValidActs[2][NUMACTIONS*2];
    
    double rPayout[2], previousPayoffs[2];
    
    int corA;
};


#endif