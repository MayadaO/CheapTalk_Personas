#include "defs.h"

#define GLEN        500

char agentNames[10][1024];
char game[1024];

char hh_prisoners_comms[12][2][1024], hh_prisoners_nocomms[10][2][1024];
char sh_prisoners_comms[12][2][1024], sh_prisoners_nocomms[10][2][1024];
char hh_chicken_comms[12][2][1024], hh_chicken_nocomms[10][2][1024];
char sh_chicken_comms[12][2][1024], sh_chicken_nocomms[10][2][1024];
char hh_blocks_comms[12][2][1024], hh_blocks_nocomms[10][2][1024];
char sh_blocks_comms[12][2][1024], sh_blocks_nocomms[10][2][1024];

char groups[22][3][1024];

void setGroup() {
    strcpy(groups[0][0], "Qustro");   strcpy(groups[0][1], "ABL");   strcpy(groups[0][2], "NICK");
    strcpy(groups[1][0], "MDM");   strcpy(groups[1][1], "ABCDE");   strcpy(groups[1][2], "Arnukk");
    strcpy(groups[2][0], "ligtho");   strcpy(groups[2][1], "winner");   strcpy(groups[2][2], "spark");
    strcpy(groups[3][0], "Lk");   strcpy(groups[3][1], "clouds");   strcpy(groups[3][2], "Lelita");
    strcpy(groups[4][0], "marvin");   strcpy(groups[4][1], "bzman");   strcpy(groups[4][2], "rghost");
    strcpy(groups[5][0], "clc");   strcpy(groups[5][1], "monkey");   strcpy(groups[5][2], "ALPHA");
    strcpy(groups[6][0], "sachin");   strcpy(groups[6][1], "biodun");   strcpy(groups[6][2], "Babay");
    strcpy(groups[7][0], "sheen");   strcpy(groups[7][1], "Hash50");   strcpy(groups[7][2], "EPE");
    strcpy(groups[8][0], "OK");   strcpy(groups[8][1], "TooToo");   strcpy(groups[8][2], "gokill");
    strcpy(groups[9][0], "private");   strcpy(groups[9][1], "NUKK");   strcpy(groups[9][2], "BO1533");
    strcpy(groups[10][0], "Ben");   strcpy(groups[10][1], "Lav");   strcpy(groups[10][2], "p97");
    strcpy(groups[11][0], "shunik");   strcpy(groups[11][1], "osenat");   strcpy(groups[11][2], "Ninkas");
    strcpy(groups[12][0], "R2D2");   strcpy(groups[12][1], "plush");   strcpy(groups[12][2], "MBEAST");
    strcpy(groups[13][0], "Hey");   strcpy(groups[13][1], "mike");   strcpy(groups[13][2], "comrad");
    strcpy(groups[14][0], "abcdz");   strcpy(groups[14][1], "vojta");   strcpy(groups[14][2], "XAB");
    strcpy(groups[15][0], "mifleh");   strcpy(groups[15][1], "spike");   strcpy(groups[15][2], "musket");
    strcpy(groups[16][0], "victor");   strcpy(groups[16][1], "asdf");   strcpy(groups[16][2], "kai");
    strcpy(groups[17][0], "L69");   strcpy(groups[17][1], "atom");   strcpy(groups[17][2], "wasbol");
    strcpy(groups[18][0], "Dust");   strcpy(groups[18][1], "bonus");   strcpy(groups[18][2], "django");
    strcpy(groups[19][0], "BOZO");   strcpy(groups[19][1], "spider");   strcpy(groups[19][2], "t4t");
    strcpy(groups[20][0], "avenger");   strcpy(groups[20][1], "ruqaia");   strcpy(groups[20][2], "Star");
    strcpy(groups[21][0], "herb");   strcpy(groups[21][1], "flyer");   strcpy(groups[21][2], "Isra");
}

int getGroup(const char *nombre) {
    int i, j;
    
    for (i = 0; i < 22; i++) {
        for (j = 0; j < 3; j++) {
            if (!strcmp(groups[i][j], nombre))
                return i;
        }
    }

    printf("%s\n", nombre);
    exit(1);

    return -1;
}


void set_hh_prisoners_comms() {
    strcpy(hh_prisoners_comms[0][0], "Babay"); strcpy(hh_prisoners_comms[0][1], "biodun");
    strcpy(hh_prisoners_comms[1][0], "BO1533"); strcpy(hh_prisoners_comms[1][1], "private");
    strcpy(hh_prisoners_comms[2][0], "clc"); strcpy(hh_prisoners_comms[2][1], "ALPHA");
    strcpy(hh_prisoners_comms[3][0], "LK"); strcpy(hh_prisoners_comms[3][1], "Lelita");
    strcpy(hh_prisoners_comms[4][0], "marvin"); strcpy(hh_prisoners_comms[4][1], "bzman");
    strcpy(hh_prisoners_comms[5][0], "MDM"); strcpy(hh_prisoners_comms[5][1], "Arnukk");
    strcpy(hh_prisoners_comms[6][0], "NICK"); strcpy(hh_prisoners_comms[6][1], "Qustro");
    strcpy(hh_prisoners_comms[7][0], "sheen"); strcpy(hh_prisoners_comms[7][1], "Hash50");
    strcpy(hh_prisoners_comms[8][0], "TooToo"); strcpy(hh_prisoners_comms[8][1], "gokill");
    strcpy(hh_prisoners_comms[9][0], "winner"); strcpy(hh_prisoners_comms[9][1], "ligtho");
    strcpy(hh_prisoners_comms[10][0], "shunik"); strcpy(hh_prisoners_comms[10][1], "Ninkas");
    strcpy(hh_prisoners_comms[11][0], "Ben"); strcpy(hh_prisoners_comms[11][1], "Lav");
}

void set_sh_prisoners_comms() {
    strcpy(sh_prisoners_comms[0][0], "clouds"); strcpy(sh_prisoners_comms[0][1], "megaS++");
    strcpy(sh_prisoners_comms[1][0], "EPE"); strcpy(sh_prisoners_comms[1][1], "megaS++");
    strcpy(sh_prisoners_comms[2][0], "rghost"); strcpy(sh_prisoners_comms[2][1], "megaS++");
    strcpy(sh_prisoners_comms[3][0], "sachin"); strcpy(sh_prisoners_comms[3][1], "megaS++");
    strcpy(sh_prisoners_comms[4][0], "monkey"); strcpy(sh_prisoners_comms[4][1], "megaS++");
    strcpy(sh_prisoners_comms[5][0], "OK"); strcpy(sh_prisoners_comms[5][1], "megaS++");
    strcpy(sh_prisoners_comms[6][0], "megaS++"); strcpy(sh_prisoners_comms[6][1], "ABCDE");
    strcpy(sh_prisoners_comms[7][0], "megaS++"); strcpy(sh_prisoners_comms[7][1], "ABL");
    strcpy(sh_prisoners_comms[8][0], "megaS++"); strcpy(sh_prisoners_comms[8][1], "NUKK");
    strcpy(sh_prisoners_comms[9][0], "megaS++"); strcpy(sh_prisoners_comms[9][1], "spark");
    strcpy(sh_prisoners_comms[10][0], "megaS++"); strcpy(sh_prisoners_comms[10][1], "p97");
    strcpy(sh_prisoners_comms[11][0], "megaS++"); strcpy(sh_prisoners_comms[11][1], "osenat");
}

void set_hh_prisoners_nocomms() {
    strcpy(hh_prisoners_nocomms[0][0], "abcdz"); strcpy(hh_prisoners_nocomms[0][1], "XAB");
    strcpy(hh_prisoners_nocomms[1][0], "comrad"); strcpy(hh_prisoners_nocomms[1][1], "mike");
    strcpy(hh_prisoners_nocomms[2][0], "R2D2"); strcpy(hh_prisoners_nocomms[2][1], "plush");
    strcpy(hh_prisoners_nocomms[3][0], "spike"); strcpy(hh_prisoners_nocomms[3][1], "musket");
    strcpy(hh_prisoners_nocomms[4][0], "avenger"); strcpy(hh_prisoners_nocomms[4][1], "Star");
    strcpy(hh_prisoners_nocomms[5][0], "BOZO"); strcpy(hh_prisoners_nocomms[5][1], "t4t");
    strcpy(hh_prisoners_nocomms[6][0], "Dust"); strcpy(hh_prisoners_nocomms[6][1], "django");
    strcpy(hh_prisoners_nocomms[7][0], "flyer"); strcpy(hh_prisoners_nocomms[7][1], "Isra");
    strcpy(hh_prisoners_nocomms[8][0], "kai"); strcpy(hh_prisoners_nocomms[8][1], "victor");
    strcpy(hh_prisoners_nocomms[9][0], "L69"); strcpy(hh_prisoners_nocomms[9][1], "wasbol");
}

void set_sh_prisoners_nocomms() {
    strcpy(sh_prisoners_nocomms[0][0], "mifleh"); strcpy(sh_prisoners_nocomms[0][1], "megaS++-");
    strcpy(sh_prisoners_nocomms[1][0], "atom"); strcpy(sh_prisoners_nocomms[1][1], "megaS++-");
    strcpy(sh_prisoners_nocomms[2][0], "ruqaia"); strcpy(sh_prisoners_nocomms[2][1], "megaS++-");
    strcpy(sh_prisoners_nocomms[3][0], "megaS++-"); strcpy(sh_prisoners_nocomms[3][1], "Hey");
    strcpy(sh_prisoners_nocomms[4][0], "megaS++-"); strcpy(sh_prisoners_nocomms[4][1], "MBEAST");
    strcpy(sh_prisoners_nocomms[5][0], "megaS++-"); strcpy(sh_prisoners_nocomms[5][1], "vojta");
    strcpy(sh_prisoners_nocomms[6][0], "megaS++-"); strcpy(sh_prisoners_nocomms[6][1], "asdf");
    strcpy(sh_prisoners_nocomms[7][0], "megaS++-"); strcpy(sh_prisoners_nocomms[7][1], "bonus");
    strcpy(sh_prisoners_nocomms[8][0], "megaS++-"); strcpy(sh_prisoners_nocomms[8][1], "herb");
    strcpy(sh_prisoners_nocomms[9][0], "megaS++-"); strcpy(sh_prisoners_nocomms[9][1], "spider");
}

void set_hh_chicken_comms() {
    strcpy(hh_chicken_comms[0][0], "ABL"); strcpy(hh_chicken_comms[0][1], "NICK");
    strcpy(hh_chicken_comms[1][0], "clc"); strcpy(hh_chicken_comms[1][1], "monkey");
    strcpy(hh_chicken_comms[2][0], "Hash50"); strcpy(hh_chicken_comms[2][1], "EPE");
    strcpy(hh_chicken_comms[3][0], "Lav"); strcpy(hh_chicken_comms[3][1], "p97");
    strcpy(hh_chicken_comms[4][0], "LK"); strcpy(hh_chicken_comms[4][1], "clouds");
    strcpy(hh_chicken_comms[5][0], "NUKK"); strcpy(hh_chicken_comms[5][1], "BO1533");
    strcpy(hh_chicken_comms[6][0], "rghost"); strcpy(hh_chicken_comms[6][1], "bzman");
    strcpy(hh_chicken_comms[7][0], "sachin"); strcpy(hh_chicken_comms[7][1], "biodun");
    strcpy(hh_chicken_comms[8][0], "winner"); strcpy(hh_chicken_comms[8][1], "spark");
    strcpy(hh_chicken_comms[9][0], "osenat"); strcpy(hh_chicken_comms[9][1], "Ninkas");
    strcpy(hh_chicken_comms[10][0], "OK"); strcpy(hh_chicken_comms[10][1], "TooToo");

    
}

