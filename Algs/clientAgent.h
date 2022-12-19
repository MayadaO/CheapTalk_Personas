#ifndef __Solver__clientAgent__
#define __Solver__clientAgent__

#include "defs.h"
#include "Player.h"
#include "MySocket.h"

class clientAgent : public Player {
public:
    clientAgent();
    clientAgent(int _me);
    ~clientAgent();
    
    void createConnection();
    
    int Move(GameEngine *ge);//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose);
	int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);//GameEngine *ge, bool _roundDone = false);
    int roundUpdate();
    
    char nombre[1024];
    char nickname[1024];
    int me;
    
    MySocket *ss;
};

#endif /* defined(__Solver__clientAgent__) */
