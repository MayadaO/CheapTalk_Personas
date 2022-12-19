#ifndef __Solver__LEGOEngine__
#define __Solver__LEGOEngine__

#include "defs.h"
#include "GameEngine.h"
#include "State.h"

enum SQRtype {SQUARE = 12, TRIANGLE = 2, CIRCLE = 0};
enum COLtype {RED = 3, BLUE = 1, GREEN = 0};

class LEGO {
public:
    LEGO();
    LEGO(int _shape, int _col);
    ~LEGO();
    
    int shape;
    int col;
    int value;
    int cx, cy;
};



class LEGOEngine : public GameEngine {
public:
    LEGOEngine();
	~LEGOEngine();
    
    State *getState(MarkovGame *mg, bool *validActs1, bool *validActs2);
	void MoveAgents();
	void initRound();
    
	bool OnGoal(int index);
	void getValidActions(bool *validActs, int index);
	double getCurrentReward(int index);
    
	int convertIt(int accion, bool *validActions, int numActs);
    int deduceIt(int accion, bool *validActions, int numActs);
    void deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]);
    
    void createGame();
    void setGame(State *currentState);
    void printState();
    void addTransition(State *currentState, State *newState);
    
    int whosTurn;
    int numPieces[2];
    int ownership[2][3];
    int orderedOwnership[2][3];
    LEGO *legos[9];
    bool available[9];
    
    int rawActs[2];
    
    
    bool isValidConstruction(int index);
    bool allSameShape(int index);
    bool allDiffShape(int index);
    bool allSameCol(int index);
    bool allDiffCol(int index);
    int cardSum(int index);
};






#endif /* defined(__Solver__LEGOEngine__) */
