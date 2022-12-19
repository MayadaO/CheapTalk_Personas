#include "mega.h"

mega::mega() {
    printf("incomplete a-bert constructor\n");
    exit(1);
}

mega::mega(MarkovGame *_mg, int _me, bool _explorar, CommAgent *_comms, double _lambda, bool _utilitarian) {
    mg = _mg;
    me = _me;
    utilitarian = _utilitarian;
    
    cLen = 3;// + rand() % 2;
    //cLen = 10.0;
    
    reset();
    numStateHistoryPrev = 0;
    bron = false;
    
    //egalPoint = -1;
    
    //deltaMax = 4.0;
    deltaMax = 0.04;
    
    backward = false;
    numHighLevelActions = 1;//mg->states[0]->numQsets + 2;
    
    comms = _comms;
}

mega::~mega() {
    int i;
    
    for (i = 0; i < numExperts; i++)
        delete xprts[i];
    delete xprts;
 
    delete mg;
}


void mega::computeMaxminExpert(double _lambda) {
    printf("computing my maximin ... "); fflush(stdout);
    mg->solveZero(me);
    printf("done\n");

    numExperts = 1;
    xprts = new Expert*[1];
    xprts[0] = new Expert(MINIMAX, -1, -1, cLen, _lambda, false, 0.0, comms);
    
}

void mega::computeMBRLExpert(double _lambda, bool _explorar) {
    printf("need to compute the MBRL expert\n"); fflush(stdout);
    
    numExperts = 1;
    xprts = new Expert*[1];
    xprts[0] = new Expert(BResp, -1, -1, cLen, _lambda, _explorar, 0.0, comms);
    
    int s, i, j;
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[me]; i++) {
            mg->states[s]->Qbr[i] = 0.0;
        }
        mg->states[s]->Vbr = 0.0;
    }
    printf("computing BR ... "); fflush(stdout);
    if (utilitarian) {
        //mg->computeBR(me, true);
        for (s = 0; s < mg->numStates; s++) {
            for (i = 0; i < mg->states[s]->numActions[0]; i++) {
                for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                    mg->states[s]->cuentas[i][j] = 0;
                }
            }
        }
        mg->computeBRu(me, true, 0);
        // reset the prior cuentas so that we can recompute
        mg->computeBRu(me, true, 0);
    }
    else
        mg->computeBR(me, true);
    printf("done\n"); fflush(stdout);
    
    bron = true;
}

void mega::computeFolkEgalExpert(double _lambda, bool lead) {
    printf("Computing the Folk-Egal strategy\n");
    
    printf("computing associate's maximin ... "); fflush(stdout);
    mg->solveZero(1-me);
    printf("done\n");
    printf("computing my maximin ... "); fflush(stdout);
    mg->solveZero(me);
    printf("done\n");
    printf("minimaxValues: %.3lf, %.3lf\n", mg->startStates[0]->mm[0]->mv, mg->startStates[0]->mm[1]->mv);
    
    // reset qvals;
    int s, i, j;
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->qsets[0]->Qs[i][j] = 0.0;
            }
        }
        mg->states[s]->qsets[0]->V = 0.0;
    }
    
    printf("computing my attack strategy ... "); fflush(stdout);
    mg->solveAttack(me);
    printf("done\n");
    
    double w[2], P[2];
    Solution *sols[MAX_EXPERTS];
    
    //double deltaMax = 4.0;
    int numSols = 0;
    int sol[2];
    double rmax_me = -99999;

    int kappa = 0;
    
    for (w[0] = mg->wvals[0]; w[0] < mg->wvals[1] + 0.000001; w[0] += mg->wvals[2]) {
        w[1] = 1.0 - w[0];
        mg->vIteration(w[0], w[1]);
        getMax(mg->startStates[0], w, P);
        printf("%i: w = %.2lf: (%.2lf, %.2lf)\n", kappa, w[0], P[0], P[1]);
        kappa ++;
        
        if (P[me] > rmax_me)
            rmax_me = P[me];
        
        sol[0] = sol[1] = numSols;
        sols[numSols] = new Solution(sol, P, mg, me, deltaMax);
        
        numSols++;
    }
    int numPureSols = numSols;
    
    double v[2];
    for (i = 0; i < numPureSols; i++) {
        for (j = i+1; j < numPureSols; j++) {
            v[0] = (sols[i]->payouts[0] + sols[j]->payouts[0]) / 2.0;
            v[1] = (sols[i]->payouts[1] + sols[j]->payouts[1]) / 2.0;
            
            sol[0] = sols[i]->indices[0];
            sol[1] = sols[j]->indices[0];
            sols[numSols] = new Solution(sol, v);
            sols[numSols]->maxGuilt[0] = sols[i]->maxGuilt[0];
            sols[numSols]->maxGuilt[1] = sols[j]->maxGuilt[1];
            printf("%i: combo(%i, %i): (%.2lf, %.2lf)\n", kappa, sols[numSols]->indices[0], sols[numSols]->indices[1], sols[numSols]->payouts[0], sols[numSols]->payouts[1]);
            kappa ++;
            sols[numSols]->computePunishmentRatio(mg, me);
            
            numSols++;
        }
    }
    
    double mvs[2];
    mvs[0] = mg->states[0]->mm[0]->mv;
    mvs[1] = mg->states[0]->mm[1]->mv;
    selectSolutions(1, sols, numSols, mvs);
    
    numExperts = 0;
    xprts = new Expert*[1];
    
    for (i = 0; i < numSols; i++) {
        if (sols[i]->selected) {
            if (lead) {
                xprts[0] = new Expert(LEADER2, sols[i]->indices[0], sols[i]->indices[1],
                                   sols[sols[i]->indices[0]]->payouts, sols[sols[i]->indices[1]]->payouts, deltaMax, cLen, _lambda, me, comms);
            }
            else {
                xprts[0] = new Expert(FOLLOWER, sols[i]->indices[0], sols[i]->indices[1],
                                      sols[sols[i]->indices[0]]->payouts, sols[sols[i]->indices[1]]->payouts, deltaMax, cLen, _lambda, me, comms);
            }
            numExperts ++;
        }
    }
    
    bron = false;
}