void set_sh_chicken_comms() {
    strcpy(sh_chicken_comms[0][0], "ALPHA"); strcpy(sh_chicken_comms[0][1], "megaS++");
    strcpy(sh_chicken_comms[1][0], "Babay"); strcpy(sh_chicken_comms[1][1], "megaS++");
    strcpy(sh_chicken_comms[2][0], "Lelita"); strcpy(sh_chicken_comms[2][1], "megaS++");
    strcpy(sh_chicken_comms[3][0], "sheen"); strcpy(sh_chicken_comms[3][1], "megaS++");
    strcpy(sh_chicken_comms[4][0], "shunik"); strcpy(sh_chicken_comms[4][1], "megaS++");
    strcpy(sh_chicken_comms[5][0], "Ben"); strcpy(sh_chicken_comms[5][1], "megaS++");
    strcpy(sh_chicken_comms[6][0], "megaS++"); strcpy(sh_chicken_comms[6][1], "gokill");
    strcpy(sh_chicken_comms[7][0], "megaS++"); strcpy(sh_chicken_comms[7][1], "ligtho");
    strcpy(sh_chicken_comms[8][0], "megaS++"); strcpy(sh_chicken_comms[8][1], "marvin");
    strcpy(sh_chicken_comms[9][0], "megaS++"); strcpy(sh_chicken_comms[9][1], "MDM");
    strcpy(sh_chicken_comms[10][0], "megaS++"); strcpy(sh_chicken_comms[10][1], "private");
    strcpy(sh_chicken_comms[11][0], "megaS++"); strcpy(sh_chicken_comms[11][1], "Qustro");
}


void set_hh_chicken_nocomms() {
    strcpy(hh_chicken_nocomms[0][0], "abcdz"); strcpy(hh_chicken_nocomms[0][1], "vojta");
    strcpy(hh_chicken_nocomms[1][0], "Hey"); strcpy(hh_chicken_nocomms[1][1], "mike");
    strcpy(hh_chicken_nocomms[2][0], "mifleh"); strcpy(hh_chicken_nocomms[2][1], "spike");
    strcpy(hh_chicken_nocomms[3][0], "plush"); strcpy(hh_chicken_nocomms[3][1], "MBEAST");
    strcpy(hh_chicken_nocomms[4][0], "bonus"); strcpy(hh_chicken_nocomms[4][1], "django");
    strcpy(hh_chicken_nocomms[5][0], "BOZO"); strcpy(hh_chicken_nocomms[5][1], "spider");
    strcpy(hh_chicken_nocomms[6][0], "herb"); strcpy(hh_chicken_nocomms[6][1], "Isra");
    strcpy(hh_chicken_nocomms[7][0], "L69"); strcpy(hh_chicken_nocomms[7][1], "atom");
    strcpy(hh_chicken_nocomms[8][0], "ruqaia"); strcpy(hh_chicken_nocomms[8][1], "Star");
    strcpy(hh_chicken_nocomms[9][0], "victor"); strcpy(hh_chicken_nocomms[9][1], "asdf");
}


void set_sh_chicken_nocomms() {
    strcpy(sh_chicken_nocomms[0][0], "musket"); strcpy(sh_chicken_nocomms[0][1], "megaS++-");
    strcpy(sh_chicken_nocomms[1][0], "R2D2"); strcpy(sh_chicken_nocomms[1][1], "megaS++-");
    strcpy(sh_chicken_nocomms[2][0], "avenger"); strcpy(sh_chicken_nocomms[2][1], "megaS++-");
    strcpy(sh_chicken_nocomms[3][0], "kai"); strcpy(sh_chicken_nocomms[3][1], "megaS++-");
    strcpy(sh_chicken_nocomms[4][0], "wasbol"); strcpy(sh_chicken_nocomms[4][1], "megaS++-");
    strcpy(sh_chicken_nocomms[5][0], "megaS++-"); strcpy(sh_chicken_nocomms[5][1], "Dust");
    strcpy(sh_chicken_nocomms[6][0], "megaS++-"); strcpy(sh_chicken_nocomms[6][1], "comrad");
    strcpy(sh_chicken_nocomms[7][0], "megaS++-"); strcpy(sh_chicken_nocomms[7][1], "XAB");
    strcpy(sh_chicken_nocomms[8][0], "megaS++-"); strcpy(sh_chicken_nocomms[8][1], "flyer");
    strcpy(sh_chicken_nocomms[9][0], "megaS++-"); strcpy(sh_chicken_nocomms[9][1], "t4t");
}

void set_hh_blocks_comms() {
    strcpy(hh_blocks_comms[0][0], "ABL"); strcpy(hh_blocks_comms[0][1], "Qustro");
    strcpy(hh_blocks_comms[1][0], "Babay"); strcpy(hh_blocks_comms[1][1], "sachin");
    strcpy(hh_blocks_comms[2][0], "Lelita"); strcpy(hh_blocks_comms[2][1], "clouds");
    strcpy(hh_blocks_comms[3][0], "ligtho"); strcpy(hh_blocks_comms[3][1], "spark");
    strcpy(hh_blocks_comms[4][0], "MDM"); strcpy(hh_blocks_comms[4][1], "ABCDE");
    strcpy(hh_blocks_comms[5][0], "monkey"); strcpy(hh_blocks_comms[5][1], "ALPHA");
    strcpy(hh_blocks_comms[6][0], "NUKK"); strcpy(hh_blocks_comms[6][1], "private");
    strcpy(hh_blocks_comms[7][0], "OK"); strcpy(hh_blocks_comms[7][1], "gokill");
    strcpy(hh_blocks_comms[8][0], "rghost"); strcpy(hh_blocks_comms[8][1], "marvin");
    strcpy(hh_blocks_comms[9][0], "sheen"); strcpy(hh_blocks_comms[9][1], "EPE");
    strcpy(hh_blocks_comms[10][0], "shunik"); strcpy(hh_blocks_comms[10][1], "osenat");
    strcpy(hh_blocks_comms[11][0], "Ben"); strcpy(hh_blocks_comms[11][1], "p97");
}

void set_hh_blocks_nocomms() {
    strcpy(hh_blocks_nocomms[0][0], "Hey"); strcpy(hh_blocks_nocomms[0][1], "comrad");
    strcpy(hh_blocks_nocomms[1][0], "mifleh"); strcpy(hh_blocks_nocomms[1][1], "musket");
    strcpy(hh_blocks_nocomms[2][0], "R2D2"); strcpy(hh_blocks_nocomms[2][1], "MBEAST");
    strcpy(hh_blocks_nocomms[3][0], "vojta"); strcpy(hh_blocks_nocomms[3][1], "XAB");
    strcpy(hh_blocks_nocomms[4][0], "atom"); strcpy(hh_blocks_nocomms[4][1], "wasbol");
    strcpy(hh_blocks_nocomms[5][0], "Dust"); strcpy(hh_blocks_nocomms[5][1], "bonus");
    strcpy(hh_blocks_nocomms[6][0], "herb"); strcpy(hh_blocks_nocomms[6][1], "flyer");
    strcpy(hh_blocks_nocomms[7][0], "kai"); strcpy(hh_blocks_nocomms[7][1], "asdf");
    strcpy(hh_blocks_nocomms[8][0], "ruqaia"); strcpy(hh_blocks_nocomms[8][1], "avenger");
    strcpy(hh_blocks_nocomms[9][0], "t4t"); strcpy(hh_blocks_nocomms[9][1], "spider");
}

void set_sh_blocks_comms() {
    strcpy(sh_blocks_comms[0][0], "biodun"); strcpy(sh_blocks_comms[0][1], "megaS++");
    strcpy(sh_blocks_comms[1][0], "clc"); strcpy(sh_blocks_comms[1][1], "megaS++");
    strcpy(sh_blocks_comms[2][0], "Hash50"); strcpy(sh_blocks_comms[2][1], "megaS++");
    strcpy(sh_blocks_comms[3][0], "Lk"); strcpy(sh_blocks_comms[3][1], "megaS++");
    strcpy(sh_blocks_comms[4][0], "TooToo"); strcpy(sh_blocks_comms[4][1], "megaS++");
    strcpy(sh_blocks_comms[5][0], "winner"); strcpy(sh_blocks_comms[5][1], "megaS++");
    strcpy(sh_blocks_comms[6][0], "Lav"); strcpy(sh_blocks_comms[6][1], "megaS++");
    strcpy(sh_blocks_comms[7][0], "megaS++"); strcpy(sh_blocks_comms[7][1], "BO1533");
    strcpy(sh_blocks_comms[8][0], "megaS++"); strcpy(sh_blocks_comms[8][1], "bzman");
    strcpy(sh_blocks_comms[9][0], "megaS++"); strcpy(sh_blocks_comms[9][1], "NICK");
    strcpy(sh_blocks_comms[10][0], "megaS++"); strcpy(sh_blocks_comms[10][1], "Ninkas");
}

void set_sh_blocks_nocomms() {
    strcpy(sh_blocks_nocomms[0][0], "plush"); strcpy(sh_blocks_nocomms[0][1], "megaS++-");
    strcpy(sh_blocks_nocomms[1][0], "spike"); strcpy(sh_blocks_nocomms[1][1], "megaS++-");
    strcpy(sh_blocks_nocomms[2][0], "BOZO"); strcpy(sh_blocks_nocomms[2][1], "megaS++-");
    strcpy(sh_blocks_nocomms[3][0], "L69"); strcpy(sh_blocks_nocomms[3][1], "megaS++-");
    strcpy(sh_blocks_nocomms[4][0], "victor"); strcpy(sh_blocks_nocomms[4][1], "megaS++-");
    strcpy(sh_blocks_nocomms[5][0], "megaS++-"); strcpy(sh_blocks_nocomms[5][1], "django");
    strcpy(sh_blocks_nocomms[6][0], "megaS++-"); strcpy(sh_blocks_nocomms[6][1], "Isra");
    strcpy(sh_blocks_nocomms[7][0], "megaS++-"); strcpy(sh_blocks_nocomms[7][1], "abcdz");
    strcpy(sh_blocks_nocomms[8][0], "megaS++-"); strcpy(sh_blocks_nocomms[8][1], "mike");
    strcpy(sh_blocks_nocomms[9][0], "megaS++-"); strcpy(sh_blocks_nocomms[9][1], "Star");
}


void setNames(char _nombre[1024]) {
    strcpy(agentNames[0], "megaS++");
    strcpy(agentNames[1], "megaExp3");
    strcpy(agentNames[2], "mbrl");
    strcpy(agentNames[3], "umbrl");
    strcpy(agentNames[4], "cfru");
    strcpy(agentNames[5], "folk");
    strcpy(agentNames[6], "bully");
    strcpy(agentNames[7], "bouncer");
    strcpy(agentNames[8], "maxmin");
    strcpy(agentNames[9], "friend");
    
    strcpy(game, _nombre);
}

int reciprocatedCoop2(double prev_payouts[2], double payouts[2], double coopVals[2], int cooplen) {
    if (cooplen == 1) {
        if ((payouts[0] >= coopVals[0]) && (payouts[1] >= coopVals[1]))
            return 1;
        else
            return 0;
    }
    else {
        if (((prev_payouts[0] + payouts[0]) >= coopVals[0]) && ((prev_payouts[1] + payouts[1]) >= coopVals[1]))
            return 1;
        else
            return 0;
    }
}

