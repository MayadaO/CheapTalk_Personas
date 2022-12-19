#include "defs.h"
#include "MySocket.h"

using namespace std;

void communicate(MySocket *rserver);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

static int ClientSocket;

void connectToRobot(int port) {
    MySocket *rserver = new MySocket(port);
    rserver->AcceptEm();
    
    communicate(rserver);
}

void communicate(MySocket *rserver) {
    char test[300];
    bzero(test, 301);
    bool loop = false;
    const char *msg;
    
    
    strcpy(test, "1");
    rserver->SendMessage(test, strlen(test));
    
    while(!loop)
    {
        bzero(test, 301);
        rserver->ReadMessage(test);
        
        if (!strncmp(test, "ready", 5)) {
            int num = rand() % 2;
            if (num == 0)
                strcpy(test, "1");
            else
                strcpy(test, "2");
            
            rserver->SendMessage(test, strlen(test));
        }

        usleep(5000000);
    }
    
    cout << "\nClosing thread and connection" << endl;
    close(rserver->hSocket);
}