void mega::computeBullyExpert(double _lambda, bool opposite) {
    printf("Computing the bully strategy\n");
    
    printf("computing associate's maximin ... "); fflush(stdout);
    mg->solveZero(1-me);
    printf("done\n");
    printf("computing my maximin ... "); fflush(stdout);
    mg->solveZero(me);
    printf("done\n");
    printf("minimaxValues: %.3lf, %.3lf\n", mg->startStates[0]->mm[0]->mv, mg->startStates[0]->mm[1]->mv);
    
    // reset qvals;
    int s, i, j;
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->qsets[0]->Qs[i][j] = 0.0;
            }
        }
        mg->states[s]->qsets[0]->V = 0.0;
    }
    
    printf("computing my attack strategy ... "); fflush(stdout);
    mg->solveAttack(me);
    printf("done\n");
    
    double w[2], P[2];
    Solution *sols[MAX_EXPERTS];
    
    //double deltaMax = 2.0;
    int numSols = 0;
    int sol[2];
    double rmax_me = -99999;
    for (w[0] = mg->wvals[0]; w[0] < mg->wvals[1] + 0.000001; w[0] += mg->wvals[2]) {
        w[1] = 1.0 - w[0];
        mg->vIteration(w[0], w[1]);
        getMax(mg->startStates[0], w, P);
        printf("w = %.2lf: (%.2lf, %.2lf)\n", w[0], P[0], P[1]);
        
        if (P[me] > rmax_me)
            rmax_me = P[me];
        
        sol[0] = sol[1] = numSols;
        sols[numSols] = new Solution(sol, P, mg, me, deltaMax);
        
        numSols++;
    }
    int numPureSols = numSols;
    
    double v[2];
    for (i = 0; i < numPureSols; i++) {
        for (j = i+1; j < numPureSols; j++) {
            v[0] = (sols[i]->payouts[0] + sols[j]->payouts[0]) / 2.0;
            v[1] = (sols[i]->payouts[1] + sols[j]->payouts[1]) / 2.0;
            
            sol[0] = sols[i]->indices[0];
            sol[1] = sols[j]->indices[0];
            sols[numSols] = new Solution(sol, v);
            sols[numSols]->maxGuilt[0] = sols[i]->maxGuilt[0];
            sols[numSols]->maxGuilt[1] = sols[j]->maxGuilt[1];
            printf("combo(%i, %i): (%.2lf, %.2lf)\n", sols[numSols]->indices[0], sols[numSols]->indices[1], sols[numSols]->payouts[0], sols[numSols]->payouts[1]);
            sols[numSols]->computePunishmentRatio(mg, me);
            
            numSols++;
        }
    }
    
    double mvs[2];
    mvs[0] = mg->states[0]->mm[0]->mv;
    mvs[1] = mg->states[0]->mm[1]->mv;
    selectSolutions(2, sols, numSols, mvs);
    
    numExperts = 0;
    xprts = new Expert*[1];
    bool primero;
    if (!backward)
        primero = opposite;
    else
        primero = !opposite;
    printf("me = %i\n", me);
    if (primero)
        printf("primero = true\n");
    else
        printf("primero = false\n");
    for (i = 0; i < numSols; i++) {
        if (sols[i]->selected) {
            if (((me == 0) && !primero) || ((me == 1) && primero)) {
                printf("right one (%i): %.2lf, %.2lf\n", i, sols[i]->payouts[0], sols[i]->payouts[1]);
                xprts[0] = new Expert(LEADER2, sols[i]->indices[0], sols[i]->indices[1],
                                      sols[sols[i]->indices[0]]->payouts, sols[sols[i]->indices[1]]->payouts, deltaMax, cLen, _lambda, me, comms);
                numExperts ++;
            }
            else {
                printf("wrong one (%i): %.2lf, %.2lf\n", i, sols[i]->payouts[0], sols[i]->payouts[1]);
            }
            primero = !primero;//opposite;
        }
    }
    
    bron = false;
}