int reciprocatedCooperation(FILE *fp, int recip[100], int cooplen, double coopVals[2], int iters) {
    int i;
    int a1, a2;
    double tmp1[100], tmp2[100];
    
    for (i = 0; i < 100; i++)
        recip[i] = 0;
    
    int count = 0;
    for (i = 0; i < iters; i++) {
        //printf("%i ... ", i); fflush(stdout);
    
        fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(tmp1[i]), &(tmp2[i]));
        
        if (cooplen == 1) {
            if ((tmp1[i] >= coopVals[0]) && (tmp2[i] >= coopVals[1]))
                recip[i] = 1;
            else
                recip[i] = 0;
        }
        else {
            if (i > 0) {
                if (((tmp1[i] + tmp1[i-1]) >= coopVals[0]) && ((tmp2[i] + tmp2[i-1]) >= coopVals[1]))
                    recip[i] = 1;
                else
                    recip[i] = 0;
            }
        }
        count += recip[i];
    }
    
    if (cooplen > 1)
        recip[0] = recip[1];
    
    return count;
}

FILE *openSesime(const char *game, const char *plyr1, const char *plyr2, int c, bool comms) {
    char fnombre[1024];
    if (comms)
        sprintf(fnombre, "../tennomResults/newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, c);
    else
        sprintf(fnombre, "../tennomResults/newResults_nocomms/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, c);
    FILE *fp = fopen(fnombre, "r");
    
    if (fp == NULL) {
        printf("File not found: %s\n", fnombre);
        exit(1);
    }
    
    //printf("file successfully opened: %s\n", fnombre); fflush(stdout);
    return fp;
}

FILE *openSesime2(const char *game, const char *plyr1, const char *plyr2, int c) {
    char fnombre[1024];
    sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, c);
    FILE *fp = fopen(fnombre, "r");
    
    if (fp == NULL) {
        printf("File not found: %s\n", fnombre);
        exit(1);
    }
    
    //printf("file successfully opened: %s\n", fnombre); fflush(stdout);
    return fp;
}

void recipCoop_SS(const char *game, bool comms, int len, double probs[100], int counts[100]) {
    FILE *fp;
    int recip[100], total[100];
    double coopVals[2] = {0.6, 0.6};
    int cooplen = 1;
    double co = 0;
    int i, j;
    int iters = 51;
    
    for (j = 0; j < 100; j++)
        total[j] = 0;
    
    if (!strcmp(game, "chicken")) {
        coopVals[0] = coopVals[1] = 0.84;
        iters = 54;
    }
    else if (!strcmp(game, "blocks2")) {
        coopVals[0] = coopVals[1] = 1.4;
        cooplen = 2;
        iters = 47;
    }
    
    for (i = 0; i < 50; i++) {
        if (comms)
            fp = openSesime2(game, "megaS++", "megaS++", i);
        else
            fp = openSesime2(game, "megaS++-", "megaS++-", i);
        int c = reciprocatedCooperation(fp, recip, cooplen, coopVals, iters);
        for (j = 0; j < 100; j++)
            total[j] += recip[j];
        co += c;
        printf("reciprocated cooperation: %i out of xx rounds\n", c);
        fclose(fp);
    }

    printf("percent coop: %lf\n\n", co / (50.0 * len));
    
    for (i = 0; i < 100; i++) {
        //printf("%i\n", total[i]);
        counts[i] += total[i];
        probs[i] = total[i] / 50.0;
    }
}

void recipCoop_HH(const char *game, bool comms, int len, double probs[100], int counts[100]) {
    FILE *fp;
    int recip[100], total[100];
    double coopVals[2] = {0.6, 0.6};
    double co = 0;
    int i, j;
    
    for (j = 0; j < 100; j++)
        total[j] = 0;
    
    int samps = 12;
    if (!comms)
        samps = 10;
    if (!strcmp(game, "prisoners")) {
        set_hh_prisoners_comms();
        set_hh_prisoners_nocomms();
        for (i = 0; i < samps; i++) {
            if (comms)
                fp = openSesime("prisoners", hh_prisoners_comms[i][0], hh_prisoners_comms[i][1], 0, comms);
            else
                fp = openSesime("prisoners", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 51 rounds\n", c);
            fclose(fp);
        }
    }
    else if (!strcmp(game, "chicken2")) {
        set_hh_chicken_comms();
        set_hh_chicken_nocomms();
        coopVals[0] = coopVals[1] = 0.84;
        
        for (i = 0; i < samps; i++) {
            if (comms) {
                fp = openSesime("chicken2", hh_chicken_comms[i][0], hh_chicken_comms[i][1], 0, comms);
                samps = 11;
            }
            else
                fp = openSesime("chicken2", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 1, coopVals, 54);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 54 rounds\n", c);
            fclose(fp);
        }
    }
    else if (!strcmp(game, "blocks2")) {
        printf("got here\n");
        set_hh_blocks_comms();
        set_hh_blocks_nocomms();
        coopVals[0] = coopVals[1] = 1.4;
        //samps = 1;
        
        for (i = 0; i < samps; i++) {
            if (comms) {
                fp = openSesime("blocks2", hh_blocks_comms[i][0], hh_blocks_comms[i][1], 0, comms);
            }
            else
                fp = openSesime("blocks2", hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 2, coopVals, 47);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 54 rounds\n", c);
            fclose(fp);
        }
    }
    
    printf("percent coop: %lf\n\n", co / (double)(samps * len));
    
    for (i = 0; i < 100; i++) {
        counts[i] += total[i];
        probs[i] = total[i] / (double)samps;
    }
}

void recipCoop_SH(const char *game, bool comms, int len, double probs[100], int counts[100]) {
    FILE *fp;
    int recip[100], total[100];
    double coopVals[2] = {0.6, 0.6};
    double co = 0;
    int i, j;
    
    for (j = 0; j < 100; j++)
        total[j] = 0;
    
    int samps = 12;
    if (!comms)
        samps = 10;
    if (!strcmp(game, "prisoners")) {
        set_sh_prisoners_comms();
        set_sh_prisoners_nocomms();
        for (i = 0; i < samps; i++) {
            if (comms)
                fp = openSesime("prisoners", sh_prisoners_comms[i][0], sh_prisoners_comms[i][1], 0, comms);
            else
                fp = openSesime("prisoners", sh_prisoners_nocomms[i][0], sh_prisoners_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 1, coopVals, 54);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 51 rounds\n", c);
            fclose(fp);
        }
    }
    else if (!strcmp(game, "chicken2")) {
        set_sh_chicken_comms();
        set_sh_chicken_nocomms();
        coopVals[0] = coopVals[1] = 0.84;
        
        for (i = 0; i < samps; i++) {
            if (comms) {
                fp = openSesime("chicken2", sh_chicken_comms[i][0], sh_chicken_comms[i][1], 0, comms);
                samps = 11;
            }
            else
                fp = openSesime("chicken2", sh_chicken_nocomms[i][0], sh_chicken_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 1, coopVals, 54);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 54 rounds\n", c);
            fclose(fp);
        }
    }
    else if (!strcmp(game, "blocks2")) {
        printf("got to here\n");
        set_sh_blocks_comms();
        set_sh_blocks_nocomms();
        coopVals[0] = coopVals[1] = 1.4;
        
        for (i = 0; i < samps; i++) {
            if (comms) {
                fp = openSesime("blocks2", sh_blocks_comms[i][0], sh_blocks_comms[i][1], 0, comms);
                samps = 11;
            }
            else
                fp = openSesime("blocks2", sh_blocks_nocomms[i][0], sh_blocks_nocomms[i][1], 0, comms);
            int c = reciprocatedCooperation(fp, recip, 2, coopVals, 47);
            for (j = 0; j < 100; j++)
                total[j] += recip[j];
            co += c;
            printf("reciprocated cooperation: %i out of 47 rounds\n", c);
            fclose(fp);
        }
    }
    
    printf("percent coop: %lf\n\n", co / (double)(samps * len));
    
    for (i = 0; i < 100; i++) {
        counts[i] += total[i];
        probs[i] = total[i] / (double)samps;
    }
}


bool newRead(const char *game, const char *plyr1, const char *plyr2, double vals[2][100], int numSamps, int iters, bool ag = false) {
    char fnombre[1024];
    int i, j;
    FILE *fp;
    
    printf("%s vs %s\n", plyr1, plyr2); fflush(stdout);
    
    for (i = 0; i < 100; i++) {
        vals[0][i] = vals[1][i] = 0.0;
    }
    
    double tmp1, tmp2;
    int a1, a2;
    for (i = 0; i < numSamps; i++) {
        if (ag)
            sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, i);
        else
            sprintf(fnombre, "../tennomResults/newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, i);
        fp = fopen(fnombre, "r");
        
        if (fp == NULL) {
            printf("no data ... ");
            printf("fnombre: %s\n", fnombre);
            
            if (i == 0)
                return -1;
            break;
        }
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &tmp1, &tmp2);
            
            vals[0][j] += tmp1;
            vals[1][j] += tmp2;
        }
        
        fclose(fp);
    }

    for (j = 0; j < iters; j++) {
        vals[0][j] /= numSamps;
        vals[1][j] /= numSamps;
    }
    
    return true;
}

bool newRead2(const char *game, const char *plyr1, const char *plyr2, double vals[2][100],
                double points[50], int leng, int numSamps, int iters, bool ag = false) {
    char fnombre[1024];
    int i, j;
    FILE *fp;
    
    printf("%s vs %s\n", plyr1, plyr2); fflush(stdout);
    
    for (i = 0; i < 100; i++) {
        vals[0][i] = vals[1][i] = 0.0;
    }
    
    double tmp1, tmp2;
    int a1, a2;
    double sum;
    for (i = 0; i < numSamps; i++) {
        if (ag)
            sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, i);
        else
            sprintf(fnombre, "../tennomResults/newResults/%s_%s_%s_activity_%i.txt", game, plyr1, plyr2, i);
        fp = fopen(fnombre, "r");
        
        if (fp == NULL) {
            printf("no data ... ");
            printf("fnombre: %s\n", fnombre);
            if (i == 0)
                return -1;
            break;
        }
        
        points[i] = 0.0;
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &tmp1, &tmp2);
            
            vals[0][j] += tmp1;
            vals[1][j] += tmp2;
            
            if (j < leng)
                points[i] += tmp1 + tmp2;
        }
        points[i] /= 2.0 * leng;
        
        fclose(fp);
    }

    for (j = 0; j < iters; j++) {
        vals[0][j] /= numSamps;
        vals[1][j] /= numSamps;
    }
    
    return true;
}


void printSummaries(const char *game, double with[2][100], double without[2][100], int bn) {
    char fnombre[1024];
    sprintf(fnombre, "../newResults/%s_megaS++_megaS++_summary.csv", game);
    FILE *fp = fopen(fnombre, "w");
    
    fprintf(fp, "With,Without\n");
    double sm_w, sm_wo;
    for (int i = 0; i < 100; i+=bn) {
        sm_w = sm_wo = 0.0;
        for (int j = i; j < i+bn; j++) {
            sm_w += with[0][j] + with[1][j];
            sm_wo += without[0][j] + without[1][j];
        }
        
        fprintf(fp, "%lf,%lf\n", sm_w / (bn * 2.0), sm_wo / (bn * 2.0));
    }
    
    fclose(fp);
}

void printSome(const char *game, double hh[100], double hs[100], double sh[100], double ss[100], int bn, int len) {
    char fnombre[1024];
    sprintf(fnombre, "../newResults/%s_all_summary.csv", game);
    FILE *fp = fopen(fnombre, "w");
    
    fprintf(fp, "hmnhmn,hmnspp,spphmn,sppspp\n");
    double s_hh, s_hs, s_sh, s_ss;
    for (int i = 0; i < len; i+=bn) {
        s_hh = s_hs = s_sh = s_ss = 0;
        for (int j = i; j < i+bn; j++) {
            s_hh += hh[j];
            s_hs += hs[j];
            s_sh += sh[j];
            s_ss += ss[j];
        }
        
        fprintf(fp, "%lf,%lf,%lf,%lf\n", s_hh / bn, s_hs / bn, s_sh / bn, s_ss / bn);
    }
    
    fclose(fp);
}

