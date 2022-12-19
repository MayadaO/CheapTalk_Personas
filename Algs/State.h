#ifndef __Solver__State__
#define __Solver__State__

#include "defs.h"
#include "minimaxLog.h"
#include "Qset.h"

#define mTRANS  10
//#define nRBINS  4


class State {
public:
    State();
    State(int _ID, int _numFeatures, double *_featureVector, int _numActions[2], double wvals[3]);
    ~State();
    
    void addTransition(State *_sprime, int _actions[2], double _rewards[2]);
    
    int jointCode(int _actions[2]);
    int jointCode(int a0, int a1);
    bool sameState(double *_fvector);
    
    void printFeatureVector();
    
    int numFeatures;
    int ID;
    double *featureVector;
    
    // properties of the world
    int numActions[2], numJointActions;
    int *numTrans;
    State ***nextState;
    double **transProb;
    int **transCount;
    double ***rewards;
    
    // joint-action value functions
    int numQsets;
    Qset **qsets;
    
    // best response
    double **cuentas;
    double *Qbr;
    double Vbr;//, fV;
    double Vsarsa;
    
    double ***Qreal;
    double Vreal[2];
    double **cuentas_real;

    //double **cuentasLong[nRBINS][nRBINS][nRBINS][nRBINS];
    //double *QbrLong[nRBINS][nRBINS][nRBINS][nRBINS];
    //double VbrLong[nRBINS][nRBINS][nRBINS][nRBINS];
    
    int findQset(double _w);
    void setV(int qst);
    double maxM(int ind, int qst);
    double actualM(int ind);
    double maxM(int ind);
    void getProbs_real(double prob[NUMACTIONS*2], int ind);
    void getProbs_regular(double prob[NUMACTIONS*2], int ind);
    
    // maximin stuff
    minimaxLog **mm;
    minimaxLog **attack;
    
    int added;
};


#endif