void mega::computeAllExperts(double _lambda, bool _explorar, bool fullCycle) {
    printf("Computing experts for player %i\n", me);
    
    printf("computing associate's maximin ... "); fflush(stdout);
    mg->solveZero(1-me);
    printf("done\n");
    printf("computing my maximin ... "); fflush(stdout);
    mg->solveZero(me);
    printf("done\n");    
    printf("minimaxValues: %.3lf, %.3lf\n", mg->startStates[0]->mm[0]->mv, mg->startStates[0]->mm[1]->mv);
    
    // reset qvals;
    int s, i, j;
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[0]; i++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->qsets[0]->Qs[i][j] = 0.0;
            }
        }
        mg->states[s]->qsets[0]->V = 0.0;
    }

    printf("computing associate's attack strategy ... "); fflush(stdout);
    mg->solveAttack(1-me);
    printf("done\n");
    printf("computing my attack strategy ... "); fflush(stdout);
    mg->solveAttack(me);
    printf("done\n");
    
    double w[2], P[2];
    Solution *sols[MAX_EXPERTS];
    
    //double deltaMax = 4.0;
    int numSols = 0;
    int sol[2];
    double rmax_me = -99999;
    int kappa = 0;
    for (w[0] = mg->wvals[0]; w[0] < mg->wvals[1] + 0.000001; w[0] += mg->wvals[2]) {
        w[1] = 1.0 - w[0];
        mg->vIteration(w[0], w[1]);
        getMax(mg->startStates[0], w, P);
        printf("%i) w = %.2lf: (%.2lf, %.2lf)\n", kappa, w[0], P[0], P[1]);
        kappa ++;
        
        if (P[me] > rmax_me)
            rmax_me = P[me];
        
        sol[0] = sol[1] = numSols;
        sols[numSols] = new Solution(sol, P, mg, me, deltaMax);
        
        numSols++;
    }
    int numPureSols = numSols;

    double v[2];
    for (i = 0; i < numPureSols; i++) {
        for (j = i+1; j < numPureSols; j++) {
            v[0] = (sols[i]->payouts[0] + sols[j]->payouts[0]) / 2.0;
            v[1] = (sols[i]->payouts[1] + sols[j]->payouts[1]) / 2.0;

            sol[0] = sols[i]->indices[0];
            sol[1] = sols[j]->indices[0];
            sols[numSols] = new Solution(sol, v);
            sols[numSols]->maxGuilt[0] = sols[i]->maxGuilt[0];
            sols[numSols]->maxGuilt[1] = sols[j]->maxGuilt[1];
            printf("%i) combo(%i, %i): (%.2lf, %.2lf)\n", kappa, sols[numSols]->indices[0], sols[numSols]->indices[1], sols[numSols]->payouts[0], sols[numSols]->payouts[1]);
            kappa ++;
            sols[numSols]->computePunishmentRatio(mg, me);
            
            numSols++;
        }
    }

    double mvs[2];
    mvs[0] = mg->states[0]->mm[0]->mv;
    mvs[1] = mg->states[0]->mm[1]->mv;
    
    //if (!strcmp(partidito, "endless")) {
    //    selectSolutions(1, sols, numSols, mvs); // major hack to make it work for endless game; need to fix this
    //}
    //else {
        selectSolutions(5, sols, numSols, mvs);
    //}
    
    numExperts = 3;
    xprts = new Expert*[100];
    if (!fullCycle) {
        xprts[0] = new Expert(MINIMAX, -1, -1, cLen, _lambda, false, deltaMax, comms);
        xprts[1] = new Expert(BResp, -1, -1, cLen, _lambda, _explorar, deltaMax, comms);
    }
    else {
        xprts[0] = new Expert(MINIMAX, -1, -1, cLen, _lambda, false, deltaMax, comms);
        xprts[1] = new Expert(BResp, -1, -1, cLen, _lambda, _explorar, deltaMax, comms);
    }
    xprts[2] = new Expert(PREVENT, -1, -1, cLen, _lambda, false, deltaMax, comms, mg, me);
    
    char buf[1024];
    strcpy(buf, "");
    findHighFair(xprts[2]->barR, buf);
    printf("Preventer barR: %.2lf, %.2lf\n", xprts[2]->barR[0], xprts[2]->barR[1]);
    
    for (i = 0; i < numSols; i++) {
        if (sols[i]->selected) {
            printf("(%i) ", numExperts);
            xprts[numExperts] = new Expert(FOLLOWER, sols[i]->indices[0], sols[i]->indices[1],
                                           sols[sols[i]->indices[0]]->payouts, sols[sols[i]->indices[1]]->payouts, deltaMax, cLen, _lambda, me, comms, sols[i]->counter);
            numExperts++;
            //if (sols[i]->punishmentRatio < cLen) {
                printf("(%i) ", numExperts);
                xprts[numExperts] = new Expert(LEADER2, sols[i]->indices[0], sols[i]->indices[1],
                                               sols[sols[i]->indices[0]]->payouts, sols[sols[i]->indices[1]]->payouts, deltaMax, cLen, _lambda, me, comms, sols[i]->counter);
                numExperts++;
            //}
        }
    }
    
    // we need to establish "sameness"; if xprts suggest the same target solution, they are the same
    for (i = 0; i < numExperts; i++) {
        for (j = i; j < numExperts; j++) {
            if (i == j)
                sameNess[i][j] = true;
            else if ((i < 3) || (j < 3)) {
                sameNess[i][j] = false;
                sameNess[j][i] = false;
            }
            else {
                sameNess[i][j] = sameNess[j][i] = sameActionSequence(i, j);
            }
        }
    }
    
    for (i = 0; i < numExperts; i++) {
        for (j = 0; j < numExperts; j++) {
            printf("%i", (int)(sameNess[i][j]));
        }
        printf("\n");
    }

    printf("number of experts: %i\n", numExperts);
    
    // reset qvalues (to computeBR)
    for (s = 0; s < mg->numStates; s++) {
        for (i = 0; i < mg->states[s]->numActions[me]; i++) {
            mg->states[s]->Qbr[i] = 0.0;
        }
        mg->states[s]->Vbr = 0.0;
    }
    //adjustBRPriors();
    printf("computing BR ... "); fflush(stdout);
    mg->computeBR(me, true);
    printf("done\n"); fflush(stdout);

    // need to set all of the vu's
    printf("initializing vu: %.2lf\n", rmax_me); fflush(stdout);
    for (i = 0; i < numExperts; i++)
        xprts[i]->vu = rmax_me;
    
    bron = true;
}