void printPoints(const char *game, double hh[12], double hs[12], double sh[12], double ss[12], int len) {
    char fnombre[1024];
    sprintf(fnombre, "../newResults/%s_all_point.csv", game);
    FILE *fp = fopen(fnombre, "w");
    
    fprintf(fp, "hmnhmn,hmnspp,spphmn,sppspp\n");
    double s_hh, s_hs, s_sh, s_ss;
    for (int i = 0; i < len; i++) {
        fprintf(fp, "%lf,%lf,%lf,%lf\n", hh[i], hs[i], sh[i], ss[i]);
    }
    
    fclose(fp);
}


void processNew(char *param) {
    char gm[1024];
    strcpy(gm, param);

    double vals_w[2][100];
    double vals_wo[2][100];
    newRead(gm, "megaS++", "megaS++", vals_w, 50, 100, true);
    newRead(gm, "megaS++-", "megaS++-", vals_wo, 50, 100, true);

    printSummaries(gm, vals_w, vals_wo, 5);
}

void addTo(double sum[2][100], double vals[2][100]) {
    int i;
    
    for (i = 0; i < 100; i++) {
        sum[0][i] += vals[0][i];
        sum[1][i] += vals[1][i];
    }
}

double theMean(double vals[2][100], int len) {
    int i;
    double sum = 0.0;
    
    for (i = 0; i < len; i++)
        sum += (vals[0][i] + vals[1][i]) / 2.0;
    
    return sum / len;
}

void getMean2(double vals[2][100], int len, double ans[2]) {
    int i;
    double sum1 = 0.0;
    double sum2 = 0.0;
    
    for (i = 0; i < len; i++) {
        sum1 += vals[0][i];
        sum2 += vals[1][i];
    }
    
    ans[0] = sum1 / len;
    ans[1] = sum2 / len;
}

