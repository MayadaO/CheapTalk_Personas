#include "Expert.h"

//extern void addMessage(char msg[1024], int _origin, int _lineHeight);

Expert::Expert() {
    printf("incomplete Expert constructor\n");
    exit(1);
}

Expert::Expert(int _tipo, int _strat1, int _strat2, int _cycleLen, double _lambda, bool _explorar, double _deltaMax, CommAgent *_comms, MarkovGame *_mg, int _me) {
    tipo = _tipo;
    strat[0] = _strat1;
    strat[1] = _strat2;
    stratVals[0][0] = stratVals[0][1] = -99999;
    stratVals[1][0] = stratVals[1][1] = -99999;
    deltaMax = _deltaMax;
    comms = _comms;
    
    currentStep = -1;
    
    printf("Expert of type %i: %i %i\n", tipo, strat[0], strat[1]);
    
    guilt = 0.0;
    usage = 0;
    barR[0] = barR[1] = 0.0;
    
    cycleLen = _cycleLen;
    
    lambda = _lambda;
    
    culpable = false;
    explorar = _explorar;
    
    if (tipo == PREVENT) {
        preventer = new Preventer(_me, _mg, 0.5, false);
        //barR[0] = 16.0;//_mg->states[0]->Vreal[0];
        //barR[1] = 16.0;//_mg->states[0]->Vreal[1];
        //printf("Preventer Bar R = %lf, %lf\n", barR[0], barR[1]);
    }
    
    if (tipo == MINIMAX) {
        //strcpy(description, "");
        description[0] = 32;
        descriptionLen = 1;
        
        strcpy(dcode, "Minimax");
    }
    else if (tipo == BResp) {
        description[0] = 31;
        descriptionLen = 1;

        strcpy(dcode, "BResp");
    }
    else {
        //strcpy(description, "I'm not going to let you cheat me!|I insist on equal payoffs.|");
        description[0] = 1;
        descriptionLen = 1;
        
        strcpy(dcode, "Bouncer");
    }
    
    hosed = false;
    completedCycleCount = 0;
    seguida = 0;
    goodnessIndex = 0;
    gotGuilty = false;
    //describeExpert = false;
}


