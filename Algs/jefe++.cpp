#include "jefe++.h"

bool doTheSpecify = true, yaDicho = true, yaDicho2 = true;

jefe_plus::jefe_plus() {
    printf("incomplete jefe_plus constructor\n");
    exit(1);
}

jefe_plus::jefe_plus(const char *nombre, int _me, MarkovGame *_mg, double _lambda, int addport, bool _plusplus, bool _XAI) {
    int i;
    
    XAI = _XAI;
    plusplus = _plusplus;
    me = ctm.me = _me;
    lambda = _lambda;
    firstSelection = true;
    
    outputTipo = TEXT_TIPO;
    comms = new CommAgent(outputTipo, 4000+addport, me);

    t = 0;
    numMovemientos = 0;
    experto = -1;
    cycled = false;
    
    cycleFull = true;
    
    ab = new mega(_mg, me, false, comms, lambda);
    ab->computeAllExperts(lambda, false, false);
    ab->determineHighLevelActions();

    satisficingExperts = new bool[ab->numExperts];
    ctExperts = new bool[ab->numExperts];
    X = new bool[ab->numExperts];
    for (i = 0; i < ab->numExperts; i++) {
        satisficingExperts[i] = true;
        ctExperts[i] = false;
        X[i] = false;
    }
    numSatExperts = 1;

    double highval = -99999, lowval = 99999;
    for (i = 3; i < ab->numExperts; i++) {
        if (highval < ab->xprts[i]->barR[me])
            highval = ab->xprts[i]->barR[me];
        if (lowval > ab->xprts[i]->barR[me])
            lowval = ab->xprts[i]->barR[me];
    }
    //printf("highval = %lf; lowval = %lf\n", highval, lowval);
    
    if (!strcmp(nombre, "S++")) {
        learner = new a(_me, _lambda, ab->numExperts, highval, lowval, _mg->states[0]->mm[me]->mv);//   ab->xprts[0]->barR[me]);
        cycleFull = false;
        strcpy(nickname, "S#");
    }
    else if (!strcmp(nombre, "eee")) {
        printf("EEE#\n");
        
        double *ps = new double[ab->numExperts];
        for (i = 0; i < ab->numExperts; i++) {
            ps[i] = ab->xprts[i]->barR[me];
        }
        learner = new eee(_me, _lambda, ab->numExperts, _mg->states[0]->mm[me]->mv, ps);
        delete[] ps;
        strcpy(nickname, "EEE#");
    }
//    else if (!strcmp(nombre, "exp3w++") || !strcmp(nombre, "exp3"))
//        learner = new Exp3(me, (int)_lambda, 0.99, ab->numExperts);
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

    alwaysMM = tempMM = false;
    MMcount = 0;
    //permissibleLoss = 150.0;
    permissibleLoss = 10.0;
    
    lowAspiration = 1.0;
    
    lastState = NULL;
    rPayout[0] = rPayout[1] = 0.0;
    
    //realbad = false;
    previousPayoffs[0] = previousPayoffs[1] = lastSegmentScore = -99999;
    for (i = 0; i < 10; i++) {
        payoffHistory[0][i] = -1.0;
        payoffHistory[1][i] = -1.0;
        actionHistory[0][i] = actionHistory[1][i] = -1;
    }
    
    
    oldExpert = -1;
    
    tau = 0;
    latestJacts[0][0] = latestJacts[1][0] = -1;
    
    somethingBetter = false;
    
    listeningBetrayalCount = totalBetrayalCount = oldBetrayalCount = 0; // added this July 2016
    
    newSelection(9999.0, 9999.0);
    
    MMtimes = 0;
    heldTrueCount = 0;
    
    //if (partido[0] == 'p')
    //    dlyr = new delayer(1);
    //else
    //    dlyr = new delayer(2);
    
    //if (outputTipo == ROBOT_TIPO)
    //    comms->sendRobotMessages();
    
    //char buf[1024];
    //sprintf(buf, "%s_%i.txt", nomlogger, me);
    //fpExpert = fopen(buf, "w");
    
    //V_tell = V_thresh_tell = (ab->mg->states[0]->mm[me]->mv + ab->xprts[0]->deltaMax) * 5.1;
    //V_listen = V_thresh_listen = (learner->aspiration - ab->mg->states[0]->mm[me]->mv - ab->xprts[0]->deltaMax) * 3.5;
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
    
    none_rate = 1500;
    some_rate = 10000;
}

