#ifndef __Solver__Preventer__
#define __Solver__Preventer__

#include "defs.h"
#include "Player.h"
#include "MarkovGame.h"
#include "GameEngine.h"
#include "State.h"

class Preventer : public Player {
public:
    Preventer();
    Preventer(int _me, MarkovGame *_mg, double _myWeight, bool _originalMG = true);
    ~Preventer();
    
	int Move(GameEngine *ge);//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose);
	int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);//GameEngine *ge, bool _roundDone = false);
    int selectAction(State *s);
    int roundUpdate();
    
    int mover(State *s);
    int moverUpdate(int acts[2], double dollars[2]);
    int rounderUpdate();
    
    void simulateVsMBRL(int ind, int rounds);
    bool isGoalState(int s);
    
    //double getPreventativeValue(double *probs, int ind, int a);
    void adjustPriors();
    int getBestResponse(State *s, double probs[NUMACTIONS*2]);
    void initQreals();
    
    int me;
    MarkovGame *mg;
    double rPayouts[2];
    double myWeight;
    
    bool lastValidActs[2][NUMACTIONS*2];
    State *currentState;
    
    bool originalMG;
};

#endif /* defined(__Solver__Preventer__) */
