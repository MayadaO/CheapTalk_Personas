#ifndef __Solver__gameWriter__
#define __Solver__gameWriter__

#include "defs.h"
#include "MarkovGame.h"
#include "GameEngine.h"
#include "MazeEngine.h"
#include "State.h"

class gameWriter {
public:
    gameWriter();
    gameWriter(char *filename);
    ~gameWriter();
    
    void bruteGame();
    int keyboardInput(int index);
    State *getState(MarkovGame *mg, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]);
    void getStateFeatureVector(double fvector[7]);
    void jointExplore(State *cState, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]);
 
    //void printScenarios(int iter, char ****Configurations);
    //void printScenarios(int iter);

    
    MazeEngine *ge;

    char fnombrito[1024];
};

#endif