Expert::Expert(int _tipo, int _strat1, int _strat2, double _P1[2], double _P2[2], double _deltaMax, int _cycleLen, double _lambda, int _me, CommAgent *_comms, int counter) {
    tipo = _tipo;
    strat[0] = _strat1;
    strat[1] = _strat2;
    stratVals[0][0] = _P1[0];    stratVals[0][1] = _P1[1];
    stratVals[1][0] = _P2[0];    stratVals[1][1] = _P2[1];
    deltaMax = _deltaMax;
    barR[0] = (stratVals[0][0] + stratVals[1][0]) / 2.0;
    barR[1] = (stratVals[0][1] + stratVals[1][1]) / 2.0;
    comms = _comms;
    
    currentStep = -1;
    
    printf("Expert of type %i: %i %i designed to produce (%.2lf, %.2lf)\n", tipo, strat[0], strat[1], barR[0], barR[1]);
    
    guilt = 0.0;
    usage = 0;
    
    cycleLen = _cycleLen;
    
    lambda = _lambda;
    
    culpable = false;
    
    explorar = false;
    
    if ((tipo == LEADER) || (tipo == LEADER2)) {
        sprintf(dcode, "Lead_%i", counter);
        if (fabs(barR[0] - barR[1]) < deltaMax) {
            description[0] = 2;
            descriptionLen = 1;
            //strcpy(description, "Here's the deal:  Let's cooperate with each other.|");
            if ((fabs(stratVals[0][0] - stratVals[1][0]) > deltaMax) || (fabs(stratVals[0][1] - stratVals[1][1]) > deltaMax)) {
                description[1] = 3;
                descriptionLen++;
                //strcat(description, "We can do this by taking turns receiving|the higher payout.|");
            }
            description[descriptionLen] = 4;
            descriptionLen++;
            //strcat(description, "If you don't do your part, I'll make you pay.|");
        }
        else {
            if ((fabs(stratVals[0][0] - stratVals[1][0]) > deltaMax) || (fabs(stratVals[0][1] - stratVals[1][1]) > deltaMax)) {
                //strcpy(description, "Let's take turns.|");
                description[0] = 5;
                descriptionLen = 1;
            }
            else {
                //sprintf(description, "Let me have %.0lf points each round.|", stratVals[0][_me]);
                description[0] = 6;
                descriptionLen = 1;
            }
            if (tipo == LEADER) {
                //strcat(description, "Otherwise, I'll make you pay in future rounds.|");
                description[descriptionLen] = 7;
                descriptionLen++;
            }
            else if (tipo == LEADER2) {
                //strcat(description, "Otherwise, I'll make you pay in future moves.|");
                description[descriptionLen] = 8;
                descriptionLen++;
            }
        }
    }
    else {
        sprintf(dcode, "Follow_%i", counter);    
        if (fabs(barR[0] - barR[1]) < deltaMax) {
            //sprintf(description, "Let's cooperative with each other.|");
            description[0] = 9;
            descriptionLen = 1;
            if ((fabs(stratVals[0][0] - stratVals[1][0]) > deltaMax) || (fabs(stratVals[0][1] - stratVals[1][1]) > deltaMax)) {
                //strcat(description, "We can do this by taking turns receiving|the higher payout.|");
                description[1] = 3;
                descriptionLen = 2;
            }
        }
        else {
            if ((fabs(stratVals[0][0] - stratVals[1][0]) > deltaMax) || (fabs(stratVals[0][1] - stratVals[1][1]) > deltaMax)) {
                //strcpy(description, "Let's take turns.|");
                description[0] = 5;
                descriptionLen = 1;
            }
            else {
                description[0] = 6;
                descriptionLen = 1;
                //sprintf(description, "Let me have %.0lf points each round.|", stratVals[0][_me]);
                //strcpy(description, "Give me what I want.|");//I insist on getting what I want.|");
            }
        }
        //else
        //    strcpy(description, "Let's take turns.|");
    }
    
    hosed = false;
    completedCycleCount = 0;
    seguida = 0;
    goodnessIndex = 0;
    gotGuilty = false;
    //describeExpert = false;
}

Expert::~Expert() {
    if (tipo == PREVENT)
        delete preventer;
}

bool Expert::congruent(MarkovGame *mg, int lastProposed[2][2]) {
    int acts[2][2];
    
    acts[0][0] = mg->states[0]->qsets[strat[0]]->correctA(0);
    acts[0][1] = mg->states[0]->qsets[strat[0]]->correctA(1);
    acts[1][0] = mg->states[0]->qsets[strat[1]]->correctA(0);
    acts[1][1] = mg->states[0]->qsets[strat[1]]->correctA(1);

    if (((acts[0][0] == lastProposed[0][0]) && (acts[0][1] == lastProposed[0][1]) && (acts[1][0] == lastProposed[1][0]) && (acts[1][1] == lastProposed[1][1])) ||
        ((acts[1][0] == lastProposed[0][0]) && (acts[1][1] == lastProposed[0][1]) && (acts[0][0] == lastProposed[1][0]) && (acts[0][1] == lastProposed[1][1]))) {
        
        return true;
    }
    
    // no congruence found, but maybe there are ties
    if (mg->states[0]->qsets[strat[0]]->amongBest(lastProposed[0]) && mg->states[0]->qsets[strat[1]]->amongBest(lastProposed[1])) {
        printf("best among1: (%i, %i) then (%i, %i); from ", lastProposed[0][0], lastProposed[0][1], lastProposed[1][0], lastProposed[1][1]);
        //exit(1);
        
        mg->states[0]->qsets[strat[0]]->setTie(lastProposed[0]);
        mg->states[0]->qsets[strat[1]]->setTie(lastProposed[1]);
        
        printf("\n");
        
        return true;
    }
    else if (mg->states[0]->qsets[strat[1]]->amongBest(lastProposed[0]) && mg->states[0]->qsets[strat[0]]->amongBest(lastProposed[1])) {
        printf("best among2: (%i, %i) then (%i, %i); from ", lastProposed[0][0], lastProposed[0][1], lastProposed[1][0], lastProposed[1][1]);
        //exit(1);

        mg->states[0]->qsets[strat[1]]->setTie(lastProposed[0]);
        mg->states[0]->qsets[strat[0]]->setTie(lastProposed[1]);
        
        printf("\n");
        
        return true;
    }
    
    return false;
}

