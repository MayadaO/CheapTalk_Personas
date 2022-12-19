#ifndef STAGEGAME_H
#define STAGEGAME_H

#include "defs.h"
#include "State.h"

#define FRIENDVI    0


// variables C and delta are hardcoded right now in stageGame.cpp
class stageGame {
public:

    stageGame();
    stageGame(int _me, State *_s, int _tipo);
    ~stageGame();
    
    int selectAction();
    void update(int actions[2]);
    void updateV();
    
    void print();

    double ***M;
    
    double V[2];

    int me;
    int tipo;
    State *s;
};

#endif