void processNewH_H_prisoners(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 51;
    
    newRead("prisoners", "Babay", "biodun", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "BO1533", "private", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "clc", "ALPHA", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "LK", "Lelita", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "marvin", "bzman", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "MDM", "Arnukk", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "NICK", "Qustro", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "sheen", "Hash50", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "TooToo", "gokill", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "winner", "ligtho", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "shunik", "Ninkas", vals, 1, blockslen);
    pts[10] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "Ben", "Lav", vals, 1, blockslen);
    pts[11] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 12;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewH_H_chicken2(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 54;
    
    //newRead("chicken2", "ABCDE", "Arnukk", vals, 1, blockslen);
    //addTo(sum, vals);
    newRead("chicken2", "ABL", "NICK", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "clc", "monkey", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "Hash50", "EPE", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "Lav", "p97", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "LK", "clouds", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "NUKK", "BO1533", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "rghost", "bzman", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "sachin", "biodun", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "winner", "spark", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "osenat", "Ninkas", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "OK", "TooToo", vals, 1, blockslen);
    pts[10] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 11;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewH_H_blocks2(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 47;
    
    newRead("blocks2", "ABL", "Qustro", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "Babay", "sachin", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "Lelita", "clouds", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "ligtho", "spark", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "MDM", "ABCDE", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "monkey", "ALPHA", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "NUKK", "private", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "OK", "gokill", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "rghost", "marvin", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "sheen", "EPE", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "shunik", "osenat", vals, 1, blockslen);
    pts[10] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "Ben", "p97", vals, 1, blockslen);
    pts[11] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 12;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewS_H_blocks2(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 47;
    
    newRead("blocks2", "biodun", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "clc", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "Hash50", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "Lk", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[1];
    pts_h[3] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "TooToo", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[1];
    pts_h[4] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "winner", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[1];
    pts_h[5] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "Lav", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[1];
    pts_h[6] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    //newRead("blocks2", "megaS++", "Arnukk", vals, 1, blockslen);
    //addTo(sum, vals);
    newRead("blocks2", "megaS++", "BO1533", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++", "bzman", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++", "NICK", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++", "Ninkas", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[10] = ans[0];
    pts_h[10] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 11;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

void processNewS_H_chicken2(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 54;
    
    newRead("chicken2", "ALPHA", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "Babay", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "Lelita", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "sheen", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[1];
    pts_h[3] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "shunik", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[1];
    pts_h[4] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "Ben", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[1];
    pts_h[5] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    newRead("chicken2", "megaS++", "gokill", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[0];
    pts_h[6] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++", "ligtho", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++", "marvin", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++", "MDM", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++", "private", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[10] = ans[0];
    pts_h[10] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++", "Qustro", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[11] = ans[0];
    pts_h[11] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 12;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

void processNewS_H_prisoners(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 51;
    
    newRead("prisoners", "clouds", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "EPE", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "rghost", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "sachin", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[1];
    pts_h[3] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "monkey", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[1];
    pts_h[4] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "OK", "megaS++", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[1];
    pts_h[5] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    newRead("prisoners", "megaS++", "ABCDE", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[0];
    pts_h[6] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++", "ABL", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++", "NUKK", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++", "spark", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++", "p97", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[10] = ans[0];
    pts_h[10] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++", "osenat", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[11] = ans[0];
    pts_h[11] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 12;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

void processNewH_H_prisoners_no(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 51;
    
    newRead("prisoners", "abcdz", "XAB", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "comrad", "mike", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "R2D2", "plush", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "spike", "musket", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "avenger", "Star", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "BOZO", "t4t", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "Dust", "django", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "flyer", "Isra", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "kai", "victor", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("prisoners", "L69", "wasbol", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewH_H_chicken2_no(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 54;
    
    newRead("chicken2", "abcdz", "vojta", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "Hey", "mike", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "mifleh", "spike", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "plush", "MBEAST", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "bonus", "django", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "BOZO", "spider", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "herb", "Isra", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "L69", "atom", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "ruqaia", "Star", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("chicken2", "victor", "asdf", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewH_H_blocks2_no(double averages[100], double pts[12]) {
    double vals[2][100];
    double sum[2][100];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 47;
    
    newRead("blocks2", "Hey", "comrad", vals, 1, blockslen);
    pts[0] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "mifleh", "musket", vals, 1, blockslen);
    pts[1] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "R2D2", "MBEAST", vals, 1, blockslen);
    pts[2] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "vojta", "XAB", vals, 1, blockslen);
    pts[3] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "atom", "wasbol", vals, 1, blockslen);
    pts[4] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "Dust", "bonus", vals, 1, blockslen);
    pts[5] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "herb", "flyer", vals, 1, blockslen);
    pts[6] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "kai", "asdf", vals, 1, blockslen);
    pts[7] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "ruqaia", "avenger", vals, 1, blockslen);
    pts[8] = theMean(vals, blockslen);
    addTo(sum, vals);
    newRead("blocks2", "t4t", "spider", vals, 1, blockslen);
    pts[9] = theMean(vals, blockslen);
    addTo(sum, vals);
    
    double ave = 0.0;
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        averages[i] = (sum[0][i] + sum[1][i]) / (2.0 * numEntries);
        printf("%lf\n", (sum[0][i] + sum[1][i]) / (2.0 * numEntries));
        ave += sum[0][i] + sum[1][i];
    }
    printf("\naverage: = %lf\n", ave / (blockslen * 2 * numEntries));
}

void processNewS_H_blocks2_no(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 47;
    
    newRead("blocks2", "plush", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "spike", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "BOZO", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "L69", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[1];
    pts_h[3] = ans[0];
    addTo(sum, vals);
    newRead("blocks2", "victor", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[1];
    pts_h[4] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    newRead("blocks2", "megaS++-", "django", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[0];
    pts_h[5] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++-", "Isra", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[0];
    pts_h[6] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++-", "abcdz", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++-", "mike", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("blocks2", "megaS++-", "Star", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

void processNewS_H_chicken2_no(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 54;
    
    newRead("chicken2", "musket", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "R2D2", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "avenger", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "kai", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[1];
    pts_h[3] = ans[0];
    addTo(sum, vals);
    newRead("chicken2", "wasbol", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[1];
    pts_h[4] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    newRead("chicken2", "megaS++-", "Dust", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[0];
    pts_h[5] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++-", "comrad", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[0];
    pts_h[6] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++-", "XAB", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++-", "flyer", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("chicken2", "megaS++-", "t4t", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

void processNewS_H_prisoners_no(double spp[100], double hmn[100], double pts_s[12], double pts_h[12]) {
    double vals[2][100];
    double sum[2][100];
    double ans[2];
    
    int i;
    for (i = 0; i < 100; i++)
        sum[0][i] = sum[1][i] = 0.0;
    
    int blockslen = 51;
    
    newRead("prisoners", "mifleh", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[0] = ans[1];
    pts_h[0] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "atom", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[1] = ans[1];
    pts_h[1] = ans[0];
    addTo(sum, vals);
    newRead("prisoners", "ruqaia", "megaS++-", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[2] = ans[1];
    pts_h[2] = ans[0];
    addTo(sum, vals);
    
    // switch
    double tmp;
    for (i = 0; i < 100; i++) {
        tmp = sum[0][i];
        sum[0][i] = sum[1][i];
        sum[1][i] = tmp;
    }
    
    newRead("prisoners", "megaS++-", "Hey", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[3] = ans[0];
    pts_h[3] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "MBEAST", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[4] = ans[0];
    pts_h[4] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "vojta", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[5] = ans[0];
    pts_h[5] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "asdf", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[6] = ans[0];
    pts_h[6] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "bonus", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[7] = ans[0];
    pts_h[7] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "herb", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[8] = ans[0];
    pts_h[8] = ans[1];
    addTo(sum, vals);
    newRead("prisoners", "megaS++-", "spider", vals, 1, blockslen);
    getMean2(vals, blockslen, ans);
    pts_s[9] = ans[0];
    pts_h[9] = ans[1];
    addTo(sum, vals);
    
    double ave[2] = {0.0, 0.0};
    int numEntries = 10;
    for (i = 0; i < blockslen; i++) {
        spp[i] = sum[0][i] / numEntries;
        hmn[i] = sum[1][i] / numEntries;
        printf("%lf\t%lf\n", sum[0][i] / numEntries, sum[1][i] / numEntries);
        ave[0] += sum[0][i];
        ave[1] += sum[1][i];
    }
    printf("\naverages: = %lf\t%lf\n", ave[0] / (blockslen * numEntries), ave[1] / (blockslen * numEntries));
}

bool readPayoffs(char *game, char *plyr1, char *plyr2, double vals[2][GLEN]) {
    char fnombre[1024];
    int i, j;
    FILE *fp;
    
    char pl1[1024], pl2[1024];
    if (!strcmp(game, "prisoners") && (!strcmp(plyr1, "maxmin") || !strcmp(plyr1, "cfru")))
        strcpy(pl1, "friend");
    else
        strcpy(pl1, plyr1);
    if (!strcmp(game, "prisoners") && (!strcmp(plyr2, "maxmin") || !strcmp(plyr2, "cfru")))
        strcpy(pl2, "friend");
    else
        strcpy(pl2, plyr2);
    
    
    printf("%s vs %s\n", pl1, pl2); fflush(stdout);
    
    for (i = 0; i < GLEN; i++) {
        vals[0][i] = vals[1][i] = 0.0;
    }
    
    bool predetermined = false;
    double tmp1, tmp2;
    for (i = 0; i < 25; i++) {
        sprintf(fnombre, "../results/%s/%s_%s_%i.txt", game, pl1, pl2, i);
        fp = fopen(fnombre, "r");
        
        if (fp == NULL) {
            printf("no data ... ");
            printf("fnombre: %s\n", fnombre);
            predetermined = true;
            break;
        }
        
        for (j = 0; j < GLEN; j++) {
            fscanf(fp, "%lf %lf", &tmp1, &tmp2);
            
            vals[0][j] += tmp1;
            vals[1][j] += tmp2;
        }
        
        fclose(fp);
    }

    if (predetermined) {
/*        sprintf(fnombre, "../results/%s/aa_all.txt", game);
        fp = fopen(fnombre, "r");
        char bf1[1024], bf2[1024];
        for (int k = 0; k < 20; k++) {
            fscanf(fp, "%s %s", bf1, bf2);
            fscanf(fp, "%lf %lf", &tmp1, &tmp2);
            if (!strcmp(bf1, pl1) && !strcmp(bf2, pl2)) {
                printf("%lf, %lf\n", tmp1, tmp2);
                for (j = 0; j < GLEN; j++) {
                    vals[0][j] = tmp1;
                    vals[1][j] = tmp2;
                }
                break;
            }
            
            if (k == 19) {
                printf("not found\n");
                return false;
            }
        }
        
        fclose(fp); */
        return false;
    }
    else {
        for (j = 0; j < GLEN; j++) {
            vals[0][j] /= 25.0;
            vals[1][j] /= 25.0;
        }
    }
    
    return true;
}

double getMean(double *vals, int strt, int fin) {
    int i;
    double sum = 0.0;
    
    for (i = strt; i < fin; i++) {
        sum += vals[i];
    }
    
    return sum / (fin - strt);
}

void addOn(double sum[GLEN], double nvals[GLEN], int len) {
    int i;
    
    for (i = 0; i < len; i++)
        sum[i] += nvals[i];
}

void evolve(double outcomes[10][10], int iters, double proportion[101][10]) {

    //printf("\n");

    int i, j, k;
    double fitness[10], mag;
    for (i = 0; i < iters; i++) {
        mag = 0.0;
        for (j = 0; j < 10; j++) {
            fitness[j] = 0.0;
            for (k = 0; k < 10; k++) {
                fitness[j] += proportion[i][k] * outcomes[j][k];
            }
            mag += fitness[j] * proportion[i][j];
        }
        
        double sum = 0.0;
        for (j = 0; j < 10; j++) {
            proportion[i+1][j] = fitness[j] * proportion[i][j] / mag;
            sum += proportion[i+1][j];
        }
        //printf("sum = %lf\n", sum);
    }

    //for (i = 0; i < iters+1; i++) {
    //    for (j = 0; j < 10; j++) {
    //        printf("%.4lf\t", proportion[i][j]);
    //    }
    //    printf("\n");
    //}

}

void testRobustness(double outcomes[10][10], double count[10]) {
    int i, j, tau;
    for (i = 0; i < 10; i++)
        count[i] = 0.0;

    int gens = 100;

    //printf("\n\n");
    int nsamps = 1000;
    for (tau = 0; tau < nsamps; tau++) {
        // randomly generate initial population share
        double proportion[101][10];
        double sum = 0.0;
        for (i = 0; i < 10; i++) {
            //if (i == 0)
            //    proportion[0][i] = 0;//rand() % 100;
            //else
            proportion[0][i] = rand() % 100;
            sum += proportion[0][i];
        }
        for (i = 0; i < 10; i++)
            proportion[0][i] /= sum;
        
        //for (i = 0; i < 10; i++) {
        //    printf("%.4lf\t", proportion[0][i]);
        //}
        //printf("\n");
        
        
        evolve(outcomes, gens, proportion);
        
        for (i = 1; i < gens+1; i++) {
            for (j = 0; j < 10; j++) {
                count[j] += proportion[i][j];
            }
        }
    }
    
    printf("\n");
    for (i = 0; i < 10; i++) {
        count[i] /= gens * nsamps;
        printf("%.4lf\t", count[i]);
    }
    printf("\n");
    
}

void usageInfo(char game[1024], char pl1[1024], char pl2[1024], int timesUsed[13], int indexer) {
    int i, t;
    char fnombre[1024], buf1[1024], buf2[1024];
    FILE *fp;
    
    if ((!strcmp(pl2, "cfru") || !strcmp(pl2, "maxmin")) && !strcmp(game, "prisoners")) {
        strcpy(pl2, "friend");
    }
    
    int ind;
    for (i = 0; i < 25; i++) {
        sprintf(fnombre, "../results/%s/usage_%s_%s_%i_%i.txt", game, pl1, pl2, i, indexer);
        fp = fopen(fnombre, "r");

        if (fp == NULL) {
            printf("flipping: game better be prisoners\n");
            sprintf(fnombre, "../results/%s/usage_%s_%s_%i_%i.txt", game, pl2, pl1, i, 1 - indexer);
            fp = fopen(fnombre, "r");
        }

        if (fp != NULL) {
            int round = -1;
            
            while (true) {
                if (fscanf(fp, "%i %s %s", &t, buf1, buf2) == EOF)
                    break;

                if (t == round)
                    continue;
                
                if (!strcmp(buf2, "Minimax"))
                    timesUsed[0] ++;
                else if (!strcmp(buf2, "BResp"))
                    timesUsed[1] ++;
                else if (!strcmp(buf2, "Bouncer"))
                    timesUsed[2] ++;
                else if (!strncmp(buf2, "Lead", 4)) {
                    ind = atoi(buf2+5) + 3;
                    timesUsed[ind] ++;
                }
                else if (!strncmp(buf2, "Follow", 6)) {
                    ind = atoi(buf2+7) + 8;
                    timesUsed[ind] ++;
                }
            }
        }
        else {
            printf("data against %s not found\n", pl2);
        }

        fclose(fp);
    }


}

int previousCode(int argc, char *argv[]) {
    if (argc < 4) {
        printf("not enough arguments\n");
        exit(1);
    }
    
    srand(time(NULL));
    int i, j;
    
    setNames(argv[1]);
    int strt = atoi(argv[2]);
    int fin = atoi(argv[3]);
    
    double vals[2][GLEN];
    double totalMeans[10][10];

    int timesUsed[13];
    for (i = 0; i < 13; i++)
        timesUsed[i] = 0;
    
    //usageInfo(game, agentNames[0], agentNames[9], timesUsed);
    for (i = 0; i < 10; i++)
        usageInfo(game, agentNames[1], agentNames[i], timesUsed, 0);
    for (i = 0; i < 10; i++)
        usageInfo(game, agentNames[i], agentNames[1], timesUsed, 1);

    double summer = 0.0;
    for (i = 0; i < 13; i++)
        summer += timesUsed[i];

    for (i = 0; i < 13; i++)
        printf("%.4lf\n", timesUsed[i] / summer);
    
    return 0;

    
    strt = 0;
    double count[10][10];
    int index = 0;
    for (fin = 50; fin <= 500; fin += 50) {
    
    
//    readPayoffs(game, agentNames[6], agentNames[6], vals);
    
    //printf("%s: %lf\n", agentNames[6], getMean(vals[0], strt, fin));
    //printf("%s: %lf\n", agentNames[8], getMean(vals[1], strt, fin));
//    printf("%lf\n", (getMean(vals[0], strt, fin) + getMean(vals[1], strt, fin)) / 2.0);

    //readPayoffs(game, agentNames[3], agentNames[3], vals);
    //return 0;
    
    double sumTotal[10][GLEN];
    
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            totalMeans[i][j] = 0.0;
        }
        for (j = 0; j < GLEN; j++) {
            sumTotal[i][j] = 0.0;
        }
    }
/*
    readPayoffs(game, agentNames[2], agentNames[2], vals);
    addOn(sumTotal[0], vals[0], GLEN);
    addOn(sumTotal[0], vals[1], GLEN);

    readPayoffs(game, agentNames[4], agentNames[4], vals);
    addOn(sumTotal[1], vals[0], GLEN);
    addOn(sumTotal[1], vals[1], GLEN);
    
    printf("\n\n\nOver time:\n");
    int binss = 10;
    int len = 500;
    //for (i = 0; i < 10; i++) {
    for (j = 0; j < len / binss; j++) {
        printf("%lf\t", getMean(sumTotal[0], j*binss, (j+1)*binss) / 2.0);
        printf("%lf\n", getMean(sumTotal[1], j*binss, (j+1)*binss) / 2.0);
    }
    //    printf("\n");
    //}
    
    //return 0;
*/
    bool fnd;
    int s = 0;
    for (i = 0; i < 10; i++) {
        if (!strcmp(game, "prisoners"))
            s = i;
        for (j = s; j < 10; j++) {
            fnd = readPayoffs(game, agentNames[i], agentNames[j], vals);

            if (fnd) {
                totalMeans[i][j] += getMean(vals[0], strt, fin);
                totalMeans[j][i] += getMean(vals[1], strt, fin);
            
                addOn(sumTotal[i], vals[0], GLEN);
                addOn(sumTotal[j], vals[1], GLEN);

                if (!strcmp(game, "prisoners") && (i != j)) {
                    addOn(sumTotal[i], vals[0], GLEN);
                    addOn(sumTotal[j], vals[1], GLEN);
                }
            }
            else {
                readPayoffs(game, agentNames[j], agentNames[i], vals);
                
                totalMeans[i][j] += getMean(vals[1], strt, fin);
                totalMeans[j][i] += getMean(vals[0], strt, fin);
                
                addOn(sumTotal[i], vals[1], GLEN);
                addOn(sumTotal[j], vals[0], GLEN);
                
                if (!strcmp(game, "prisoners") && (i != j)) {
                    addOn(sumTotal[i], vals[1], GLEN);
                    addOn(sumTotal[j], vals[0], GLEN);
                }
            }
        }
    }
    
    double outcomes[10][10];
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (!strcmp(game, "prisoners") && (j != i)) {
                printf("%.3lf\t", totalMeans[i][j]);
                outcomes[i][j] = totalMeans[i][j];
            }
            else {
                printf("%.3lf\t", totalMeans[i][j] / 2.0);
                outcomes[i][j] = totalMeans[i][j] / 2.0;
            }
        }
        printf("\n");
    }
    

    for (i = 0; i < GLEN; i++) {
        for (j = 0; j < 10; j++) {
            sumTotal[j][i] /= 20;
        }
    }
/*
    printf("\n\n\nOver time:\n");
    int binss = 10;
    int len = 500;
    //for (i = 0; i < 10; i++) {
        for (j = 0; j < len / binss; j++) {
            printf("%lf\t", getMean(sumTotal[0], j*binss, (j+1)*binss));
            printf("%lf\t", getMean(sumTotal[1], j*binss, (j+1)*binss));
            printf("%lf\t", getMean(sumTotal[2], j*binss, (j+1)*binss));
            printf("%lf\t", getMean(sumTotal[4], j*binss, (j+1)*binss));
            printf("%lf\n", getMean(sumTotal[5], j*binss, (j+1)*binss));
        }
    //    printf("\n");
    //}
*/

    testRobustness(outcomes, count[index]);
    index ++;
    }
    
    printf("\n\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%.4lf\t", count[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void thecomms() {
    double hh_blocks[100], hh_chicken[100], hh_prisoners[100];
    double hs_blocks[100], hs_chicken[100], hs_prisoners[100];
    double sh_blocks[100], sh_chicken[100], sh_prisoners[100];
    double ss_blocks[100], ss_chicken[100], ss_prisoners[100];

    double hh_pt_blocks[50], hh_pt_chicken[50], hh_pt_prisoners[50];
    double hs_pt_blocks[50], hs_pt_chicken[50], hs_pt_prisoners[50];
    double sh_pt_blocks[50], sh_pt_chicken[50], sh_pt_prisoners[50];
    double ss_pt_blocks[50], ss_pt_chicken[50], ss_pt_prisoners[50];
    
    int i;
    for (i = 0; i < 50; i++) {
        hh_pt_prisoners[i] = hs_pt_prisoners[i] = sh_pt_prisoners[i] = ss_pt_prisoners[i] = -99999;
        hh_pt_blocks[i] = hs_pt_blocks[i] = sh_pt_blocks[i] = ss_pt_blocks[i] = -99999;
        hh_pt_chicken[i] = hs_pt_chicken[i] = sh_pt_chicken[i] = ss_pt_chicken[i] = -99999;
    }
    
    processNewH_H_blocks2(hh_blocks, hh_pt_blocks);
    processNewS_H_blocks2(sh_blocks, hs_blocks, sh_pt_blocks, hs_pt_blocks);
    processNewH_H_chicken2(hh_chicken, hh_pt_chicken);
    processNewS_H_chicken2(sh_chicken, hs_chicken, sh_pt_chicken, hs_pt_chicken);
    processNewH_H_prisoners(hh_prisoners, hh_pt_prisoners);
    processNewS_H_prisoners(sh_prisoners, hs_prisoners, sh_pt_prisoners, hs_pt_prisoners);
    
    double valss[2][100];
    newRead2("blocks2", "megaS++", "megaS++", valss, ss_pt_blocks, 47, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_blocks[i] = (valss[0][i] + valss[1][i]) / 2.0;
    newRead2("chicken", "megaS++", "megaS++", valss, ss_pt_chicken, 54, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_chicken[i] = (valss[0][i] + valss[1][i]) / 2.0;
    newRead2("prisoners", "megaS++", "megaS++", valss, ss_pt_prisoners, 51, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_prisoners[i] = (valss[0][i] + valss[1][i]) / 2.0;
    
    printSome("prisoners", hh_prisoners, hs_prisoners, sh_prisoners, ss_prisoners, 5, 50);
    printSome("blocks", hh_blocks, hs_blocks, sh_blocks, ss_blocks, 5, 45);
    printSome("chicken", hh_chicken, hs_chicken, sh_chicken, ss_chicken, 5, 50);
    
    printPoints("prisoners", hh_pt_prisoners, hs_pt_prisoners, sh_pt_prisoners, ss_pt_prisoners, 50);
    printPoints("blocks", hh_pt_blocks, hs_pt_blocks, sh_pt_blocks, ss_pt_blocks, 50);
    printPoints("chicken", hh_pt_chicken, hs_pt_chicken, sh_pt_chicken, ss_pt_chicken, 50);
}

void nocomms() {
    double hh_blocks[100], hh_chicken[100], hh_prisoners[100];
    double hs_blocks[100], hs_chicken[100], hs_prisoners[100];
    double sh_blocks[100], sh_chicken[100], sh_prisoners[100];
    double ss_blocks[100], ss_chicken[100], ss_prisoners[100];

    double hh_pt_blocks[50], hh_pt_chicken[50], hh_pt_prisoners[50];
    double hs_pt_blocks[50], hs_pt_chicken[50], hs_pt_prisoners[50];
    double sh_pt_blocks[50], sh_pt_chicken[50], sh_pt_prisoners[50];
    double ss_pt_blocks[50], ss_pt_chicken[50], ss_pt_prisoners[50];
    
    int i;
    for (i = 0; i < 50; i++) {
        hh_pt_prisoners[i] = hs_pt_prisoners[i] = sh_pt_prisoners[i] = ss_pt_prisoners[i] = -99999;
        hh_pt_blocks[i] = hs_pt_blocks[i] = sh_pt_blocks[i] = ss_pt_blocks[i] = -99999;
        hh_pt_chicken[i] = hs_pt_chicken[i] = sh_pt_chicken[i] = ss_pt_chicken[i] = -99999;
    }
    
    processNewH_H_blocks2_no(hh_blocks, hh_pt_blocks);
    processNewS_H_blocks2_no(sh_blocks, hs_blocks, sh_pt_blocks, hs_pt_blocks);
    processNewH_H_chicken2_no(hh_chicken, hh_pt_chicken);
    processNewS_H_chicken2_no(sh_chicken, hs_chicken, sh_pt_chicken, hs_pt_chicken);
    processNewH_H_prisoners_no(hh_prisoners, hh_pt_prisoners);
    processNewS_H_prisoners_no(sh_prisoners, hs_prisoners, sh_pt_prisoners, hs_pt_prisoners);
    
    double valss[2][100];
    newRead2("blocks2", "megaS++-", "megaS++-", valss, ss_pt_blocks, 47, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_blocks[i] = (valss[0][i] + valss[1][i]) / 2.0;
    newRead2("chicken", "megaS++-", "megaS++-", valss, ss_pt_chicken, 54, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_chicken[i] = (valss[0][i] + valss[1][i]) / 2.0;
    newRead2("prisoners", "megaS++-", "megaS++-", valss, ss_pt_prisoners, 51, 50, 100, true);
    for (i = 0; i < 100; i++)
        ss_prisoners[i] = (valss[0][i] + valss[1][i]) / 2.0;
    
    printSome("prisoners", hh_prisoners, hs_prisoners, sh_prisoners, ss_prisoners, 5, 50);
    printSome("blocks", hh_blocks, hs_blocks, sh_blocks, ss_blocks, 5, 45);
    printSome("chicken", hh_chicken, hs_chicken, sh_chicken, ss_chicken, 5, 50);
    
    printPoints("prisoners", hh_pt_prisoners, hs_pt_prisoners, sh_pt_prisoners, ss_pt_prisoners, 50);
    printPoints("blocks", hh_pt_blocks, hs_pt_blocks, sh_pt_blocks, ss_pt_blocks, 50);
    printPoints("chicken", hh_pt_chicken, hs_pt_chicken, sh_pt_chicken, ss_pt_chicken, 50);
}

void recipCoopData() {
    
    int counts_hh[100], counts_ss[100], counts_sh[100];
    int counts_hh_no[100], counts_ss_no[100], counts_sh_no[100];
    
    for (int k = 0; k < 100; k++) {
        counts_hh[k] = counts_ss[k] = counts_sh[k] = 0;
        counts_hh_no[k] = counts_ss_no[k] = counts_sh_no[k] = 0;
    }
    
    double probs_hh_prisoners[100], probs_ss_prisoners[100], probs_sh_prisoners[100];
    double probs_hh_prisoners_no[100], probs_ss_prisoners_no[100], probs_sh_prisoners_no[100];
    
    recipCoop_HH("prisoners", true, 51, probs_hh_prisoners, counts_hh);
    recipCoop_HH("prisoners", false, 51, probs_hh_prisoners_no, counts_hh_no);
    recipCoop_SS("prisoners", true, 51, probs_ss_prisoners, counts_ss);
    recipCoop_SS("prisoners", false, 51, probs_ss_prisoners_no, counts_ss_no);
    recipCoop_SH("prisoners", true, 51, probs_sh_prisoners, counts_sh);
    recipCoop_SH("prisoners", false, 51, probs_sh_prisoners_no, counts_sh_no);

    FILE *fpw = fopen("../newResults/coopTime_prisoners.txt", "w");
    for (int k = 0; k < 51; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_prisoners[k], probs_sh_prisoners[k], probs_ss_prisoners[k]);
    }
    fflush(fpw);

    fpw = fopen("../newResults/coopTime_prisoners_no.txt", "w");
    for (int k = 0; k < 51; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_prisoners_no[k], probs_sh_prisoners_no[k], probs_ss_prisoners_no[k]);
    }
    fflush(fpw);

    double probs_hh_chicken[100], probs_ss_chicken[100], probs_sh_chicken[100];
    double probs_hh_chicken_no[100], probs_ss_chicken_no[100], probs_sh_chicken_no[100];

    recipCoop_HH("chicken2", true, 54, probs_hh_chicken, counts_hh);
    recipCoop_HH("chicken2", false, 54, probs_hh_chicken_no, counts_hh_no);
    recipCoop_SS("chicken", true, 54, probs_ss_chicken, counts_ss);
    recipCoop_SS("chicken", false, 54, probs_ss_chicken_no, counts_ss_no);
    recipCoop_SH("chicken2", true, 54, probs_sh_chicken, counts_sh);
    recipCoop_SH("chicken2", false, 54, probs_sh_chicken_no, counts_sh_no);

    fpw = fopen("../newResults/coopTime_chicken.txt", "w");
    for (int k = 0; k < 54; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_chicken[k], probs_sh_chicken[k], probs_ss_chicken[k]);
    }
    fflush(fpw);

    fpw = fopen("../newResults/coopTime_chicken_no.txt", "w");
    for (int k = 0; k < 54; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_chicken_no[k], probs_sh_chicken_no[k], probs_ss_chicken_no[k]);
    }
    fflush(fpw);

    double probs_hh_blocks[100], probs_ss_blocks[100], probs_sh_blocks[100];
    double probs_hh_blocks_no[100], probs_ss_blocks_no[100], probs_sh_blocks_no[100];

    recipCoop_HH("blocks2", true, 47, probs_hh_blocks, counts_hh);
    recipCoop_HH("blocks2", false, 47, probs_hh_blocks_no, counts_hh_no);
    recipCoop_SS("blocks2", true, 47, probs_ss_blocks, counts_ss);
    recipCoop_SS("blocks2", false, 47, probs_ss_blocks_no, counts_ss_no);
    recipCoop_SH("blocks2", true, 47, probs_sh_blocks, counts_sh);
    recipCoop_SH("blocks2", false, 47, probs_sh_blocks_no, counts_sh_no);

    fpw = fopen("../newResults/coopTime_blocks.txt", "w");
    for (int k = 0; k < 47; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_blocks[k], probs_sh_blocks[k], probs_ss_blocks[k]);
    }
    fflush(fpw);

    fpw = fopen("../newResults/coopTime_blocks_no.txt", "w");
    for (int k = 0; k < 47; k++) {
        fprintf(fpw, "%lf\t%lf\t%lf\n", probs_hh_blocks_no[k], probs_sh_blocks_no[k], probs_ss_blocks_no[k]);
    }
    fflush(fpw);

    double probs_hh[100], probs_ss[100], probs_sh[100];
    double probs_hh_no[100], probs_ss_no[100], probs_sh_no[100];
    for (int k = 0; k < 47; k++) {
        probs_hh[k] = counts_hh[k] / 35.0;
        probs_sh[k] = counts_sh[k] / 35.0;
        probs_ss[k] = counts_ss[k] / 150.0;
        probs_hh_no[k] = counts_hh_no[k] / 30.0;
        probs_sh_no[k] = counts_sh_no[k] / 30.0;
        probs_ss_no[k] = counts_ss_no[k] / 150.0;
    }
    
    for (int k = 47; k < 51; k++) {
        probs_hh[k] = counts_hh[k] / 23.0;
        probs_sh[k] = counts_sh[k] / 23.0;
        probs_ss[k] = counts_ss[k] / 100.0;
        probs_hh_no[k] = counts_hh_no[k] / 20.0;
        probs_sh_no[k] = counts_sh_no[k] / 20.0;
        probs_ss_no[k] = counts_ss_no[k] / 100.0;
    }
    
    for (int k = 51; k < 54; k++) {
        probs_hh[k] = counts_hh[k] / 11.0;
        probs_sh[k] = counts_sh[k] / 11.0;
        probs_ss[k] = counts_ss[k] / 50.0;
        probs_hh_no[k] = counts_hh_no[k] / 10.0;
        probs_sh_no[k] = counts_sh_no[k] / 10.0;
        probs_ss_no[k] = counts_ss_no[k] / 50.0;
    }
    
    int bin = 5;
    double hh, sh, ss;
    
    fpw = fopen("../newResults/coopTime.csv", "w");
    fprintf(fpw, "hh,sh,ss\n");
    for (int k = 0; k < 45; k+=bin) {
        hh = sh = ss = 0;
        for (int j = k; j < k+bin; j++) {
            hh += probs_hh[j];
            sh += probs_sh[j];
            ss += probs_ss[j];
        }
        //fprintf(fpw, "%lf,%lf,%lf\n", probs_hh[k], probs_sh[k], probs_ss[k]);
        fprintf(fpw, "%lf,%lf,%lf\n", hh / bin, sh / bin, ss / bin);
    }
    fflush(fpw);

    fpw = fopen("../newResults/coopTime_no.csv", "w");
    fprintf(fpw, "hh,sh,ss\n");
    for (int k = 0; k < 45; k+=bin) {
        hh = sh = ss = 0;
        for (int j = k; j < k+bin; j++) {
            hh += probs_hh_no[j];
            sh += probs_sh_no[j];
            ss += probs_ss_no[j];
        }
        fprintf(fpw, "%lf,%lf,%lf\n", hh / bin, sh / bin, ss / bin);
        //fprintf(fpw, "%lf,%lf,%lf\n", probs_hh_no[k], probs_sh_no[k], probs_ss_no[k]);
    }
    fflush(fpw);
}

void addSS_filtered(FILE *dsfp, const char *game, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    int a1, a2;
    //double payouts[2] = {-1.0, -1.0}, prev[2];
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    int iters = 51;
    double sum1, sum2, coop;
    
    char game2[1024];
    strcpy(game2, game);
    
    if (!strcmp(game, "chicken")) {
        iters = 54;
        coopVals[0] = coopVals[1] = 0.84;
        strcpy(game2, "chicken2");
    }
    else if (!strcmp(game, "blocks2")) {
        iters = 47;
        cooplen = 2;
        coopVals[0] = coopVals[1] = 1.4;
    }
    
    int add = 0;
    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++", "megaS++", i);


        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        if ((iters % 2) == 0)
            add = 1;
        for (j = len/2; j < iters-len/2+add; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,Yes,SS,megaSpp,megaSpp,%i,%lf,%lf,%lf\n", game2, j+1, sum2, sum1, coop);
        }
        
        fclose(fp);
    }

    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++-", "megaS++-", i);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        if ((iters % 2) == 0)
            add = 1;
        for (j = len/2; j < iters-len/2+add; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,No,SS,megaSpp,megaSpp,%i,%lf,%lf,%lf\n", game2, j+1, sum2, sum1, coop);
        }
        
        fclose(fp);
    }

}

void addSS_filtered2(FILE *dsfp, const char *game, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    int a1, a2;
    //double payouts[2] = {-1.0, -1.0}, prev[2];
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    int iters = 51;
    double sum1, sum2, coop, tot;
    
    char game2[1024];
    strcpy(game2, game);
    
    if (!strcmp(game, "chicken")) {
        iters = 54;
        coopVals[0] = coopVals[1] = 0.84;
        strcpy(game2, "chicken2");
    }
    else if (!strcmp(game, "blocks2")) {
        iters = 47;
        cooplen = 2;
        coopVals[0] = coopVals[1] = 1.4;
    }
    
    int add = 0;
    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++", "megaS++", i);


        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        if ((iters % 2) == 0)
            add = 1;
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,Yes,SS,megaSpp,megaSpp,%i,%lf,%lf,%lf\n", game2, j+1, sum2/tot, sum1/tot, coop/tot);
        }
        
        fclose(fp);
    }

    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++-", "megaS++-", i);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        if ((iters % 2) == 0)
            add = 1;
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,No,SS,megaSpp,megaSpp,%i,%lf,%lf,%lf\n", game2, j+1, sum2/tot, sum1/tot, coop/tot);
        }
        
        fclose(fp);
    }

}

void addSS(FILE *dsfp, const char *game) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2] = {-1.0, -1.0}, prev[2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    int iters = 51;
    
    char game2[1024];
    strcpy(game2, game);
    
    if (!strcmp(game, "chicken")) {
        iters = 54;
        coopVals[0] = coopVals[1] = 0.84;
        strcpy(game2, "chicken2");
    }
    else if (!strcmp(game, "blocks2")) {
        iters = 47;
        cooplen = 2;
        coopVals[0] = coopVals[1] = 1.4;
    }
    
    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++", "megaS++", i);
        
        for (j = 0; j < iters; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,Yes,SS,megaSpp,megaSpp,%i,%i,%i,%i,%lf,%lf,%i\n", game2, i, j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }

    for (i = 0; i < 50; i++) {
        fp = openSesime2(game, "megaS++-", "megaS++-", i);
        
        for (j = 0; j < iters; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,No,SS,megaSpp,megaSpp,%i,%i,%i,%i,%lf,%lf,%i\n", game2, i, j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }

}

void addHH_prisoners_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop;
    
    int iters = 51;
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_hh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", hh_prisoners_comms[i][0], hh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_prisoners_comms[i][0], hh_prisoners_comms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }

    set_hh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("prisoners", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }
}

void addHH_prisoners_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop, tot;
    
    int iters = 51;
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_hh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", hh_prisoners_comms[i][0], hh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_prisoners_comms[i][0], hh_prisoners_comms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
            
            //printf("tot: %lf\n", tot);
            //break;
        }
        
        //break;
        
        fclose(fp);
    }

    //return;

    set_hh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("prisoners", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
        }
        
        fclose(fp);
    }
}


void addHH_prisoners(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2] = {-1.0, -1.0}, prev[2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_hh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", hh_prisoners_comms[i][0], hh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < 51; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_prisoners_comms[i][0], hh_prisoners_comms[i][1], getGroup(hh_prisoners_comms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }
    
    set_hh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        //printf("%s vs %s\n", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1]);
        fp = openSesime("prisoners", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < 51; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1], getGroup(hh_prisoners_nocomms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }
}

void addSH_prisoners_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop;
    
    int iters = 51;
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_sh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", sh_prisoners_comms[i][0], sh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_prisoners_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_comms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_comms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }

    set_sh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("prisoners", sh_prisoners_nocomms[i][0], sh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_prisoners_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_nocomms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_nocomms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }
}

void addSH_prisoners_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop, tot;
    
    int iters = 51;
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_sh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", sh_prisoners_comms[i][0], sh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_prisoners_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_comms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_comms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }

    set_sh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("prisoners", sh_prisoners_nocomms[i][0], sh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_prisoners_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_nocomms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_prisoners_nocomms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }
}

void addSH_prisoners(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2] = {-1.0, -1.0}, prev[2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    
    char game[1024];
    strcpy(game, "prisoners");
    
    set_sh_prisoners_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("prisoners", sh_prisoners_comms[i][0], sh_prisoners_comms[i][1], 0, true);
        
        for (j = 0; j < 51; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_prisoners_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_prisoners_comms[i][0], getGroup(sh_prisoners_comms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_prisoners_comms[i][1], getGroup(sh_prisoners_comms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
        }
        
        fclose(fp);
    }
    
    set_sh_prisoners_nocomms();
    for (i = 0; i < 10; i++) {
        //printf("%s vs %s\n", hh_prisoners_nocomms[i][0], hh_prisoners_nocomms[i][1]);
        fp = openSesime("prisoners", sh_prisoners_nocomms[i][0], sh_prisoners_nocomms[i][1], 0, false);
        
        for (j = 0; j < 51; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_prisoners_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_prisoners_nocomms[i][0], getGroup(sh_prisoners_nocomms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_prisoners_nocomms[i][1], getGroup(sh_prisoners_nocomms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
        }
        
        fclose(fp);
    }
}

void addHH_chicken_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop;
    
    int iters = 54;
    
    
    //printf("got here\n");
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_hh_chicken_comms();
    for (i = 0; i < 11; i++) {
        fp = openSesime("chicken2", hh_chicken_comms[i][0], hh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2+1; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_chicken_comms[i][0], hh_chicken_comms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }

    set_hh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("chicken2", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2+1; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }
}

void addHH_chicken_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.6, 0.6};
    double sum1, sum2, coop, tot;
    
    int iters = 54;
    
    
    //printf("got here\n");
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_hh_chicken_comms();
    for (i = 0; i < 11; i++) {
        fp = openSesime("chicken2", hh_chicken_comms[i][0], hh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_chicken_comms[i][0], hh_chicken_comms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
        }
        
        fclose(fp);
    }

    set_hh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("chicken2", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
        }
        
        fclose(fp);
    }
}

void addHH_chicken(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2] = {-1.0, -1.0}, prev[2];
    int cooplen = 1;
    double coopVals[2] = {0.84, 0.84};
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_hh_chicken_comms();
    for (i = 0; i < 11; i++) {
        fp = openSesime("chicken2", hh_chicken_comms[i][0], hh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < 54; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_chicken_comms[i][0], hh_chicken_comms[i][1], getGroup(hh_chicken_comms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }
    
    set_hh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        //printf("%s vs %s\n", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1]);
        fp = openSesime("chicken2", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < 54; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1], getGroup(hh_chicken_nocomms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
        }
        
        fclose(fp);
    }
    
}

void addSH_chicken_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.84, 0.84};
    double sum1, sum2, coop;
    
    int iters = 54;
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_sh_chicken_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("chicken2", sh_chicken_comms[i][0], sh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2+1; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_chicken_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_comms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_comms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }

    set_sh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("chicken2", sh_chicken_nocomms[i][0], sh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2+1; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_chicken_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_nocomms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_nocomms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }
}

void addSH_chicken_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 1;
    double coopVals[2] = {0.84, 0.84};
    double sum1, sum2, coop, tot;
    
    int iters = 54;
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_sh_chicken_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("chicken2", sh_chicken_comms[i][0], sh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_chicken_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_comms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_comms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }

    set_sh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("chicken2", sh_chicken_nocomms[i][0], sh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_chicken_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_nocomms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_chicken_nocomms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }
}

void addSH_chicken(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2] = {-1.0, -1.0}, prev[2];
    int cooplen = 1;
    double coopVals[2] = {0.84, 0.84};
    
    char game[1024];
    strcpy(game, "chicken2");
    
    set_sh_chicken_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("chicken2", sh_chicken_comms[i][0], sh_chicken_comms[i][1], 0, true);
        
        for (j = 0; j < 54; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_chicken_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_chicken_comms[i][0], getGroup(sh_chicken_comms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i, %i,%i,%i,%lf,%lf,%i\n", game, sh_chicken_comms[i][1], getGroup(sh_chicken_comms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }
    
    set_sh_chicken_nocomms();
    for (i = 0; i < 10; i++) {
        //printf("%s vs %s\n", hh_chicken_nocomms[i][0], hh_chicken_nocomms[i][1]);
        fp = openSesime("chicken2", sh_chicken_nocomms[i][0], sh_chicken_nocomms[i][1], 0, false);
        
        for (j = 0; j < 54; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_chicken_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_chicken_nocomms[i][0], getGroup(sh_chicken_nocomms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_chicken_nocomms[i][1], getGroup(sh_chicken_nocomms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], reciprocatedCoop2(prev, payouts, coopVals, cooplen));
            }
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }
}

void addHH_blocks_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    double sum1, sum2, coop;
    
    int iters = 47;
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_hh_blocks_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("blocks2", hh_blocks_comms[i][0], hh_blocks_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_blocks_comms[i][0], hh_blocks_comms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }

    set_hh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1],
                    j+1, sum1, sum2, coop);
        }
        
        fclose(fp);
    }
}

void addHH_blocks_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    double sum1, sum2, coop, tot;
    
    int iters = 47;
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_hh_blocks_comms();
    for (i = 0; i < 12; i++) {
        fp = openSesime("blocks2", hh_blocks_comms[i][0], hh_blocks_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_blocks_comms[i][0], hh_blocks_comms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
        }
        
        fclose(fp);
    }

    set_hh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%lf,%lf,%lf\n", game, hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1],
                    j+1, sum1/tot, sum2/tot, coop/tot);
        }
        
        fclose(fp);
    }
}