int Expert::Move(State *s, double previousPayoffs[2], int _me) {
    
    if (tipo == MINIMAX) {
        return generateAction(s->mm[_me]->ms, s->numActions[_me]);
    }
    else if (tipo == BResp) {
        int i, j;

        /*for (i = 0; i < s->numActions[_me]; i++) {
            printf("%.2lf\t", s->Qbr[i]);
        }
        printf("\nbrVal = %.2lf\n\n", s->Vbr);*/

        int bInd = 0;
        for (i = 1; i < s->numActions[_me]; i++) {
            if (s->Qbr[i] > s->Qbr[bInd])
                bInd = i;
            else if ((s->Qbr[i] == s->Qbr[bInd]) && (rand() % 2))
                bInd = i;
        }

        if (explorar) {
            // add random exploration
            double num = rand() / (double)RAND_MAX;
            double sum = 0.0;
            for (i = 0; i < s->numActions[0]; i++) {
                for (j = 0; j < s->numActions[1]; j++) {
                    sum += s->cuentas[i][j];
                }
            }
            double xplor = 1.0 / (10.0 + sum/10.0);
            if (xplor < 0.05)
                xplor = 0.05;
            
            if (num <= xplor) {
                bInd = rand() % s->numActions[_me];
            }
        }
        
        return bInd;
    }
    else if (tipo == FOLLOWER) {
        if (currentStep < 0) {
            //printf("move current step called\n");
            getCurrentStep(previousPayoffs, _me, false);
        }
        
        //double probs[2*NUMACTIONS];
        //s->getProbs_regular(probs, _me);
        //return s->qsets[strat[currentStep]]->highestAction(_me, probs);
        //printf("%i: ", strat[currentStep]);
        return s->qsets[strat[currentStep]]->correctA(_me);
    }
    else if (tipo == LEADER) {
        if (guilt > 0.00001) {// if guilt, punish him
            return generateAction(s->attack[_me]->ms, s->numActions[_me]);
        }
        
        if (currentStep < 0) {
            //printf("move current step called\n");
            getCurrentStep(previousPayoffs, _me, false);
        }

        double probs[2*NUMACTIONS];
        s->getProbs_regular(probs, _me);
        int a = s->qsets[strat[currentStep]]->highestAction(_me, probs);
        //printf("%i: picked %i\n", _me, a); fflush(stdout);
        
        return a;
    }
    else if (tipo == LEADER2) {
        //printf("@%.1lf@", guilt);
        
        if ((guilt > 0.00001) || betrayed) {// if guilt, punish him
            //printf("PUNISH: %lf %lf %lf", s->attack[_me]->ms[0], s->attack[_me]->ms[1], s->attack[_me]->ms[2]);
            printf("************** he is guilty ... \n");
            return generateAction(s->attack[_me]->ms, s->numActions[_me]);
        }
        
        if (currentStep < 0) {
            //printf("move current step called\n");
            getCurrentStep(previousPayoffs, _me, false);
        }
        
        //double probs[2*NUMACTIONS];
        //s->getProbs_regular(probs, _me);
        //return s->qsets[strat[currentStep]]->highestAction(_me, probs);

        //printf("%i: ", strat[currentStep]);
        return s->qsets[strat[currentStep]]->correctA(_me);
    }
    else if (tipo == PREVENT) {
        return preventer->mover(s);
    }
    
    
    return 0;
}

int Expert::generateAction(double *v, int numActs) {
    int i;
    
    double num = rand() / (double)RAND_MAX;
    double sum = 0.0;
    for (i = 0; i < numActs; i++) {
        sum += v[i];
        if (num <= sum)
            return i;
    }
    
    printStrategy(v, numActs);
    
    printf("Expert: generate action failed: %lf\n", num);
    
    return numActs-1;
}