bool mega::sameActionSequence(int e1, int e2) {
    int a1[2][2], a2[2][2];
    
    a1[0][0] = mg->states[0]->qsets[xprts[e1]->strat[0]]->correctA(me);
    a1[0][1] = mg->states[0]->qsets[xprts[e1]->strat[0]]->correctA(1-me);
    a1[1][0] = mg->states[0]->qsets[xprts[e1]->strat[1]]->correctA(me);
    a1[1][1] = mg->states[0]->qsets[xprts[e1]->strat[1]]->correctA(1-me);
    a2[0][0] = mg->states[0]->qsets[xprts[e2]->strat[0]]->correctA(me);
    a2[0][1] = mg->states[0]->qsets[xprts[e2]->strat[0]]->correctA(1-me);
    a2[1][0] = mg->states[0]->qsets[xprts[e2]->strat[1]]->correctA(me);
    a2[1][1] = mg->states[0]->qsets[xprts[e2]->strat[1]]->correctA(1-me);
    
    if ((((a1[0][0] == a2[0][0]) && (a1[1][0] == a2[1][0])) && ((a1[0][1] == a2[0][1]) && (a1[1][1] == a2[1][1]))) ||
        (((a1[0][0] == a2[1][0]) && (a1[1][0] == a2[0][0])) && ((a1[0][1] == a2[1][1]) && (a1[1][1] == a2[0][1]))))
        return true;
    else
        return false;
}

bool mega::isUnique2(int ind, int numUnique, int *uniqueSolutions) {
    int b_ind[2];
    double Vme = mg->startStates[0]->qsets[ind]->V;
    int i, j;
    
    for (i = 0; i < mg->startStates[0]->numActions[0]; i++) {
        for (j = 0; j < mg->startStates[0]->numActions[1]; j++) {
            if (mg->startStates[0]->qsets[ind]->Qs[i][j] == Vme) {
                b_ind[0] = i;
                b_ind[1] = j;
            }
        }
    }
    
    double d;
    for (i = 0; i < numUnique; i++) {
        d = fabs(mg->startStates[0]->qsets[uniqueSolutions[i]]->M[b_ind[0]][b_ind[1]][0] - mg->startStates[0]->qsets[ind]->M[b_ind[0]][b_ind[1]][0]);
        d += fabs(mg->startStates[0]->qsets[uniqueSolutions[i]]->M[b_ind[0]][b_ind[1]][1] - mg->startStates[0]->qsets[ind]->M[b_ind[0]][b_ind[1]][1]);
        if (d < 0.1)
            return false;
    }
    
    return true;
}


