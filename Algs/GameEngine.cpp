#include "GameEngine.h"


GameEngine::GameEngine() {
    //printf("GameEngine constructor\n");
    //exit(1);
}

GameEngine::~GameEngine() {}

State *GameEngine::getState(MarkovGame *mg, bool *validActs1, bool *validActs2) { return NULL; }
void GameEngine::MoveAgents() {}
void GameEngine::initRound() {}

bool GameEngine::OnGoal(int index) { return false; }
void GameEngine::getValidActions(bool *validActs, int index) {}
double GameEngine::getCurrentReward(int index) { return 0.0; }

int GameEngine::convertIt(int accion, bool *validActions, int numActs) { return 0; }
void GameEngine::deduceActions(int rawActions[2], int geActions[2], bool lastValidActs[2][2*NUMACTIONS]) {}

