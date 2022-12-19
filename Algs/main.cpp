#include "defs.h"
#include "MarkovGame.h"
#include "gameWriter.h"
#include "GameEngine.h"
#include "MatrixEngine.h"
#include "MazeEngine.h"
#include "GridWarEngine.h"
#include "LEGOEngine.h"
#include "Player.h"
#include "Xprt.h"
#include "jefe++.h"
#include "Preventer.h"
#include "rndmPlyr.h"
#include "clientAgent.h"
#include "CFR.h"
#include "client_socket.h"
//#include "delayer.h"
#include "RL.h"
#include "personality.h"

void runGame(double ***M, int A[2], int len, char partido[1024], char name[1024], int me_o, char host[1024], int addport);
Player *createPlayer(int me, char partido[1024], char name[1024]);

int getWordCount(char buf[1024]);
double ***readPayoffMatrixFromFile(int _A[2], char *game);
void cleanPayoffMatrix(double ***_M, int _A[2]);
void printGame(int _A[2], double ***_M);

GameEngine *ge;

bool cheapTalk = true;
bool delaysOn = true;//false;

char stringToCommunicate[1024];
Personality *personality;

/* ****************************************************
//
//	./Solver <game> <iters> <playertype> <me> <hostIP> <addport>
//
// **************************************************** */
int main(int argc, char** argv) {
    if (argc < 6) {
        printf("Not enough commandline parameters\n");
        exit(1);
    }

	srand(time(NULL));

	double ***M;
	int A[2];
    M = readPayoffMatrixFromFile(A, argv[1]);
    int iters = atoi(argv[2]);
    printf("iters = %i\n", iters);
    int me_o = atoi(argv[4]);
    printf("I am %i\n", me_o);
    
    int addport = atoi(argv[6]);

    int numSamps = 1;
    for (int i = 0; i < numSamps; i++) {
        runGame(M, A, iters, argv[1], argv[3], me_o, argv[5], addport+i*3);
        
        if (me_o == 0)
            usleep(1000000);
        else
            usleep(1500000);
    }
    
    
	cleanPayoffMatrix(M, A);
	delete personality;
    
	return 0;
}

