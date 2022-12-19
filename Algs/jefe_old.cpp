#include "jefe.h"


extern bool doTheSpecify, yaDicho, yaDicho2;

//extern void addMessage(char msg[1024], int _origin, int _lineHeight);
//extern char blank[1024];

//extern char nomlogger[1024];

jefe::jefe() {
    printf("incomplete jefe constructor\n");
    exit(1);
}

jefe::jefe(const char *nombre, int _me, MarkovGame *_mg, double _lambda, int addport) {
    int i;
    
    me = ctm.me = _me;
    lambda = _lambda;

    outputTipo = TEXT_TIPO;
    comms = new CommAgent(outputTipo, 4000+addport, me);
    
    t = 0;
    experto = -1;
    cycled = false;
    
    cycleFull = true;
    
    ab = new mega(_mg, me, false, comms, lambda);
    ab->computeAllExperts(lambda, false, true);
    
    ab->determineHighLevelActions();
    
    satisficingExperts = new bool[ab->numExperts];
    for (i = 0; i < ab->numExperts; i++)
        satisficingExperts[i] = true;
    numSatExperts = 1;
    
    double highval = -99999, lowval = 99999;
    for (i = 3; i < ab->numExperts; i++) {
        if (highval < ab->xprts[i]->barR[me])
            highval = ab->xprts[i]->barR[me];
        if (lowval > ab->xprts[i]->barR[me])
            lowval = ab->xprts[i]->barR[me];
    }
    //printf("highval = %lf; lowval = %lf\n", highval, lowval);
    
    //if (!strcmp(nombre, "S++")) {
    //    learner = new a(_me, _lambda, ab->numExperts, highval, lowval, _mg->states[0]->mm[me]->mv);//   ab->xprts[0]->barR[me]);
    //    cycleFull = false;
    //}
    if (!strcmp(nombre, "exp3w") || !strcmp(nombre, "exp3"))
        learner = new Exp3(me, (int)_lambda, 0.99, ab->numExperts, highval, 0.0);
    else if (!strcmp(nombre, "eee"))
        learner = new eee(_me, _lambda, ab->numExperts);
    //    else if (!strcmp(nombre, "ucbw++"))
    //        learner = new ucb(_me, _lambda, ab->numExperts);
    else {
        printf("expert learner not found\n");
        exit(1);
    }
    
    //setAspirationHigh();
    setAspirationFolkEgal();
    //setAspirationHighestEnforceable();
	mu = 0.0;
    
    alwaysMM = false;
    permissibleLoss = 500.0;
    
    lowAspiration = 1.0;
    
    lastState = NULL;
    rPayout[0] = rPayout[1] = 0.0;
    
    //strcpy(myMessage, "");
    
    //realbad = false;
    previousPayoffs[0] = previousPayoffs[1] = lastSegmentScore = -99999;
    for (i = 0; i < 10; i++) {
        payoffHistory[0][i] = -1.0;
        payoffHistory[1][i] = -1.0;
    }
    
    
    oldExpert = -1;
    
    tau = 0;
    latestJacts[0][0] = latestJacts[1][0] = -1;
    
    somethingBetter = false;
    
    newSelection();
    
    //char buf[1024];
    //sprintf(buf, "%s_%i.txt", nomlogger, me);
    //fpExpert = fopen(buf, "w");
    
    doTheSpecify = true;
    alreadyForgiven = false;
    yaDicho = yaDicho2 = false;
    whipeoutguilt = false;
    
    listenCount = listened = 5;
    tellCount = honest = 5;
    
    listeningBetrayalCount = totalBetrayalCount = oldBetrayalCount = 0;
    onSilentTreatment = false;    
    
    notfair = false;
    distrustful = false;
    
    comms->printRobotMessages(false, true);
}

jefe::~jefe() {
    delete learner;
    delete satisficingExperts;
    delete ab;
    delete comms;
    
    //fflush(fpExpert);
    //fclose(fpExpert);
}

double jefe::processCheapTalk(char buf[10000]) {
    //printf("Received communications: %s\n", buf);
    
    if (ctm.update(buf, ab->mg->states[0]->numActions) && !alreadyForgiven) {
        //V_listen = V_thresh_listen;
        alreadyForgiven = true;
    }

    // see if I should change my strategy
    //printf("_______________set the dicho = false\n");
    yaDicho = yaDicho2 = false;
    
    printf("check if there is a new good proposal ... ");
    
    if (preemptiveCT()) {
        printf("yes!!!\n");
        doTheSpecify = false;
        newSelection();
        cycled = false;
        ctm.tsProposal = 1;
        
        if (oldExpert == experto) {
            if (ab->xprts[experto]->guilt < 0.00001)
                ab->xprts[experto]->currentStepMessage(me);
        }
    }
    else {
        printf("No.\n");
    }
    printf("end preemitive\n");
    
    return ctm.elapsed;
}

