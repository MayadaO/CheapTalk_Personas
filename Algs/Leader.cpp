#include "Leader.h"

extern void speakLETS_COOP(char buf[1024], char ja_1[2], int count);
extern void speakTAKE_TURNS(char buf[1024], char ja_1[2], char ja_2[2], int count);
extern void speakSPECIFY(char buf[1024], char ja_1[2], int count);
extern void speakIACCEPT(char buf[1024]);
extern void speakUNACCEPTABLE_DTRUST(char buf[1024]);
extern void speakUNACCEPTABLE_NFAIR(char buf[1024]);
extern void speakCHANGE_O_HEART(char buf[1024]);
extern void speakDOBETTER(char buf[1024]);
extern void speakCURSES(char buf[1024]);
extern void speakYOULL_PAY(char buf[1024]);
extern void speakINYOURFACE(char buf[1024]);
extern void speakFORGIVENESS(char buf[1024]);
extern void speakEXCELLENT(char buf[1024], int count);
extern void speakTHREAT(char buf[1024], int count);

extern bool doTheSpecify;
extern bool yaDicho, yaDicho2;
extern int LETS_COOPcount, TAKE_TURNScount, SPECIFYcount, IACCEPTcount, UNACCEPTABLE_DTRUSTcount;
extern int UNACCEPTABLE_NFAIRcount, CHANGE_O_HEARTcount, DOBETTERcount, CURSEScount, YOULL_PAYcount;
extern int INYOURFACEcount, FORGIVENESScount, EXCELLENTcount, THREATcount;

extern bool differentTarget;

Leader::Leader() {
    printf("incomplete Leader constructor");
    exit(1);
}

Leader::Leader(bool _alternate, bool _bully) {
    alternate = _alternate;
    bully = _bully;
    
    count = 0;
    lowered = false;
}

Leader::~Leader() {
}

//void Leader::Introduce(int currentExpert, int previousExpert, char ja_1[1024], char ja_2[1024], bool myTurn, bool alter, bool _iaccept, char stringToCommunicate[1024]) {
void Leader::Introduce(bool newExpert, bool _forgiveness, char ja_1[1024], char ja_2[1024], bool myTurn, bool alter, bool _iaccept, bool _notrust, bool _notfairo, char stringToCommunicate[1024]) {
    char buf[1024], buf2[1024];
    
    printf("1F: %s\n", stringToCommunicate);
    printf("---------------- Leader Introduce (%i) ------------------\n", _notfairo);
    printf("newExpert = %i\n", (int)newExpert);
    printf("differentTarget = %i\n", (int)differentTarget);

    if (newExpert && differentTarget)
        count = 0;
    
    if (_iaccept) {
        //IACCEPTcount ++;
        //sprintf(buf, "%i;", IACCEPT);
        speakIACCEPT(buf);        
        strcpy(stringToCommunicate, buf);
        //strcat(stringToCommunicate, buf);
    }
    else if (_notrust && !yaDicho) {
        //printf("<<<<<<<<<<<<unnacceptable_dtrust leader\n");// exit(1);
        //UNACCEPTABLE_DTRUSTcount ++;
        //sprintf(buf, "%i;", UNACCEPTABLE_DTRUST);
        speakUNACCEPTABLE_DTRUST(buf);
        strcat(stringToCommunicate, buf);
        yaDicho = true;
    }
    else if (_notfairo && !yaDicho) {
        //printf("<<<<<<<<<<<<unnacceptable_nfair leader\n");// exit(1);
        //UNACCEPTABLE_NFAIRcount ++;
        //sprintf(buf, "%i;", UNACCEPTABLE_NFAIR);
        speakUNACCEPTABLE_NFAIR(buf);
        strcat(stringToCommunicate, buf);
        yaDicho = true;
    }
    
    //if ((currentExpert != previousExpert)) { // && !bully) {
    if ((newExpert && differentTarget) || _forgiveness) {
            //sprintf(buf, "%s ", intros[0]);
            if (!alternate && !yaDicho2) {
                //LETS_COOPcount ++;
                //THREATcount ++;
                //sprintf(buf, "%s;%i;", buf, THREAT);
                speakLETS_COOP(buf, ja_1, count);
                speakTHREAT(buf2, count);
                sprintf(buf, "%s%s", buf, buf2);
                
                strcat(stringToCommunicate, buf);
            }
            else {
                //sprintf(buf, "%s ", intros[1]);
                
                if (!yaDicho2) {
                    //TAKE_TURNScount ++;
                    //sprintf(buf, "%i %s %s;%i;", TAKE_TURNS, ja_1, ja_2, THREAT);
                    
                    speakTAKE_TURNS(buf, ja_1, ja_2, count);
                    speakTHREAT(buf2, count);
                    //THREATcount ++;
                    //sprintf(buf, "%s;%i;", buf, THREAT);
                    sprintf(buf, "%s%s", buf, buf2);
                    strcat(stringToCommunicate, buf);
                }
               
                if (doTheSpecify) {
                    //SPECIFYcount ++;
                
                    //if (myTurn)
                    //    sprintf(buf, "%i %s;", SPECIFY, ja_2);
                    //else
                    //    sprintf(buf, "%i %s;", SPECIFY, ja_1);
                    if (myTurn)
                        speakSPECIFY(buf, ja_2, count);
                    else
                        speakSPECIFY(buf, ja_1, count);

                    strcat(stringToCommunicate, buf);
                    
                }
            }
            //sprintf(buf, "%s ", intros[3]);
            //if (!yaDicho2) {
            //    sprintf(buf, "%i;", COMPLY_OR_ELSE);
            //    strcat(stringToCommunicate, buf);
            //}
        
        //printf("************* Leader: set the dicho = true\n");
        yaDicho2 = true;
    }
    //else if (alternate && (count < 5) && alter) {
    else if (alternate && alter) {
            // I NEED TO WORK ON THIS
            
            if (doTheSpecify) {
                //SPECIFYcount ++;
                
                //if (myTurn)
                //    sprintf(buf, "%i %s;", SPECIFY, ja_2);
                //else
                //    sprintf(buf, "%i %s;", SPECIFY, ja_1);
                if (myTurn)
                    speakSPECIFY(buf, ja_2, count);
                else
                    speakSPECIFY(buf, ja_1, count);
                strcat(stringToCommunicate, buf);
                
                //printf("************* Leader: set the dicho = true\n");
                //yaDicho = true;
            }
    }

    //printf("count = %i\n", count);
    
    //if (count == 5) {
        //sprintf(buf, "%s ", intros[11]);
        //EXCELLENTcount++;
        //sprintf(buf, "%i;", SWEET);
        //speakEXCELLENT(buf, count);
        //strcat(stringToCommunicate, buf);
    //}
    
    count = count + 1;
    
    printf("C: %s\n", stringToCommunicate);
    
    printf("------------------- End Introduce ----------------------\n");
}

