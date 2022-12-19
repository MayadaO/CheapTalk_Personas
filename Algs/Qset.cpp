#include "Qset.h"

Qset::Qset() {
    printf("incomplete Qset constructor\n");
    exit(1);
}

Qset::Qset(double _w, int _numActions[2]) {
    int i, j;
    
    w = _w;
    numActions[0] = _numActions[0];
    numActions[1] = _numActions[1];
    
    // value function
    Qs = new double*[numActions[0]];
    for (i = 0; i < numActions[0]; i++) {
        Qs[i] = new double[numActions[1]];
        for (j = 0; j < numActions[1]; j++)
            Qs[i][j] = 0.0;
    }
    V = 0.0;
    
    M = new double**[numActions[0]];
    for (i = 0; i < numActions[0]; i++) {
        M[i] = new double*[numActions[1]];
        for (j = 0; j < numActions[1]; j++) {
            M[i][j] = new double[2];
            M[i][j][0] = 0.0;
            M[i][j][1] = 0.0;
        }
    }
    
    tieIndex = -1;
    //setTheTie = false;
    
    bestCount = 0;
}

Qset::~Qset() {
    int i, j;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++)
            delete M[i][j];
        delete Qs[i];
        delete M[i];
    }
    
    delete Qs;
    delete M;
}

int Qset::highestAction(int _me, double *probs) {
    //int bacts[2];
    double mx = -99999;
    int i, j;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if ((Qs[i][j] > mx)) {
                mx = Qs[i][j];
                //bacts[0] = i;
                //bacts[1] = j;
            }
        }
    }
    
    // find out which of my actions have a high
    bool hasIt[2*NUMACTIONS];
    for (i = 0; i < numActions[_me]; i++) {
        hasIt[i] = false;
        for (j = 0; j < numActions[1-_me]; j++) {
            if (_me == 0) {
                if (fabs(Qs[i][j] - mx) < 0.0001)
                    hasIt[i] = true;
            }
            else {
                if (fabs(Qs[j][i] - mx) < 0.0001)
                    hasIt[i] = true;
            }
        }
    }
    
    double vals[2*NUMACTIONS];
    //printf("between: ");
    for (i = 0; i < numActions[_me]; i++) {
        if (hasIt[i]) {
            //printf("%i ", i);
            vals[i] = 0.0;
            for (j = 0; j < numActions[1-_me]; j++) {
                if (_me == 0)
                    vals[i] += Qs[i][j] * probs[j];
                else
                    vals[i] += Qs[j][i] * probs[j];
            }
        }
        else {
            vals[i] = -99999.0;
        }
    }
    //printf("\n");
    
    int bact = 0;
    mx = vals[0];
    
    for (i = 1; i < numActions[_me]; i++) {
        if (vals[i] > mx) {
            bact = i;
            mx = vals[i];
        }
    }
    
    return bact;
}

double Qset::maxVal() {
    double mx = -99999;
    int i, j;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if ((Qs[i][j] > mx)) {
                mx = Qs[i][j];
            }
        }
    }
    
    return mx;
}

double Qset::correctM(int index) {
    double mx = -99999;
    int b_ind[2];
    int i, j;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if ((Qs[i][j] > mx)) {// || ((Qs[i][j] == mx) && (rand() % 2))) {
                mx = Qs[i][j];
                b_ind[0] = i;
                b_ind[1] = j;
            }
        }
    }
    
    return M[b_ind[0]][b_ind[1]][index];
}

int Qset::correctA(int index) {

    if (tieIndex < 0) {
    //if (!setTheTie) {
        double mx = -99999;
        int i, j;
        
        for (i = 0; i < numActions[0]; i++) {
            for (j = 0; j < numActions[1]; j++) {
                if ((Qs[i][j] > mx)) {// || ((Qs[i][j] == mx) && (rand() % 2))) {
                    mx = Qs[i][j];
                    
                    bestActions[0][0] = i;
                    bestActions[1][0] = j;
                    bestCount = 1;
                }
                else if (Qs[i][j] == mx) {
                    bestActions[0][bestCount] = i;
                    bestActions[1][bestCount] = j;
                    bestCount ++;
                }
            }
        }

        tieIndex = rand() % bestCount;
    }
    
    return bestActions[index][tieIndex];
}

bool Qset::amongBest(int v[2]) {
    int i;
    
    for (i = 0; i < bestCount; i++) {
        if ((bestActions[0][i] == v[0]) && (bestActions[1][i] == v[1]))
            return true;
    }

    return false;
}

void Qset::setTie(int v[2]) {
    int i;
    
    for (i = 0; i < bestCount; i++) {
        if ((bestActions[0][i] == v[0]) && (bestActions[1][i] == v[1])) {
            tieIndex = i;
            //printf("%i <%i, %i>; ", tieIndex, bestActions[0][i], bestActions[1][i]);
            //setTheTie = true;
            return;
        }
    }
    
    printf("bad news\n");
    exit(1);
}


double Qset::maxM(int index) {
    double mx = -99999;
    int i, j;
    
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            if ((M[i][j][index] > mx)) {// || ((Qs[i][j] == mx) && (rand() % 2))) {
                mx = M[i][j][index];
            }
        }
    }
    
    return mx;
}


void Qset::printQs() {
    int i, j;
    for (i = 0; i < numActions[0]; i++) {
        for (j = 0; j < numActions[1]; j++) {
            printf("%.2lf (%.2lf, %.2lf)\t", Qs[i][j], M[i][j][0], M[i][j][1]);
        }
        printf("\n");
    }
}

double Qset::getConformance(int ind, int act) {
    int i, j;
    double high = -99999;
    double low = 99999;
    double actual = -99999;
    for (i = 0; i < numActions[ind]; i++) {
        for (j = 0; j < numActions[1-ind]; j++) {
            if (ind == 0) {
                if (Qs[i][j] > high)
                    high = Qs[i][j];
                if (Qs[i][j] < low)
                    low = Qs[i][j];

                if (act == i) {
                    if (Qs[i][j] > actual)
                        actual = Qs[i][j];
                }
            }
            else {
                if (Qs[j][i] > high)
                    high = Qs[j][i];
                if (Qs[j][i] < low)
                    low = Qs[j][i];

                if (act == i) {
                    if (Qs[j][i] > actual)
                        actual = Qs[j][i];
                }
            }
        }
    }
    
    //double val = 0.0;
    //if (fabs(high - actual) < 0.00001)
    //    val = 1.0;
    
    //printf("%i (%.2lf): val = %lf\n", ind, w, val);
    
    if (fabs(high - low) < 0.00001)
        return 0.5;
    
    double val = (actual - low) / (high - low);
    
    //printf("%i (%.2lf): val = %lf\n", ind, w, val);
    
    return val;
}