int jefe::Move(GameEngine *ge) {
    
    
    //if (cycled) {
    //    newSelection();
    //    cycled = false;
    //}
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    lastState = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);
    /*
     int i, j;
     for (i = 0; i < lastState->numActions[me]; i++) {
     printf("%.2lf\t", lastState->Qbr[i]);
     }
     printf("\nbrVal = %.2lf\n\n", lastState->Vbr);
     */
    
    
    int b = ab->xprts[experto]->Move(lastState, previousPayoffs, me);
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);
    
    corA = -1;
    if ((ab->xprts[experto]->tipo == FOLLOWER) || (ab->xprts[experto]->tipo == LEADER) || (ab->xprts[experto]->tipo == LEADER2)) {
        int c = lastState->qsets[ab->xprts[experto]->strat[ab->xprts[experto]->currentStep]]->correctA(1-me);
        corA = ge->convertIt(c, lastValidActs[me], NUMACTIONS*2);
    }
    
    fprintf(fpExpert, "%i: %s\n", t, ab->xprts[experto]->dcode);
    
    return a;
}

void jefe::newSelection() {
    printf("entering newSelection()\n"); fflush(stdout);
    
    oldExpert = experto;
    
    resetCycle();
    
    determineSatisficingExperts();
    
    int i;
    numSatExperts = 0;
    for (i = 0; i < ab->numExperts; i++) {
        if (satisficingExperts[i])
            numSatExperts ++;
    }
    
    if (t > 0) {
        //printf("****************** got to here\n"); fflush(stdout);
        if (learner->rho < 1.0) // if the learner is not fully happy
            comms->logEvent(MSG_EVENT, 7, "");
        else
            comms->logEvent(MSG_EVENT, 8, "");
    }
    
    comms->logEvent(MSG_ROUND, t, "");
    
    experto = learner->select(satisficingExperts);
    printf("experto = %i\n", experto);
    
    if ((experto != oldExpert) && (t > 0)) {
        comms->logEvent(MSG_EXPERT, getTypeNumber(experto), "");
    }    
    
    if ((experto > 1) && (experto != oldExpert)) {
        
        ab->xprts[experto]->reset(previousPayoffs, me);
        ab->xprts[experto]->currentStepMessage(me);
        
        int a[2][2];
        double probs[2*NUMACTIONS];
        
        //printf("about to create the message from %i, %i\n", ab->mg->states[0]->qsets[ab->xprts[experto]->strat[0]]->tieIndex, ab->mg->states[0]->qsets[ab->xprts[experto]->strat[1]]->tieIndex);
        a[0][me] = ab->mg->states[0]->qsets[ab->xprts[experto]->strat[0]]->correctA(me);
        a[0][1-me] = ab->mg->states[0]->qsets[ab->xprts[experto]->strat[0]]->correctA(1-me);
        a[1][me] = ab->mg->states[0]->qsets[ab->xprts[experto]->strat[1]]->correctA(me);
        a[1][1-me] = ab->mg->states[0]->qsets[ab->xprts[experto]->strat[1]]->correctA(1-me);
    
        char actionString[1024], lab1[1024], lab2[1024];
        labelConv(ab->mg->actionLabels[a[0][1]], lab1);
        labelConv(ab->mg->actionLabels[a[1][1]], lab2);
        sprintf(actionString, "%s%s %s%s", ab->mg->actionLabels[a[0][0]], lab1, ab->mg->actionLabels[a[1][0]], lab2);

        comms->logEvent(MSG_JACTION, 0, actionString);
    }
    else if ((experto == 0) || (experto == 1)) {
        if (oldExpert >= 0) {
            char buf[1024];
            if (ab->xprts[oldExpert]->guilt > 0.0001) {
            }
            else {
                if (somethingBetter) {
                    comms->logEvent(MSG_ASSESSMENT, 5, "0 ");
                }
            }
        }
    }
}

