#ifndef __Solver__MarkovGame__
#define __Solver__MarkovGame__

#include "defs.h"
#include "State.h"

#define mSTATES         100000
#define mSTART_STATES   10
#define mGOAL_STATES    100000

class MarkovGame {
public:
    MarkovGame();
    MarkovGame(char *fname);
    ~MarkovGame();
    
    void loadMG(char *fname);
    void writeGame(char *filename);
    State *getState(int _numFeatures, double *_featureVector, int _numActions[2]);
    State *stateDefined(double *_featureVector);
    void addStartState(State *s);
    void addGoalState(State *s);
    
    void vIteration(double w1, double w2);
    void solveZero(int ind);
    void solveAttack(int ind);
    void computeBR(int ind, bool first);
    void computeBRu(int ind, bool first, int iter);
    double computeBestDefection(int sol, int me);
    void computeDefectBR_VI(int sol, int me);
    void computeReal(bool first);
    void computePrevent(int _iter);//int ind, double myWeight);
    //double preventerUtility1(double v[2]);
    //double preventerUtility2(double v[2]);
    //void computeBR_SARSA(int ind);
    
    int numStates, numStartStates, numGoalStates;
    State **states, **startStates, **goalStates;
    
    double wvals[3];
    
    char actionLabels[10][1024];
    int numLabels;
    char actionDescription[1024];
};



#endif
