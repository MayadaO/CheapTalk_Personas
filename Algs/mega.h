#ifndef __Solver__mega__
#define __Solver__mega__

#include "defs.h"
#include "MarkovGame.h"
#include "Expert.h"
#include "Solution.h"
#include "CommAgent.h"

#define MAX_EXPERTS 100

#define UNRECOG        0

class mega {
public:
    mega();
    mega(MarkovGame *_mg, int _me, bool _explorar, CommAgent *_comms, double _lambda = 0.99, bool _utilitarian = false);
    ~mega();
    
    void computeAllExperts(double _lambda, bool _explorar, bool fullCycle);
    void computeMaxminExpert(double _lambda);
    void computeMBRLExpert(double _lambda, bool _explorar);
    void computeFolkEgalExpert(double _lambda, bool lead);
    void computeBullyExpert(double _lambda, bool opposite);
    
    void getMax(State *s, double w[2], double R[2]);
    bool isUnique(int ind, int numUnique, int *uniqueSolutions);
    bool isUnique2(int ind, int numUnique, int *uniqueSolutions);
    bool sameActionSequence(int e1, int e2);
    
    void moveUpdate(State *s, State *sprime, int acts[2], int experto, double rPayout[2], double aspiration, int corA);
    void roundUpdate(double rPayout[2]);
    
    bool repeatedVisit();
    void reset();
    
    int selectSolutions(int maxSols, Solution *sols[MAX_EXPERTS], int numSols, double mv[2]);
    void findHighFair(double barR[2], char gname[1024]);
    bool isParetoDominated(int index, Solution *sols[MAX_EXPERTS], int numSols);
    double distance(int index, Solution *sols[MAX_EXPERTS], int numSols);
    double dist(double P1[2], double P2[2]);
    
    void determineRewardBins(double separator[2][3], Solution *sols[MAX_EXPERTS], int numPureSols);
    
    void adjustBRPriors();
    
    MarkovGame *mg;
    int me;
    
    int numExperts;
    Expert **xprts;
    
    int numStateHistory, numStateHistoryPrev;
    int stateHistory[MAX_MOVES_PER_ROUND], stateHistoryPrev[MAX_MOVES_PER_ROUND];
    
    int cLen;
    bool bron;
    bool utilitarian;
    
    //int egalPoint;
    
    double deltaMax;
    bool backward;
    
    // determining actions stuff
    void updateConformance(State *s, int acts[2]);
    void determineHighLevelActions();
    //bool isGoalState(int s);
    double policyAgreement(double *p1, double *p2, int numActs);
    
    double conformance[2][20];
    int numHighLevelActions;
    int lastActions[2][20];
    int highestInd[2], lacts[2];
    //int highlevel[2][20];
    
    CommAgent *comms;
    bool sameNess[20][20];    
};

#endif
