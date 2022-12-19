#include "Follower.h"

extern void speakLETS_COOP(char buf[1024], char ja_1[2], int count);
extern void speakTAKE_TURNS(char buf[1024], char ja_1[2], char ja_2[2], int count);
extern void speakSPECIFY(char buf[1024], char ja_1[2], int count);
extern void speakIACCEPT(char buf[1024]);
extern void speakUNACCEPTABLE_DTRUST(char buf[1024]);
extern void speakUNACCEPTABLE_NFAIR(char buf[1024]);
extern void speakCHANGE_O_HEART(char buf[1024]);
extern void speakDOBETTER(char buf[1024]);
extern void speakCURSES(char buf[1024]);
extern void speakEXCELLENT(char buf[1024], int count);

extern bool doTheSpecify;
extern bool yaDicho, yaDicho2;
extern int LETS_COOPcount, TAKE_TURNScount, SPECIFYcount, IACCEPTcount, UNACCEPTABLE_DTRUSTcount;
extern int UNACCEPTABLE_NFAIRcount, CHANGE_O_HEARTcount, DOBETTERcount, CURSEScount, YOULL_PAYcount;
extern int INYOURFACEcount, FORGIVENESScount, EXCELLENTcount, THREATcount;

extern bool differentTarget;

Follower::Follower() {
    printf("incomplete Follower constructor");
    exit(1);
}

Follower::Follower(bool _alternate, bool _bully) {
    alternate = _alternate;
    bully = _bully;
    count = 0;
}

Follower::~Follower() {
}

//void Follower::Introduce(int currentExpert, int previousExpert, char ja_1[1024], char ja_2[1024], bool myTurn, bool _iaccept, char stringToCommunicate[1024]) {
void Follower::Introduce(bool newExpert, bool _forgiveness, char ja_1[1024], char ja_2[1024], bool myTurn, bool _iaccept, bool _notrust, bool _notfairo, char stringToCommunicate[1024]) {
    char buf[1024];
    
    printf("1F: %s\n", stringToCommunicate);
    printf("---------------- Follower Introduce (%i) ------------------\n", _notfairo);
    
    if (newExpert && differentTarget)
        count = 0;
    
    if (_iaccept) {
        printf("_accept\n");
        //IACCEPTcount ++;
        //sprintf(buf, "%i;", IACCEPT);
        speakIACCEPT(buf);
        strcpy(stringToCommunicate, buf);
        //strcat(stringToCommunicate, buf);
    }
    else if (_notrust && !yaDicho) {
        printf("_notrust\n");
        //printf("<<<<<<<<<<<<unnacceptable_dtrust follower\n"); //exit(1);

        //UNACCEPTABLE_DTRUSTcount ++;
        //sprintf(buf, "%i;", UNACCEPTABLE_DTRUST);
        speakUNACCEPTABLE_DTRUST(buf);
        strcat(stringToCommunicate, buf);
        yaDicho = true;
    }
    else if (_notfairo && !yaDicho) {
        printf("_notfairo\n");
        //printf("<<<<<<<<<<<<unnacceptable_nfair follower\n"); //----------------exit(1);
        //UNACCEPTABLE_NFAIRcount ++;
        //sprintf(buf, "%i;", UNACCEPTABLE_NFAIR);
        speakUNACCEPTABLE_NFAIR(buf);
        strcat(stringToCommunicate, buf);
        yaDicho = true;
    }
    
    //if ((currentExpert != previousExpert)) {// && !bully) {
    if ((newExpert && differentTarget) || _forgiveness) {
        printf("_newExpert\n");
        //sprintf(buf, "%s ", intros[0]);
        if (!alternate && !yaDicho2) {
            //LETS_COOPcount ++;
            //sprintf(buf, "%i %s;", LETS_COOP, ja_1);
            speakLETS_COOP(buf, ja_1, count);
            strcat(stringToCommunicate, buf);
        }
        else {
            if (!yaDicho2) {
                //TAKE_TURNScount ++;
                //sprintf(buf, "%i %s %s;", TAKE_TURNS, ja_1, ja_2);
                speakTAKE_TURNS(buf, ja_1, ja_2, count);
                strcat(stringToCommunicate, buf);
            }
            
            if (doTheSpecify) {
                //SPECIFYcount ++;
                
                if (myTurn)
                    speakSPECIFY(buf, ja_2, count);
                    //sprintf(buf, "%i %s;", SPECIFY, ja_2);
                else
                    speakSPECIFY(buf, ja_1, count);
                    //sprintf(buf, "%i %s;", SPECIFY, ja_1);
                strcat(stringToCommunicate, buf);
            }
        }
        yaDicho2 = true;
    }
    //else if (alternate && (count < 5)) {
    else if (alternate) {
        printf("_alternate\n");
        if (doTheSpecify) {
            //SPECIFYcount ++;
                
            if (myTurn)
                speakSPECIFY(buf, ja_2, count);
                //sprintf(buf, "%i %s;", SPECIFY, ja_2);
            else
                speakSPECIFY(buf, ja_1, count);
                //sprintf(buf, "%i %s;", SPECIFY, ja_1);
            strcat(stringToCommunicate, buf);
            
            //printf("************* follower: set the dicho = true\n");
            //yaDicho2 = true;
        }
    }
    
    //printf("count = %i\n", count);
    //if (count == 5) {
        //sprintf(buf, "%s ", intros[7]);
        //EXCELLENTcount++;
        //sprintf(buf, "%i;", SWEET);
        //speakEXCELLENT(buf, count);
        //strcat(stringToCommunicate, buf);
    //}
    
    count = count + 1;
    
    printf("C: %s\n", stringToCommunicate);
    
    printf("------------------- End Introduce ----------------------\n");
}

void Follower::roundUpdate(bool happyRound, bool forgiveness, char stringToCommunicate[1024]) {
    char buf[1024];
    
    //lowered = false;
    if (happyRound) {// && (count < 4)) {
        //sprintf(buf, "%s ", happythoughts[rand() % 4]);
        if (!yaDicho and !yaDicho2) {
            //EXCELLENTcount ++;
            //if (count < 2)
            //    sprintf(buf, "%i;", EXCELLENT);
            //else
            //    sprintf(buf, "%i;", EXCELLENT2);
            speakEXCELLENT(buf, count);
            strcat(stringToCommunicate, buf);
        }
    }
/*
    if (forgiveness) {
        printf("say I forgive you.\n");
        system("say I forgive you.\n");
        
        if (!bully) {
            sprintf(buf, "say %s", intros[1]);
            printf("%s", buf);
            system(buf);
        }
        else {
            sprintf(buf, "say But %s", intros[8]);
            printf("%s", buf);
            system(buf);
        }
        
        //if (alternate) {
        if (count > 3)
            count = 3;
        //}
    }*/
}

void Follower::moveEvent(bool shoulda, char shouldaMsg[1024], bool _heLowered, bool _guilty, char stringToCommunicate[1024]) {
    char buf[1024];
    
    if (shoulda) {
        //CURSEScount ++;
        //if (rand() % 2)
        //    sprintf(buf, "%i;", BETRAYED);
        //else
        //    sprintf(buf, "%i;", CURSE_YOU);
        speakCURSES(buf);
        strcat(stringToCommunicate, buf);
        
        if (count > 2)
            count = 2;
    }
    
    
    if (_guilty and (count > 2))
        count = 2;
}
