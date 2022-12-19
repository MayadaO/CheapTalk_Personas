#ifndef __Solver__delayer__
#define __Solver__delayer__

#include "defs.h"

class delayer {
public:
    delayer();
    delayer(int gameType);
    ~delayer();
    
    void delay(int _round, int _move);
    int newNeighbor(int neighbors[12], int numNeighbors, double dist[12], int newGuy, double ndist);
    int readSGPD();
    int readLegos();
    
    void printNeighbors(int neighbors[12], int numNeighbors, double dist[12]);
    
    double entries[15000][4];
    int numEntries;
    int gameTypo;
};

#endif
