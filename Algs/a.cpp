#include "a.h"

a::a() {
    printf("incomplete constructor\n");
    exit(1);
}

a::a(int _me, double _lambda, int _numExperts, double hval, double lval, double mm) {
    me = _me;
    numExperts = _numExperts;
	lambda = _lambda;
    highPay = hval;
    lowPay = lval;
    mmPay = mm;
    
    printf("mmPay = %lf\n", mmPay);
    
    //printf("aspiration = %.3lf\n", aspiration);
    lastExpert = -1;
    
    int i;
    x_phi = new double[numExperts];
    cuenta = new int[numExperts];
    S_e = new int[numExperts];
    recentSuccess = new double[numExperts];
    for (i = 0; i < numExperts; i++) {
        x_phi[i] = 0;
        cuenta[i] = 0;
        S_e[i] = 0;
        recentSuccess[i] = 0.0;
    }
    
    listening2Him = false;
    
    rho = -1.0;
}

a::~a() {
    delete S_e;
    delete recentSuccess;
}

int a::select(bool *choices) {
	if (lastExpert == -1)
        lastExpert = randomlySelect(choices);
    else {
        if (!choices[lastExpert])
            rho = -1.0; // takes care of override
        
        printf("aspiration = %lf\n", aspiration);
        printf("rho = %lf\n", rho);
        
        double num = ((double)(rand())) / RAND_MAX;
        //printf("satisfied? (%.2lf vs. %.2lf)\n", num, rho);
        if (num > rho) 
            lastExpert = randomlySelect(choices);
    }
    
    //printf("Selected: %i\n", lastExpert);
	
	return lastExpert;
}

int a::select(bool *choices, bool *proposed, CommAgent *_comms, bool shListen, int tsProposal) {
    int olOne = lastExpert;
    
    printf("Satisficing experts: ");
    for (int i = 0; i < numExperts; i++) {
        if (choices[i])
            printf("%i ", i);
    }
    printf("\n");
    
    
	if (lastExpert == -1)
        lastExpert = randomlySelect(choices);
    else {
        if (!choices[lastExpert])
            rho = -1.0; // takes care of override
        
        printf("aspiration = %lf\n", aspiration);
        printf("rho = %lf\n", rho);
        
        bool picks[100];
        int cnt = numSatProp(choices, proposed, picks);
        //printf("num satisficing proposals: %i\n", cnt);
        
        if ((rho < 0.9999) && (cnt > 0) && shListen) {
            lastExpert = randomlySelect(picks);
            //if (olOne != lastExpert)  // has to be picking a new expert
            if (lastExpert != olOne) {
                if (tsProposal > 1)
                    _comms->logEvent(MSG_ASSESSMENT, 7, "1 ");
                else
                    _comms->logEvent(MSG_ASSESSMENT, 7, "0 ");
            }
        }
        else {
            double num = ((double)(rand())) / RAND_MAX;
            if (num > rho)
                lastExpert = randomlySelect(choices);
        }
    }
    
    //printf("Selected: %i\n", lastExpert);
    
    if ((proposed[lastExpert]) || ((lastExpert > 2) && (((lastExpert-3) / 2) == ((olOne-3) / 2)) && listening2Him)) {
        listening2Him = true;
        //printf("\ta.cpp: listening\n");
        //printf("------------ I am listening ------------\n");
    }
    else {
        //printf("\ta.cpp: not listening\n");
        //printf("\t\tlastExpert = %i\n", lastExpert);
        //printf("\t\tdivs: %i vs %i\n", (lastExpert-3) / 2, (olOne-3) / 2);
        //printf("\t\tlistening2Him = %i\n", (int)listening2Him);
        listening2Him = false;
        //printf("------------ NOT Listening -------------\n");
    }
	
	return lastExpert;
}

int a::randomlySelect(bool *choices) {
    int i, cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i])
            cnt ++;
    }
    
    if (cnt == 0) {
        printf("no choices\naspiration = %lf\n", aspiration);
        exit(1);
    }
    
    int pick = rand() % cnt;
    //printf("rndpick = %i\n", pick);
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