void addHH_blocks(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2], prev[2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    int recip[100];
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_hh_blocks_comms();
    for (i = 0; i < 12; i++) {
        payouts[0] = payouts[1] = -1.0;
        fp = openSesime("blocks2", hh_blocks_comms[i][0], hh_blocks_comms[i][1], 0, true);
        reciprocatedCooperation(fp, recip, cooplen, coopVals, 47);
        fclose(fp);
        fp = openSesime("blocks2", hh_blocks_comms[i][0], hh_blocks_comms[i][1], 0, true);
        for (j = 0; j < 47; j++) {
            //printf("%i: %i\n", j+1, recip[j]);
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,Yes,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_blocks_comms[i][0], hh_blocks_comms[i][1], getGroup(hh_blocks_comms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], recip[j]);
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }

    set_hh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], 0, false);
        reciprocatedCooperation(fp, recip, cooplen, coopVals, 47);
        fclose(fp);
        fp = openSesime("blocks2", hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], 0, false);
        for (j = 0; j < 47; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            fprintf(dsfp, "%s,No,HH,%s,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, hh_blocks_nocomms[i][0], hh_blocks_nocomms[i][1], getGroup(hh_blocks_nocomms[i][1]),
                    j+1, a1, a2, payouts[0], payouts[1], recip[i]);
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }

}

