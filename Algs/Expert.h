#ifndef __Solver__Expert__
#define __Solver__Expert__

#include "defs.h"
#include "MarkovGame.h"
#include "State.h"
#include "Preventer.h"
#include "CommAgent.h"

#define     FOLLOWER    0
#define     LEADER      1
#define     MINIMAX     2
#define     BResp       3
#define     PREVENT     5
#define     LEADER2     6


class Expert {
public:
    Expert();
    Expert(int _tipo, int _strat1, int _strat2, int _cycleLen, double _lambda, bool _explorar, double _deltaMax, CommAgent *_comms, MarkovGame *_mg = NULL, int _me = -1);
    Expert(int _tipo, int _strat1, int _strat2, double _P1[2], double _P2[2], double _deltaMax, int _cycleLen, double _lambda, int _me, CommAgent *_comms, int counter = -1);
    ~Expert();
    
    int Move(State *s, double previousPayoffs[2], int _me);
    int generateAction(double *v, int numActs);
    
    bool congruent(MarkovGame *mg, int lastProposed[2][2]);
    
    void Update(double dollars[2], int _me, double _aspiration, bool _heldTrue, double aspiration);
    void updateMove(State *sprime, double rPayout[2], int me, int acts[2], double aspiration, char message[1024]);
    
    void reset(double previousPayoffs[2], int _me);
    void getCurrentStep(double previousPayoffs[2], int _me, bool _heldTrue);
    //bool checkCurrentCompatibility(double previousPayoffs[2]);
    
    void printStrategy(double *v, int numActs);
    
    bool showedLeadership(int hacts[20], double payoffHistory[2][10], int _me);
    bool showedLeadership(MarkovGame *mg, int actionHistory[2][10], int _me);
    
    int tipo, strat[2];
    double stratVals[2][2];
    double deltaMax;
    int cycleLen;
    
    double lambda;
    
    double vu, barR[2];
    int usage;
    
    int currentStep;
    
    double guilt, addedGuilt;
    bool culpable;
    
    bool explorar;
    
    Preventer *preventer;
    
    void currentStepMessage(int _me);
    
    int description[10], descriptionLen;
    //bool describeExpert;
    //char description[1024];
    
    //bool heldTrue;
    bool betrayed, favored;
    
    CommAgent *comms;
    
    char dcode[1024];
    
    bool hosed;
    int completedCycleCount;
    int seguida;
    double goodnessIndex;
    
    bool gotGuilty;
};

#endif
