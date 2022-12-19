#ifndef __Solver__mensaje__
#define __Solver__mensaje__

#include "defs.h"

class Mensaje {
public:
    Mensaje();
    ~Mensaje();

    void createEntry(char msg[1024], int _lineHeight, int _origin);
    
    char txts[8][1024];
    int numLines;
    int lineHeight;
    int origin;
    float r, g, b;
};

#endif /* defined(__Solver__mensaje__) */