jefe_plus::~jefe_plus() {
    delete learner;
    delete satisficingExperts;
    delete ctExperts;
    delete X;
    delete ab;
    delete comms;
    //delete dlyr;
    
    //fflush(fpExpert);
    //fclose(fpExpert);
}

double jefe_plus::processCheapTalk(char buf[10000]) {
    //printf("Received communications: %s\n", buf);
    
    if (ctm.update(buf, ab->mg->states[0]->numActions, XAI) && !alreadyForgiven) {
        //V_listen = V_thresh_listen;
        alreadyForgiven = true;
    }
    
    printf("numMessagesReceived = %i (%s)\n", ctm.numMessagesReceived, buf);
    if ((ctm.numMessagesReceived == 0) || ((buf[0] == '9') && (buf[1] == '9'))) {
        none_rate = ctm.elapsed;
        
        if (none_rate > 5000.0)
            none_rate = 5000.0;
    }
    else {
        some_rate = ctm.elapsed / ctm.numMessagesReceived;
        if (some_rate > 6000.0)
            some_rate = 6000.0;
        
        printf(">>>>>>> -!-!-!-!-!-!-!-!-!-! updated the some_rate: %lf\n", some_rate);
    }

    // see if I should change my strategy
    //printf("_______________set the dicho = false\n");
    yaDicho = yaDicho2 = false;
    
    printf("check if there is a new good proposal ... "); fflush(stdout);
    
    if (preemptiveCT()) {
        printf("yes!!!\n"); fflush(stdout);
        doTheSpecify = false;
        newSelection(9999.0, 9999.0);
        cycled = false;
        ctm.tsProposal = 1;
        
        if (oldExpert == experto) {
            if (ab->xprts[experto]->guilt < 0.00001)
                ab->xprts[experto]->currentStepMessage(me);
        }
    }
    else {
        printf("No.\n"); fflush(stdout);
    }
    printf("end preemitive\n"); fflush(stdout);
    
    return ctm.elapsed;
}

int jefe_plus::Move(GameEngine *ge) {
    if (learner->listening2Him) {
        printf("------------ I am listening ------------\n");
    }
    else {
        printf("------------ NOT Listening -------------\n");
    }


    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);
    lastState = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);

    int b = ab->xprts[experto]->Move(lastState, previousPayoffs, me);
    int a = ge->convertIt(b, lastValidActs[me], NUMACTIONS*2);

    corA = -1;
    if ((ab->xprts[experto]->tipo == FOLLOWER) || (ab->xprts[experto]->tipo == LEADER) || (ab->xprts[experto]->tipo == LEADER2)) {
        int c = lastState->qsets[ab->xprts[experto]->strat[ab->xprts[experto]->currentStep]]->correctA(1-me);
        corA = ge->convertIt(c, lastValidActs[me], NUMACTIONS*2);
    }

    return a;
}