bool mega::isUnique(int ind, int numUnique, int *uniqueSolutions) {
    int i, j, k;
    int acts[2];
    
    acts[0] = mg->startStates[0]->numActions[0];
    acts[1] = mg->startStates[0]->numActions[1];
    //printf("acts: (%i, %i); numUnique = %i\n", acts[0], acts[1], numUnique);
    //printf("test vs. %i ... ", numUnique);
    
    double d;
    for (i = 0; i < numUnique; i++) {
        bool diff = false;
        for (j = 0; j < acts[0]; j++) {
            for (k = 0; k < acts[1]; k++) {
                d = fabs(mg->startStates[0]->qsets[uniqueSolutions[i]]->M[j][k][0] - mg->startStates[0]->qsets[ind]->M[j][k][0]);
                d += fabs(mg->startStates[0]->qsets[uniqueSolutions[i]]->M[j][k][1] - mg->startStates[0]->qsets[ind]->M[j][k][1]);
                //printf("%lf ... ", d);
                if (d >= 0.01) {
                    diff = true;
                }
            }
        }
        if (!diff)
            return false;
    }
    
    return true;
}

void mega::getMax(State *s, double w[2], double R[2]) {
    int i, j;
    int mIndi = -1, mIndj = -1;
    double mx = -999999, val;
    
    int qind = s->findQset(w[0]);
    
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            //val = w[0] * (s->qsets[qind]->M[i][j][0] - s->mm[0]->mv) + w[1] * (s->qsets[qind]->M[i][j][1] - s->mm[1]->mv);
            val = w[0] * s->qsets[qind]->M[i][j][0] + w[1] * s->qsets[qind]->M[i][j][1];
            if (val > mx) {
                mx = val;
                mIndi = i;
                mIndj = j;
            }
        }
    }
    
    //R[0] = s->qsets[qind]->M[mIndi][mIndj][0] - s->mm[0]->mv;
    //R[1] = s->qsets[qind]->M[mIndi][mIndj][1] - s->mm[1]->mv;
    R[0] = s->qsets[qind]->M[mIndi][mIndj][0];
    R[1] = s->qsets[qind]->M[mIndi][mIndj][1];
}

void mega::moveUpdate(State *s, State *sprime, int acts[2], int experto, double rPayout[2], double aspiration, int corA) {
    //printf("mega: moveUpdate\n");

    //printf("<%i %i>", acts[0], acts[1]); fflush(stdout);

    s->cuentas[acts[0]][acts[1]]++;

    //printf("o"); fflush(stdout);
    
    lacts[0] = acts[0];
    lacts[1] = acts[1];

    stateHistory[numStateHistory] = s->ID;
    numStateHistory++;
    
    //printf("o"); fflush(stdout);
    
    if (repeatedVisit() && bron) {
        if (utilitarian)
            mg->computeBRu(me, true, 0);
        else
            mg->computeBR(me, true);
    }
    
    // update conformance to each high-level action
    updateConformance(s, acts);

    //printf("b"); fflush(stdout);
    
    char message[1024];
    if ((xprts[experto]->tipo == FOLLOWER) || (xprts[experto]->tipo == LEADER) || (xprts[experto]->tipo == LEADER2)) {
        if ((corA >= 0) && (corA < mg->numLabels)) {
            //printf("!%i!", corA);
            //sprintf(message, "%s %s", mg->actionDescription, mg->actionLabels[corA]);
            sprintf(message, "%s", mg->actionLabels[corA]);
        }
        else
            strcpy(message, "");
    }
    else {
        strcpy(message, "");
    }

    xprts[experto]->updateMove(sprime, rPayout, me, acts, aspiration, message);
}

