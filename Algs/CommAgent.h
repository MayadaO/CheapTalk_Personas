#ifndef __Solver__CommAgent__
#define __Solver__CommAgent__

#include "defs.h"
#include "MySocket.h"
#include "Bouncer.h"
#include "Br.h"
#include "Leader.h"
#include "Follower.h"
#include "personality.h"

#define NONE_TIPO       0
#define TEXT_TIPO       1
#define ROBOT_TIPO      2
#define SAY_TIPO        3

#define MSG_EXPERT      0
#define MSG_EVENT       1
#define MSG_ASSESSMENT  2
#define MSG_STRING      3
#define MSG_ROUND       4
#define MSG_JACTION     5

#define EMOTION_ONLY        0
#define EMOTION_PLANNER     1

#define CONNECT_PORT_BASE   4000

class CommAgent {
public:
    CommAgent();
    CommAgent(int _commtipo, int port, int _me);
    ~CommAgent();
    
    int commtipo, me;
    MySocket *rserver;
    
    void logEvent(int eventType, int value, const char *message);
    void printRobotMessages(bool silentTreatment, bool firstTime);
    
    int tipos[20];
    int valores[20];
    char strgs[20][1024];
    int numMessages;
    
    void speechActs(bool silentTreatment, bool firstTime);
    void parseMsg(char buf[1024]);
    void setEvent(int evnt);
    void setAssessment(int ment, int cur);
    
    int tipo;
    
    //ClientSocket *cs;
    Bouncer *bouncer;
    Br *br;
    Leader *pureLead, *altLead, *bullyLead;
    Follower *pureFollow, *altFollow, *bullyFollow;
    
    // system state
    int currentExpert, previousExpert;
    int currentRound;
    bool roundDone;
    bool happyExpert;
    bool happyRound;
    bool bothBetter;
    bool theresmore, iaccept;
    bool myTurn;
    bool currentlyGuilty;
    bool associateDecreased, associateIncreased;
    bool imDecreased, imIncreased;
    bool shoulda;
    char shouldaMsg[1024], ja_1[1024], ja_2[1024];
    bool forgiveness;
    
    bool newExpert;
    bool notrust;
    bool notfairo;
    
    //char stringToCommunicate[1024];
    bool sentmsg[20];
    
    int distrustCount, notfairCount;
};

#endif /* defined(__Solver__CommAgent__) */