void addSH_blocks_filtered(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    double sum1, sum2, coop;
    
    int iters = 47;
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_sh_blocks_comms();
    for (i = 0; i < 11; i++) {
        fp = openSesime("blocks2", sh_blocks_comms[i][0], sh_blocks_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_blocks_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_comms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_comms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }

    set_sh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", sh_blocks_nocomms[i][0], sh_blocks_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = len/2; j < iters-len/2; j+=len) {
            sum1 = sum2 = coop = 0.0;
            for (k = j; k < j+len; k++) {
                sum1 += payouts[k-len/2][0] * fracs[k-j];
                sum2 += payouts[k-len/2][1] * fracs[k-j];
                coop += reciprocatedCoop2(payouts[k-len/2 - 1], payouts[k-len/2], coopVals, cooplen) * fracs[k-j];
            }
            
            if (!strcmp(sh_blocks_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_nocomms[i][0],
                        j+1, sum2, sum1, coop);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_nocomms[i][1],
                        j+1, sum1, sum2, coop);
            }
        }
        
        fclose(fp);
    }
}

void addSH_blocks_filtered2(FILE *dsfp, int len, double fracs[100]) {
    int i, j, k;
    FILE *fp;
    double payouts[100][2];
    int actions[100][2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    double sum1, sum2, coop, tot;
    
    int iters = 47;
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_sh_blocks_comms();
    for (i = 0; i < 11; i++) {
        fp = openSesime("blocks2", sh_blocks_comms[i][0], sh_blocks_comms[i][1], 0, true);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_blocks_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_comms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_comms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }

    set_sh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", sh_blocks_nocomms[i][0], sh_blocks_nocomms[i][1], 0, false);
        
        for (j = 0; j < iters; j++) {
            fscanf(fp, "%i %i %lf %lf", &(actions[j][0]), &(actions[j][1]), &(payouts[j][0]), &(payouts[j][1]));
        }
        
        for (j = 0; j < iters; j++) {
            sum1 = sum2 = coop = 0.0;
            tot = 0.0;
            for (k = j-2; k < j+3; k++) {
                if ((k < 0) || (k >= iters))
                    continue;
                tot += fracs[k-(j-2)];
                sum1 += payouts[k][0] * fracs[k-(j-2)];
                sum2 += payouts[k][1] * fracs[k-(j-2)];
                coop += reciprocatedCoop2(payouts[k - 1], payouts[k], coopVals, cooplen) * fracs[k-(j-2)];
            }
            
            if (!strcmp(sh_blocks_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_nocomms[i][0],
                        j+1, sum2/tot, sum1/tot, coop/tot);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%lf,%lf,%lf\n", game, sh_blocks_nocomms[i][1],
                        j+1, sum1/tot, sum2/tot, coop/tot);
            }
        }
        
        fclose(fp);
    }
}

