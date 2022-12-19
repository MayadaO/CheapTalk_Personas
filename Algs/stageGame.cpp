#include "stageGame.h"

stageGame::stageGame() {
    printf("incomplete stageGame constructor\n");
    exit(1);
}

stageGame::stageGame(int _me, State *_s, int _tipo) {
    int i, j, k;
    
    me = _me;
    s = _s;
    tipo = _tipo;
    
    M = new double**[2];
    for (i = 0; i < 2; i++) {
        M[i] = new double*[s->numActions[0]];
        for (j = 0; j < s->numActions[0]; j++) {
            M[i][j] = new double[s->numActions[1]];
            for (k = 0; k < s->numActions[1]; k++)
                M[i][j][k] = 0.0;
        }
    }
}

stageGame::~stageGame() {
    int i, j;
    
    for (j = 0; j < 2; j++) {
        for (i = 0; i < s->numActions[0]; i++) {
            delete M[j][i];
        }
        delete M[j];
    }
    delete M;
}

int stageGame::selectAction() {
    if (tipo == FRIENDVI) {
        // select the action that has the highest M-value for me
        int i, j;
        int a = -1;
        double mx = -99999;
        
        for (i = 0; i < s->numActions[0]; i++) {
            for (j = 0; j < s->numActions[1]; j++) {
                if (M[me][i][j] > mx) {
                    mx = M[me][i][j];
                    if (me == 0)
                        a = i;
                    else
                        a = j;
                }
            }
        }
        
        //printf("selected %i (%lf)\n", a, mx);
        
        return a;
    }

    return -1;
}

void stageGame::update(int actions[2]) {
    if (tipo == FRIENDVI) {
        int i, j, k;

        V[0] = V[1] = -99999;
        for (k = 0; k < 2; k++) {
            for (i = 0; i < s->numActions[0]; i++) {
                for (j = 0; j < s->numActions[1]; j++) {
                    if (M[k][i][j] > V[k])
                        V[k] = M[k][i][j];
                }
            }
        }
    }
}

void stageGame::print() {
    int i, j;
    
    //printf("actions: %i x %i\n", s->numActions[0], s->numActions[1]);
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            printf("(%.1lf, %.1lf)  ", M[0][i][j], M[1][i][j]);
        }
        printf("\n");
    }
    printf("V: %.1lf, %.1lf\n", V[0], V[1]);
}
