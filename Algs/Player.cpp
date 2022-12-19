#include "Player.h"

Player::Player() {
    strcpy(nickname, "null");
    
}

Player::~Player() {
}

void Player::Reset() {
}

double Player::processCheapTalk(char buf[10000]) {
    return 0.0;
}

int Player::Move(GameEngine *ge) {//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose) {
    printf("virtual move\n");
	return 0;
}

int Player::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
	printf("virtual update\n");
    
    return 0;
}

int Player::roundUpdate() {
	printf("virtual update\n");
    
    return 0;    
}

void Player::print(int config, int x1, int y1, int x2, int y2) {
}