void runGame(double ***M, int A[2], int len, char partido[1024], char name[1024], int me_o, char host[1024], int addport) {
	int i;
	int act;//, *acts = new int[2];
	char buf[10000];
    double payoffs[2];

    char dummy[1024];

    strcpy(stringToCommunicate, "");

	Player *player = createPlayer(me_o, partido, name);
    ClientSocket *cs = new ClientSocket(host, 3000+me_o+addport);
    
    if (cheapTalk)
        cs->SendMessage(name, strlen(name));
    else {
        char nombre2[1024];
        sprintf(nombre2, "%s-", name);
        cs->SendMessage(nombre2, strlen(nombre2));
    }
    
    if (player == NULL) {
        if (player != NULL)
            delete player;
        return;
    }
    
    cs->ReadMessage(buf);
    
    //gettimeofday(&eltiempo, NULL);
    //sTime = eltiempo.tv_sec + (eltiempo.tv_usec / 1000000.0);
    
    double ttalk = (double)(rand() % 10);
    double tact[2];
    int amount, wordCount;
    tact[0] = tact[1] = (double)(rand() % 10000);
    
	for (i = 0; i < len; i++) {
    
        printf("\nRound: %i\n", i);
        
        //player->select();
    
        //printf("a"); fflush(stdout);
        //printf("stringToCommunicate: %s\n", stringToCommunicate);
        ge->initRound();
        player->Reset();
        
        if (cheapTalk) {
            // delay cheap talk
            //printf("-------------- stringToCommunicate length: %i\n", (int)strlen(stringToCommunicate));
            //if (ttalk < ((int)strlen(stringToCommunicate)))
            //    ttalk = (int)strlen(stringToCommunicate);

            //printf("1"); fflush(stdout);
            
            //if (ttalk > 20.0)
            //    ttalk = 20.0;
            
            //amount = 0;//(int)(ttalk * 1000000);
            //printf("stringToCommunicate: %s\n", stringToCommunicate);

            personality->createCheapTalk(stringToCommunicate, A);
            strcat(stringToCommunicate, "$ 0.000000");

            //printf("%s; len(stringToCommunicate) = %i\n", stringToCommunicate, (int)strlen(stringToCommunicate));
            //if ((int)strlen(stringToCommunicate) < 11) {
            printf("numMessagesLast = %i\n", personality->numMessagesLast);
            if (personality->numMessagesLast < 1) {
                printf("!!!!!delay by the none_rate: %.2lf\n", player->none_rate); fflush(stdout);
                amount = player->none_rate * 1000;
            }
            else {
                printf("delay by the some_rate: %.2lf\n", player->some_rate); fflush(stdout);
                amount = personality->numMessagesLast * player->some_rate * 1000;
            }
            
            if (amount > 20000000)
                amount = 20000000;
            
            printf("speechDelay = %i\n", amount);
            
            if (delaysOn)
                usleep(amount);
            
            //printf("2"); fflush(stdout);
            
            printf("numMessagesLast = %i\n", personality->numMessagesLast);
            
            //strcat(stringToCommunicate, "$");
            //printf("sending: %s\n", stringToCommunicate);
            cs->SendMessage(personality->personalityString, strlen(personality->personalityString));
        
            cs->ReadMessage(buf);
            wordCount = getWordCount(buf);
            //printf("Received a %i-word chat\n", wordCount);
            
            printf("read msg: %s\n", buf);
            //printf("3"); fflush(stdout);
            
            ttalk = player->processCheapTalk(buf);
            printf("ttalk = %lf\n", ttalk);
            printf("none_rate = %.2lf\nsome_rate = %.2lf\n", player->none_rate, player->some_rate);
            
            //printf("4"); fflush(stdout);
        }
        
        //printf("b"); fflush(stdout);

        strcpy(stringToCommunicate, "");
    
        if (tact[1-me_o] > 20000.0)
            amount = 20000000;
        else
            amount = (int)(tact[1-me_o] * 1000);
        
        //printf("initial delay: %i\n", amount);
        
        amount += (wordCount / 220.0) * 60000000;
        
        printf("totalDelay = %i (%i - %i)\n", amount, (int)((wordCount / 220.0) * 60000000), wordCount);
        
        if (delaysOn)
            usleep(amount);
    
		act = player->Move(ge);
        sprintf(buf, "%i$ 0.000000", act);
        cs->SendMessage(buf, strlen(buf));

        //printf("c"); fflush(stdout);
        
        // read the result
        cs->ReadMessage(buf);
        //printf("message: %s\n", buf);
        
        //strcat(buf, " 0.000000 0.000000");
        
        sscanf(buf, "%i %i %s %lf %lf", &(ge->actions[0]), &(ge->actions[1]), dummy, &(tact[0]), &(tact[1]));
        //printf("%i: actions: %i %i\n", i, acts[0], acts[1]); fflush(stdout);
        printf("he took %lf miliseconds\n", tact[1-me_o]);
        //printf("d"); fflush(stdout);

        
        if (ge->actions[me_o] != act) {
            printf("Server isn't properly listenting to me.  I QUIT!!!\n");
            exit(1);
        }
        
        ge->MoveAgents();
        
        //printf("acting done\n");
        //printf("t"); fflush(stdout);
        
        //printf("acts: %i %i\n", ge->actions[0], ge->actions[1]);
        payoffs[0] = M[0][ge->actions[0]][ge->actions[1]];
        payoffs[1] = M[1][ge->actions[0]][ge->actions[1]];
		player->moveUpdate(ge, ge->actions, payoffs);

        //printf("updated\n");
        //printf("e"); fflush(stdout);
        
        player->roundUpdate();
        
        //printf("f\n"); fflush(stdout);
        
	}
    
    printf("finished\n");
    
    cs->ReadMessage(buf);
    printf("My percentile is %s\n", buf);

    if (me_o == 0)
        usleep(500000);
    else
        usleep(250000);

    delete cs;
	delete player;
}