void jefe_plus::newSelection(double ScoreMe, double ScoreHim) {
    printf("newSelection at time %i\n", t);

    oldExpert = experto;
    
    resetCycle();
    
    determineSatisficingExperts();
    mapCT2Experts();
    
    int i;
    numSatExperts = 0;
    for (i = 0; i < ab->numExperts; i++) {
        if (satisficingExperts[i])
            numSatExperts ++;
    }
    
    if (((a*)learner)->rho < 1.0)
        comms->logEvent(MSG_EVENT, 7, "");
    else
        comms->logEvent(MSG_EVENT, 8, "");
    
    comms->logEvent(MSG_ROUND, t, "");
    
    //printf("selecting at time %i\n", t);
    
//    if (t != 0) {
    if (!firstSelection) {
        experto = learner->select(satisficingExperts, ctExperts, comms, shouldListen(), ctm.tsProposal);
        
        if (ctExperts[experto]) {
            // i selected what he last suggested, so now "forget" what he suggested
            ctm.alreadyPickedLast = true;
        }
    }
    else {
        if (plusplus)
            experto = learner->lastExpert = 1;
        else {
            experto = learner->select(satisficingExperts, ctExperts, comms, shouldListen(), ctm.tsProposal);
        }
    }
    
    printf("experto = %i\n", experto);
    
    if (experto != oldExpert) {
        char bufer[1024];
/*        if (experto == oldExpert)
            strcpy(bufer, "0");
        else if (experto < 3)
            strcpy(bufer, "1");
        else {
            if (((experto+1)/2) == ((oldExpert+1)/2)) {
                strcpy(bufer, "0");
            }
            else
                strcpy(bufer, "1");
        }
*/
        printf("is %i and %i the same? ", experto, oldExpert);
        if (ab->sameNess[experto][oldExpert]) {
            printf("same\n");
            strcpy(bufer, "0");
        }
        else{
            printf("not same\n");
            strcpy(bufer, "1");
        }
        
        printf("logging an expert: %s\n", bufer);
        comms->logEvent(MSG_EXPERT, getTypeNumber(experto), bufer);
    }
    
    if (experto > 1) {
    
        if (ab->xprts[experto]->currentStep < 0) {
            ab->xprts[experto]->getCurrentStep(previousPayoffs, me, false);
        }
        
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
    
    if (experto != oldExpert) {
        if (oldExpert >= 0) {
            char buf[1024];
            if (ab->xprts[oldExpert]->guilt > 0.0001) {
            }
            else {
                bool proposingBetter = false;
                if (experto > 2) {
                    if ((ab->xprts[experto]->barR[me] > ScoreMe) && (ab->xprts[experto]->barR[1-me] > ScoreHim))
                        proposingBetter = true;
                }
                
                if (somethingBetter && proposingBetter) {
                    comms->logEvent(MSG_ASSESSMENT, 5, "0 ");
                }
            }
        }
        
        ab->xprts[experto]->reset(previousPayoffs, me);
        ab->xprts[experto]->currentStepMessage(me);
    }
    
    ctm.notThis = -1;
    
    firstSelection = false;
    
    giveSilentTreatment();
    if (onSilentTreatment)
        printf("***************** time for silentTreatment\n");
}

void jefe_plus::labelConv(char str[1024], char conv[1024]) {
    //printf("-----------------%s\n", str);
    if (ab->mg->states[0]->numActions[0] == 2) {
        if (!strcmp(str, "A"))
            strcpy(conv, "X");
        else if (!strcmp(str, "B"))
            strcpy(conv, "Y");
    }
    else {
        if (!strcmp(str, "A"))
            strcpy(conv, "X");
        if (!strcmp(str, "B"))
            strcpy(conv, "Y");
        else if (!strcmp(str, "C"))
            strcpy(conv, "Z");
    }
}

int jefe_plus::getTypeNumber(int exprt) {
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

int jefe_plus::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    //printf("jefe_plus: moveUpdate\n");

    int i;
    for (i = 9; i > 0; i--) {
        actionHistory[0][i] = actionHistory[0][i-1];
        actionHistory[1][i] = actionHistory[1][i-1];
    }
    actionHistory[0][0] = actions[0];
    actionHistory[1][0] = actions[1];

    int acts[2];
    ge->deduceActions(acts, actions, lastValidActs);
    
    //printf("u"); fflush(stdout);
    
    rPayout[0] += dollars[0];
    rPayout[1] += dollars[1];

    ge->getValidActions(lastValidActs[0], 0);
    ge->getValidActions(lastValidActs[1], 1);

    //printf("p"); fflush(stdout);
    
    State* sprime = ge->getState(ab->mg, lastValidActs[0], lastValidActs[1]);
    
    if (corA >= 0)
        listenCount++;
    //printf("d"); fflush(stdout);
    if (corA == actions[1-me]) {
        heldTrue = true;
        heldTrueCount ++;
        
        listened ++;
    }
    else {
        heldTrue = false;
    }
    
    
    ab->moveUpdate(lastState, sprime, acts, experto, rPayout, learner->aspiration, corA);
    
    if ((experto > 2) && ab->xprts[experto]->hosed) {
        // increment the number of times he has rebelled
        if (!onSilentTreatment)
            totalBetrayalCount ++;
        if (learner->listening2Him) {
            listeningBetrayalCount ++;
        }
        if (onSilentTreatment) {
            // give him back his point
            ab->xprts[experto]->goodnessIndex -= 1.0;
        }
        printf("****** I got hosed!!! (%i) *******\n", listeningBetrayalCount);
    }
    
    numMovemientos++;
    
    //printf("ate"); fflush(stdout);
    
    //if (outputTipo == ROBOT_TIPO)
    //    comms->sendRobotMessages();
    //printf("------------- move update -------------\n");
    comms->printRobotMessages(onSilentTreatment, false);
    //printf("------------- end -------------\n");

    
    whipeoutguilt = false;
    if (!doTheSpecify) {
        //printf("---------- should whipe out guilt\n");
        whipeoutguilt = true;
    }
    doTheSpecify = true;
    
    return 0;
}
    
    
int jefe_plus::roundUpdate() {
    //printf("jefe_plus: roundUpdate\n");

    int i;
    for (i = 9; i > 0; i--) {
        payoffHistory[0][i] = payoffHistory[0][i-1];
        payoffHistory[1][i] = payoffHistory[1][i-1];
    }
    previousPayoffs[0] = payoffHistory[0][0] = rPayout[0];
    previousPayoffs[1] = payoffHistory[1][0] = rPayout[1];
    
    
	R += rPayout[me];
    Rnot += rPayout[1-me];
	mu += rPayout[me];
    
    // find out whether I should update V_tell
    //if ((experto > 2) && (V_tell > 0.0) && (((experto % 2) == 1) || (ab->xprts[experto]->guilt <= 0.00001)) && !tempMM) {
    //    V_tell += rPayout[me] - (ab->mg->states[0]->mm[me]->mv + ab->xprts[experto]->deltaMax);
    //    //V_tell += rPayout[me] - (learner->aspiration - ab->xprts[experto]->deltaMax);
    //    if (V_tell > V_thresh_tell)
    //        V_tell = V_thresh_tell;
    //}
    
    //if (learner->listening2Him) {
    //    V_listen += rPayout[me] - (learner->aspiration - ab->xprts[experto]->deltaMax);//(ab->mg->states[0]->mm[me]->mv + ab->xprts[experto]->deltaMax);
    //    if (V_listen > V_thresh_listen)
    //        V_listen = V_thresh_listen;
    //}
    
    //printf("\tV_tell = %lf\n", V_tell);
    //printf("\tV_listen = %lf\n", V_listen);
    
    //printf("\tPr_L = %lf\n", listened / (double)listenCount);
    
    ab->roundUpdate(rPayout);
    latestJacts[0][tau+1] = ab->highestInd[0];
    latestJacts[1][tau+1] = ab->highestInd[1];

    ab->xprts[experto]->Update(rPayout, me, learner->aspiration, heldTrue, learner->aspiration);
    
	t++;
	tau ++;

    //if ((tau >= ab->xprts[experto]->cycleLen) && (ab->xprts[experto]->guilt == 0))
    if (!cycleFull) {
        //printf("this question ... ");
        if (repeatJaction() && ((ab->xprts[experto]->guilt < 0.00001) || (experto < 3) || ((experto % 2) == 1))) {
            cycled = true;
        }
        //printf("no\n");
    }
    else {
        if (tau >= ab->xprts[experto]->cycleLen)
            cycled = true;
    }
    
	if (cycled) {
        prepareSelection();
    }
    
    if (learner->aspiration < lowAspiration)
        lowAspiration = learner->aspiration;

    if (experto == 1) {
        //printf("\t\texperto = 1: %lf vs %lf\n", rPayout[me], learner->aspiration);
        if (rPayout[me] < (learner->aspiration - ab->deltaMax))
            comms->logEvent(MSG_EVENT, 5, "");
        else
            comms->logEvent(MSG_EVENT, 6, "");
    }
    
    rPayout[0] = rPayout[1] = 0.0;    
    
    if (cycled || preemptiveCT()) {
        // check to see if we both could do better
        somethingBetter = isSomethingBetter(R/tau,Rnot/tau);
        
        newSelection(R/tau,Rnot/tau);
        cycled = false;
        
        if (((R/tau) > (ab->xprts[experto]->barR[me]-ab->xprts[experto]->deltaMax)) || ((Rnot/tau) > (ab->xprts[experto]->barR[1-me]-ab->xprts[experto]->deltaMax))) {
            somethingBetter = false;
        }
        
        if (oldExpert == experto) {
            if (ab->xprts[experto]->guilt < 0.00001)
                ab->xprts[experto]->currentStepMessage(me);
        }
    }
    else {
        comms->logEvent(MSG_ROUND, t, "");

        if (ab->xprts[experto]->guilt < 0.00001)
            ab->xprts[experto]->currentStepMessage(me);
    }
    
    numMovemientos = 0;
    
    //if (outputTipo == ROBOT_TIPO)
    //    comms->sendRobotMessages();
    
    if (notfair)
        comms->logEvent(MSG_ASSESSMENT, 9, "0 ");
    if (distrustful)
        comms->logEvent(MSG_ASSESSMENT, 8, "0 ");
    
    //printf("------------- round update -------------\n");
    comms->printRobotMessages(onSilentTreatment, false);
    //printf("----------------- end ------------------\n");
    
    notfair = distrustful = false;
    
    if (whipeoutguilt)
        ab->xprts[experto]->guilt = 0.0;
    
    return 0;
}

void jefe_plus::prepareSelection() {
    lastSegmentScore = R / tau;
    
    learner->update(R / tau, heldTrueCount, tau);

    if (plusplus) {
        if (((ab->mg->startStates[0]->mm[me]->mv * t) - mu) > permissibleLoss) {
            printf("%i: I'm losing!!!!! mu = %lf; mv*t = %lf; permissibleLoss = %lf\n", me, mu, ab->mg->startStates[0]->mm[me]->mv * t, permissibleLoss);
            alwaysMM = true;
        }
    }

    heldTrueCount = 0;
}



bool jefe_plus::isSomethingBetter(double _mine, double _his) {
    int i;
    
    //printf("mine = %lf; his = %lf\n", _mine, _his);
    for (i = 0; i < ab->numExperts; i++) {
        //printf("\t%lf, %lf\n", (ab->xprts[i]->barR[me]-ab->xprts[i]->deltaMax), (ab->xprts[i]->barR[1-me]-ab->xprts[i]->deltaMax));
        if ((_mine < (ab->xprts[i]->barR[me] - ab->xprts[i]->deltaMax)) &&
            (_his < (ab->xprts[i]->barR[1-me] - ab->xprts[i]->deltaMax))) {
            //printf("somethingBetter = true\n");
            //printf("actual: (mine = %lf; _his = %lf); proposed: (mine = %lf; _his = %lf)\n", _mine, _his, ab->xprts[i]->barR[me], ab->xprts[i]->barR[1-me]);
            
            return true;
        }
    }
    //printf("somethingBetter = false\n");
    
    return false;
}


bool jefe_plus::preemptiveCT() {
    //printf("******************** time since proposal: %i\n", ctm.tsProposal);
    //return false;
    if (!XAI)
        return false;
    
    if (!strcmp(nickname, "EEE#")) {
        return preemptiveCT_EEE();
    }

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
        printf("------------- should send the distrust message ---------------\n"); fflush(stdout);
        //comms->logEvent(MSG_ASSESSMENT, 8, "0 ");
        distrustful = true;
        //learner->listening2Him = false;
        return false;
    }
    
    int j1 = ab->mg->states[0]->jointCode(ctm.lastProposed[0][0], ctm.lastProposed[0][1]);
    int j2 = ab->mg->states[0]->jointCode(ctm.lastProposed[1][0], ctm.lastProposed[1][1]);
    
    double propValue = (ab->mg->states[0]->rewards[j1][0][me] + ab->mg->states[0]->rewards[j2][0][me]) / 2.0;
    
    if (learner->aspiration > (propValue + 0.00001)) {  // not a satisficing proposal
        printf("------------- should send the no-fair message ---------------\n"); fflush(stdout);
        //comms->logEvent(MSG_ASSESSMENT, 9, "0 ");
        notfair = true;
        //learner->listening2Him = false;
        return false;
    }
    
    if ((propValue - ab->xprts[experto]->deltaMax) < learner->recentSuccess[experto])  // not substantially better
        return false;
    
    if (tau > 0) {
        if (learner->willAccept(R / tau, heldTrueCount, tau)) {
            printf("will accept\n");
            prepareSelection();
        }
        else {
            //printf("false alarm; should send the not fair\n");
            //learner->listening2Him = false;
            //comms->logEvent(MSG_ASSESSMENT, 9, "0 ");
            //notfair = true;
            return false;
        }
    }

    return true;
}