int a::numSatProp(bool *choices, bool *proposed, bool picks[100]) {
    int i, cnt = 0;
    for (i = 0; i < numExperts; i++) {
        if (choices[i] && proposed[i]) {
            picks[i] = true;
            cnt ++;
        }
        else {
            picks[i] = false;
        }
    }
    return cnt;
}

void a::update(double R, int tau) {
    printf("shouldn't get to this update\n");
    exit(1);
/*    int i;
    for (i = 0; i < tau; i++)
        aspiration = lambda * aspiration + (1.0 - lambda) * R;

    double beta = 1.0 / (cuenta[lastExpert]+1);
    if (beta < 0.2)
        beta = 0.2;
    recentSuccess[lastExpert] = beta * R + (1.0 - beta) * recentSuccess[lastExpert];
    
    printf("recent (%i): %lf\n", cuenta[lastExpert]+1, recentSuccess[lastExpert]);
    
    double Rprime = R;
    if (recentSuccess[lastExpert] > Rprime)
        Rprime = recentSuccess[lastExpert];
    
    if (aspiration > mmPay)
        rho = (Rprime-mmPay) / (aspiration-mmPay);
    else {
        if (Rprime > aspiration)
            rho = 1.0;
        else
            rho = 0.0;
    }
    
	if (rho > 1.0)
		rho = 1.0;
    if (rho < 0.0)
        rho = 0.0;
    
	rho = pow(rho, tau);
    
    S_e[lastExpert] += tau;
    x_phi[lastExpert] += ((double)tau / S_e[lastExpert]) * (R - x_phi[lastExpert]);
    cuenta[lastExpert] ++;*/
}

void a::update(double R, int heldTrueCount, int tau) {
    if (tau == 0)
        return;

    int i;
    for (i = 0; i < tau; i++)
        aspiration = lambda * aspiration + (1.0 - lambda) * R;

    double beta = 1.0 / (cuenta[lastExpert]+1);
    if (beta < 0.2)
        beta = 0.2;
    recentSuccess[lastExpert] = beta * R + (1.0 - beta) * recentSuccess[lastExpert];
    
    printf("recent (%i): %lf\n", cuenta[lastExpert]+1, recentSuccess[lastExpert]);
    printf("R = %lf\n", R);
    
    printf("tau = %i; heldTrueCount = %i\n", tau, heldTrueCount);
    if (heldTrueCount == tau) {
        rho = 1.0;
    }
    else {
        double Rprime = R;
        if (recentSuccess[lastExpert] > Rprime)
            Rprime = recentSuccess[lastExpert];
        
        if (aspiration > mmPay)
            rho = (Rprime-mmPay) / (aspiration-mmPay);
        else {
            if (Rprime > aspiration)
                rho = 1.0;
            else
                rho = 0.0;
        }
        
        if (rho > 1.0)
            rho = 1.0;
        if (rho < 0.0)
            rho = 0.0;
        
        rho = pow(rho, tau);
    }
    
    S_e[lastExpert] += tau;
    x_phi[lastExpert] += ((double)tau / S_e[lastExpert]) * (R - x_phi[lastExpert]);
    cuenta[lastExpert] ++;
}

bool a::willAccept(double R, int heldTrueCount, int tau) {
    double beta = 1.0 / (cuenta[lastExpert]+1);
    if (beta < 0.2)
        beta = 0.2;
    double recent = beta * R + (1.0 - beta) * recentSuccess[lastExpert];

    double Rprime = R;
    if (recent > Rprime)
        Rprime = recentSuccess[lastExpert];
    
    double jo;
    if (aspiration > mmPay)
        jo = (Rprime-mmPay) / (aspiration-mmPay);
    else {
        if (Rprime > aspiration)
            jo = 1.0;
        else
            jo = 0.0;
    }
    
    if (jo > 1.0)
        jo = 1.0;
    if (jo < 0.0)
        jo = 0.0;
    
    printf("jo = %lf ... ", jo);
    jo = pow(jo, tau);
    printf("%lf\n", jo);
    
    if (jo < 0.9999)
        return true;
    else
        return false;
}

