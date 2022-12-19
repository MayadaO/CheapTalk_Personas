#ifndef __Solver__CFR__
#define __Solver__CFR__

#include "defs.h"
#include "Player.h"
#include "MarkovGame.h"
#include "Node.h"
#include "GameEngine.h"

#define MAX_NODES       50000
#define MAX_DEPTH       40

class CFR : public Player {
public:
    CFR();
    CFR(int _me, MarkovGame *_mg, int _maxDepth, char gameName[1024], bool _update);
    ~CFR();
    
    int Move(GameEngine *ge);
    int moveUpdate(GameEngine *ge, int actions[2], double dollars[2]);
    int roundUpdate();
    int generateAction(double *v, int numActs);
    
    void buildGameTree();
    void printGameTree(char gameName[1024]);
    void readGameTree(char gameName[1024]);
    int findNode(State *s, int nivel);
    bool isGoalState(int s);
    int alreadyExists(State *s, int strt, int nd);
    int alreadyExists(State *s);
    int alreadyExists(int s);
    void initAlready();
    void computeNE(char gameName[1024]);
    
    void reset();
    void initCounts();
    void updateMe();
    void computeUtilities(int index);
    void computeReachability(int index);
    void updateRegret(int index);
    void updatePolicy(int index, int T);
    
    int me;
    MarkovGame *mg;
    int maxDepth, actualDepth;
    
    Node *gameTree[MAX_NODES*MAX_DEPTH];
    int numNodes;
    int levelCounter[MAX_DEPTH+1];
    
    Node *cNode;
    bool lastValidActs[2][NUMACTIONS*2];
    
    bool update;
    
    int numMovemientos, t;
};

#endif /* defined(__Solver__CFR__) */
