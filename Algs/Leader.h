#ifndef __Robot__Leader__
#define __Robot__Leader__

#include "defs.h"

//#define LEAD_NUM_INTROS     12
//#define LEAD_NUM_HAPPY      4
//#define LEAD_NUM_PUNISH     3
//#define LEAD_NUM_DEFECTION  9

class Leader {
public:
    Leader();
    Leader(bool _alternate, bool bully);
    ~Leader();
    
    //void Introduce(int currentExpert, int previousExpert, char ja_1[1024], char ja_2[1024], bool myTurn, bool alter, bool _iaccept, char stringToCommunicate[1024]);
    void Introduce(bool newExpert, bool _forgiveness, char ja_1[1024], char ja_2[1024], bool myTurn, bool alter, bool _iaccept, bool _notrust, bool _notfairo, char stringToCommunicate[1024]);
    void roundUpdate(bool happyRound, bool forgiveness, char stringToCommunicate[1024]);
    void moveEvent(bool shoulda, char shouldaMsg[1024], bool _heLowered, bool _guilty, bool forgiveness, bool theresmore, char stringToCommunicate[1024]);
    
    bool alternate, bully;
    
    //char intros[LEAD_NUM_INTROS][1024];
    //char defects[LEAD_NUM_DEFECTION][1024];
    //char happythoughts[LEAD_NUM_HAPPY][1024];
    //char inyourface[LEAD_NUM_PUNISH][1024];
    
    int count;
    bool lowered;

};

#endif /* defined(__Robot__Leader__) */