void mega::roundUpdate(double rPayout[2]) {

    if (bron) {
        if (utilitarian)
            mg->computeBRu(me, true, 0);
        else
            mg->computeBR(me, true);
    }
    
    int i;
    
/*
    for (i = 0; i < 2; i++) {
        printf("conformance of player %i:\n", i);
        for (int j = 0; j < mg->states[0]->numQsets; j++) {
            printf("%i: %lf\n", j, conformance[i][j]);
        }
    }
*/

    //highestInd[0] = highestInd[1] = 0;
    highestInd[0] = lacts[0];
    highestInd[1] = lacts[1];
/*    for (i = 0; i < numHighLevelActions; i++) {
        if (conformance[0][i] > conformance[0][highestInd[0]])
            highestInd[0] = i;
        if (conformance[1][i] > conformance[1][highestInd[1]])
            highestInd[1] = i;
    }*/
    for (i = 0; i < numHighLevelActions; i++) {
        if (fabs(conformance[0][i] - conformance[0][highestInd[0]]) < 0.001)
            lastActions[0][i] = 1;
        else
            lastActions[0][i] = 0;
    }

    for (i = 0; i < numHighLevelActions; i++) {
        if (fabs(conformance[1][i] - conformance[1][highestInd[1]]) < 0.001)
            lastActions[1][i] = 1;
        else
            lastActions[1][i] = 0;
    }
    
    numStateHistoryPrev = numStateHistory;
    for (i = 0; i < numStateHistory; i++) {
        stateHistoryPrev[i] = stateHistory[i];
    }
    
    reset();
}

bool mega::repeatedVisit() {
    int i;
    
    //printf("check ... "); fflush(stdout);
    for (i = 0; i < numStateHistory-1; i++) {
        if (stateHistory[i] == stateHistory[numStateHistory-1]) {
            //printf("repeated visit\n");
            return true;
        }
    }
    //printf("none\n"); fflush(stdout);
    
    return false;
}

void mega::reset() {
    numStateHistory = 0;
    //numStateHistoryPrev = 0;
    //rPayout[0] = rPayout[1] = 0.0;

    int i;
    for (i = 0; i < numHighLevelActions; i++) {
        conformance[0][i] = conformance[1][i] = 0.0;
    }
}

int mega::selectSolutions(int maxSols, Solution *sols[MAX_EXPERTS], int numSols, double mv[2]) {
    int selected[MAX_EXPERTS];
    double score[MAX_EXPERTS];
    int numSelected = 0;
    int i, j;
    int ind;
    double mx, mn;
    
    printf("select %i solutions\n", maxSols); fflush(stdout);
    printf("mv = (%lf, %lf)\n", mv[0], mv[1]);
    
    if (maxSols > 1) {
        // find the extreme points (highest for each person, but greater than mv)
        for (j = 0; j < 2; j++) {
            mx = -99999;
            ind = -1;
            for (i = 0; i < numSols; i++) {
            if (sols[i]->selected || (sols[i]->payouts[0] < (mv[0]-0.000001)) || (sols[i]->payouts[1] < (mv[1]-0.00000001)))
                    continue;
                
                if ((sols[i]->payouts[j] > mx) || ((sols[i]->payouts[j] == mx) && (rand() % 2))) {
                    mx = sols[i]->payouts[j];
                    ind = i;
                }
            }
            
            if (ind < 0) {
                printf("selection gone bad\n");
                exit(1);
            }
            
            sols[ind]->selected = true;
            sols[ind]->counter = numSelected;
            selected[numSelected] = ind;
            numSelected++;
            
            //printf("extreme %i = %i: (%lf, %lf)\n", j, ind, sols[ind]->payouts[0], sols[ind]->payouts[1]);
        }
        
        printf("selected[0] = %i\nselected[1] = %i\n", selected[0], selected[1]);
        if (selected[0] < selected[1])
            backward = true;
    }
    
    
    if ((maxSols == 1) || (maxSols > 2)) {
        
        // select the egalitarian equilibrium
        mx = -99999;
        ind = -1;
        for (i = 0; i < numSols; i++) {
            if (sols[i]->selected || (sols[i]->payouts[0] < (mv[0]-0.000001)) || (sols[i]->payouts[1] < (mv[1]-0.00000001)))
                continue;
            
            if (sols[i]->payouts[1] < sols[i]->payouts[0])
                mn = sols[i]->payouts[1];
            else
                mn = sols[i]->payouts[0];
            
            if ((mn > mx) || ((mn == mx) && (rand() % 2))) {
                mx = mn;
                ind = i;
            }
        }
        sols[ind]->selected = true;
        sols[ind]->counter = numSelected;
        selected[numSelected] = ind;
        numSelected++;
        
        printf("selected[%i] = %i\n", numSelected - 1, selected[numSelected-1]);
        
        //printf("egalitarian point = %i: (%lf, %lf)\n", ind, sols[ind]->payouts[0], sols[ind]->payouts[1]);
        //egalPoint = ind;
    }
    
    while (numSelected < maxSols) {
        double normDist = dist(sols[selected[0]]->payouts, sols[selected[1]]->payouts);
        //printf("normDist = %lf\n", normDist);
        
        bool one = false;
        for (i = 0; i < numSols; i++) {
            if (sols[i]->selected || (sols[i]->payouts[0] < (mv[0]-0.000001)) || (sols[i]->payouts[1] < (mv[1]-0.00000001)))
                score[i] = -1;
            else {
                score[i] = distance(i, sols, numSols) / normDist;
                if (score[i] > 0.00001) {
                    one = true;
                    if (!isParetoDominated(i, sols, numSols))
                        score[i] += 0.05;
                    if (sols[i]->punishmentRatio <= 1)
                        score[i] += 0.05;
                    if (sols[i]->indices[0] == sols[i]->indices[1])
                        score[i] += 0.05;
                }
                //printf("%i: %lf\n", i, score[i]);
            }
        }
        
        if (!one)
            break;
        
        ind = -1;
        mx = -99999;
        for (i = 0; i < numSols; i++) {
            if ((score[i] > mx) || ((score[i] == mx) && (rand() % 2))) {
                ind = i;
                mx = score[i];
            }
        }
        
        if (ind == -1)
            break;

        //printf("picked %i: (%lf, %lf)\n", ind, sols[ind]->payouts[0], sols[ind]->payouts[1]);
        sols[ind]->selected = true;
        sols[ind]->counter = numSelected;
        selected[numSelected] = ind;
        numSelected++;
        
        printf("selected[%i] = %i\n", numSelected - 1, selected[numSelected-1]);
    }
    
    return numSelected;
}

