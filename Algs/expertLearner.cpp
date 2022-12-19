#include "expertLearner.h"

expertLearner::expertLearner() {
    satisficing = false;
}

expertLearner::~expertLearner() {
}

int expertLearner::select(bool *choices) {
	return 0;
}

int expertLearner::select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal) {
	return 0;
}

void expertLearner::update(double R, int tau) {
}

void expertLearner::update(double R, int heldTrueCount, int tau) {
}

bool expertLearner::willAccept(double R, int heldTrueCount, int tau) {
    return false;
}

double expertLearner::computeERegret(double ave) {
    // see how much e-Regret the agent has
    int i;
    
    double val = 0;
    for (i = 0; i < numExperts; i++) {
        //printf("%i: %lf (%i)\n", i, x_phi[i], cuenta[i]);
        if (val < x_phi[i])
            val = x_phi[i];
    }
    //printf("\n");
    
    return val - ave;
}