void addSH_blocks(FILE *dsfp) {
    int i, j;
    FILE *fp;
    int a1, a2;
    double payouts[2], prev[2];
    int cooplen = 2;
    double coopVals[2] = {1.4, 1.4};
    int recip[100];
    
    char game[1024];
    strcpy(game, "blocks2");
    
    set_sh_blocks_comms();
    for (i = 0; i < 11; i++) {
        payouts[0] = payouts[1] = -1.0;
        fp = openSesime("blocks2", sh_blocks_comms[i][0], sh_blocks_comms[i][1], 0, true);
        reciprocatedCooperation(fp, recip, cooplen, coopVals, 47);
        fclose(fp);
        fp = openSesime("blocks2", sh_blocks_comms[i][0], sh_blocks_comms[i][1], 0, true);
        for (j = 0; j < 47; j++) {
            //printf("%i: %i\n", j+1, recip[j]);
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_blocks_comms[i][1], "megaS++")) {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_blocks_comms[i][0], getGroup(sh_blocks_comms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], recip[j]);
            }
            else {
                fprintf(dsfp, "%s,Yes,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_blocks_comms[i][1], getGroup(sh_blocks_comms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], recip[j]);
            }
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }

    set_sh_blocks_nocomms();
    for (i = 0; i < 10; i++) {
        fp = openSesime("blocks2", sh_blocks_nocomms[i][0], sh_blocks_nocomms[i][1], 0, false);
        reciprocatedCooperation(fp, recip, cooplen, coopVals, 47);
        fclose(fp);
        fp = openSesime("blocks2", sh_blocks_nocomms[i][0], sh_blocks_nocomms[i][1], 0, false);
        for (j = 0; j < 47; j++) {
            prev[0] = payouts[0];
            prev[1] = payouts[1];
            fscanf(fp, "%i %i %lf %lf", &a1, &a2, &(payouts[0]), &(payouts[1]));
        
            if (!strcmp(sh_blocks_nocomms[i][1], "megaS++-")) {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_blocks_nocomms[i][0], getGroup(sh_blocks_nocomms[i][0]),
                        j+1, a1, a2, payouts[1], payouts[0], recip[j]);
            }
            else {
                fprintf(dsfp, "%s,No,SH,megaSpp,%s,%i,%i,%i,%i,%lf,%lf,%i\n", game, sh_blocks_nocomms[i][1], getGroup(sh_blocks_nocomms[i][1]),
                        j+1, a1, a2, payouts[0], payouts[1], recip[j]);
            }
        }
        
        //int c = reciprocatedCooperation(fp, recip, 1, coopVals, 51);
        
        fclose(fp);
    }

}

void createDataSet() {
    FILE *dsfp = fopen("../newResults/DataSet.csv", "w");

    fprintf(dsfp, "Game,Comms,Pairing,Name1,Name2,Group,Round,Action1,Action2,Payoff1,Payoff2,RecipCoop\n");
    
    setGroup();
    
    addHH_prisoners(dsfp);
    addHH_chicken(dsfp);
    addHH_blocks(dsfp);

    addSH_prisoners(dsfp);
    addSH_chicken(dsfp);
    addSH_blocks(dsfp);
    
    addSS(dsfp, "prisoners");
    addSS(dsfp, "chicken");
    addSS(dsfp, "blocks2");

    fclose(dsfp);
}

void createFilteredDataSet() {
    FILE *dsfp = fopen("../newResults/DataSet_filtered.csv", "w");

    fprintf(dsfp, "Game,Comms,Pairing,Name1,Name2,Round,Payoff1,Payoff2,RecipCoop\n");
    
    double filt[100];
    filt[0] = filt[1] = filt[2] = filt[3] = filt[4] = 0.2;
    
    addHH_prisoners_filtered(dsfp, 5, filt);
    addHH_chicken_filtered(dsfp, 5, filt);
    addHH_blocks_filtered(dsfp, 5, filt);

    addSH_prisoners_filtered(dsfp, 5, filt);
    addSH_chicken_filtered(dsfp, 5, filt);
    addSH_blocks_filtered(dsfp, 5, filt);
    
    addSS_filtered(dsfp, "prisoners", 5, filt);
    addSS_filtered(dsfp, "chicken", 5, filt);
    addSS_filtered(dsfp, "blocks2", 5, filt);

    fclose(dsfp);
}

void createFilteredDataSet2() {
    FILE *dsfp = fopen("../newResults/DataSet_filtered2.csv", "w");

    fprintf(dsfp, "Game,Comms,Pairing,Name1,Name2,Round,Payoff1,Payoff2,RecipCoop\n");
    
    double filt[100];
    //filt[0] = filt[1] = filt[2] = filt[3] = filt[4] = 0.2;
    filt[1] = filt[2] = filt[3] = 0.25;
    filt[0] = filt[4] = 0.125;
    
    addHH_prisoners_filtered2(dsfp, 5, filt);
    addHH_chicken_filtered2(dsfp, 5, filt);
    addHH_blocks_filtered2(dsfp, 5, filt);

    addSH_prisoners_filtered2(dsfp, 5, filt);
    addSH_chicken_filtered2(dsfp, 5, filt);
    addSH_blocks_filtered2(dsfp, 5, filt);
    
    addSS_filtered2(dsfp, "prisoners", 5, filt);
    addSS_filtered2(dsfp, "chicken", 5, filt);
    addSS_filtered2(dsfp, "blocks2", 5, filt);

    fclose(dsfp);
}

void createSummaryDataSet() {
    FILE *dsfp = fopen("../newResults/DataSet_summary.csv", "w");

    fprintf(dsfp, "Game,Comms,Pairing,Name1,Name2,Round,Payoff1,Payoff2,RecipCoop\n");
    
    double filt[100];
    int i;
    
    for (i = 0; i < 51; i++)
        filt[i] = 1.0 / 51.0;
    
    addHH_prisoners_filtered(dsfp, 51, filt);
    addSS_filtered(dsfp, "prisoners", 51, filt);
    addSH_prisoners_filtered(dsfp, 51, filt);

    for (i = 0; i < 54; i++)
        filt[i] = 1.0 / 54.0;

    addHH_chicken_filtered(dsfp, 54, filt);
    addSH_chicken_filtered(dsfp, 54, filt);
    addSS_filtered(dsfp, "chicken", 54, filt);

    for (i = 0; i < 47; i++)
        filt[i] = 1.0 / 47.0;
    addHH_blocks_filtered(dsfp, 47, filt);
    addSH_blocks_filtered(dsfp, 47, filt);
    addSS_filtered(dsfp, "blocks2", 47, filt);

    fclose(dsfp);
}

int main(int argc, char *argv[]) {
    //processNew(argv[1]);
    //thecomms();
    //nocomms();
    
    //recipCoopData();
    
    //createDataSet();
    createFilteredDataSet();
    //createFilteredDataSet2();
    //createSummaryDataSet();

    return 0;
}

