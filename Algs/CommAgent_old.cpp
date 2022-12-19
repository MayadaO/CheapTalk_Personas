#include "CommAgent.h"

extern void addMessage(char msg[1024], int _origin, int _lineHeight);

CommAgent::CommAgent() {
    printf("incomplete CommAgent constructor\n");
    exit(1);
}

CommAgent::CommAgent(int _commtipo, int port, int _me) {
    commtipo = _commtipo;
    me = _me;
    
    if (commtipo == ROBOT_TIPO) {
        rserver = new MySocket(port);
        rserver->AcceptEm();
    }
    
    numMessages = 0;
}

CommAgent::~CommAgent() {
    if (commtipo == ROBOT_TIPO)
        delete rserver;
}

void CommAgent::logEvent(int eventType, int value, const char *message) {
    //printf("log an event (%i) ... \n", numMessages);
    //fflush(stdout);
    
    tipos[numMessages] = eventType;
    valores[numMessages] = value;
    strcpy(strgs[numMessages], message);
    
    numMessages++;

    //printf("logged\n");
    //fflush(stdout);
}


void CommAgent::sendRobotMessages() {
    if (commtipo != ROBOT_TIPO)
        return;
    
    int i;
    char msg[1024];
    strcpy(msg, "");
    char buf[1024];
    
    for (i = 0; i < numMessages; i++) {
        if (tipos[i] == MSG_EXPERT) {
            sprintf(buf, "Expert %i ", valores[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_EVENT) {
            sprintf(buf, "Event %i ", valores[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_ASSESSMENT) {
            sprintf(buf, "Assess %i ", valores[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_STRING) {
            sprintf(buf, "String %i %s ", valores[i], strgs[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_ROUND) {
            sprintf(buf, "Round %i ", valores[i]);
            strcat(msg, buf);
        }
    }
    
    strcat(msg, "*");
    
    rserver->SendMessage(msg, strlen(msg));
    
    // then must wait for the robot to finish before moving on
    strcpy(buf, "");
    while (strncmp(buf, "ready", 5))
        rserver->ReadMessage(buf);
    
    numMessages = 0;
}

void CommAgent::postMessage(char *msg) {
    if (commtipo == TEXT_TIPO) {
        // post the message via the glStuff interface
        //printf("%s\n", msg);
        
        addMessage(msg, 15, me);
    }
    else if (commtipo == ROBOT_TIPO) {
        // send the message to the robot via rserver
        rserver->SendMessage(msg, strlen(msg));
        
        // then must wait for the robot to finish before moving on
        char buf[1024];
        strcpy(buf, "");
        while (strncmp(buf, "ready", 5))
            rserver->ReadMessage(buf);
    }
    else if (commtipo == SAY_TIPO) {
        // say the message via the voice command
        char *pch, buf[1024];
        pch = strtok(msg, "|");
        while (pch != NULL) {
            sprintf(buf, "say \"%s\"", pch);
            system(buf);
            pch = strtok(NULL, "|");
        }
        //sprintf(buf, "say \"%s\"", msg);
        //system(buf);
    }
}

void CommAgent::postMessage(int msgNum) {
    if (commtipo == ROBOT_TIPO) {
        char buf[1024];
        sprintf(buf, "%i$", msgNum);
        rserver->SendMessage(buf, strlen(buf));
        
        // then must wait for the robot to finish before moving on
        strcpy(buf, "");
        while (strncmp(buf, "ready", 5))
            rserver->ReadMessage(buf);
    }
    else {
        // look up the message
        char msg[1024];
        lookUpMsg(msgNum, msg);
        
        postMessage(msg);
    }
}

void CommAgent::postMessage(int msgNum, char add[1024]) {
    if (commtipo == ROBOT_TIPO) {
        char buf[1024], add2[1024];
        cleanMessage(add, add2);
        sprintf(buf, "%i$%s$", msgNum, add2);
        rserver->SendMessage(buf, strlen(buf));
    }
    else {
        // look up the message
        char msg[1024];
        lookUpMsg(msgNum, msg);
        
        char buf[1024];
        sprintf(buf, "%s %s", msg, add);
        
        postMessage(buf);
    }
}

void CommAgent::cleanMessage(char buf2[1024], char buf[1024]) {
    int len = (int)strlen(buf2);
    int i, j;
    
    for (i = 0; i < len; i++)
        buf[i] = buf2[i];
    
    for (i = len-1; i >= 0; i--) {
        if (buf[i] == '|') {
            for (j = i; j < len; j++) {
                buf[j] = buf[j+1];
            }
            
            len--;
        }
    }
    
    printf("clean: %s\n", buf);
}

void CommAgent::lookUpMsg(int msgNum, char msg[1024]) {
    //printf("msgNum = %i\n", msgNum);
    
    switch (msgNum) {
        case 1: strcpy(msg, "I'm not going to let you cheat me!|I insist on equal payoffs.|"); break;
        case 2: strcpy(msg, "Here's the deal:  Let's cooperate with each other.|"); break;
        case 3: strcpy(msg, "We can do this by taking turns receiving |the higher payout.|"); break;
        case 4: strcpy(msg, "If you don't do your part, I'll make you pay.|"); break;
        case 5: strcpy(msg, "Let's take turns.|"); break;
        case 6: strcpy(msg, "I get the higher payout each round.|"); break;
        case 7: strcpy(msg, "Otherwise, I'll make you pay in future rounds.|"); break;
        case 8: strcpy(msg, "Otherwise, I'll make you pay in future moves.|"); break;
        case 9: strcpy(msg, "Let's cooperate with each other.|"); break;
        case 10: strcpy(msg, "Excellent.|"); break;
        case 11: strcpy(msg, "That was selfish of you.|"); break;
        case 12: strcpy(msg, "You've confused me.|"); break;
        case 13: strcpy(msg, "That's what I had hoped for.|"); break;
        case 14: strcpy(msg, "Fine.|"); break;
        case 15: strcpy(msg, "You fool. I deserved better than that.|I'm going to punish you.|"); break;
        case 16: strcpy(msg, "Serves you right, jerk.|"); break;
        case 17: strcpy(msg, "Why did you do that for?|"); break;
        case 18: strcpy(msg, "Okay. I forgive you.|"); break;
        case 19: strcpy(msg, "Good.  That's fair"); break;
        case 20: strcpy(msg, "I'm happy with this.|"); break;
        case 21: strcpy(msg, "On second thought, I'll forgive you for now.|"); break;
        case 22: strcpy(msg, "I've changed my mind.|"); break;
        case 23: strcpy(msg, "You betrayed me.  I expected you to|"); break;
        case 24: strcpy(msg, "That is nicer than I expected.|"); break;
        case 25: strcpy(msg, "You FOOL!  I trusted you to |"); break;
        case 26: strcpy(msg, "I'm going to teach you a lesson you won't forget.|"); break;
        case 27: strcpy(msg, "Take that, jerk!|"); break;
        case 28: strcpy(msg, "I won't cheat you if you don't cheat me.|"); break;
        case 29: strcpy(msg, "It's my turn now.  You'll get a higher payout|next time.|"); break;
        case 30: strcpy(msg, "It's your turn to get a higher payout.|"); break;
        case 31: strcpy(msg, "I'm just feeling things out.|"); break;
        case 32: strcpy(msg, "I'm going into defense mode now.|"); break;
        case 33: strcpy(msg, "We could both do better than this.|"); break;
        default: strcpy(msg, ""); break;
    }
}
