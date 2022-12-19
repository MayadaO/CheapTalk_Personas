#include "defs.h"
#include "MySocket.h"

double ***readPayoffMatrixFromFile(int _A[2], const char *game);
void printGame(int _A[2], double ***_M);
void cleanPayoffMatrix(double ***_M, int _A[2]);

void runGame(double ***M, int A[2], int iters, int addport, const char *partido, int num);

void highScoreStuff(const char *game, char nombres[2][1024], double ave1, double ave2, MySocket *coms[2]);
void highScoreStuff2(const char *game, double ave1, double ave2, MySocket *coms[2]);

bool cheapTalk = false;

/* **************************************
//
//  ./CheapTalk [game] [iters] [addport] [cheaptalk]
//
// ************************************** */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("not enough commandline parameters\n");
        exit(1);
    }

    // read in the game file
	double ***M;
	int A[2];
    int iters = atoi(argv[2]);
    printf("iters = %i\n", iters);
    int addport = atoi(argv[3]);
    if (argc > 4) {
        if (!strcmp(argv[4], "cheaptalk")) {
            cheapTalk = true;
            printf("cheapTalk = true\n");
        }
    }
    M = readPayoffMatrixFromFile(A, argv[1]);

    int numSamps = 1;
    for (int i = 0; i < numSamps; i++) {
        runGame(M, A, iters, addport, argv[1], i);
        
        usleep(700000);
    }

    cleanPayoffMatrix(M, A);

    return 0;
}

void runGame(double ***M, int A[2], int iters, int addport, const char *partido, int num) {
    int i, j;
    int acts[2];
    char talk[2][1024];
    char nombres[2][1024];
    char buf[1024];
    double sum[2] = {0.0, 0.0};

    // get connections
    MySocket *coms[2];
    for (j = 0; j < 2; j++) {
        coms[j] = new MySocket(3000+j+addport+(num*3));
        coms[j]->AcceptEm();
        coms[j]->ReadMessage(nombres[j]);
        printf("name: %s\n", nombres[j]);
    }
    
    //strcpy(buf, "Go\n");
    for (j = 0; j < 2; j++) {
        sprintf(buf, "%s\n", nombres[1-j]);
        coms[j]->SendMessage(buf, strlen(buf));
    }
    
    char fnombre[1024];
    sprintf(fnombre, "../newResults/%s_%s_%s_messages_%i.txt", partido, nombres[0], nombres[1], num);
    FILE *messfp = fopen(fnombre, "w");
    sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", partido, nombres[0], nombres[1], num);
    FILE *actfp = fopen(fnombre, "w");
    sprintf(fnombre, "../newResults/%s_%s_%s_both_%i.txt", partido, nombres[0], nombres[1], num);
    FILE *bothfp = fopen(fnombre, "w");
    
    double tact[j];
    
    for (i = 0; i < iters; i++) {
        if (cheapTalk) {
            //printf("Reading messages:\n");
            for (j = 0; j < 2; j++) {
                coms[j]->ReadMessage(talk[j]);
                //printf("%i: %s\n", j, talk[j]);
            }
            
            fprintf(messfp, "%s\n%s\n", talk[0], talk[1]);
            fprintf(bothfp, "Round %i\n\n", i);
            fprintf(bothfp, "Player 1 said:\n%s\n\nPlayer 2 said:\n%s\n\n", talk[0], talk[1]);
            
            //printf("Sending messages:\n");
            for (j = 0; j < 2; j++) {
                //printf("%s\n", talk[1-j]);
                strcat(talk[1-j], "\n");
                coms[j]->SendMessage(talk[1-j], strlen(talk[1-j]));
            }
        }
    
        for (j = 0; j < 2; j++) {
            coms[j]->ReadMessage(buf);
            //printf("buf: %s\n", buf);
            acts[j] = atoi(buf);
            tact[j] = atof(buf+3);
            //printf("Took p%i %lf miliseconds\n", j, tact[j]);
        }

        printf("%i: (%i, %i) -> %.2lf, %.2lf\n", i, acts[0], acts[1], M[0][acts[0]][acts[1]], M[1][acts[0]][acts[1]]);
        fprintf(actfp, "%i\t%i\t%lf\t%lf\n", acts[0], acts[1], M[0][acts[0]][acts[1]], M[1][acts[0]][acts[1]]);
        
        fprintf(bothfp, "%i\t%i\t%lf\t%lf\n\n", acts[0], acts[1], M[0][acts[0]][acts[1]], M[1][acts[0]][acts[1]]);
        
        sprintf(buf, "%i %i $ %lf %lf\n", acts[0], acts[1], tact[0], tact[1]);
        //sprintf(buf, "%i %i $", acts[0], acts[1]);
        //printf("sending: %s\n", buf);
        for (j = 0; j < 2; j++) {
            coms[j]->SendMessage(buf, strlen(buf));
            sum[j] += M[j][acts[0]][acts[1]];
        }
    }
    
    fclose(messfp);
    fclose(actfp);
    fclose(bothfp);
    
    printf("\nAverages: %lf, %lf\n", sum[0] / iters, sum[1] / iters);

    //highScoreStuff(partido, nombres, sum[0] / iters, sum[1] / iters, coms);
    highScoreStuff2(partido, sum[0] / iters, sum[1] / iters, coms);
    
    usleep(2000000);
    
    delete coms[0];
    delete coms[1];
}

