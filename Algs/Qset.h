#ifndef __Solver__Qset__
#define __Solver__Qset__

#include "defs.h"

class Qset {
public:
    Qset();
    Qset(double _w, int _numActions[2]);
    ~Qset();
    
    int highestAction(int _me, double *probs);
    double maxVal();
    double maxM(int index);
    double correctM(int index);
    int correctA(int index);
    void printQs();
    bool amongBest(int v[2]);
    void setTie(int v[2]);
    
    double getConformance(int ind, int act);
    
    double w;
    int numActions[2];
    double **Qs, V;
    double ***M;
    
    int tieIndex;
    //bool setTheTie;
    int bestActions[2][10], bestCount;
};

#endif