void Leader::roundUpdate(bool happyRound, bool forgiveness, char stringToCommunicate[1024]) {
    char buf[1024];
    
    lowered = false;
    //if (happyRound && (count < 4)) {
    if (happyRound) {
        //sprintf(buf, "%s ", happythoughts[rand() % 4]);
        if (!yaDicho && !yaDicho2) {
            //EXCELLENTcount ++;
            //if (count < 2)
            //    sprintf(buf, "%i;", EXCELLENT);
            //else
            //    sprintf(buf, "%i;", EXCELLENT2);
            speakEXCELLENT(buf, count);
            strcat(stringToCommunicate, buf);
        }
    }
    
    if (forgiveness) {
        //FORGIVENESScount ++;
        //if (rand() % 2)
        //    sprintf(buf, "%i;", FORGIVENESS);
        //else
        //    sprintf(buf, "%i;", FORGIVENESS2);
        
        speakFORGIVENESS(buf);
        strcat(stringToCommunicate, buf);
        
        //sprintf(buf, "%i;", DOBETTER);
        //strcat(stringToCommunicate, buf);
        
        //if (alternate) {
        if (count > 2)
            count = 2;
        //}
    }
}

void Leader::moveEvent(bool shoulda, char shouldaMsg[1024], bool _heLowered, bool _guilty, bool forgiveness, bool theresmore, char stringToCommunicate[1024]) {
    char buf[1024], buf2[1024];
    
    //printf("moveEvent: %i %i\n", (int)_guilty, _heLowered);
    
    if (shoulda) {
        //CURSEScount++;
            
        if (_guilty) {
            //sprintf(buf, "%i;%i %c;%i;", BETRAYED+(rand() % 2), I_TRUSTED, shouldaMsg[0], YOULL_PAY+(rand() % 2));
            speakCURSES(buf);
            speakYOULL_PAY(buf2);
            //if (rand() % 2) {
                sprintf(buf, "%s%s", buf, buf2);
            //}
            //else {
            //    char buf3[1024];
            //    sprintf(buf3, "%s%s", buf2, buf);
            //    strcpy(buf, buf3);
            //}
            
            //YOULL_PAYcount ++;
            //if (rand() % 2)
            //    sprintf(buf, "%i;%i;", BETRAYED, YOULL_PAY);
            //else
            //    sprintf(buf, "%i;%i;", CURSE_YOU, YOULL_PAY);
            
            //printf("guilty\n");
            if (count > 2)
                count = 2;
        }
        else {

            //if (rand() % 2)
            //    sprintf(buf, "%i;", BETRAYED);
            //else
            //    sprintf(buf, "%i;", CURSE_YOU);
            speakCURSES(buf);
            //printf("not guilty\n");
            if (count > 2)
                count = 2;
        }


        strcat(stringToCommunicate, buf);
    }
    
    //printf("heLowered = %i; guilty = %i\n", (int)_heLowered, (int)_guilty);
    
    //if (!lowered && _heLowered && _guilty) {
    if (_heLowered && _guilty) {
        //INYOURFACEcount ++;
        //sprintf(buf, "%i;", INYOURFACE);
        speakINYOURFACE(buf);
        strcat(stringToCommunicate, buf);
        
        lowered = true;
    }
    
//    if (theresmore) {
//        sprintf(buf, "%i;", THERESMORE);
//        strcat(stringToCommunicate, buf);
//    }
    
    if (_guilty and (count > 2))
        count = 2;
    
}