double ***readPayoffMatrixFromFile(int _A[2], const char *game) {
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
    
    printf("game: %s\n", game);
    printGame(_A, _M);
    
	return _M;
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

void cleanPayoffMatrix(double ***_M, int _A[2]) {
	int i, j;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < _A[0]; j++)
			delete _M[i][j];
		delete _M[i];
	}
	delete _M;
}

void highScoreStuff2(const char *game, double ave1, double ave2, MySocket *coms[2]) {
    char fnombre[1024];
    double list[1000];
    int i, len = 0;
    
    // read in the highscore list
    sprintf(fnombre, "../HighScores/%s_scores.txt", game);
    FILE *highfp = fopen(fnombre, "r");
    
    while (fscanf(highfp, "%lf", &(list[len])) != EOF) {
        len ++;
    }
    
    fclose(highfp);
    
    double per1, per2;
    for (i = 0; i < len; i++) {
        if (ave1 > list[i]) {
            per1 = (double)(len - i - 1) / len;
            if (per1 < 0)
                per1 = 0;
            break;
        }
    }

    for (i = 0; i < len; i++) {
        if (ave2 > list[i]) {
            per2 = (double)(len - i - 1) / len;
            if (per2 < 0)
                per2 = 0;
            break;
        }
    }
    
    printf("Player 1 percentile: %i\n", (int)(per1 * 100 + 0.5));
    printf("player 2 percentile: %i\n", (int)(per2 * 100 + 0.5));
    
    char perc[2][1024];
    sprintf(perc[0], "%i\n", (int)(per1 * 100 + 0.5));
    sprintf(perc[1], "%i\n", (int)(per2 * 100 + 0.5));
    
    usleep(200000);
    
    coms[0]->SendMessage(perc[0], strlen(perc[0]));
    coms[1]->SendMessage(perc[1], strlen(perc[1]));
}

void highScoreStuff(const char *game, char nombres[2][1024], double ave1, double ave2, MySocket *coms[2]) {
    char fname[1024];
    char name_list[1000][1024];
    double score_list[1000];
    int count = 0;
    
    sprintf(fname, "hs_%s.txt", game);
    FILE *fp = fopen(fname, "r");
    
    
    if (fp != NULL) {
        printf("file found\n"); fflush(stdout);
    
        while (fscanf(fp, "%s %lf", name_list[count], &(score_list[count])) != EOF) {
            count ++;
        }
        
        fclose(fp);
    }

    int i, j;
    for (i = 0; i < count; i++) {
        if (score_list[i] < ave1) {
            for (j = count; j > i; j--) {
                strcpy(name_list[j], name_list[j-1]);
                score_list[j] = score_list[j-1];
            }
            
            break;
        }
    }
    strcpy(name_list[i], nombres[0]);
    score_list[i] = ave1;

    count++;
    
    for (i = 0; i < count; i++) {
        if (score_list[i] < ave2) {
            for (j = count; j > i; j--) {
                strcpy(name_list[j], name_list[j-1]);
                score_list[j] = score_list[j-1];
            }
            
            break;
        }
    }
    strcpy(name_list[i], nombres[1]);
    score_list[i] = ave2;
    count++;
    
    fp = fopen(fname, "w");
    
    char msg[100000], tmp[1024];
    strcpy(msg, "");
    for (i = 0; i < count; i++) {
        fprintf(fp, "%s\t%lf\n", name_list[i], score_list[i]);
        sprintf(tmp, "%s %lf;", name_list[i], score_list[i]);
        strcat(msg, tmp);
    }
    fclose(fp);
    strcat(msg, "$");
    printf("%s\n", msg);
    for (j = 0; j < 2; j++) {
        coms[j]->SendMessage(msg, strlen(msg));
    }

}