void Expert::printStrategy(double *v, int numActs) {
    int i;
    
    for (i = 0; i < numActs; i++) {
        printf("%.2lf ", v[i]); fflush(stdout);
    }
    printf("\n");
}

void Expert::Update(double dollars[2], int _me, double _aspiration, bool _heldTrue, double aspiration) {
    
    if (_heldTrue) {
        seguida ++;
        if (seguida == 2) {
            seguida = 0;
            completedCycleCount ++;
            goodnessIndex -= 0.2;
            if (goodnessIndex < 0.0)
                goodnessIndex = 0.0;
        }
    }
    else {
        seguida = 0;
    }
    
    //printf("completedCycleCount = %i\n", completedCycleCount);
    
    // update observed performance
    usage++;
    double betita = 1.0 / usage;
    double lambda = 0.9;
    if (betita <= (2.0 * (1.0 - lambda)))
        betita = 2.0 * (1.0 - lambda);
    if (betita > 0.1)
        betita = 0.1;
    vu = betita * dollars[_me] + (1.0 - betita) * vu;
    
    if (tipo == FOLLOWER) {
        //printf("follower update\n");
        if (_heldTrue) {
            double diff = stratVals[currentStep][_me] - dollars[_me];
            if (diff < deltaMax) {
                comms->logEvent(MSG_EVENT, 6, "");
            }
            else {
                //printf("weird event 11 - 1\n");// exit(1);
            
                comms->logEvent(MSG_EVENT, 11, "");
            }
        }
        else if (dollars[1-_me] >= stratVals[currentStep][1-_me]) {
            comms->logEvent(MSG_EVENT, 5, "");
        }
        else {
            //printf("weird event 11 - 2\n");// exit(1);
            
            comms->logEvent(MSG_EVENT, 11, "");
        }
        
        getCurrentStep(dollars, _me, _heldTrue);
    }
    
    if ((tipo == LEADER) || (tipo == LEADER2)) {
        double epsilon = 0.01;

        //printf("leader update\n");

        double diff = stratVals[currentStep][_me] - dollars[_me];
        if (diff < 0)
            diff = 0.0;
        double value = (dollars[_me] + (stratVals[1-currentStep][_me] - diff)) / 2.0;
        if ((guilt < 0.00001) && _heldTrue) {
            guilt = 0.0;
            //printf("I got to here: heldTrue = %i;!!!!!\n", (int)(_heldTrue));
            
            if (diff < deltaMax) {
                comms->logEvent(MSG_EVENT, 6, "");
            }
            else {
                comms->logEvent(MSG_EVENT, 5, "");
            }
        }
        else if ((guilt < 0.00001) && (value >= aspiration)) {
            //printf("weird event 9\n"); exit(1);
            
            guilt = 0.0;
            comms->logEvent(MSG_EVENT, 9, "");
        }
        else {
            double delta = 0.0;

            //double vexpected = barR[1-_me];
            double vexpected = stratVals[currentStep][1-_me];
            double guiltUp = guilt + dollars[1-_me] - vexpected + deltaMax;
            
            //bool posGuilt = true;
            //if (guilt < 0.0001)
            //    posGuilt = false;
            
            //printf("guiltUp = %lf (%lf)\n", guiltUp, guilt);
            //printf("********here***********\n"); fflush(stdout);
            if (guilt < 0.00001) {
                if (guiltUp > 0.0001) {
                    comms->logEvent(MSG_EVENT, 5, "");
                    
                    //starting = true;
                    delta = deltaMax;
                }
                
                guilt += dollars[1-_me] - vexpected + delta;
            }
            else {
                //printf("guilt = %lf\n", guilt);
                comms->logEvent(MSG_EVENT, 10, "");
                
                //printf("change in guilt: %lf\n", dollars[1-_me] - barR[1-_me]);
                guilt += dollars[1-_me] - barR[1-_me];

                if ((dollars[1-_me] - barR[1-_me]) < 0.0) {
                    //printf("\tPUNISHMENT DELIVERED: %lf\n", guilt);
                    comms->logEvent(MSG_EVENT, 4, "");
                }
                else {
                    //printf("\tI'M STILL MAD AT YOU: %lf\n", guilt);
                    comms->logEvent(MSG_ASSESSMENT, 6, "0 ");
                }
            }
            //double vexpected = (stratVals[0][1-_me] + stratVals[1][1-_me]) / 2.0;
            
            
            if (guilt < 0.000001) {
                if (gotGuilty) {
                    currentStep = rand() % 2;
                    gotGuilty = false;
                    comms->logEvent(MSG_ASSESSMENT, 4, "0 ");
                }
                
                //else
                if (tipo != LEADER2) {
                    //printf("weird event 11 - 3\n");// exit(1);
            
                    comms->logEvent(MSG_EVENT, 11, "");
                }
                guilt = 0.0;
            }
            //else if (!posGuilt) {
            else {
                comms->logEvent(MSG_ASSESSMENT, 3, "0 ");
            }
        }
 
        printf("guilt = %lf\n", guilt);
        
        //printf("guilt = %.1lf\n", guilt);
        
        //if (currentStep >= 0) {
        if (guilt < 0.00001)
            getCurrentStep(dollars, _me, _heldTrue);
        //}
    }
    else if (tipo == PREVENT) {
        preventer->rounderUpdate();
        
        if ((dollars[_me] < (barR[_me]-deltaMax)) && (dollars[1-_me] < (barR[1-_me]-deltaMax))) {
            comms->logEvent(MSG_EVENT, 5, "");
        }
        else if ((dollars[_me] > (barR[_me]-deltaMax)) && (dollars[1-_me] > (barR[1-_me]-deltaMax))) {
            comms->logEvent(MSG_EVENT, 6, "");
        }
    }
    //printf("guilt = %.1lf\n", guilt);
    
    betrayed = false;
    favored = false;
}

