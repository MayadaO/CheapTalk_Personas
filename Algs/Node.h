#ifndef __Solver__Node__
#define __Solver__Node__

#include "defs.h"
#include "State.h"

class Node {
public:
    Node();
    Node(int _nID, int _me, State *_s, int _depth, bool isGoal);
    ~Node();
    
    double computeUtility(int index);
    void updateRegret(int index);
    void updatePolicy(int index, int T);
    void initCounts();
    void updateSigmaNot();
    
    void print(int index, int T);
    int mfactorial(int choices);
    
    State *s;
    int depth, me;
    Node *next[20];
    int numNext;
    int duplo;
    int nID;
    
    double **sigma_t, **regret;
    double reachability[2], utility[2];
    
    bool visited[2];
    
    double *cuentita;
};

#endif /* defined(__Solver__Node__) */
