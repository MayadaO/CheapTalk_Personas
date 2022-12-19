#ifndef experts_jefe_plus_h
#define experts_jefe_plus_h

#include "defs.h"
#include "Player.h"
#include "expertLearner.h"
#include "GameEngine.h"
#include "State.h"
#include "mega.h"
#include "CommAgent.h"
#include "ctModel.h"
#include "a.h"
//#include "Exp3.h"
#include "eee.h"
//#include "ucb.h"

class jefe_plus : public Player {
public:
    jefe_plus();
    jefe_plus(const char *nombre, int _me, MarkovGame *_mg, double _lambda, int addport, bool _plusplus, bool _XAI);
    ~jefe_plus();
    
    double processCheapTalk(char buf[10000]);
    
	int Move(GameEngine *ge);
    int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);
    int roundUpdate();
    
    void newSelection(double ScoreMe, double ScoreHim);
    void prepareSelection();
    
    void mapCT2Experts();
    void determineSatisficingExperts();
    bool override();
    void resetCycle();
    bool repeatJaction();
    //double pay(int me, int sol);
    
    void setAspirationHigh();
    void setAspirationFolkEgal();
    void setAspirationHighestEnforceable();
    
    bool preemptiveCT();
    bool preemptiveCT_EEE();
    bool isSomethingBetter(double mine, double his);
    bool somethingBetter;
    
    void labelConv(char str[1024], char conv[1024]);
    int getTypeNumber(int exprt);
    
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
    
    double lambda;
    
    int actionHistory[2][10];
    double previousPayoffs[2], lastSegmentScore;
    double payoffHistory[2][10];
    
    bool alwaysMM, tempMM;
    int MMcount;
    double permissibleLoss;
    
    double lowAspiration;
    
    State *lastState;
    bool lastValidActs[2][NUMACTIONS*2];
    double rPayout[2];
    //bool realbad;
    
    int corA;
    bool heldTrue;
    int numMovemientos;
    
    int outputTipo;
    
    FILE *fpExpert;
    
    int MMtimes;
    
    ctModel ctm;
    bool *ctExperts, *X;
    int heldTrueCount;
    
    //double V_tell, V_listen, V_thresh_tell, V_thresh_listen;
    bool alreadyForgiven;
    
    int listenCount, listened;
    int tellCount, honest;
    bool whipeoutguilt;
    
    bool shouldListen();
    bool shouldListen_EEE();
    bool giveSilentTreatment();
    
    int listeningBetrayalCount, totalBetrayalCount, oldBetrayalCount;
    bool firstSelection, brOverride;
    bool onSilentTreatment;
    bool distrustful, notfair;
    
    bool plusplus, avalueThing;
    bool XAI;
};

#endif
