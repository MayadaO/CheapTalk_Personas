#ifndef __Solver__GridWarEngine__
#define __Solver__GridWarEngine__

#include "defs.h"
#include "GameEngine.h"
#include "MarkovGame.h"
#include "State.h"

#define NUM_HOURS   24
#define MAX_LOADS   10

class Load {
public:
    Load();
    Load(int _strt, int _fin, double _valor, double _sz, int _id);
    ~Load();
    
    int strt, fin;
    double valor, sz;
    int ID;
};

class GridWarEngine : public GameEngine {
public:
    GridWarEngine();
    GridWarEngine(char filename[1024]);
    ~GridWarEngine();
    
    void readTheFile(char filename[1024]);
    void createGame();
    
    void addLoads(int tau);
    void removeLoad(int l, int index);
    void printState();
    
    // functions that have to be implemented
    State *getState(MarkovGame *mg, bool *validActs1, bool *validActs2);
	void MoveAgents();
	void initRound();
    
	bool OnGoal(int index);
	void getValidActions(bool *validActs, int index);
	double getCurrentReward(int index);
    
	int convertIt(int accion, bool *validActions, int numActs);
    void deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]);
    
    // scenario properties
    double generation[NUM_HOURS];
    double storageCapacity;
    Load **loads1, **loads2;
    int numLoads1, numLoads2;
    //int actionCount1_max[NUM_HOURS], actionCount2_max[NUM_HOURS];
    
    // real-time data (defines state)
    int t;
    double currentStorageLevel;
    Load **currentLoads1, **currentLoads2;
    int numCurLoads1, numCurLoads2;
};


#endif
