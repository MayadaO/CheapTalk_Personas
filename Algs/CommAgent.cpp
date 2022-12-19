#include "CommAgent.h"

void speakLETS_COOP(char buf[1024], char ja_1[2], int count);
void speakTAKE_TURNS(char buf[1024], char ja_1[2], char ja_2[2], int count);
void speakSPECIFY(char buf[1024], char ja_1[2], int count);
void speakIACCEPT(char buf[1024]);
void speakUNACCEPTABLE_DTRUST(char buf[1024]);
void speakUNACCEPTABLE_NFAIR(char buf[1024]);
void speakCHANGE_O_HEART(char buf[1024]);
void speakDOBETTER(char buf[1024]);
void speakCURSES(char buf[1024]);
void speakYOULL_PAY(char buf[1024]);
void speakINYOURFACE(char buf[1024]);
void speakFORGIVENESS(char buf[1024]);
void speakEXCELLENT(char buf[1024], int count);
void speakTHREAT(char buf[1024], int count);

extern char stringToCommunicate[1024];
extern bool yaDicho, yaDicho2;
extern Personality *personality;

int LETS_COOPcount = 0, TAKE_TURNScount = 0, SPECIFYcount = 0, IACCEPTcount = 0, UNACCEPTABLE_DTRUSTcount = 0;
int UNACCEPTABLE_NFAIRcount = 0, CHANGE_O_HEARTcount = 0, DOBETTERcount = 0, CURSEScount = 0, YOULL_PAYcount = 0;
int INYOURFACEcount = 0, FORGIVENESScount = 0, EXCELLENTcount = 0, THREATcount = 0;

bool differentTarget = true;
int longago;

CommAgent::CommAgent() {
    printf("incomplete CommAgent constructor\n");
    exit(1);
}

CommAgent::CommAgent(int _commtipo, int port, int _me) {
    commtipo = _commtipo;
    me = _me;
    
    //printf("commAgent\n"); fflush(stdout);

    if (commtipo == ROBOT_TIPO) {
        rserver = new MySocket(port);
        rserver->AcceptEm();
        
        //printf("all set up\n"); fflush(stdout);
    }

    numMessages = 0;
    
    bouncer = new Bouncer();
    br = new Br();
    pureLead = new Leader(false, false);
    altLead = new Leader(true, false);
    bullyLead = new Leader(false, true);
    pureFollow = new Follower(false, false);
    altFollow = new Follower(true, false);
    bullyFollow = new Follower(false, true);
    
    currentExpert = previousExpert = -1;
    roundDone = false;
    happyExpert = false;
    happyRound = false;
    bothBetter = false;
    theresmore = false;
    myTurn = false;
    currentlyGuilty = false;
    associateDecreased = associateIncreased = false;
    imDecreased = imIncreased = false;
    shoulda = false;
    forgiveness = false;
    iaccept = false;
    notrust = false;
    notfairo = false;
    
    newExpert = false;
    differentTarget = false;
    
    notfairCount = distrustCount = 0;
}

CommAgent::~CommAgent() {
    if (commtipo == ROBOT_TIPO)
        delete rserver;
    
    delete pureFollow;
    delete altFollow;
    delete bullyFollow;
    delete br;
    delete bouncer;
    delete pureLead;
    delete altLead;
    delete bullyLead;    
}

void CommAgent::logEvent(int eventType, int value, const char *message) {
    printf("logEvent: %i %i %s\n", eventType, value, message);

    tipos[numMessages] = eventType;
    valores[numMessages] = value;
    strcpy(strgs[numMessages], message);
    
    numMessages++;
}