bool jefe_plus::preemptiveCT_EEE() {
    if ((ctm.tsProposal > 0))
        return false;
    
    if (experto > 2) {
        if (ab->xprts[experto]->congruent(ab->mg, ctm.lastProposed)) {
            //printf("I'm already doing what you suggest\n");
            learner->listening2Him = true;
            return false;
        }
    }
    
    if (shouldListen_EEE()) {
        printf("will accept\n");
        prepareSelection();
        ((eee *)learner)->berandom = false;
        
        return true;
    }
    
    // need to express distrust or notfair
    if (!avalueThing)
        distrustful = true;
    else
        notfair = true;

    return false;
}

bool jefe_plus::override() {
    if (!plusplus)
        return false;

    //printf("beginning the override\n"); fflush(stdout);
    
    brOverride = false;
    
    if (alwaysMM) {
        printf("%i: MM override!!!!!!!!!!\n", me);
        satisficingExperts[0] = true;
        return true;
    }
    else if (MMcount > 0) {
        printf("%i: **************temp MM override!\n", me);
        satisficingExperts[0] = true;
        MMcount --;
        tempMM = true;
        return true;
    }
    tempMM = false;
    
    double brVal = ab->mg->startStates[0]->Vbr;
    
    //printf("brVal = %.2lf\n", brVal); fflush(stdout);

    int i;
    
    if (brVal >= learner->aspiration) {
        double highVal = -99999;
        
        for (i = 2; i < ab->numExperts; i++) {
            if (ab->xprts[i]->barR[me] >= learner->aspiration) {
                if (highVal < ab->xprts[i]->vu)
                    highVal = ab->xprts[i]->vu;
            }
        }
 
        //printf("high = %.4lf\n", highVal);
    
        if (ab->xprts[1]->vu >= highVal) {
            satisficingExperts[1] = true;
            printf("%i: BR override!!!!!!!!!!: %lf < %lf\n", me, learner->aspiration, brVal);
            printf("%i: vu[1] = %lf; highVal = %lf; brVal = %lf\n", me, ab->xprts[1]->vu, highVal, brVal);
            brOverride = true;
            //br->printV();
            return true;
        }
    }

    return false;
}

