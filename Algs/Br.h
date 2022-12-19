#ifndef __Robot__Br__
#define __Robot__Br__

#include "defs.h"

#define BR_NUM_LOWS     3
#define BR_NUM_HIGHS    3

class Br {
public:
    Br();
    ~Br();
    
    void Introduce(char stringToCommunicate[1024]);
    void roundUpdate(bool happy, char stringToCommunicate[1024]);
    
    bool first;
    int count;
    
    //char lows[BR_NUM_LOWS][1024];
    //char highs[BR_NUM_HIGHS][1024];
};

#endif /* defined(__Robot__Br__) */
