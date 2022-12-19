#ifndef Solver_personality_h
#define Solver_personality_h

#include "defs.h"

class Personality {
public:
    Personality();
    Personality(char filename[1024]);
    ~Personality();
    
    void createCheapTalk(char _stringToCommunicate[1024], int _A[2]);
    void replaceHolder(char tmp[1024], char ch1, char ch2);
    int lookUp(char ch, int index, int _A[2]);

    char speechActs[15][8][1024];
    char personalityString[1024];
    
    char pNombre[1024];
    
    int numMessagesLast;
};

#endif