Player *createPlayer(int me, char partido[1024], char name[1024]) {
    char buf[1024], fnombre[1024];

    strcpy(buf, partido);
    if (!strcmp(buf, "prisoners"))
        strcpy(buf, "pd");
    
    if ((!strcmp(buf, "pd")) || (!strcmp(buf, "chicken")) || (!strcmp(buf, "chicken2")) || (!strcmp(buf, "blocks")) || (!strcmp(buf, "blocks2")) || (!strcmp(buf, "shapleys")) || (!strcmp(buf, "tricky")) ||  (!strcmp(buf, "endless"))) {
        ge = new MatrixEngine();
    }
    else {
        printf("game not found\n");
        exit(1);
    }
    
    sprintf(fnombre, "MG_%s.txt", buf);
    
    Player *pl = NULL;
    
    if (!strcmp(name, "maxmin")) {
        printf("player: maxmin\n");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "mbrl")) {
        printf("player: mbrl\n");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "umbrl")) {
        printf("player: umbrl\n");
        strcpy(buf, "umbrl");
        pl = new Xprt(me, new MarkovGame(fnombre), buf, true);
    }
    else if (!strcmp(name, "folk")) {
        printf("player: folk\n");
        strcpy(buf, "folk");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "bully")) {
        printf("player: bully\n");
        strcpy(buf, "bully");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strncmp(name, "S#", 2)) {
        printf("player: %s\n", name);
        personality = new Personality(name+3);
        bool XAI = true;
        printf("XAI postfix: [%s]\n", name + strlen(name) - 4);
        if (!strcmp("NXAI", name + strlen(name) - 4)) {
            printf("No XAI\n");
            XAI = false;
        }
        pl = new jefe_plus("S++", me, new MarkovGame(fnombre), 0.99, me, true, XAI);
    }
    else if (!strncmp(name, "EEE#", 4)) {
        printf("player: %s\n", name);
        personality = new Personality(name+5);
        bool XAI = true;
        printf("XAI postfix: [%s]\n", name + strlen(name) - 4);
        if (!strcmp("NXAI", name + strlen(name) - 4)) {
            printf("No XAI\n");
            XAI = false;
        }
        pl = new jefe_plus("eee", me, new MarkovGame(fnombre), 0.99, me, false, XAI);
    }
    else if (!strcmp(name, "bouncer")) {
        printf("player: Preventer\n");
        pl = new Preventer(me, new MarkovGame(fnombre), 0.0);  // 0.2
    }
    else if (!strcmp(name, "cfr")) {
        printf("player: CFV\n");
        pl = new CFR(me, new MarkovGame(fnombre), 40, partido, false);
    }
    else if (!strcmp(name, "cfru")) {
        printf("player: CFV\n");
        pl = new CFR(me, new MarkovGame(fnombre), 40, partido, true);
    }
    else if (!strcmp(name, "friend")) {
        printf("player: Friend-VI\n");
        pl = new RL(me, new MarkovGame(fnombre), FRIENDVI);
    }
    else {
        printf("player type %s not found\n", name);
        exit(1);
    }
    
    return pl;
}

int getWordCount(char buf[1024]) {
    int i;
    int numWords = 0;
    
    for (i = 0; i < (int)strlen(buf); i++) {
        if (buf[i] == '$')
            break;
        
        if (buf[i] == ' ')
            numWords ++;
    }
    
    if (i > 6)
        numWords ++;

    return numWords;
}

double ***readPayoffMatrixFromFile(int _A[2], char *game) {
	double ***_M;
    
	char filename[1024];
	sprintf(filename, "..//..//games//%s.txt", game);
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		// check in an alternate directory before giving up
		sprintf(filename, "..//games//%s.txt", game);
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("file %s not found\n", filename);
			exit(1);
		}
	}

	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[1]));
	
	int i, j;
	_M = new double**[2];
	for (i = 0; i < 2; i++) {
		_M[i] = new double*[_A[0]];
		for (j = 0; j < _A[0]; j++)
			_M[i][j] = new double[_A[1]];
	}

	for (i = 0; i < _A[1]; i++) {
		for (j = 0; j < _A[0]; j++) {
			fscanf(fp, "%lf %lf", &(_M[0][j][i]), &(_M[1][j][i]));
		}
	}

    printGame(_A, _M);
    
	return _M;
}

void cleanPayoffMatrix(double ***_M, int _A[2]) {
	int i, j;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < _A[0]; j++)
			delete _M[i][j];
		delete _M[i];
	}
	delete _M;
}

void printGame(int _A[2], double ***_M) {
    int i, j;
    
    printf("\n   |      ");
    
    for (i = 0; i < _A[1]; i++)
        printf("%i      |      ", i);
    printf("\n");
    for (i = 0; i < _A[0]; i++) {
        printf("--------------------------------------\n %i | ", i);
        for (j = 0; j < _A[1]; j++) {
            printf("%.2lf , %.2lf | ", _M[0][i][j], _M[1][i][j]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n\n");
}