void jefe::labelConv(char str[1024], char conv[1024]) {
    if (ab->mg->states[0]->numActions[0] == 2) {
        if (!strcmp(str, "A"))
            strcpy(conv, "C");
        else if (!strcmp(str, "B"))
            strcpy(conv, "D");
    }
    else {
        if (!strcmp(str, "A"))
            strcpy(conv, "D");
        if (!strcmp(str, "B"))
            strcpy(conv, "E");
        else if (!strcmp(str, "C"))
            strcpy(conv, "F");
    }
}

int jefe::getTypeNumber(int exprt) {
    double delta = ab->xprts[experto]->deltaMax;

    if (ab->xprts[exprt]->tipo == MINIMAX)
        return 0;
    else if (ab->xprts[exprt]->tipo == BResp)
        return 1;
    else if (ab->xprts[exprt]->tipo == PREVENT)
        return 2;
    else if (ab->xprts[exprt]->tipo == LEADER2) {
        if (fabs(ab->xprts[exprt]->stratVals[0][0] - ab->xprts[exprt]->stratVals[1][0]) < delta) {
            if (fabs(ab->xprts[exprt]->barR[0] - ab->xprts[exprt]->barR[1]) > delta*2.0)
                return 5;
            else
                return 3;
        }
        else
            return 4;
    }
    else if (ab->xprts[exprt]->tipo == FOLLOWER) {
        if (fabs(ab->xprts[exprt]->stratVals[0][0] - ab->xprts[exprt]->stratVals[1][0]) < delta) {
            if (fabs(ab->xprts[exprt]->barR[0] - ab->xprts[exprt]->barR[1]) > delta*2.0)
                return 8;
            else
                return 6;
        }
        else
            return 7;
    }
    
    printf("ERROR: Expert not found\n");
    exit(1);
    
    return -1;
}

int jefe::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    int acts[2];
    ge->deduceActions(acts, actions, lastValidActs);
    
    //if (me == 0)
    //    printf("r = %lf\n", ge->getCurrentReward(0));
    
    rPayout[0] += dollars[0];
    rPayout[1] += dollars[1];
    
    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    State* sprime = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);
    
    ab->moveUpdate(lastState, sprime, acts, experto, rPayout, learner->aspiration, corA);
    
    //comms->printRobotMessages(onSilentTreatment, false);
    comms->printRobotMessages(false, false);  // for now, no silent treatment
    
    return 0;
}