void CommAgent::printRobotMessages(bool silentTreatment, bool firstTime) {
    
    //printf("************ printing Robot Messages: %i\n", numMessages);
    
    //if (commtipo != ROBOT_TIPO)
    //    return;
    
    int i;
    char msg[1024];
    strcpy(msg, "");
    char buf[1024];
    
    //printf("numMessages = %i\n", numMessages); fflush(stdout);
    
    for (i = 0; i < numMessages; i++) {
        if (tipos[i] == MSG_EXPERT) {
            sprintf(buf, "Expert %i %s ", valores[i], strgs[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_EVENT) {
            sprintf(buf, "Event %i ", valores[i]);
            strcat(msg, buf);
        }
        else if (tipos[i] == MSG_ASSESSMENT) {
            sprintf(buf, "Assess %i %s ", valores[i], strgs[i]);
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
        else if (tipos[i] == MSG_JACTION) {
            //printf("************** got to here: %s\n", strgs[i]);
            sprintf(buf, "JAction %s ", strgs[i]);
            strcat(msg, buf);
        }
    }
    
    strcat(msg, "*");
    
    //strcpy(stringToCommunicate, "");
    
    //printf("to parse the message\n"); fflush(stdout);
    
    parseMsg(msg);
    //printf("parsed\n");
    speechActs(silentTreatment, firstTime);
    
    
    numMessages = 0;
}


void CommAgent::speechActs(bool silentTreatment, bool firstTime) {
    // decide if I should speak to him

    bool alter = true;
    if (currentlyGuilty && !forgiveness)
        alter = false;
    
//    if ((currentExpert > 2) && (notrust)) {
//        char buf[1024];
    
//        sprintf(buf, "%i;", NOTRUST);
//        strcat(stringToCommunicate, buf);
//    }

    //printf("\tCurrentExpert = %i; firstTime = %i\n", currentExpert, (int)firstTime);
    
    printf("A: %s\n", stringToCommunicate);
    
    //printf("currentExpert = %i\n", currentExpert);
    if (firstTime) {
        char buf[1024];
        sprintf(buf, "%i 0;", GREETING);
        //strcat(stringToCommunicate, buf);
        // delete anything generated before this
        strcpy(stringToCommunicate, buf);
        
        if (!strcmp(personality->pNombre, "Dale"))
            return;
    }
    //else if ((currentExpert == 0) && (previousExpert != 0)) {
        //char buf[1024];
        //sprintf(buf, "%i;", NOTRUST);
        //strcat(stringToCommunicate, buf);
    //}
    if ((currentExpert == 1) && (previousExpert < 2)) { // Br
        if (br->first)
            br->Introduce(stringToCommunicate);
        else if (roundDone)
            br->roundUpdate(happyRound, stringToCommunicate);
        
        if (notfairo && !yaDicho) {
            //printf("<<<<<<<<<<<<unnacceptable_nfair comms\n"); //exit(1);
            char buf[1024];
            
            //UNACCEPTABLE_NFAIRcount ++;
            //sprintf(buf, "%i;", UNACCEPTABLE_NFAIR);
            speakUNACCEPTABLE_NFAIR(buf);
            strcat(stringToCommunicate, buf);
            yaDicho = true;
        }
    }
    else if (currentExpert == 2) { // Bouncer
        if (roundDone) {
            if (newExpert) //currentExpert != previousExpert)
                bouncer->Introduce(stringToCommunicate);
            else {
                bouncer->roundUpdate(happyRound, stringToCommunicate);
            }
        }
    }
    else if (currentExpert == 3) {
        //if (associateDecreased)
        //    printf("helowered\n");
        //else
        //    printf("!!!!Not heLowered\n");
        pureLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, forgiveness, theresmore, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                pureLead->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //pureLead->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, alter, iaccept, stringToCommunicate);
            pureLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else if (firstTime) {
        //    printf("intro to come\n");
        //    pureLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    else if (currentExpert == 4) {
        altLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, forgiveness, theresmore, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                altLead->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //altLead->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, alter, iaccept, stringToCommunicate);
            altLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else if (firstTime) {
        //    printf("intro to come\n");
        //    altLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    else if (currentExpert == 5) {
        bullyLead->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, forgiveness, theresmore, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                bullyLead->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //bullyLead->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, alter, iaccept, stringToCommunicate);
            bullyLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else if (firstTime) {
        //    printf("intro to come\n");
        //    bullyLead->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, alter, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    
    else if (currentExpert == 6) {
        pureFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                pureFollow->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //pureFollow->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, iaccept, stringToCommunicate);
            pureFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else if (firstTime) {
        //    printf("intro to come\n");
        //    pureFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    else if (currentExpert == 7) {
        altFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                altFollow->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //altFollow->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, iaccept, stringToCommunicate);
            altFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else if (firstTime) {
        //    printf("intro to come\n");
        //    altFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    else if (currentExpert == 8) {
        bullyFollow->moveEvent(shoulda, shouldaMsg, associateDecreased, currentlyGuilty, stringToCommunicate);
        //printf("s2c: %s\n", stringToCommunicate);
        if (roundDone) {
            //printf("que pasa?\n");
            if (currentExpert == previousExpert) {
                bullyFollow->roundUpdate(happyRound, forgiveness, stringToCommunicate);
            }
            //printf("@@@@@@@@@@@@@@@should introduce\n");
            //bullyFollow->Introduce(currentExpert, previousExpert, ja_1, ja_2, myTurn, iaccept, stringToCommunicate);
            bullyFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        }
        //else {
        //    printf("intro to come\n");
        //    bullyFollow->Introduce(newExpert, forgiveness, ja_1, ja_2, myTurn, iaccept, notrust, notfairo, stringToCommunicate);
        //}
    }
    
    //printf("doner\n");
    printf("B: %s\n", stringToCommunicate);
    
    if (roundDone) {
        if (forgiveness) {
            currentlyGuilty = false;
            forgiveness = false;
        }
        happyRound = false;
        bothBetter = false;
        theresmore = false;
        iaccept = false;
        notrust = false;
        notfairo = false;
        associateDecreased = associateIncreased = imIncreased = imDecreased = false;
    }
    
    if (silentTreatment) {
        printf("*********** applying the silent treatment ************\n");
        strcpy(stringToCommunicate, "");
    }
}

void CommAgent::parseMsg(char buf[1024]) {
    char *tkn;
    
    //printf("Message: %s\n", buf);
    
    tkn = strtok(buf, " ");
    roundDone = false;
    shoulda = false;
    strcpy(shouldaMsg, "");
    bothBetter = false;
    theresmore = false;
    iaccept = false;
    notrust = false;
    notfairo = false;
    while (true) {
        //printf("kind: %s\n", tkn);
        if (!strcmp(tkn, "*"))
            break;
        else if (!strcmp(tkn, "Event")) {
            //printf("Event\n");
            tkn = strtok(NULL, " ");
            setEvent(atoi(tkn));
        }
        else if (!strcmp(tkn, "Expert")) {
            tkn = strtok(NULL, " ");
            printf("e1 = %s\n", tkn);
            int oldExprt = currentExpert;
            currentExpert = atoi(tkn);
            tkn = strtok(NULL, " ");
            printf("e2 = %s\n", tkn);
            differentTarget = (bool)(atoi(tkn));
            
            printf("differentTarget = %i\n", differentTarget);
            
            newExpert = true;
            //printf("Expert: %i\n", currentExpert);
        }
        else if (!strcmp(tkn, "Round")) {
            //printf("Round\n");
            tkn = strtok(NULL, " ");
            roundDone = true;
            previousExpert = currentExpert;
            currentRound = atoi(tkn);
            newExpert = false;
            differentTarget = false;
            //associateDecreased = associateIncreased = imIncreased = imDecreased = false;
        }
        else if (!strcmp(tkn, "Assess")) {
            //printf("Assess\n");
            tkn = strtok(NULL, " ");
            int assessment = atoi(tkn);
            tkn = strtok(NULL, " ");
            setAssessment(assessment, atoi(tkn));
        }
        else if (!strcmp(tkn, "String")) {
            //printf("String\n");
            tkn = strtok(NULL, " ");
            shoulda = true;
            strcpy(shouldaMsg, "");
            
            tkn = strtok(NULL, " ");
            while (strcmp(tkn, "$")) {
                strcat(shouldaMsg, tkn);
                strcat(shouldaMsg, " ");
                tkn = strtok(NULL, " ");
            }
            
            //printf("shouldaMsg: %s\n", shouldaMsg);
        }
        else if (!strcmp(tkn, "JAction")) {
            //printf("JAction\n");
            tkn = strtok(NULL, " ");
            strcpy(ja_1, tkn);
            tkn = strtok(NULL, " ");
            strcpy(ja_2, tkn);

            //printf("ja's: %s %s\n", ja_1, ja_2);
        }
        else {
            printf("command %s not found\n", tkn);
        }
        
        tkn = strtok(NULL, " ");
    }
    
    //if (tipo == EMOTION_PLANNER_TYPE) {
        //if ((currentExpert != previousExpert) && roundDone && (currentExpert != 0)) {
        if (newExpert && roundDone && (currentExpert != 0)) {
            char buf[1024];

            if ((currentExpert > 2) && differentTarget) {
                speakCHANGE_O_HEART(buf);
                strcpy(stringToCommunicate, buf);
            }

            if (bothBetter) {
                //strcpy(buf, "We can both do better than this. ");
                //DOBETTERcount ++;
                //sprintf(buf, "%i;", DOBETTER);
                speakDOBETTER(buf);
                strcat(stringToCommunicate, buf);
            }
            
            forgiveness = false;
            
            //printf("------ not guilty ------\n");
            currentlyGuilty = false;
        }
    //}
}

void CommAgent::setEvent(int evnt) {
    printf("Event: %i\n", evnt);

    switch (evnt) {
        case 1: imIncreased = true; break;
        case 2: imDecreased = true; break;
        case 3: associateIncreased = true; break;
        case 4: associateDecreased = true; break;
        case 5: happyRound = false; break;
        case 6: happyRound = true; break;
        case 7: happyExpert = false; break;
        case 8: happyExpert = true; break;
        case 9: break; //printf("not sure what to do with event 11\n"); break;
        case 10: break;
        case 11: break; //printf("not sure what to do with event 11\n"); break;
    }
    
    //printf("associateDecreased: %i\n", (int)associateDecreased);
}

void CommAgent::setAssessment(int ment, int cur) {
    //printf("Assessment: %i %i\n", ment, cur);

    switch (ment) {
        case 1:
        case 2: if (cur == 0)
                    myTurn = false;
                else
                    myTurn = true;
                break;
        case 3: currentlyGuilty = true; break;
        case 4: forgiveness = true; break;
        case 5: bothBetter = true; break;
        case 6: theresmore = true; break;
        case 7: iaccept = true;
                if (cur == 1)
                    longago = true;
                else
                    longago = false;
                break;
        case 8: if (distrustCount < 2)
                    notrust = true;
                distrustCount ++;
                printf("---------- got the NO-TRUST message (%i) -------------\n", distrustCount);
                break;
        case 9: if (notfairCount < 2)
                    notfairo = true;
                printf("---------- got the NO-FAIR message (%i) -------------\n", notfairCount);
                notfairCount ++;
                break;
    }
}


// Global speaking stuff
void speakLETS_COOP(char buf[1024], char ja_1[2], int count) {
    printf("LETS_COOPcount = %i (%i)\n", LETS_COOPcount, count); fflush(stdout);
    if ((count == 0) || ((LETS_COOPcount+TAKE_TURNScount) < 3)) {
        sprintf(buf, "%i 0 %s;", LETS_COOP, ja_1);
        //printf("buf: %s\n", buf);
    }
    else {
        strcpy(buf, "");
    }

    LETS_COOPcount ++;
}

void speakTAKE_TURNS(char buf[1024], char ja_1[2], char ja_2[2], int count) {
    printf("TAKE_TURNScount = %i (%i)\n", TAKE_TURNScount, count); fflush(stdout);
    if ((count == 0) || ((LETS_COOPcount+TAKE_TURNScount) < 3))
        sprintf(buf, "%i 0 %s %s;", TAKE_TURNS, ja_1, ja_2);
    else
        strcpy(buf, "");

    TAKE_TURNScount ++;
}

void speakSPECIFY(char buf[1024], char ja_1[2], int count) {
    if ((count < 3) || (SPECIFYcount < 3))
        sprintf(buf, "%i 0 %s;", SPECIFY, ja_1);
    else
        strcpy(buf, "");

    SPECIFYcount ++;
}

void speakIACCEPT(char buf[1024]) {
    if (longago) {
        sprintf(buf, "%i %i;", IACCEPT, 4);
    }
    else {
        if (IACCEPTcount < 3)
            sprintf(buf, "%i %i;", IACCEPT, IACCEPTcount);
        else
            sprintf(buf, "%i %i;", IACCEPT, 3);
        
        IACCEPTcount ++;
    }
}

void speakUNACCEPTABLE_DTRUST(char buf[1024]) {
    if (UNACCEPTABLE_DTRUSTcount < 3)
        sprintf(buf, "%i %i;", UNACCEPTABLE_DTRUST, UNACCEPTABLE_DTRUSTcount);
    else
        sprintf(buf, "%i %i;", UNACCEPTABLE_DTRUST, 2);
    
    UNACCEPTABLE_DTRUSTcount ++;
}


void speakUNACCEPTABLE_NFAIR(char buf[1024]) {
    if (UNACCEPTABLE_NFAIRcount < 3)
        sprintf(buf, "%i %i;", UNACCEPTABLE_NFAIR, UNACCEPTABLE_NFAIRcount);
    else
        sprintf(buf, "%i %i;", UNACCEPTABLE_NFAIR, rand() % 2 + 1);
    
    UNACCEPTABLE_NFAIRcount ++;
}

void speakCHANGE_O_HEART(char buf[1024]) {
    if (CHANGE_O_HEARTcount < 3)
        sprintf(buf, "%i %i;", CHANGE_O_HEART, CHANGE_O_HEARTcount);
    else
        sprintf(buf, "%i %i;", CHANGE_O_HEART, 3);
    
    CHANGE_O_HEARTcount ++;
}

void speakDOBETTER(char buf[1024]) {
    if (DOBETTERcount < 3)
        sprintf(buf, "%i %i;", DOBETTER, DOBETTERcount);
    else
        sprintf(buf, "%i %i;", DOBETTER, 3);
    
    DOBETTERcount ++;
}

void speakCURSES(char buf[1024]) {
    //printf("<<<<<<<<<< speaking CURSES: %i >>>>>>>>>\n", CURSEScount);

    if (CURSEScount < 5)
        sprintf(buf, "%i %i;", CURSES, CURSEScount);
    else {
        if ((rand() % (CURSEScount / 2)) == 0)
            sprintf(buf, "%i %i;", CURSES, (rand() % 4) + 4);
        else
            strcpy(buf, "");
    }
    //if (rand() % 2)
    //    sprintf(buf, "%i;", CURSES);
    //else
    //    sprintf(buf, "%i;", CURSES);
    
    CURSEScount ++;
}

void speakYOULL_PAY(char buf[1024]) {
    printf("YOULL_PAY: %i\n", YOULL_PAYcount);
    if (YOULL_PAYcount < 8)
        sprintf(buf, "%i %i;", YOULL_PAY, YOULL_PAYcount % 8);
    else
        sprintf(buf, "%i %i;", YOULL_PAY, YOULL_PAYcount % 5 + 3);
    
    YOULL_PAYcount ++;
}

void speakINYOURFACE(char buf[1024]) {
    if (INYOURFACEcount < 8)
        sprintf(buf, "%i %i;", INYOURFACE, INYOURFACEcount % 8);
    else
        sprintf(buf, "%i %i;", INYOURFACE, INYOURFACEcount % 5 + 3);

    INYOURFACEcount ++;
}

void speakFORGIVENESS(char buf[1024]) {
    printf("FORGIVENESScount = %i\n", FORGIVENESScount);
    if (FORGIVENESScount < 8)
        sprintf(buf, "%i %i;", FORGIVENESS, FORGIVENESScount % 8);
    else
        //sprintf(buf, "%i 7;", FORGIVENESS);
        strcpy(buf, "");

    //if (rand() % 2)
    //    sprintf(buf, "%i;", FORGIVENESS);
    //else
    //    sprintf(buf, "%i;", FORGIVENESS);
    
    FORGIVENESScount ++;
}

void speakEXCELLENT(char buf[1024], int count) {
    printf("<EXCELLENTcount> = %i (%i)\n", EXCELLENTcount, count);
    if ((count < 6) && (EXCELLENTcount < 8)) {
        if ((rand() % 4) < 3)
            sprintf(buf, "%i %i;", EXCELLENT, EXCELLENTcount % 8);
        else {
            printf("**Random silence.\n");
            strcpy(buf, "");
        }
    }
    else {
        int chance = EXCELLENTcount / 2;
        if ((count <= 6) && ((rand() % chance) == 0))
            sprintf(buf, "%i %i;", EXCELLENT, rand() % 3 + 5);
        else
            strcpy(buf, "");
    }
/*
    if (count < 4) {
        if (count < 2)
            sprintf(buf, "%i;", EXCELLENT);
        else
            sprintf(buf, "%i;", EXCELLENT);
    }
    else if (count == 5) {
        sprintf(buf, "%i;", EXCELLENT);
    }
*/
    EXCELLENTcount ++;
}

void speakTHREAT(char buf[1024], int count) {
    //printf("THREATcount = %i (%i)\n", THREATcount, count);
    if ((count == 0) || ((rand() % 4) == 0)) {
        sprintf(buf, "%i %i;", THREAT, THREATcount % 3);
        //printf("buf: %s\n", buf);
    }
    else {
        strcpy(buf, "");
    }
    
    THREATcount ++;
}
