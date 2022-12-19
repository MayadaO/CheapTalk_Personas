#include "Solution.h"


Solution::Solution() {
    printf("incomplete Solution constructor\n");
    exit(1);
}

Solution::Solution(int _indices[2], double _payouts[2]) {
    int i;
    
    for (i = 0; i < 2; i++) {
        indices[i] = _indices[i];
        payouts[i] = _payouts[i];
    }
    
    maxGuilt[0] = maxGuilt[1] = 0.0;
    
    selected = false;
    counter = -1;
}

Solution::Solution(int _indices[2], double _payouts[2], MarkovGame *mg, int me, double deltaMax) {
    int i;
    
    for (i = 0; i < 2; i++) {
        indices[i] = _indices[i];
        payouts[i] = _payouts[i];
    }
    
    maxGuilt[0] = mg->computeBestDefection(indices[0], me) - payouts[1-me] + deltaMax;
    if (indices[0] == indices[1])
        maxGuilt[1] = maxGuilt[0];
    else
        maxGuilt[1] = mg->computeBestDefection(indices[1], me) - payouts[1-me] + deltaMax;

    computePunishmentRatio(mg, me);
    
    selected = false;
    counter = -1;
}

Solution::~Solution() {
}

void Solution::computePunishmentRatio(MarkovGame *mg, int me) {
    double punishability = payouts[1-me] - mg->startStates[0]->mm[1-me]->mv;
    double maximumGuilt;
    if (maxGuilt[1] > maxGuilt[0])
        maximumGuilt = maxGuilt[1];
    else
        maximumGuilt = maxGuilt[0];
    
    punishmentRatio = (int)((maximumGuilt / punishability) + 0.99);
    
    //printf("punishability = %lf; maximumGuilt = %lf (%lf, %lf)\n", punishability, maximumGuilt, maxGuilt[0], maxGuilt[1]);
    
    printf("punishmentRatio = %lf\n", punishmentRatio);
}