void jefe_plus::mapCT2Experts() {
    int i;
        
    if (ctm.lastProposed[0][0] > -1) {
        ctExperts[0] = ctExperts[1] = ctExperts[2] = false;
        X[0] = X[1] = X[2] = false;
        for (i = 3; i < ab->numExperts; i++) {
            X[i] = ab->xprts[i]->congruent(ab->mg,ctm.lastProposed);
            if (!ctm.alreadyPickedLast)
                ctExperts[i] = X[i];
            else
                ctExperts[i] = false;
        }
    }
    else {
        for (i = 0; i < ab->numExperts; i++) {
            X[i] = ctExperts[i] = false;
        }
    }
    
    printf("\t\tProposed: ");
    for (i = 3; i < ab->numExperts; i++) {
        if (ctExperts[i])
            printf("%i ", i);
    }
    printf("\n");
}

void jefe_plus::determineSatisficingExperts() {
    int i;

    if (!plusplus) {
        for (i = 0; i < ab->numExperts; i++)
            satisficingExperts[i] = true;
        
        satisficingExperts[2] = false; // not going to consider the PREVENTATIVE strategy in matrix games
    
        return;
    }

    for (i = 0; i < ab->numExperts; i++)
        satisficingExperts[i] = false;
    
    bool verbose = false;
    //if (me == 0)
    //    verbose = true;
    
    if (verbose) {
        //printf("  Satisficing Experts for %i (alpha = %.3lf):\n", me, learner->aspiration); fflush(stdout);
        //im->print();
        printf("Satisficing experts: ");
    }
    
    if (override()) {
        //addMessage("  |", 5, -1);
        //addMessage("Bare with me for a moment ... |", 15, me);
        return;
    }
    
    // adding this exception in.  Do I want to keep it?
    if ((learner->satisficing) && (experto >= 0))
        satisficingExperts[experto] = true;
    
    //printf("past the override\n");
        
    int c = 0;
    //if (mnmx[me]->mv >= learner->aspiration) {
    if (ab->mg->startStates[0]->mm[me]->mv >= learner->aspiration) {
        satisficingExperts[0] = true;
        c ++;
        //printf("0 ");
        
        if (verbose) {
            //printf("Expert maximin: val = %lf\n", ab->mg->startStates[0]->mm[me]->mv); fflush(stdout);
        }
    }

    for (i = 2; i < ab->numExperts; i++) {
        if ((ab->xprts[i]->tipo == FOLLOWER) && (experto != i)) {
//            if (!(ab->xprts[i]->showedLeadership(ab->lastActions[1-me], payoffHistory, me))) {
            if (!(ab->xprts[i]->showedLeadership(ab->mg, actionHistory, me))) {
                if (verbose)
                    printf("(not %i) ", i);
                continue;
            }
        }
 
        if (ab->xprts[i]->barR[me] >= learner->aspiration) {
            c ++;
            satisficingExperts[i] = true;
            if (verbose) {
                //printf("Expert %i: val = %lf\n", i, ab->xprts[i]->barR[me]); fflush(stdout);
                printf("%i ", i);
            }
        }
    }
    
    double brVal = ab->mg->startStates[0]->Vbr;
    
    if ((c == 0) && (experto >= 0)) {
        satisficingExperts[experto] = true;
        c++;
    }
    else if ((c == 0) || (brVal >= learner->aspiration)) {
        satisficingExperts[1] = true;
        c ++;
        
        if (verbose) {
            //printf("Expert BR: val = %lf\n", brVal); fflush(stdout);
            printf("1 (%.2lf)", brVal);
        }
    }
    
    //printf("-%i-", c);
    
    if (verbose)
        printf("\n");
    
/*    printf("<(%.3lf)", learner->aspiration);
    for (i = 0; i < numExperts; i++)
        printf("%i", (int)(satisficingExperts[i]));
    printf(">");*/
}

