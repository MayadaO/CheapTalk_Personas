#include "defs.h"

void getTestProfile(int group);
void playGame(int sport, char *hostname);

char testprofile[3][1024];
int sid;
pthread_t theagent;

// ***********************************************
//
//      ./awrapper [hostname] [sid]
//
// ***********************************************
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("not enough argument\n");
        return 0;
    }
    
    sid = atoi(argv[2]);
    
    int verify;
    system("clear");
    printf("Please verify the subject ID ... ");
    scanf("%i", &verify);
    
    if (sid != verify) {
        printf("Group mismatch.  Please carefully determine the group and try again.\n");
        return 0;
    }
    
    printf("run subject h%i\n", sid);
    getTestProfile(sid);
    
    printf("test profile %s %s %s\n", testprofile[0], testprofile[1], testprofile[2]);

    return 0;
    
    
    char palabra[1024];
    char passcode[1024];
    sprintf(passcode, "juego%i", sid);
    do {
        printf("To proceed, enter the passcode provided by the experiment administrator ... ");
        scanf("%s", palabra);
    } while (strcmp(palabra, passcode));

    // connect to SGPD game
    playGame(3000, argv[1]);

    system("clear");
    sprintf(passcode, "game%i", sid);
    do {
        printf("To proceed, enter the passcode provided by the experiment administrator ... ");
        scanf("%s", palabra);
    } while (strcmp(palabra, passcode));

    // connect to LEGO game
    playGame(3012, argv[1]);

    return 0;
}

void playGame(int sport, char *hostname) {
    // determine the port
    int port;
    int themod = (sid-1) % 4;
    if (testprofile[1][0] != 'h')
        port = sport + 3 * themod;
    else {
        //if (sport == 3000) {
            if ((themod == 0) || (themod == 1))
                port = sport;
            else
                port = sport + 6;
        //}
        //else {
        //    if ((themod == 0) || (themod == 2))
        //        port = sport;
        //    else
        //        port = sport + 6;
        //}
    }
    
    int playerIndex = 0;
    if ((themod == 1) || (themod == 3))
        playerIndex = 1;
/*
    int playerIndex = 1;
    if (testprofile[1][0] == 'h') {
        if ((sport == 3000) && ((themod == 0) || (themod == 2)))
            playerIndex = 0;
        else if ((sport > 3000) && ((themod == 0) || (themod == 1)))
            playerIndex = 0;
    }
    else {
        if (((themod % 2) == 0) && (sport > 3000))
            playerIndex = 0;
    }
*/    
    char mandato[1024];
    if (sport == 3000)
        sprintf(mandato, "./Solver prisoners agent human %i 54 %s %i", playerIndex, hostname, port - 3000);
    else
        sprintf(mandato, "./Solver legos agent human %i 51 %s %i", playerIndex, hostname, port - 3000);
    system(mandato);
}


void getTestProfile(int _sid) {
    FILE *fpSched = fopen("schedule.txt", "r");
    int i, j, k;
    
    for (i = 0; i < _sid; i++) {
        for (k = 0; k < 3; k++) {
            fscanf(fpSched, "%s", testprofile[k]);
        }
    }
    fclose(fpSched);
    
    printf("\n%i's profile:\n", _sid);
    for (k = 0; k < 3; k++) {
        printf("%s\t", testprofile[k]);
    }
    printf("\n");
}