int jefe::roundUpdate() {
    
    int i;
    for (i = 9; i > 0; i--) {
        payoffHistory[0][i] = payoffHistory[0][i-1];
        payoffHistory[1][i] = payoffHistory[1][i-1];
    }
    previousPayoffs[0] = payoffHistory[0][0] = rPayout[0];
    previousPayoffs[1] = payoffHistory[1][0] = rPayout[1];
    
    // update the payoffs for this "cycle"
	R += rPayout[me];
    Rnot += rPayout[1-me];
	mu += rPayout[me];
    
    //printf("Rtally = %lf\n", R);
    
    ab->roundUpdate(rPayout);
    latestJacts[0][tau+1] = ab->highestInd[0];
    latestJacts[1][tau+1] = ab->highestInd[1];
    
    //for (i = 0; i < tau+2; i++)
    //    printf("%i, %i\n", latestJacts[0][i], latestJacts[1][i]);
    //printf("\n"); fflush(stdout);
    
    //printf("Rresult (%i): %.2lf, %.2lf\n", me, rPayout[0], rPayout[1]);
    bool heldTrue = false;
    if ((ab->xprts[experto]->tipo == FOLLOWER) || (ab->xprts[experto]->tipo == LEADER) || (ab->xprts[experto]->tipo == LEADER2)) {
        if (ab->lastActions[1-me][ab->xprts[experto]->strat[ab->xprts[experto]->currentStep]+2] == 1) {
            //printf("held true\n");
            heldTrue = true;
        }
    }
    ab->xprts[experto]->Update(rPayout, me, learner->aspiration, heldTrue, learner->aspiration);
    
    rPayout[0] = rPayout[1] = 0.0;
    
	t++;
	tau ++;
    
    //if ((tau >= ab->xprts[experto]->cycleLen) && (ab->xprts[experto]->guilt == 0))
    if (!cycleFull) {
        //printf("this question ... ");
        if (repeatJaction()) {
            //printf("yes\n");
            cycled = true;
        }
        //printf("no\n");
    }
    else {
        //printf("%i tau = %i; cLen = %i\n", me, tau, ab->xprts[experto]->cycleLen);
        if (tau >= ab->xprts[experto]->cycleLen)
            cycled = true;
    }
    
    //strcpy(myMessage, ab->xprts[experto]->currentMessage);
    int myMessage = ab->xprts[experto]->currentMessage;
    
	if (cycled) {
        lastSegmentScore = R / tau;
/*        if (R / tau < ab->mg->startStates[0]->mm[me]->mv) {
            if (ab->xprts[experto]->tipo == FOLLOWER)
                realbad = true;
        }
        else {
            if ((R / tau >= learner->aspiration) && (ab->xprts[experto]->guilt < 0.00001)) {
                //strcpy(myMessage, "I'm happy with this.|");
                myMessage = 20;
            }
            
            realbad = false;
        }
*/
        //    strcpy(myMessage, "I think we could do better than this.  I might try something different.");
        
		learner->update(R / tau, tau);
		//if (((ab->mg->startStates[0]->mm[me]->mv * t) - mu) > permissibleLoss) {
        //    printf("%i: I'm losing!!!!! mu = %lf; mv*t = %lf; permissibleLoss = %lf\n", me, mu, ab->mg->startStates[0]->mm[me]->mv * t, permissibleLoss);
		//	alwaysMM = true;
        //}
        //printf("aspiration_%i=%.2lf\n", me,learner->aspiration);
    }
    
    if (learner->aspiration < lowAspiration)
        lowAspiration = learner->aspiration;
    
    //printf("a%i=%lf>", me,learner->aspiration);
    //if (strcmp(myMessage,"")) {
    
    if (cycled) {
        // check to see if we both could do better
        somethingBetter = isSomethingBetter(R/tau, Rnot/tau);
        
        newSelection();
        cycled = false;
        
        if (((R/tau) > (ab->xprts[experto]->barR[me]-ab->xprts[experto]->deltaMax)) || ((Rnot/tau) > (ab->xprts[experto]->barR[1-me]-ab->xprts[experto]->deltaMax))) {
            somethingBetter = false;
        }
        
        if (oldExpert == experto) {
            //if (myMessage > 0) {
                //addMessage(blank, 5, -1);
                //addMessage(myMessage, 15, me);
//                comms->postMessage(myMessage);
            //    if (ab->xprts[experto]->describeExpert) {
//                    for (int l = 0; l < ab->xprts[experto]->descriptionLen; l++) {
//                        comms->postMessage(ab->xprts[experto]->description[l]);
//                    }
                    
            //        ab->xprts[experto]->describeExpert = false;
            //    }
            //}
            if (ab->xprts[experto]->guilt < 0.00001)
                ab->xprts[experto]->currentStepMessage(me);
        }
    }
    else {
        //if (myMessage > 0) {
            //addMessage(blank, 5, -1);
            //addMessage(myMessage, 15, me);
//            comms->postMessage(myMessage);
        //    if (ab->xprts[experto]->describeExpert) {
//                for (int l = 0; l < ab->xprts[experto]->descriptionLen; l++) {
//                    comms->postMessage(ab->xprts[experto]->description[l]);
//                }
                
        //        ab->xprts[experto]->describeExpert = false;
        //    }
        //}
        if (ab->xprts[experto]->guilt < 0.00001)
            ab->xprts[experto]->currentStepMessage(me);
    }
    
    //comms->printRobotMessages(onSilentTreatment, false);
    comms->printRobotMessages(false, false);    // for now, no silent treatment
    
    return 0;
}

void jefe::prepareSelection() {
    lastSegmentScore = R / tau;
    
    learner->update(R / tau, heldTrueCount, tau);
    
    int wndw = 5;
    if (((ab->mg->startStates[0]->mm[me]->mv * t) - mu) > permissibleLoss) {
        printf("%i: I'm losing!!!!! mu = %lf; mv*t = %lf; permissibleLoss = %lf\n", me, mu, ab->mg->startStates[0]->mm[me]->mv * t, permissibleLoss);
        alwaysMM = true;
    }
/*
    else if ((t > (wndw-1)) && (MMcount == 0)) {
        double sm = 0.0;
        for (i = 0; i < wndw; i++) {
            sm += payoffHistory[me][i];
        }
        
        double margin = ab->mg->startStates[0]->mm[me]->mv * (wndw / 2.0);
        if ((sm+margin) <= (ab->mg->startStates[0]->mm[me]->mv * wndw)) {
            printf("%i: Tloss!\n", me);
            MMcount = 3+MMtimes;//wndw;
            MMtimes ++;
            if (MMtimes > 2)
                MMtimes = 2;
        }
    }
*/
    heldTrueCount = 0;
}