void Expert::updateMove(State *sprime, double rPayout[2], int me, int acts[2], double aspiration, char message[1024]) {
    //printf("-"); fflush(stdout);
    hosed = false;
    
    if (tipo == PREVENT)
        preventer->moverUpdate(acts, rPayout);
    
    else if ((tipo == FOLLOWER) || ((tipo == LEADER) && (guilt < 0.00001))) {
        double gettin = rPayout[me] + sprime->qsets[strat[currentStep]]->correctM(me);
        double shoulda = stratVals[currentStep][me];

        double diff = stratVals[currentStep][me] - gettin;
        if (diff < 0)
            diff = 0.0;
        double value = (gettin + (stratVals[1-currentStep][me] - diff)) / 2.0;
        
        if ((((gettin+0.00001) < shoulda) && (value < aspiration)) && !betrayed) {
        //if (((gettin + 0.00001) < shoulda) && !betrayed) {
            char buf[1024];
            sprintf(buf, "%s.|", message);
            
            //strcpy(buf, "You betrayed me.|");
            //sprintf(buf, "You betrayed me.  I expected you to|%s.|", message);
            
            strcat(message, " $");
            comms->logEvent(MSG_EVENT, 2, "");
            comms->logEvent(MSG_STRING, 1, message);
            hosed = true;
            
            goodnessIndex += 1.0;
            
            betrayed = true;
        }
        else if (((gettin - 0.1) > shoulda) && !favored) {
            char buf[1024];
            
            //strcpy(buf, "That is nicer than I expected.|");
            
            comms->logEvent(MSG_EVENT, 1, "");
            
            favored = true;
        }
    }
    else if ((tipo == LEADER2) && (guilt < 0.00001)) {
        //printf("t"); fflush(stdout);
        
        double gettin = rPayout[me] + sprime->qsets[strat[currentStep]]->correctM(me);
        double shoulda = stratVals[currentStep][me];

        //printf("&%.0lf,%.0lf&", gettin, shoulda); fflush(stdout);
        
        double diff = stratVals[currentStep][me] - gettin;
        if (diff < 0)
            diff = 0.0;
        double value = (gettin + (stratVals[1-currentStep][me] - diff)) / 2.0;

        
        //printf("a"); fflush(stdout);
        //printf("gettin (%.2lf) vs. shoulda (%.2lf); value = %lf\n", gettin, shoulda, value);
        
        if ((((gettin+0.00001) < shoulda) && (value < aspiration)) && !betrayed) {
            // check to see if he has benefited
            double gettin2 = rPayout[1-me] + sprime->qsets[strat[currentStep]]->correctM(1-me);
            double shoulda2 = stratVals[currentStep][1-me];
            //printf("gettin2 (%.2lf) vs. shoulda2 (%.2lf)\n", gettin2, shoulda2);

            comms->logEvent(MSG_EVENT, 2, "");
            
            //printf("n<%s>", message); fflush(stdout);
            
            if (shoulda2 < (gettin2+0.00001)) {//+deltaMax)) {
                char buf[1024];
                sprintf(buf, "%s.|", message);
            
                //sprintf(buf, "You jerk.  I trusted you to %s.|", message);
                
                strcat(message, " $");
                comms->logEvent(MSG_EVENT, 3, "");
                comms->logEvent(MSG_STRING, 1, message);
                comms->logEvent(MSG_ASSESSMENT, 3, "0 ");
                hosed = true;
                gotGuilty = true;
                
                goodnessIndex += 1.0;

                //sprintf(buf, "I'm going to teach you a lesson you won't forget.|");
                
                betrayed = true;
            }
            //printf("e"); fflush(stdout);
        }
        else if (betrayed) {
            //printf("b"); fflush(stdout);
            
            //double gettin2 = rPayout[1-me] + sprime->qsets[strat[currentStep]]->correctM(1-me);
            double gettin2;
            if (me == 0)
                gettin2 = rPayout[1-me] + sprime->qsets[0]->correctM(1-me);
            else
                gettin2 = rPayout[1-me] + sprime->qsets[sprime->numQsets-1]->correctM(1-me);
            
            double shoulda2 = stratVals[currentStep][1-me];
            //printf("gettin2 (%.2lf) vs. shoulda2 (%.2lf)\n", gettin2, shoulda2);
            //printf("%i: toGo: %lf; sofar: %lf\n", currentStep, sprime->qsets[strat[currentStep]]->correctM(1-me), rPayout[1-me]);
            if (gettin2 < (shoulda2 - deltaMax)) {
                char buf[1024];
                
                //strcpy(buf, "Take that!|");
                
                comms->logEvent(MSG_EVENT, 4, "");
                
                betrayed = false;
            }
        }
        
        //printf("r"); fflush(stdout);
    }
}

