#ifndef a_h
#define a_h

#include "defs.h"
#include "expertLearner.h"
#include "CommAgent.h"

class a : public expertLearner {
public:
    a();
    a(int _me, double _lambda, int _numExperts, double hval, double lval, double mm);
    ~a();
    
    int select(bool *choices);
    int select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal);
    void update(double R, int tau);
    void update(double R, int heldTrueCount, int tau);
    
    bool willAccept(double R, int heldTrueCount, int tau);
    
    int numSatProp(bool *choices, bool *proposed, bool picks[100]);
    int randomlySelect(bool *choices);
    //int randomlySelect(bool *choices, bool *proposed);
    
    int me;
    
    //int lastExpert;
    //double rho;
    
    int *S_e;
    
    double highPay, lowPay, mmPay;
};



#endif