void jefe_plus::resetCycle() {

	R = 0.0;
    Rnot = 0.0;
    
    if (!cycleFull) {
        latestJacts[0][0] = latestJacts[0][tau];
        latestJacts[1][0] = latestJacts[1][tau];
    }

   	tau = 0;
}

void jefe_plus::setAspirationHigh() {
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

void jefe_plus::setAspirationFolkEgal() {
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

void jefe_plus::setAspirationHighestEnforceable() {
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

bool jefe_plus::repeatJaction() {
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

bool jefe_plus::shouldListen() {
    if (!plusplus) {
        return shouldListen_EEE();
    }

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

bool jefe_plus::shouldListen_EEE() {
   // compute the effect of the proposal
    double *tmp = new double[ab->numExperts];
    bool *prop = new bool[ab->numExperts];
    int i;
    double value = -99999;
    
    printf("$$$$$$$$$$$$$$$$$$$\n");
    
    for (i = 0; i < ab->numExperts; i++) {
        if (i < 3)
            prop[i] = false;
        else
            prop[i] = ab->xprts[i]->congruent(ab->mg, ctm.lastProposed);
        tmp[i] = ((eee *)learner)->getValue(i, prop[i]);
        
        if (prop[i]) {
            if (value < ab->xprts[i]->barR[me]) {
                value = ab->xprts[i]->barR[me];
            }
        }
        
        printf("%i: %i (%lf)\n", i, (int)(prop[i]), tmp[i]);
    }

    // something congruent must have a higher payoff than what I'm getting
    bool bestCongruent = false;
    double best = -99999;
    
    for (i = 0; i < ab->numExperts; i++) {
        if (tmp[i] > best) {
            best = tmp[i];
            bestCongruent = false;
        }

        if ((tmp[i] > (best - 0.00001)) && prop[i])
            bestCongruent = true;
    }
    printf("best = %lf\n", best);
    
    if (!bestCongruent) {
        printf("Comparison: Proposal value = %lf; My best = %lf\n", value, best);
        // compute a value that can then be used to determine distrustful vs. notfair
        if (value < best)
            avalueThing = true;
        else
            avalueThing = false;
    }

    printf("$$$$$$$$$$$$$$$$$$$\n");
    
    delete[] prop;
    delete[] tmp;
    return bestCongruent;
}

bool jefe_plus::giveSilentTreatment() {
    if (!XAI) {
        onSilentTreatment = false;
        return onSilentTreatment;
    }

    //printf("goodnessIndex = %lf\n", ab->xprts[experto]->goodnessIndex);

    if (!onSilentTreatment && (oldBetrayalCount == totalBetrayalCount))
        onSilentTreatment = false;
    else {
        double num;
        
        printf("totalBetrayalCount = %i\n", totalBetrayalCount);
        
        // old version (Tennom's study)
        //switch (totalBetrayalCount) {
        //    case 0: num = 0.0; break;
        //    case 1: num = 0.2; break;
        //    case 2: num = 0.4; break;
        //    case 3: num = 0.6; break;
        //    case 4: num = 0.8; break;
        //    default: num = (100 - 20 * (1.0 / pow(2, (totalBetrayalCount - 4)))) / 100.0; break;
        //}
        
        if (totalBetrayalCount < 10)
            num = totalBetrayalCount / 10.0;
        else
            num = (100 - 20 * (1.0 / pow(2, (totalBetrayalCount - 4)))) / 100.0;
        
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