bool jefe::isSomethingBetter(double _mine, double _his) {
    int i;
    
    //printf("mine = %lf; his = %lf\n", _mine, _his);
    for (i = 0; i < ab->numExperts; i++) {
        //printf("\t%lf, %lf\n", (ab->xprts[i]->barR[me]-ab->xprts[i]->deltaMax), (ab->xprts[i]->barR[1-me]-ab->xprts[i]->deltaMax));
        if ((_mine < (ab->xprts[i]->barR[me]-ab->xprts[i]->deltaMax)) && (_his < (ab->xprts[i]->barR[1-me]-ab->xprts[i]->deltaMax))) {
            //printf("somethingBetter = true\n");
            return true;
        }
    }
    //printf("somethingBetter = false\n");
    
    return false;
}

bool jefe::preemptiveCT() {
    //printf("******************** time since proposal: %i\n", ctm.tsProposal);
    //return false;

    if ((ctm.tsProposal > 0) || brOverride)
        return false;
    
    if (experto > 2) {
        if (ab->xprts[experto]->congruent(ab->mg, ctm.lastProposed)) {
            //printf("I'm already doing what you suggest\n");
            learner->listening2Him = true;
            return false;
        }
    }    
    
    if (!shouldListen()) {//V_listen <= 0.00) {
        //printf("______________________ get here _________________________\n");
        printf("------------- should send the distrust message ---------------\n");
        //comms->logEvent(MSG_ASSESSMENT, 8, "0 ");
        distrustful = true;
        //learner->listening2Him = false;
        return false;
    }
    
    int j1 = ab->mg->states[0]->jointCode(ctm.lastProposed[0][0], ctm.lastProposed[0][1]);
    int j2 = ab->mg->states[0]->jointCode(ctm.lastProposed[1][0], ctm.lastProposed[1][1]);
    
    double propValue = (ab->mg->states[0]->rewards[j1][0][me] + ab->mg->states[0]->rewards[j2][0][me]) / 2.0;
    //printf("propValue = %lf\n", propValue);
    
    //printf("still alive\n");
    
    if (learner->aspiration > (propValue + 0.00001)) {  // not a satisficing proposal
        printf("------------- should send the no-fair message ---------------\n");
        //comms->logEvent(MSG_ASSESSMENT, 9, "0 ");
        notfair = true;
        //learner->listening2Him = false;
        return false;
    }

    //printf("still alive2\n");

    //printf("recentSuccess = %lf\n", learner->recentSuccess[experto]);
    
    if ((propValue - ab->xprts[experto]->deltaMax) < learner->recentSuccess[experto])  // not substantially better
        return false;
    
    //printf("still alive3\n");
    
    //printf("still alive3\n");
    //printf("preemptive selection\n");
    //exit(1);
    
    if (tau > 0) {
        if (learner->willAccept(R / tau, heldTrueCount, tau)) {
            printf("will accept\n");
            prepareSelection();
        }
        else {
            printf("false alarm; should send the not fair\n");
            //learner->listening2Him = false;
            //comms->logEvent(MSG_ASSESSMENT, 9, "0 ");
            notfair = true;
            return false;
        }
    }

    return true;
}

void jefe::determineSatisficingExperts() {
    int i;
    for (i = 0; i < ab->numExperts; i++)
        satisficingExperts[i] = true;
}

void jefe::resetCycle() {
    
	R = 0.0;
    Rnot = 0.0;
    
    if (!cycleFull) {
        latestJacts[0][0] = latestJacts[0][tau];
        latestJacts[1][0] = latestJacts[1][tau];
    }
    
   	tau = 0;
    
    /*	for (int i = 0; i < numStates; i++)
     beenThere[i] = false;
     
     if (estado >= 0)
     beenThere[estado] = true;*/
}
/*
 double jefe::pay(int me, int sol) {
 int a0, a1;
 
 a0 = sol / A[1];
 a1 = sol % A[1];
 
 //printf("a0 = %i\n", a0);
 //printf("a1 = %i\n", a1);
 
 return M[me][a0][a1];
 }
 */
void jefe::setAspirationHigh() {
    if (ab->numExperts == 3) {
        learner->aspiration = ab->mg->startStates[0]->mm[me]->mv;
        printf("no good expert\n");
        return;
    }
    
	int i, j, index = -1;
	double high = ab->mg->startStates[0]->mm[me]->mv;
	int s;
    
	for (i = 3; i < ab->numExperts; i++) {
        if (ab->xprts[i]->barR[me] > high) {
            high = ab->xprts[i]->barR[me];
            index = i;
        }
	}
    
	learner->aspiration = high;
	printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
}

