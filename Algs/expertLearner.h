#ifndef EXPERTLEARNER_H
#define EXPERTLEARNER_H

#include "defs.h"
#include "CommAgent.h"

class expertLearner {
public:
	expertLearner();
	virtual ~expertLearner();
	
	virtual int select(bool *choices);
    virtual int select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal);
	virtual void update(double R, int tau);
	virtual void update(double R, int heldTrueCount, int tau);
    virtual bool willAccept(double R, int heldTrueCount, int tau);
    
    double computeERegret(double ave);
    
    
    double aspiration, lambda;
    
    int numExperts;
    double *x_phi;
    int *cuenta;
    
    bool satisficing;
    double *recentSuccess;
    int lastExpert;
    double rho;
    
    bool listening2Him;
};

#endif
