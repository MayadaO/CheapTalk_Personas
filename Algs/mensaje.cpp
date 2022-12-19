#include "mensaje.h"

Mensaje::Mensaje() {
    numLines = 0;
}

Mensaje::~Mensaje() {
}

void Mensaje::createEntry(char msg[1024], int _lineHeight, int _origin) {
    origin = _origin;
    lineHeight = _lineHeight;
    
    int strt = 0, ind = 0;
    numLines = 0;
    while (ind < strlen(msg)) {
        if (msg[ind] == '|') {
            strncpy(txts[numLines], msg+strt, ind-strt);
            numLines++;
            strt=ind+1;
        }
        ind++;
    }
    
    //printf("message: %s\n", msg);
    //printf("message entry created with %i lines\n", numLines);
    
    if (origin < 0)
        r = g = b = 0.0f;
    else if (origin == 0) {
        r = 0.6f;
        g = b = 0.0f;
    }
    else {
        b = 0.6f;
        r = g = 0.0f;
    }    
}