void jefe::setAspirationFolkEgal() {
    if (ab->numExperts == 3) {
        learner->aspiration = ab->mg->startStates[0]->mm[me]->mv;
        printf("no good expert\n");
        return;
    }
    
	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, theMin;
	int s;
    
	for (i = 3; i < ab->numExperts; i++) {
        theMin = ab->xprts[i]->barR[me];
        if (theMin > ab->xprts[i]->barR[1-me])
            theMin = ab->xprts[i]->barR[1-me];
        
        //printf("theMin = %lf\n", theMin);
        
        if (theMin > high) {
            high = theMin;
            index = i;
        }
	}
    
    //printf("index = %i\n", index);
    //fflush(stdout);
    
	learner->aspiration = ab->xprts[index]->barR[me];
    if (learner->aspiration < ab->mg->startStates[0]->mm[me]->mv)
        learner->aspiration = ab->mg->startStates[0]->mm[me]->mv;
	printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
}

void jefe::setAspirationHighestEnforceable() {
    if (ab->numExperts == 3) {
        learner->aspiration = ab->mg->startStates[0]->mm[me]->mv;
        printf("no good expert\n");
        return;
    }
    
	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, val;
	int s;
    
    //printf("REcount = %i\n", REcount);
    //fflush(stdout);
    
	for (i = 3; i < ab->numExperts; i++) {
        if (((ab->xprts[i]->tipo == LEADER) || (ab->xprts[i]->tipo == LEADER2)) && (ab->xprts[i]->barR[me] > high)) {
            high = ab->xprts[i]->barR[me];
            index = i;
        }
	}
    
    //printf("index = %i\n", index);
    //fflush(stdout);
    if (index == -1) {
        printf("nothing is enforceable\n"); fflush(stdout);
        setAspirationFolkEgal();
    }
    else {
        learner->aspiration = ab->xprts[index]->barR[me];
        if (learner->aspiration < ab->mg->startStates[0]->mm[me]->mv)
            learner->aspiration = ab->mg->startStates[0]->mm[me]->mv;
        printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
    }
}

bool jefe::repeatJaction() {
    int i;
    
    //printf("tau = %i (%i, %i)\n", tau, latestJacts[0][tau], latestJacts[1][tau]);
    for (i = 0; i < tau; i++) {
        //printf("%i, %i\n", latestJacts[0][i], latestJacts[1][i]);
        if ((latestJacts[0][i] == latestJacts[0][tau]) && (latestJacts[1][i] == latestJacts[1][tau])) {
            //printf("repeated\n");
            return true;
        }
    }
    
    //printf("i = %i\n", i);
    
    return false;
}

bool jefe::shouldListen() {
    double num;
    
    switch (listeningBetrayalCount) {
        case 0: num = 0.0; break;
        case 1:
        case 2:
        case 3:
            num = pow(2, listeningBetrayalCount) / 20.0;
            break;
        default:
            num = 1.0 - 2.0 / (5.0 * pow(2, listeningBetrayalCount - 4));
            break;
    }

    double r = ((double)(rand())) / RAND_MAX;
    if (r < num) {
        printf("shouldn't listen: %lf < %lf\n", r, num);
        return false;
    }
    
    printf("should listen: %lf >= %lf\n", r, num);
    
    return true;
}

bool jefe::giveSilentTreatment() {
    //onSilentTreatment = false;
    //return onSilentTreatment;

    //printf("goodnessIndex = %lf\n", ab->xprts[experto]->goodnessIndex);

    if (!onSilentTreatment && (oldBetrayalCount == totalBetrayalCount))
        onSilentTreatment = false;
    else {
        double num;
        
        printf("totalBetrayalCount = %i\n", totalBetrayalCount);
        
        switch (totalBetrayalCount) {
            case 0: num = 0.0; break;
            case 1: num = 0.2; break;
            case 2: num = 0.4; break;
            case 3: num = 0.6; break;
            case 4: num = 0.8; break;
            default: num = (100 - 20 * (1.0 / pow(2, (totalBetrayalCount - 4)))) / 100.0; break;
        }
        
        double r = ((double)(rand())) / RAND_MAX;
        if (r < num) {
            printf("give 'im the silent treatment: %lf < %lf\n", r, num);
            onSilentTreatment = true;
        }
        else {
            printf("keep talking: %lf >= %lf\n", r, num);
            onSilentTreatment = false;
        }
    }
    oldBetrayalCount = totalBetrayalCount;

    return onSilentTreatment;
}
