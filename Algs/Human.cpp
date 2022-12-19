#include "Human.h"

void *openGlThreader(void *lparam);


extern pthread_mutex_t count_mutex;
extern int direccion;
extern int playedDireccion;


Human::Human() {
    printf("incomplete Human constructor\n");
    exit(1);
}

Human::Human(int _me, bool _thePD) {
    me = _me;
    thePD = _thePD;
    
    printf("Enter your nickname: ");
    scanf("%s", nickname);
    
    printf("Please wait for the system to load ... \n");
}

Human::~Human() {}

int Human::Move(GameEngine *ge) {//bool validActions[NUMACTIONS], bool _onGoal, bool _verbose) {
	//char s[1024];
	int a;
	//int u, d, r, l;
    
    
    //printf("valid actions ... "); fflush(stdout);
	bool validActions[2*NUMACTIONS];
    ge->getValidActions(validActions, me);
    //printf("done\n"); fflush(stdout);

    int i;
    if (validActions[4] && thePD)
        return 0;
    else if (!thePD && (((LEGOEngine *)ge)->whosTurn != me))
        return 0;
    
    pthread_mutex_lock( &count_mutex );
    int c = 0;
    while (direccion == NONE) {
        pthread_mutex_unlock( &count_mutex );
        usleep(100000);
        pthread_mutex_lock( &count_mutex );
        
/*        c++;
        if (((c % 100) == 99) && (c < 1000)) {
            if ((c % 300) == 99)
                system("say \"It is your turn\"");
            else if ((c % 300) == 199)
                system("say \"Hello?\"");
            else if ((c % 300) == 299)
                system("say \"Are you there?\"");
            //else if (num == 3) {
            //    system("tput bel");
            //}
        }
*/    }

    if (thePD)
        a = convertIt(direccion, validActions);
    else
        a = direccion;//((LEGOEngine *)ge)->convertIt(direccion, NULL, 0);
    //    a = convertIt2(direccion, validActions);
//    printf("a = %i\n", a);
    playedDireccion = direccion;
    direccion = NONE;
    
    //printf("human: direccion = %i\n", direccion);
    
    pthread_mutex_unlock( &count_mutex );

    //printf("a = %i\n", a); fflush(stdout);
    
    return a;
}

int Human::moveUpdate(GameEngine *ge, int actions[2], double dollars[2]) {
	return 0;
}

int Human::roundUpdate() {
    return 0;
}

int Human::convertIt(int accion, bool validActions[NUMACTIONS]) {
	int i;
	int count = 0;
	
	for (i = 0; i < NUMACTIONS; i++) {
		if (validActions[i]) {
			if (count == accion) {
				return (i + 1);
            }
			count ++;
		}
	}
	
	return -1;
}

int Human::convertIt2(int accion, bool validActions[2*NUMACTIONS]) {
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