void mega::findHighFair(double barR[2], char gname[1024]) {
    barR[0] = barR[1] = 0.0;
/*
    if (!strcmp(gname, "prisoners"))
        barR[0] = barR[1] = 16.0;
    else if (!strcmp(gname, "legos"))
        barR[0] = barR[1] = 18.0;
    else if (!strcmp(gname, "gridwar1")) {
        barR[0] = 20.40;
        barR[1] = 19.70;
    }
    else {
        barR[0] = barR[1] = 0.0;
        //printf("preventer barR not found\n");
        //exit(1);
    }
*/
}

bool mega::isParetoDominated(int index, Solution *sols[MAX_EXPERTS], int numSols) {
    int i;
    
    for (i = 0; i < numSols; i++) {
        if (i == index)
            continue;
        
        if ((sols[i]->payouts[0] > sols[index]->payouts[0]) && (sols[i]->payouts[1] > sols[index]->payouts[1])) {
            //printf("%lf, %lf is pareto dominated\n", sols[index]->payouts[0], sols[index]->payouts[1]);
            return true;
        }
    }
    
    return false;
}

double mega::distance(int index, Solution *sols[MAX_EXPERTS], int numSols) {
    int i;
    double minDist = 99999, d;
    
    for (i = 0; i < numSols; i++) {
        if (sols[i]->selected) {
            d = dist(sols[i]->payouts, sols[index]->payouts);
            if (d < minDist)
                minDist = d;
        }
    }
    
    //printf("minDist = %lf\n", minDist);
    
    return minDist;
}

double mega::dist(double P1[2], double P2[2]) {
    //printf("P1: %lf, %lf; P2: %lf, %lf\n", P1[0], P1[1], P2[0], P2[1]);
    
    return sqrt((P1[0] - P2[0]) * (P1[0] - P2[0]) + (P1[1] - P2[1]) * (P1[1] - P2[1]));
}

