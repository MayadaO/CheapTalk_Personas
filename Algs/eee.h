#ifndef eee_H
#define eee_H

#include "defs.h"
#include "expertLearner.h"

class eee : public expertLearner {
public:
	eee();
	eee(int _me, double _lambda, int _numExperts, double _maxmin, double *_potentials);
	~eee();

    int select(bool *choices);
    int select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal);
    void update(double R, int tau);
    void update(double R, int heldTrueCount, int tau);
    void findTheBest();
    double getValue(int index, bool proposed);

    int randomlySelect(bool *choices);
    
	int me;
	double *M_e;
	int *N_e, *S_e;
	int t, experto;
    
    double theBest, maxmin, *potentials;
    
    bool berandom;
};


#endif