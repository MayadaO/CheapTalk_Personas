#ifndef experts_jefe_h
#define experts_jefe_h

#include "defs.h"
#include "Player.h"
#include "expertLearner.h"
#include "GameEngine.h"
#include "State.h"
#include "mega.h"
//#include "a.h"
#include "Exp3.h"
#include "eee.h"
//#include "ucb.h"
#include "ctModel.h"

class jefe : public Player {
public:
    jefe();
    jefe(const char *nombre, int _me, MarkovGame *_mg, double _lambda, int addport);
    ~jefe();
    
    double processCheapTalk(char buf[10000]);    
    
	int Move(GameEngine *ge);
    int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);
    int roundUpdate();
    
    void newSelection();
    void labelConv(char str[1024], char conv[1024]);
    int getTypeNumber(int exprt);
    
    void determineSatisficingExperts();
    void resetCycle();
    bool repeatJaction();
    
    void setAspirationHigh();
    void setAspirationFolkEgal();
    void setAspirationHighestEnforceable();
    
    void prepareSelection();
    bool isSomethingBetter(double mine, double his);
    bool preemptiveCT();
    
    
    bool somethingBetter;
    
	int me;
	int t, experto;
    double R, mu, Rnot;
    int tau;
    bool cycled;
    int oldExpert;
    
    expertLearner *learner;
    mega *ab;
    
    bool *satisficingExperts;
    int numSatExperts;
    
    bool cycleFull;
    int latestJacts[2][20];
    
    //    double *vu;
    //    int *usage;
    double lambda;
    int outputTipo;
    
    //int previousActs[2];
    double previousPayoffs[2], lastSegmentScore;
    double payoffHistory[2][10];
    
    bool alwaysMM;
    double permissibleLoss;
    
    double lowAspiration;
    
    // new stuff
	//int convertIt(int accion, bool *validActions, int numActs);
    //void deduceActions(int rawActions[2], int geActions[2]);
    
    State *lastState;
    bool lastValidActs[2][NUMACTIONS*2];
    double rPayout[2];
    //bool realbad;
    
    int corA;
    
    FILE *fpExpert;
    
    ctModel ctm;
    bool *ctExperts, *X;
    int heldTrueCount;
    
    //double V_tell, V_listen, V_thresh_tell, V_thresh_listen;
    bool alreadyForgiven;
    
    int listenCount, listened;
    int tellCount, honest;
    bool whipeoutguilt;
    
    bool shouldListen();
    bool giveSilentTreatment();
    
    int listeningBetrayalCount, totalBetrayalCount, oldBetrayalCount;
    bool firstSelection, brOverride;
    bool onSilentTreatment;
    bool distrustful, notfair;
};

#endif
