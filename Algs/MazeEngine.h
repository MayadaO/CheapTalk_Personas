#ifndef MAZEENGINE_H
#define MAZEENGINE_H

#include "defs.h"
#include "PhysicsRule.h"
#include "MarkovGame.h"
#include "State.h"
#include "GameEngine.h"

class MazeEngine : public GameEngine {
public:
	MazeEngine();
	MazeEngine(char *filename);
	~MazeEngine();

    void playGame(MarkovGame *mg, double _w);
    void playMinimax(MarkovGame *mg);
    State *getState(MarkovGame *mg, bool *validActs1, bool *validActs2);
    void getStateFeatureVector(double fvector[7]);
    void pickJointAction(State *s, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS], double _w);
    void pickMinimax(State *s, bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]);
    void convertActions(int acts[2], bool validActs1[NUMACTIONS], bool validActs2[NUMACTIONS]);
    
	void ReadTheFile(char *filename);
	void MoveAgents();
	int getActualAction(int index, int intendedAct);
	void GetStartPositions();
	void initRound();
    void setPos(int px1, int py1, int px2, int py2, int g1, int g2, int g3);

	int ReadCoord(char *strng, int *x, int *y);
	void outputGame(int iter);
	bool OnGoal(int index);
	void getValidActions(bool *validActs, int index);
	void getValidActions(int **Posicion, bool *validActs, int index);
	void getNumActions(int *nActs);
	double getCurrentReward(int index);

	int convertIt(int accion, bool *validActions, int numActs);
    void deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]);
    
	//int numAgents;

	// all current state of the agents
	//int *actions;
	//int **Pos,
    int **oldPos;
	int *numMoves;
	int **sPos;

	//double *rewardMove;

	int wWidth, wHeight;
	char ***world, ***currentWorld;

	PhysicsRule **pR;
	int numRules, numPhysicsRules, numPayoffRules, numActionRules;

	bool *hitWall;
	double wallPenalty;

	double stochastic;


	// soccer game only stuff
	void outputGameSoccer(int iter, int *score);
	void MoveAgentsSoccer();
	void initRoundSoccer();
	bool endEpisodeSoccer(int k, int numMovemientos);
	double getCurrentRewardSoccer(int j);
	bool OnGoalSoccer(int index);
	
	bool notMoveQuestion(int index, int a, int x, int y);

	int possession;

	//bool oneGoalNeeded;
	bool chocaron;
    
    char fnombrito[1024];
    
};

#endif