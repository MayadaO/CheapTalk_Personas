#include "delayer.h"


delayer::delayer() {
    printf("incomplete delayer constructor\n");
    exit(1);
}

delayer::delayer(int gameType) {
    gameTypo = gameType;
    
    if (gameType == 1)
        numEntries = readSGPD();
    else if (gameType == 2)
        numEntries = readLegos();
    else {
        printf("delayer: which game?\n");
        exit(1);
    }
    
    printf("numEntries = %i\n", numEntries);
}

delayer::~delayer() {
}

void delayer::delay(int _round, int _move) {
    int i;
    int neighbors[12], numNeighbors = 0;
    double dist[12], ndist;
    
    // find the 12 nearest neighbors
    for (i = 0; i < numEntries; i++) {
        //if (gameTypo == 1)  // prisoners
            ndist = fabs((int)(entries[i][0]) - _round) + fabs((int)(entries[i][1]) - _move);
        //else // legos
        //    ndist = fabs((int)(entries[i][0]) - _round) + fabs((int)(entries[i][1]) - _move)
        if (numNeighbors < 12)
            numNeighbors = newNeighbor(neighbors, numNeighbors, dist, i, ndist);
        else {
            if (ndist < dist[11])
                numNeighbors = newNeighbor(neighbors, numNeighbors, dist, i, ndist);
        }
    }
    
    printNeighbors(neighbors, numNeighbors, dist);
    
    // randomly pick one of them
    int num = rand() % 12;
    
    double d = entries[neighbors[num]][3];
    
    // make sure it isn't more than 9.61 seconds
    if ((d > 8.61) && (gameTypo == 1))
        d = 8.61;
    else if ((d > 8.61) && (gameTypo == 2))
        d = 10.1;
    
    if (gameTypo == 1)
        d /= (1.0 + (_round / 20.0));
    
    printf("delay (%i, %i): %lf\n", _round, _move, d);
    
    usleep(d * 1000000);
}

int delayer::readSGPD() {
    FILE *fp = fopen("SGPDdelays.txt", "r");
    int num = 0;
    
    while (fscanf(fp, "%lf %lf %lf %lf", &(entries[num][0]), &(entries[num][1]), &(entries[num][2]), &(entries[num][3])) != EOF) {
        num ++;
    }
    
    fclose(fp);
    
    return num;
}

int delayer::readLegos() {
    FILE *fp = fopen("Legosdelays.txt", "r");
    int num = 0;
    
    while (fscanf(fp, "%lf %lf %lf %lf", &(entries[num][0]), &(entries[num][1]), &(entries[num][2]), &(entries[num][3])) != EOF) {
        num ++;
    }
    
    fclose(fp);
    
    return num;
}

int delayer::newNeighbor(int neighbors[12], int numNeighbors, double dist[12], int newGuy, double ndist) {
    int i, spot = numNeighbors;
    
    for (i = 0; i < numNeighbors; i++) {
        if (ndist < dist[i]) {
            spot = i;
            break;
        }
    }
    
    for (i = 11; i > spot; i--) {
        neighbors[i] = neighbors[i-1];
        dist[i] = dist[i-1];
    }
    
    neighbors[spot] = newGuy;
    dist[spot] = ndist;
    
    numNeighbors++;
    if (numNeighbors > 12)
        numNeighbors = 12;
    
    return numNeighbors;
}

void delayer::printNeighbors(int neighbors[12], int numNeighbors, double dist[12]) {
    int i;
    
    printf("nearest neighbors (%i):\n", numNeighbors);
    for (i = 0; i < numNeighbors; i++) {
        printf("%.0lf %.0lf %.3lf (%lf)\n", entries[neighbors[i]][0], entries[neighbors[i]][1], entries[neighbors[i]][3], dist[i]);
    }
}






