#include "defs.h"
#include "client_socket.h"
#include "Bouncer.h"
#include "Br.h"
#include "Leader.h"
#include "Follower.h"

#define EMOTION_ONLY        0
#define EMOTION_PLANNER     1

#define CONNECT_PORT_BASE   4000

void speechActs();
void parseMsg(char buf[1024]);
void setEvent(int evnt);
void setAssessment(int ment);

int tipo = EMOTION_ONLY;
int addport = 0;

ClientSocket *cs;
Bouncer *bouncer;
Br *br;
Leader *pureLead, *altLead, *bullyLead;
Follower *pureFollow, *altFollow, *bullyFollow;

// system state
int currentExpert = -1, previousExpert = -1;
int currentRound;
bool roundDone = false;
bool happyExpert = false;
bool happyRound = false;
bool bothBetter = false;
bool myTurn = false;
bool currentlyGuilty = false;
bool associateDecreased = false, associateIncreased = false;
bool imDecreased = false, imIncreased = false;
bool shoulda = false;
char shouldaMsg[1024];
bool forgiveness = false;

/* ***********************************

./roboto [emotion,planner] [host] [addport]
 
************************************** */
int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc < 4) {
        printf("not enough parameters specified\n");
        exit(1);
    }

    if (!strcmp(argv[1], "planner"))
        tipo = EMOTION_PLANNER;
    
    addport = atoi(argv[3]);
    cs = new ClientSocket(argv[2], CONNECT_PORT_BASE + addport);
    
    bouncer = new Bouncer(tipo);
    br = new Br(tipo);
    pureLead = new Leader(tipo, false, false);
    altLead = new Leader(tipo, true, false);
    bullyLead = new Leader(tipo, false, true);
    pureFollow = new Follower(tipo, false, false);
    altFollow = new Follower(tipo, true, false);
    bullyFollow = new Follower(tipo, false, true);
    
    char buf[1024], buf2[1024];
    while (true) {
        
        strcpy(buf, "");
        while (buf[((int)strlen(buf) - 1)] != '*') {
            cs->ReadMessage(buf2);
            strcat(buf, buf2);
        }
        printf("%s\n", buf);
        
        parseMsg(buf);
        
        //printf("%i, %i\n", currentExpert, previousExpert);
        speechActs();
        
        strcpy(buf2, "ready");
        cs->SendMessage(buf2, strlen(buf2));
    }
    
    delete cs;
    delete pureFollow;
    delete altFollow;
    delete bullyFollow;
    delete br;
    delete bouncer;
    delete pureLead;
    delete altLead;
    delete bullyLead;
}

void speechActs() {
    
    if ((currentExpert == 1) && (previousExpert < 2)) { // Br
        if (br->first)
            br->Introduce();
        else if (roundDone)
            br->roundUpdate(happyRound);
            
    }
    else if (currentExpert == 2) { // Bouncer
        if (roundDone) {
            if (currentExpert != previousExpert)
                bouncer->Introduce();
            else {
                bouncer->roundUpdate(happyRound);
            }
        }
    }
    else if (currentExpert == 3) {
        pureLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                pureLead->roundUpdate(happyRound, forgiveness);
            }
            pureLead->Introduce(currentExpert, previousExpert, myTurn);
        }
    }
    else if (currentExpert == 4) {
        altLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                altLead->roundUpdate(happyRound, forgiveness);
            }
            altLead->Introduce(currentExpert, previousExpert, myTurn);
        }
    }
    else if (currentExpert == 5) {
        bullyLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                bullyLead->roundUpdate(happyRound, forgiveness);
            }

            bullyLead->Introduce(currentExpert, previousExpert, myTurn);
        }
    }

    else if (currentExpert == 6) {
        pureFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                pureFollow->roundUpdate(happyRound, forgiveness);
            }
            pureFollow->Introduce(currentExpert, previousExpert, myTurn);
        }
    }
    else if (currentExpert == 7) {
        altFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                altFollow->roundUpdate(happyRound, forgiveness);
            }
            altFollow->Introduce(currentExpert, previousExpert, myTurn);
        }
    }
    else if (currentExpert == 8) {
        bullyFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty);
        if (roundDone) {
            if (currentExpert == previousExpert) {
                bullyFollow->roundUpdate(happyRound, forgiveness);
            }
            
            bullyFollow->Introduce(currentExpert, previousExpert, myTurn);
        }
    }
    
    
    
    if (roundDone) {
        if (forgiveness) {
            currentlyGuilty = false;
            forgiveness = false;
        }
        happyRound = false;
        bothBetter = false;
    }
}

void parseMsg(char buf[1024]) {
    char *tkn;
    
    tkn = strtok(buf, " ");
    roundDone = false;
    shoulda = false;
    strcpy(shouldaMsg, "");
    bothBetter = false;
    while (true) {
        //printf("kind: %s\n", tkn);
        if (!strcmp(tkn, "*"))
            break;
        else if (!strcmp(tkn, "Event")) {
            tkn = strtok(NULL, " ");
            setEvent(atoi(tkn));
        }
        else if (!strcmp(tkn, "Expert")) {
            tkn = strtok(NULL, " ");
            currentExpert = atoi(tkn);
        }
        else if (!strcmp(tkn, "Round")) {
            tkn = strtok(NULL, " ");
            roundDone = true;
            previousExpert = currentExpert;
            currentRound = atoi(tkn);
            associateDecreased = associateIncreased = imIncreased = imDecreased = false;
        }
        else if (!strcmp(tkn, "Assess")) {
            tkn = strtok(NULL, " ");
            setAssessment(atoi(tkn));
        }
        else if (!strcmp(tkn, "String")) {
            tkn = strtok(NULL, " ");
            shoulda = true;
            strcpy(shouldaMsg, "");
            
            tkn = strtok(NULL, " ");
            while (strcmp(tkn, "$")) {
                strcat(shouldaMsg, tkn);
                strcat(shouldaMsg, " ");
                tkn = strtok(NULL, " ");
            }
            
            printf("shouldaMsg: %s\n", shouldaMsg);
        }
        
        tkn = strtok(NULL, " ");
    }
    
    if ((currentExpert != previousExpert) && roundDone) {
        char buf[1024];
        if (bothBetter) {
            strcpy(buf, "say We can both do better than this.\n");
            printf("%s", buf);
            system(buf);
        }
        else if (currentlyGuilty && (currentExpert != 1)) {
            strcpy(buf, "say I will let you off this time.\n");
            printf("%s", buf);
            system(buf);
        }
        else if (currentExpert != 1) {
            if (rand() % 2)
                strcpy(buf, "say I\\'ve had a change of heart.\n");
            else
                strcpy(buf, "say I\\'ve changed my mind.\n");

            printf("%s", buf);
            system(buf);
        }
        
        
        currentlyGuilty = false;
    }
}

void setEvent(int evnt) {
    switch (evnt) {
        case 1: imIncreased = true; break;
        case 2: imDecreased = true; break;
        case 3: associateIncreased = true; break;
        case 4: associateDecreased = true; break;
        case 5: happyRound = false; break;
        case 6: happyRound = true; break;
        case 7: happyExpert = false; break;
        case 8: happyExpert = true; break;
        case 9: printf("not sure what to do with event 11\n"); break;
        case 10: break;
        case 11: printf("not sure what to do with event 11\n"); break;
    }
}

void setAssessment(int ment) {
    switch (ment) {
        case 1: myTurn = true; break;
        case 2: myTurn = false; break;
        case 3: currentlyGuilty = true; break;
        case 4: forgiveness = true; break;
        case 5: bothBetter = true; break;
    }
}