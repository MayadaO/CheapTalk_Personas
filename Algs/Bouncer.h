#ifndef __Robot__Bouncer__
#define __Robot__Bouncer__

#include "defs.h"

//#define NUM_INTROS  3
//#define NUM_LOWS    4
//#define NUM_HIGHS   4

class Bouncer {
public:
    Bouncer();
    ~Bouncer();
    
    void roundUpdate(bool _high, char stringToCommunicate[1024]);
    
    void Introduce(char stringToCommunicate[1024]);
    void lowPayout(char stringToCommunicate[1024]);
    void highPayout(char stringToCommunicate[1024]);
    
    int lowCount;
    int highCount;
    
    //char intros[NUM_INTROS][1024];
    //char lows[NUM_LOWS][1024];
    //char highs[NUM_HIGHS][1024];
    
    bool firstTime;
};

#endif /* defined(__Robot__Bouncer__) */
