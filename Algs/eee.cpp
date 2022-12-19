#include "eee.h"

eee::eee() {
}

eee::eee(int _me, double _lambda, int _numExperts, double _maxmin, double *_potentials) {
    printf("eee constructor: %lf\n", _maxmin);

  	me = _me;
	numExperts = _numExperts;//_nActions;
    lambda = _lambda;
    maxmin = _maxmin;

    M_e = new double[numExperts];
    N_e = new int[numExperts];
    S_e = new int[numExperts];
    potentials = new double[numExperts];
    int i;
    for (i = 0; i < numExperts; i++) {
        N_e[i] = 0;
        M_e[i] = 0.0;
        S_e[i] = 0.0;
        
        potentials[i] = _potentials[i];
    }

	t = 0;
    experto = -1;
    
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        x_phi[i] = 0;
        cuenta[i] = 0;
    }
    
    rho = -1;
    
    theBest = 0;
    
    listening2Him = false;
    berandom = true;
}

eee::~eee() {
    delete M_e;
    delete N_e;
    delete S_e;
    delete potentials;
}

int eee::select(bool *choices) {
	if (t == 0) {
		experto = randomlySelect(choices);
    }
	else {
        //double prob = 0.1;
        double prob = 1.0 / (10 + (t/10.0));
        double num = ((double)rand()) / RAND_MAX;
        if ((num < prob) && berandom) {
            experto = randomlySelect(choices);
        }
        else {
            double best = -99999;
            experto = -1;
            int bestCount = 0;
            int i;
            //printf("M_e: ");
            for (i = 0; i < numExperts; i++) {
                //printf("%.2lf", M_e[i]);
                if (choices[i]) {
                    //printf("x ");
                    if (getValue(i, false) > best) {
                        best = getValue(i, false);
                        experto = i;
                        bestCount = 1;
                    }
                    else if (getValue(i, false) == best) {
                        bestCount ++;
                    }
                }
                //else
                //    printf(" ");
            }
            
            if (bestCount > 1) {
                int num2 = rand() % bestCount;
                int c = 0;
                for (i = 0; i < numExperts; i++) {
                    if ((getValue(i, false) == best) && choices[i])
                        c ++;
                    
                    if (c > num2) {
                        experto = i;
                        break;
                    }
                }
            }
            //printf("\n");
        }
    }

    berandom = true;
    
    return experto;
}

int eee::select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal) {
    int olOne = experto;
    
    //printf("donde estoy? t = %i\n", t);
	if (experto == -1) {//(t == 0) {
        //printf("t = 0; random\n");
		experto = randomlySelect(choices);
    }
	else {
        //double prob = 0.1;
        double prob = 3.0 / (10 + t);
        double num = ((double)rand()) / RAND_MAX;
        if ((num < prob) && berandom) {
            printf("random selection\n");
            experto = randomlySelect(choices);
        }
        else {
            double best = -99999;
            experto = -1;
            int bestCount = 0;
            int i;
            for (i = 0; i < numExperts; i++) {
                if (choices[i]) {
                    if (proposed[i])
                        printf("%i: %.3lf %.3lf %lf (%i)*\n", i, M_e[i], potentials[i], getValue(i, proposed[i]), S_e[i]);
                    else
                        printf("%i: %.3lf %.3lf %lf (%i)\n", i, M_e[i], potentials[i], getValue(i, proposed[i]), S_e[i]);
                    if (getValue(i, proposed[i]) > best) {
                        best = getValue(i, proposed[i]);
                        experto = i;
                        bestCount = 1;
                    }
                    else if (getValue(i, proposed[i]) == best) {
                        bestCount ++;
                    }
                }
            }
            
            printf("bestCount = %i (%i)\n", bestCount, experto);
            
            if (bestCount > 1) {
                int num2 = rand() % bestCount;
                int c = 0;
                for (i = 0; i < numExperts; i++) {
                    if ((getValue(i, proposed[i]) == best) && choices[i])
                        c ++;
                    
                    if (c > num2) {
                        experto = i;
                        break;
                    }
                }
            }
            //printf("\n");
        }
    }
    
    if ((experto != olOne) && proposed[experto]) {
        if (tsProposal > 1)
            _comms->logEvent(MSG_ASSESSMENT, 7, "1 ");
        else
            _comms->logEvent(MSG_ASSESSMENT, 7, "0 ");
    }


    if ((proposed[experto]) || ((experto > 2) && (((experto-3) / 2) == ((olOne-3) / 2)) && listening2Him)) {
        listening2Him = true;
    }
    else {
        listening2Him = false;
    }

    berandom = true;
    
    return experto;
}

int eee::randomlySelect(bool *choices) {
    int i, cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i])
            cnt ++;
    }
    
    int pick = rand() % cnt;
    cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i]) {
            if (cnt == pick)
                return i;
            cnt ++;
        }
    }
    
    printf("we have a problem\n");
    exit(1);
    
    return -1;
}

void eee::update(double R, int tau) {
    if (tau == 0)
        return;

    int i;    
    for (i = 0; i < tau; i++) {
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    }

    // this is for assessing contentment
    if (M_e[experto] > R)
        rho = 0;
    else
        rho = 1;
    // end of assessing contentment
    
    N_e[experto] ++;
    S_e[experto] += tau;
    M_e[experto] += ((double)tau / S_e[experto]) * (R - M_e[experto]);
    
    //printf("updating %i\n", experto);
    x_phi[experto] = M_e[experto];
    cuenta[experto] = N_e[experto];
    
    t++;
}

void eee::update(double R, int heldTrueCount, int tau) {
    if (tau == 0)
        return;

    int i;    
    for (i = 0; i < tau; i++) {
        aspiration = lambda * aspiration + (1.0 - lambda) * R;
    }

    // this is for assessing contentment
    if ((theBest > R) && (heldTrueCount < tau))
        rho = 0;
    else
        rho = 1;
    // end of assessing contentment
    
    N_e[experto] ++;
    S_e[experto] += tau;
    M_e[experto] += ((double)tau / S_e[experto]) * (R - M_e[experto]);
    
    //printf("updating %i\n", experto);
    x_phi[experto] = M_e[experto];
    cuenta[experto] = N_e[experto];
    
    t++;
    
    findTheBest();
}

void eee::findTheBest() {
    int i;
    double theBest = -1;
    for (i = 0; i < numExperts; i++) {
        if (i == 2) // skip PREVENT for matrix games
            continue;

        if (getValue(i, false) > theBest)
            theBest = getValue(i, false);
    }
}

double eee::getValue(int index, bool proposed) {
    double val;

    if (S_e[index] == 0)
        val = maxmin;
    else
        val = M_e[index];
    
    double w = 1.0 / (1.0 + (S_e[index] / 9.0));
    if (!proposed)
        return val;
    else
        return (w * potentials[index]) + ((1-w) * val);
    
}

