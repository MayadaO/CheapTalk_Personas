#include "defs.h"

void getCGroup(int group);
void setUpGames(int sport);
void waitForCompletion(int sport);
void setFileStatus(int prt, int val);
void *setServer(void *port);

char cGroup[4][3][1024];
int group;
pthread_t theservers[4];

// ***********************************************
//
//      ./swrapper [group]
//
// ***********************************************
int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc < 2) {
        printf("not enough argument\n");
        return 0;
    }
    
    group = atoi(argv[1]);
    
    int verify;
    system("clear");
    printf("Please verify the group ID ... ");
    scanf("%i", &verify);
    
    if (group != verify) {
        printf("Group mismatch.  Please carefully determine the group and try again.\n");
        return 0;
    }
    
    printf("run group %i\n", group);
    getCGroup(group);

    char palabra[1024];
/*    do {
        printf("type the word \"prisoners\" once all players have completed training on the SGPD ... ");
        scanf("%s", palabra);
    } while (strcmp(palabra, "prisoners"));
*/
    // run SGPD games
    setUpGames(3000);
    waitForCompletion(3000);
    
    usleep(1000000);
    
    system("clear");
    do {
        printf("type the word \"legos\" once all players have completed training on the Lego Game ... ");
        scanf("%s", palabra);
    } while (strcmp(palabra, "legos"));
    
    // run LEGO games
    setUpGames(3012);
    waitForCompletion(3012);
    
    return 0;
}

void waitForCompletion(int sport) {
    bool done;
    int g;
    int inc = 1;
    if (cGroup[0][1][0] == 'h')
        inc = 2;    // playing against computer agents
    int port, val;
    char fname[1024];
    FILE *fp;
    
    do {
        usleep(2000000);
        done = true;
        for (g = 0; g < 4; g += inc) {
            // set the status file
            port = sport + (g*3);
            sprintf(fname, "../gameStatus/port%i.txt", port);
            fp = fopen(fname, "r");
            
            while (fp == NULL) {
                printf("problem\n"); fflush(stdout);
                usleep(100000);
                fp = fopen(fname, "r");
            }
            
            val = 0;
            fscanf(fp, "%i", &val);
            fclose(fp);
            
            //printf("%i: %i\n", g, val);
            
            if (val != 1)
                done = false;
        }
    } while (!done);
    
    
}

void setUpGames(int sport) {
    int g;
    int inc = 1;
    if (cGroup[0][1][0] == 'h')
        inc = 2;    // playing against computer agents
    
    int port[4];
    
    for (g = 0; g < 4; g += inc) {
        port[g] = sport + (g*3);
        setFileStatus(port[g], 0);
        
        // start up a server (in a thread) on port 3000 + g
        printf("*************** creating a thread for g = %i ... ", g);
        pthread_create(&theservers[g], NULL, setServer, &(port[g]));
        printf("thread %i created\n", g);
        
        usleep(1010000);
    }
}

void setFileStatus(int prt, int val) {
    if (val == 1)
        printf("mark %i as done\n", prt);
    else
        printf("mark %i as NOT done\n", prt);
    
    char fname[1024];
    FILE *fp;

    sprintf(fname, "../gameStatus/port%i.txt", prt);
    fp = fopen(fname, "w");
    fprintf(fp, "%i", val);
    fclose(fp);
}

void *setServer(void *port) {
    int prt = *((int *)port);
    
    char mandato[1024];
    if (prt >= 3012) {
        if (cGroup[0][1][0] == 'h') {
            sprintf(mandato, "./Solver legos server 51 %i &", prt - 3000);
            printf("%s\n", mandato);
            system(mandato);
        }
        else {
            sprintf(mandato, "./Solver legos server 51 %i &", prt - 3000);
            printf("%s\n", mandato);
            system(mandato);

            usleep(1000000);
            
            int index = ((prt - 3000 - 12) / 3);
            // need to worry later about playerID for legos (who goes first)
            if ((index % 2) == 0)
                sprintf(mandato, "./Solver legos agent %s 1 51 localhost %i &", cGroup[index][2], prt - 3000);
            else
                sprintf(mandato, "./Solver legos agent %s 0 51 localhost %i &", cGroup[index][2], prt - 3000);
            printf("%s\n", mandato);
            system(mandato);
        }
    }
    else  {
        if (cGroup[0][1][0] == 'h') {
            sprintf(mandato, "./Solver prisoners server 54 %i &", prt - 3000);
            printf("%s\n", mandato);
            system(mandato);
        }
        else {
            sprintf(mandato, "./Solver prisoners server 54 %i &", prt - 3000);
            printf("%s\n", mandato);
            system(mandato);

            usleep(1000000);
            
            int index = ((prt - 3000) / 3);
            sprintf(mandato, "./Solver prisoners agent %s 0 54 localhost %i &", cGroup[index][1], prt - 3000);
            printf("%s\n", mandato);
            system(mandato);
        }
    }
    
    //usleep(1000000 * (rand() % 4));
    
    //setFileStatus(prt, 1);

    return NULL;
}

void getCGroup(int group) {
    FILE *fpSched = fopen("schedule.txt", "r");
    int i, j, k;
    
    for (i = 0; i < group; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 3; k++) {
                fscanf(fpSched, "%s", cGroup[j][k]);
            }
        }
    }
    
    fclose(fpSched);
    
    printf("\nCurrent Group:\n");
    for (j = 0; j < 4; j++) {
        for (k = 0; k < 3; k++) {
            printf("%s\t", cGroup[j][k]);
        }
        printf("\n");
    }
    
}