void Expert::reset(double previousPayoffs[2], int _me) {
    guilt = 0.0;
    culpable = false;
    betrayed = false;
    favored = false;
    
    currentStep = -1;
    gotGuilty = false;
    //printf("reset getCurrentStep\n");
    getCurrentStep(previousPayoffs, _me, false);
}

void Expert::getCurrentStep(double previousPayoffs[2], int _me, bool _heldTrue) {
    double epsilon = deltaMax;
    int i;
    
    //printf("%i: in currentStep = %i\n", _me, currentStep);
    
    if (currentStep < 0) {
        currentStep = -1;
        for (i = 0; i < 2; i++) {
            if ((previousPayoffs[0] >= (stratVals[i][0]-epsilon)) && (previousPayoffs[1] >= (stratVals[i][1]-epsilon))) {
                currentStep = 1-i;
                //printf("************ currentStep = %i\n", currentStep);
                break;
            }
        }
        
        if (currentStep < 0) {
            //printf("************** picking currentStep randomly\n");
            currentStep = rand() % 2;
        }
    }
    
    
    
    else {
        if (_heldTrue) {
            //printf("heldTrue?\n");
            //printf("************** switch 1\n");
            currentStep = 1-currentStep;
        }
        else if ((previousPayoffs[0] >= (stratVals[i][0]-epsilon)) && (previousPayoffs[1] >= (stratVals[i][1]-epsilon))) {
            //printf("************** switch 2\n");
            currentStep = 1-i;
            //currentStep = rand() % 2;
        }
        else {
            //printf("************** picking currentStep randomly\n");
            currentStep = rand() % 2;
        }
    }
}

