#ifndef __Solver__CommAgent__
#define __Solver__CommAgent__

#include "defs.h"
#include "MySocket.h"

#define NONE_TIPO       0
#define TEXT_TIPO       1
#define ROBOT_TIPO      2
#define SAY_TIPO        3

#define MSG_EXPERT      0
#define MSG_EVENT       1
#define MSG_ASSESSMENT  2
#define MSG_STRING      3
#define MSG_ROUND       4


class CommAgent {
public:
    CommAgent();
    CommAgent(int _commtipo, int port, int _me);
    ~CommAgent();
    
    void postMessage(char *msg);
    void postMessage(int msgNum);
    void postMessage(int msgNum, char add[1024]);
    void lookUpMsg(int msgNum, char msg[1024]);
    
    void cleanMessage(char buf2[1024], char buf[1024]);
    
    int commtipo, me;
    MySocket *rserver;
    
    void logEvent(int eventType, int value, const char *message);
    void sendRobotMessages();
    
    int tipos[20];
    int valores[20];
    char strgs[20][1024];
    int numMessages;
};

#endif /* defined(__Solver__CommAgent__) */
