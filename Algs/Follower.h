#ifndef __Robot__Follower__
#define __Robot__Follower__

#include "defs.h"

//#define FOLLOWER_NUM_INTROS     8
//#define FOLLOWER_NUM_HAPPY      4
//#define FOLLOWER_NUM_DEFECTION  8

class Follower {
public:
    Follower();
    Follower(bool _alternate, bool bully);
    ~Follower();
    
    //void Introduce(int currentExpert, int previousExpert, char ja_1[1024], char ja_2[1024], bool myTurn, bool _iaccept, char stringToCommunicate[1024]);
    void Introduce(bool newExpert, bool _forgiveness, char ja_1[1024], char ja_2[1024], bool myTurn, bool _iaccept, bool _notrust, bool _notfairo, char stringToCommunicate[1024]);
    void roundUpdate(bool happyRound, bool forgiveness, char stringToCommunicate[1024]);
    void moveEvent(bool shoulda, char shouldaMsg[1024], bool _heLowered, bool _guilty, char stringToCommunicate[1024]);
    
    bool alternate, bully;
    
    //char intros[FOLLOWER_NUM_INTROS][1024];
    //char defects[FOLLOWER_NUM_DEFECTION][1024];
    //char happythoughts[FOLLOWER_NUM_HAPPY][1024];
    
    int count;
};


#endif /* defined(__Robot__Follower__) */
