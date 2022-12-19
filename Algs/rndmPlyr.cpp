#include "rndmPlyr.h"


rndmPlyr::rndmPlyr() {
}

rndmPlyr::rndmPlyr(int _me) {
    me = _me;
}

rndmPlyr::~rndmPlyr() {
    
}

int rndmPlyr::Move(GameEngine *ge) {
    bool validActions[2*NUMACTIONS];
    ge->getValidActions(validActions, me);

    int i;
    int c = 0;
    for (i = 0; i < 9; i++) {
        if (((LEGOEngine *)ge)->available[i])
            c++;
    }
    
    int a = rand() % c;
    
    
    return convertIt(a, validActions);
}

int rndmPlyr::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
    
    return 0;
}

int rndmPlyr::roundUpdate() {
    
    return 0;
}

int rndmPlyr::convertIt(int accion, bool validActions[NUMACTIONS]) {
	int i;
	int count = 0;
	
	for (i = 0; i < NUMACTIONS*2; i++) {
		if (validActions[i]) {
			if (count == accion) {
				return i;
            }
			count ++;
		}
	}
	
	return -1;
}

