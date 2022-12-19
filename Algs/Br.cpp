#include "Br.h"

extern void speakEXCELLENT(char buf[1024], int count);

extern int LETS_COOPcount, TAKE_TURNScount, SPECIFYcount, IACCEPTcount, UNACCEPTABLE_DTRUSTcount;
extern int UNACCEPTABLE_NFAIRcount, CHANGE_O_HEARTcount, DOBETTERcount, CURSEScount, YOULL_PAYcount;
extern int INYOURFACEcount, FORGIVENESScount, EXCELLENTcount, THREATcount;

Br::Br() {
    first = true;
    count = 0;
}

Br::~Br() {
}

void Br::Introduce(char stringToCommunicate[1024]) {
    if (first) {
        
        //system("say Hello.  Good to meet you.");
        //strcat(stringToCommunicate, "Hello.  Good to meet you. ");
    }
    
    first = false;
}

void Br::roundUpdate(bool happy, char stringToCommunicate[1024]) {
    
    double notsilent = 1.0 / (1.0 + (count / 3.0));
    //printf("probability of silence: %lf\n", 1.0 - notsilent);
    double num = rand() / (double)RAND_MAX;
    //printf("num = %lf\n", num);
    
    if (num > (1.0 - notsilent)) {
        char buf[1024];
        
        if (happy) {
            //EXCELLENTcount ++;
            //sprintf(buf, "%i;", EXCELLENT2);
            speakEXCELLENT(buf, count);
            strcat(stringToCommunicate, buf);
        }
        else {
            //sprintf(buf, "%s ", lows[rand() % BR_NUM_LOWS]);
            //sprintf(buf, "%i;", CURSE_YOU);
        }
        
        //printf("system: %s", buf);
        //system(buf);
    }
    
    count = count + 1;
}
