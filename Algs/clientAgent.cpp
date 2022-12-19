#include "clientAgent.h"

extern int addPuerto;

clientAgent::clientAgent() {
    printf("incomplete clientAgent constructor\n");
    exit(1);
}

clientAgent::clientAgent(int _me) {
    me = _me;
    createConnection();
}

clientAgent::~clientAgent() {
    
}

void clientAgent::createConnection() {
    ss = new MySocket(CONNECT_PORT + me + addPuerto);
    
    printf("############### just waiting (%i) #######################\n", CONNECT_PORT + me + addPuerto);
    
    ss->AcceptEm();
    
    ss->ReadMessage(nombre);
    printf("connection established with agent %i named %s\n", me, nombre);
    printf("############### stopped waiting (%i) #######################\n", CONNECT_PORT + me + addPuerto);
}

int clientAgent::Move(GameEngine *ge) {
    char buf[1024];
    
    ss->ReadMessage(buf);
    //printf("buf: %s\n", buf);
    
    int a = atoi(buf);
    
    //printf("%i chose action %i\n", me, a);
    
    return a;
}

int clientAgent::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    char buf[1024];
    
    sprintf(buf, "move: %i %i %lf %lf", actions[0], actions[1], dollars[0], dollars[1]);
    //printf("sending: %s\n", buf); fflush(stdout);
    ss->SendMessage(buf, (int)strlen(buf));
    
    return 0;
}

int clientAgent::roundUpdate() {
    
    return 0;
}
