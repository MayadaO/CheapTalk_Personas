#ifndef __Solver__Solution__
#define __Solver__Solution__

#include "defs.h"
#include "MarkovGame.h"

class Solution {
public:
    Solution();
    Solution(int _indices[2], double _payouts[2]);
    Solution(int _indices[2], double _payouts[2], MarkovGame *mg, int me, double deltaMax);
    ~Solution();
    
    void computePunishmentRatio(MarkovGame *mg, int me);
    
    int indices[2];
    double payouts[2];
    double maxGuilt[2];
    double punishmentRatio;
    
    bool selected;
    int counter;
};

#endif /* defined(__Solver__Solution__) */
