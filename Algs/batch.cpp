#include "defs.h"

char games[4][1024], gamesFancy[4][1024];
char partners[4][1024];

// agent, game, partner, trial, round
double payoffs[2][4][4][50][50];
double aves[2][4][4];

void setGames() {
    strcpy(games[0], "blocks2");
    strcpy(games[1], "prisoners");
    strcpy(games[2], "chicken");
    strcpy(games[3], "endless");
    strcpy(gamesFancy[0], "Alternator Game");
    strcpy(gamesFancy[1], "Prisoner's Dilemma");
    strcpy(gamesFancy[2], "Chicken");
    strcpy(gamesFancy[3], "Endless");
}

void setPartners() {
    strcpy(partners[0], "S#-Frank");
    strcpy(partners[1], "EEE#-Frank");
    strcpy(partners[2], "S#-Silent");
    strcpy(partners[3], "EEE#-Silent");
}

double readIt(int agent, int g, int p, int trial) {
    int i;
    char fnombre[1024];
    
    sprintf(fnombre, "../newResults/%s_%s_%s_activity_%i.txt", games[g], partners[p], partners[agent], trial);
    //printf("%s\n", fnombre);
    
    FILE *fp = fopen(fnombre, "r");
    
    int i1, i2;
    double f1, f2;
    for (i = 0; i < 50; i++) {
        fscanf(fp, "%i %i %lf %lf", &i1, &i2, &f1, &f2);
        payoffs[agent][g][p][trial][i] = f2;
    }
    
    fclose(fp);
    
    double sum = 0.0;
    for (i = 0; i < 50; i++)
        sum += payoffs[agent][g][p][trial][i];
    
    return sum;
}

void gatherAll() {
    FILE *fp = fopen("averages.csv", "w");
    fprintf(fp, "Agent,Partner,Game,Payoff\n");
    
    int agent, trial, p, g;
    double sum;
    for (g = 0; g < 4; g++) {
        for (agent = 0; agent < 2; agent ++) {
            for (p = 0; p < 4; p++) {
                aves[agent][0][p] = 0.0;
                for (trial = 0; trial < 50; trial++) {
                    sum = readIt(agent, g, p, trial);
                    aves[agent][g][p] += sum;
                    fprintf(fp, "%s,%s,%s,%lf\n", partners[agent], partners[p], gamesFancy[g], sum / 50.0);
                }
                
                printf("%s vs %s in %s: %lf\n", partners[agent], partners[p], games[g], aves[agent][g][p] / (50 * 50));
            }
        }
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    setGames();
    setPartners();
    
    gatherAll();

    return 0;
}