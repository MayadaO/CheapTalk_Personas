#include "Bouncer.h"

extern bool differentTarget;

Bouncer::Bouncer() {
    lowCount = highCount = 0;
    firstTime = true;
}

Bouncer::~Bouncer() {
}

void Bouncer::roundUpdate(bool _high, char stringToCommunicate[1024]) {
    if (_high) {
        highPayout(stringToCommunicate);
        highCount ++;
    }
    else {
        lowPayout(stringToCommunicate);
        lowCount ++;
    }
}

void Bouncer::Introduce(char stringToCommunicate[1024]) {
    char buf[1024];
    
    if (firstTime) {
        //sprintf(buf, "%s %s %s  ", intros[0], intros[1], intros[2]);
        strcpy(buf, "");
    }
    else {
        //sprintf(buf, "%s ", intros[rand() % NUM_INTROS]);
        strcpy(buf, "");
    }
    
    //printf("system: %s", buf);
    //system(buf);
    strcat(stringToCommunicate, buf);
    
    firstTime = false;
    
    highCount = highCount / 3;
    lowCount = lowCount / 3;
}

void Bouncer::lowPayout(char stringToCommunicate[1024]) {
    //printf("in lowPayout\n");
    
    double notsilent = 1.0 / (1.0 + (lowCount / 5.0));
    //printf("probability of silence: %lf\n", 1.0 - notsilent);
    double num = rand() / (double)RAND_MAX;
    //printf("num = %lf\n", num);
    
    if (num > (1.0 - notsilent)) {
        char buf[1024];
        //sprintf(buf, "%s %s  ", lows[rand() % NUM_LOWS], intros[rand() % NUM_INTROS]);
        strcpy(buf, "");
        strcat(stringToCommunicate, buf);
    }
}

void Bouncer::highPayout(char stringToCommunicate[1024]) {
    //printf("in highPayout\n");
    
    double notsilent = 1.0 / (1.0 + (highCount / 2.0));
    //printf("probability of silence: %lf\n", 1.0 - notsilent);
    double num = rand() / (double)RAND_MAX;
    //printf("num = %lf\n", num);
    
    if (num > (1.0 - notsilent)) {
        char buf[1024];
        //sprintf(buf, "%s  ", highs[rand() % NUM_HIGHS]);
        strcpy(buf, "");
        
        
        //printf("system: %s", buf);
        //system(buf);
        strcat(stringToCommunicate, buf);
        system(buf);
    } 
}