void Expert::currentStepMessage(int _me) {
    if (strat[0] != strat[1]) {
        //printf("que pasa, then?? (%.2lf, %.2lf)\n", stratVals[currentStep][_me], stratVals[1-currentStep][_me]);
        char buf[1024];
        if (stratVals[currentStep][_me] > stratVals[1-currentStep][_me]) {
            //sprintf(buf, "It's my turn now.  You'll get the higher payout|next time.|");//, stratVals[currentStep][_me], stratVals[1-currentStep][1-_me]);
            
            sprintf(buf, "%i ", currentStep);
            comms->logEvent(MSG_ASSESSMENT, 1, buf);
        }
        else if (stratVals[currentStep][1-_me] > stratVals[1-currentStep][1-_me]) {
            //sprintf(buf, "It's your turn to get the higher payout.|");//, stratVals[currentStep][1-_me]);

            sprintf(buf, "%i ", currentStep);
            comms->logEvent(MSG_ASSESSMENT, 2, buf);
        }
    }
}

// I need to add to this; if he isn't playing according to the plan, but I had guilt, then he could be teaching me
bool Expert::showedLeadership(int hacts[20], double payoffHistory[2][10], int _me) {
    //return true;
    
    //printf("<%i, %i: %i and %i> ", strat[0], strat[1], hacts[strat[0]+2], hacts[strat[1]+2]);
    if ((hacts[strat[0]+2] == 1) || (hacts[strat[1]+2] == 1))
        return true;

    // check if i had guilt (in the last 1, 2, or 3 rounds, I did better than I should have); if so, set to true
    //printf("<p2 = %.2lf; p1 = %.2lf>", payoffHistory[_me][1] + payoffHistory[_me][2], payoffHistory[_me][1]);
    if (((payoffHistory[_me][1] + payoffHistory[_me][2]) > (2 * (barR[_me] - deltaMax))) || (payoffHistory[_me][1] > (barR[_me] - deltaMax))) {
        // i did better than I should have; he could have decided to punish me
        return true;
    }
/*
    // maybe we are just eating up each other
    if (((payoffHistory[1-_me][1] + payoffHistory[1-_me][2]) > (2 * (barR[1-_me] - deltaMax))) || (payoffHistory[1-_me][1] > (barR[1-_me] - deltaMax))) {
        // i did better than I should have; he could have decided to punish me
        return true;
    }
*/    
    
    return false;
}

bool Expert::showedLeadership(MarkovGame *mg, int actionHistory[2][10], int _me) {
    int i;
    
    int correctActions[2][2];

    for (i = 0; i < 2; i++) {
        correctActions[i][0] = mg->states[0]->qsets[strat[0]]->correctA(i);
        correctActions[i][1] = mg->states[0]->qsets[strat[1]]->correctA(i);
    }
    
    int count = 0, encountered = 0;
    for (i = 1; i < 10; i++) {
        if ((actionHistory[0][i] == correctActions[0][0]) && (actionHistory[1][i] == correctActions[1][0])) {
            encountered ++;
            // we played strat[0] in round i; see if he played correctly in the next round
            if (actionHistory[1-_me][i-1] != correctActions[1-_me][1])
                return false;
            count ++;
        }
        else if ((actionHistory[0][i] == correctActions[0][1]) && (actionHistory[1][i] == correctActions[1][1])) {
            encountered ++;
            // we played strat[1] in round i; see if he played correctly in the next round
            if (actionHistory[1-_me][i-1] != correctActions[1-_me][0])
                return false;
            count ++;
        }
        
        if ((count > 1) || ((strat[0] == strat[1]) && (count == 1)))  // just consider the last two cases (at most)
            return true;
    }
    
    // what about when we've never encountered the target solution recently?
    //if (encountered > 0)
    //    return true;
    
    if (encountered > 0)
        return true;
    else {
        // has he recently played an action that corresponds to his part?
        for (i = 1; i < 5; i++) {
            if ((actionHistory[1-_me][i] == correctActions[1-_me][0]) || (actionHistory[1-_me][i] == correctActions[1-_me][1]))
                return true;
        }
    }
    
    
    return false;
}