void mega::determineRewardBins(double separator[2][3], Solution *sols[MAX_EXPERTS], int numPureSols) {
    int i, j;
    double *l1 = new double[numPureSols+1];
    double *l2 = new double[numPureSols+1];
    double tmp;
    int num1 = numPureSols+1, num2 = numPureSols+1;
    
    l1[0] = mg->startStates[0]->mm[0]->mv;
    l2[0] = mg->startStates[0]->mm[1]->mv;
    for (i = 0; i < numPureSols; i++) {
        l1[i+1] = sols[i]->payouts[0];
        l2[i+1] = sols[i]->payouts[1];
    }
    
    for (i = 0; i < numPureSols+1; i++) {
        for (j = 0; j < numPureSols; j++) {
            if (l1[j] > l1[j+1]) {
                tmp = l1[j];
                l1[j] = l1[j+1];
                l1[j+1] = tmp;
            }
            if (l2[j] > l2[j+1]) {
                tmp = l2[j];
                l2[j] = l2[j+1];
                l2[j+1] = tmp;
            }
        }
    }
    
//    printf("0: ");
//    for (i = 0; i < numPureSols+1; i++) {
//        printf("%lf ", l1[i]);
//    }
//    printf("\n");
    
//    printf("1: ");
//    for (i = 0; i < numPureSols+1; i++) {
//        printf("%lf ", l2[i]);
//    }
//    printf("\n");
    
    for (i = num1-1; i >= 1; i--) {
        if (fabs(l1[i] - l1[i-1]) < 0.001) {
            num1--;
            for (j = i; j < num1; j++)
                l1[j] = l1[j+1];
        }
    }
//    printf("0: ");
//    for (i = 0; i < num1; i++) {
//        printf("%lf ", l1[i]);
//    }
//    printf("\n");
    for (i = num2-1; i >= 1; i--) {
        if (fabs(l2[i] - l2[i-1]) < 0.001) {
            num2--;
            for (j = i; j < num2; j++)
                l2[j] = l2[j+1];
        }
    }
//    printf("1: ");
//    for (i = 0; i < num2; i++) {
//        printf("%lf ", l2[i]);
//    }
//    printf("\n");
    
    double temper[2][4];
    temper[0][0] = l1[0];
    temper[0][1] = l1[(int)(num1 / 4.0)];
    temper[0][2] = l1[(int)(3.0 * num1 / 4.0)-1];
    temper[0][3] = l1[num1-1];
    //printf("%lf %lf %lf %lf\n", temper[0][0], temper[0][1], temper[0][2], temper[0][3]);
    temper[1][0] = l2[0];
    temper[1][1] = l2[(int)(num1 / 4.0)];
    temper[1][2] = l2[(int)(3.0 * num1 / 4.0)-1];
    temper[1][3] = l2[num1-1];
    //printf("%lf %lf %lf %lf\n", temper[1][0], temper[1][1], temper[1][2], temper[1][3]);
    
    for (i = 0; i < 2; i++) {
        printf("separator %i: ", i);
        for (j = 0; j < 3; j++) {
            separator[0][j] = (temper[i][j] + temper[i][j+1]) / 2.0;
            printf("%.2lf ", separator[0][j]);
        }
        printf("\n");
    }
    
    delete[] l1;
    delete[] l2;
/*
    double low[2], high[2];
    
    low[0] = mg->startStates[0]->mm[0]->mv;
    low[1] = mg->startStates[0]->mm[1]->mv;
    high[0] = high[1] = -99999;
    
    for (i = 0; i < numPureSols; i++) {
        for (j = 0; j < 2; j++) {
            if (sols[i]->payouts[j] > high[j])
                high[j] = sols[i]->payouts[j];
            if (sols[i]->payouts[j] < low[j])
                low[j] = sols[i]->payouts[j];
        }
    }
    
    printf("0: [%.2lf, %.2lf)\n", low[0], high[0]);
    printf("0: [%.2lf, %.2lf)\n", low[1], high[1]);
*/
}

void mega::adjustBRPriors() {
    int s, a, j;
    
    double pwrVal = 1;
    
    for (s = 0; s < mg->numStates; s++) {
        for (a = 0; a < mg->states[s]->numActions[0]; a++) {
            for (j = 0; j < mg->states[s]->numActions[1]; j++) {
                mg->states[s]->cuentas[a][j] = mg->states[s]->cuentas_real[a][j];
            }
        }
    }
}

void mega::updateConformance(State *s, int acts[2]) {
    double pi[20];
    int i, j;
    double probs[2*NUMACTIONS];
    
    for (j = 0; j < 2; j++) {
        for (i = 0; i < s->numActions[j]; i++) {
            if (acts[j] != i)
                pi[i] = 0.0;
            else
                pi[i] = 1.0;
        }
        
        conformance[j][0] += policyAgreement(s->mm[j]->ms, pi, s->numActions[j]);
        conformance[j][1] += policyAgreement(s->attack[j]->ms, pi, s->numActions[j]);
        
        for (i = 0; i < s->numQsets; i++)
            conformance[j][2+i] += s->qsets[i]->getConformance(j, acts[j]);
    }
}

void mega::determineHighLevelActions() {
    numHighLevelActions = mg->states[0]->numQsets + 2;
    
    int i;
    for (i = 0; i < numHighLevelActions; i++)
        lastActions[0][i] = lastActions[1][i] = -1;
}

// tdiff = 0 means complete policy agreement
double mega::policyAgreement(double *p1, double *p2, int numActs) {
    double tdiff = 0.0;
    int i;
    
    //printf("policy agreement (%i)\n", numActs); fflush(stdout);
    
    for (i = 0; i < numActs; i++) {
        //printf("%i: %lf vs %lf\n", i, p1[i], p2[i]);
        tdiff += fabs(p1[i] - p2[i]);
    }
    
    //printf("tdiff = %lf\n", tdiff);
    
    tdiff = 1 - tdiff/2;
    if (tdiff < 0)
        tdiff = 0;
    
    return tdiff;

    //printf("tdiff = %lf\n", tdiff);
    
    /*if (tdiff > 0.1) {
        printf("tdiff = %.3lf\n", tdiff);
        for (i = 0; i < numActs; i++)
            printf("%i: %lf vs %lf\n", i, p1[i], p2[i]);
        printf("\n");
    }*/
    
    //return tdiff / 2.0;
}

