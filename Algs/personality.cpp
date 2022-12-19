#include "personality.h"

Personality::Personality() {
    printf("Incomplete personality constructor\n");
    exit(1);
}

Personality::Personality(char filename[1024]) {
    char fnombre[1024];

    int i, j;
    int countSpecified[15];
    for (i = 0; i < 15; i++) {
        countSpecified[i] = 0;
        for (j = 0; j < 8; j++) {
            strcpy(speechActs[i][j], "");
        }
    }
    
    strcpy(pNombre, filename);
    
    sprintf(fnombre, "../Personalities/%s.txt", filename);
    printf("Loading: %s\n", fnombre); fflush(stdout);
    FILE *fp = fopen(fnombre, "r");
    
    char *line = NULL;
    size_t linecap = 0;
    int cur;
    char strng[1024];
    while (getline(&line, &linecap, fp) != -1) {
        //printf("\n(%i) %s", (int)linecap, line); fflush(stdout);
        cur = atoi(strtok(line, "#"));
        //printf("cur = %i; %i\n", cur, countSpecified[cur]); fflush(stdout);
        //strcpy(speechActs[cur][countSpecified[cur]], strtok(NULL, "#"));
        strcpy(strng, strtok(NULL, "#"));
        //printf("%s\n", strng); fflush(stdout);
        strcpy(speechActs[cur][countSpecified[cur]], strng);
        printf("%i: %s\n", cur, speechActs[cur][countSpecified[cur]]); fflush(stdout);
        
        countSpecified[cur] ++;
    }
    printf("\n");
    
    fclose(fp);
    
    //exit(1);
}

Personality::~Personality() {

}

void Personality::createCheapTalk(char _stringToCommunicate[1024], int _A[2]) {
    printf("createCheapTalk: %s\n", _stringToCommunicate); fflush(stdout);

    strcpy(personalityString, "");
    
    int ind, num;
    char *token;
    char tmp[1024];
    
    token = strtok(_stringToCommunicate, ";");
    numMessagesLast = 0;
    while (token != NULL) {
        printf("token: (%s)\n", token); fflush(stdout);
    
        //ind = atoi(token);
        sscanf(token, "%i %i", &ind, &num);
        
        printf("ind = %i; num = %i\n", ind, num);
        printf("%s\n", speechActs[ind][num]);
    
        if (strlen(speechActs[ind][0]) > 1) {
            switch (ind) {
                case LETS_COOP:
                    strcpy(tmp, speechActs[ind][0]);
                    replaceHolder(tmp, token[4], token[5]);
                    sprintf(personalityString, "%s0 %s@%s;", personalityString, token+4, tmp);
                    break;
                case TAKE_TURNS:
                    strcpy(tmp, speechActs[ind][0]);
                    replaceHolder(tmp, token[4], token[5]);
                    replaceHolder(tmp, token[7], token[8]);
                    sprintf(personalityString, "%s1 %s@%s;", personalityString, token+4, tmp);
                    break;
                case SPECIFY:
                    strcpy(tmp, speechActs[ind][0]);
                    replaceHolder(tmp, token[4], token[5]);
                    sprintf(personalityString, "%s2 %s@%s;", personalityString, token+4, tmp);
                    break;
                default:
                    if (strcmp(speechActs[ind][num], " ") && strcmp(speechActs[ind][num], "\n")) {
                        printf("<%s>\n", speechActs[ind][num]);
                        sprintf(personalityString, "%s%i@%s;", personalityString, ind, speechActs[ind][num]);
                    }
                    break;
            }
            numMessagesLast ++;
        }
    
        token = strtok(NULL, ";");
    }
    
    // add on a dummy time component
    strcat(personalityString, "$ 0.000000");
    
    printf("cheap talk created: %s\n", personalityString); fflush(stdout);
}

void Personality::replaceHolder(char tmp[1024], char ch1, char ch2) {
    int i = 0;
    
    while (true) {
        if (tmp[i] == '<') {
            tmp[i] = ch1;
        }
        else if (tmp[i] == '>') {
            tmp[i] = ch2;
            break;
        }
    
        i++;
    }

}

int Personality::lookUp(char ch, int index, int _A[2]) {
    if (index == 0) {
        if (ch == 'A')
            return 0;
        else if (ch == 'B')
            return 1;
        else if (ch == 'C')
            return 2;
    }
    else if (index == 1) {
        if (ch == 'X')
            return 0;
        else if (ch == 'Y')
            return 1;
        else if (ch == 'Z')
            return 2;
    }
    
    printf("(P) unidentified action: %c (index = %i)\n", ch, index);
    
    return -1;
}