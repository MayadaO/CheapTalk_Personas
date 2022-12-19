#include "defs.h"
#include "GameEngine.h"
#include "MazeEngine.h"
#include "LEGOEngine.h"
#include "Mensaje.h"

#define SQR_WDTH    40

void initGl(int argc, char *argv[], const char *title, int _yo);
void SpecialKeys(int key, int x, int y);
void loggingKeys(int _dirr);
void RegularKeys(unsigned char key, int x, int y);
void myMouse(int button, int state,int x, int y);
void Resize(int width, int height);
void DrawWorld();

void FillBox(int x, int y, int width, int height);
void OutlineBox(int x, int y, int width, int height);
void drawCircle(int x, int y, int r);
void drawTriangle(int x, int y, int width, int height);
void drawHalfCircle(int x, int y, int r);
void DrawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);
void DrawText12(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);
void DrawText24(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);

void drawPD();
void drawAgents();
void drawLEGOS();
void writeHighScores();
void drawInformation();

void playedArrow(int x, int y, int d);

void drawTheLego(LEGO *l, int cx, int cy);

void addMessage(char msg[1024], int _origin, int _lineHeight);

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
int direccion = NONE;
int playedDireccion = NONE;

int WIDTH, HEIGHT;

extern GameEngine *ge;
int currentRnd = 0;
int yo = -1;
double currentPayoffs[2] = {0, 0};
double avePayoffs[2] = {0.0, 0.0};
double totalPoints[2] = {0, 0};
int currentMove = 0;
extern bool gameOver;

bool textOn = true;//false;

Mensaje messageHistory[20000];
int numMessages = 0;

extern char partido[1024];
extern int nsamps;
extern double filsPerPoint;

extern char high_namelist[100][1024];
extern double high_scorelist[100];
extern int high_numlist;
extern int high_placement[2];

extern bool logAll;
extern FILE *allFP;
extern double strtTime2;
extern bool choiceActive;

struct timeval eltiempito;

void initGl(int argc, char *argv[], const char *title, int _yo) {
    strcpy(partido, title);
    int i;
    
    yo = _yo;
    
    //printf("initializing (%s - %i) ... ", partido, argc); fflush(stdout);
    
    glutInit(&argc, argv);
    
    //printf("glutInit ... "); fflush(stdout);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    int WIDTH = 2 * 11 * SQR_WDTH;//2 * BORDER + (highx - lowx) * 200;
    int HEIGHT = 17 * SQR_WDTH;//2 * BORDER + (highy - lowy) * 120;

    //printf("glutInitDisplay ... "); fflush(stdout);
    
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGHT);
    
    int winid = glutCreateWindow(title);

    //printf("glutCreateWindow (%s) ... ", title); fflush(stdout);
    
    glutReshapeFunc(Resize);
    glutDisplayFunc(DrawWorld);
    glutIdleFunc(DrawWorld);
    
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(RegularKeys);
    glutMouseFunc(myMouse);
    
    //printf("looping\n"); fflush(stdout);
    
    glutMainLoop();
}


void SpecialKeys(int key, int x, int y) {
    if (!strncmp(partido,"legos",5))
        return;
    
    //printf("special key hit: %i\n", key);
    pthread_mutex_lock( &count_mutex );
    switch (key) {
        case GLUT_KEY_UP: direccion = UPP; loggingKeys(direccion); break;
        case GLUT_KEY_DOWN: direccion = DOWNN; loggingKeys(direccion); break;
        case GLUT_KEY_RIGHT: direccion = RIGHTT; loggingKeys(direccion); break;
        case GLUT_KEY_LEFT: direccion = LEFTT; loggingKeys(direccion); break;
    }
    
    //printf("glStuff: direccion = %i\n", direccion); fflush(stdout);
    pthread_mutex_unlock( &count_mutex );
}

void loggingKeys(int _dirr) {
    if (logAll && (allFP != NULL)) {
        gettimeofday(&eltiempito, NULL);
        double ctme = eltiempito.tv_sec + (eltiempito.tv_usec / 1000000.0);
        fprintf(allFP, "%lf: played %i\n", ctme - strtTime2, _dirr);
    }
}

void RegularKeys(unsigned char key, int x, int y) {
/*    //printf("regular key hit: %c\n", key);
    switch (key) {
        case 'h':
            printf("Entering human-control mode\n");
            //hmnControl = true;
            break;
        default:
            //printf("Entering automated-control mode\n");
            //hmnControl = false;
            break;
    }*/
}

void myMouse(int button, int state, int x, int y) {
    if (!strncmp(partido,"prisoners",9))
        return;
    
    if (state == 1) {
        //printf("click: %i, %i\n", x, HEIGHT - y);
        int i;
        for (i = 0; i < 9; i++) {
            if (((LEGOEngine *)ge)->available[i]) {
                if ((fabs(x - ((LEGOEngine *)ge)->legos[i]->cx) < 25) && (fabs((HEIGHT-y) - ((LEGOEngine *)ge)->legos[i]->cy) < 25)) {
                    if (logAll && (allFP != NULL)) {
                        gettimeofday(&eltiempito, NULL);
                        double ctme = eltiempito.tv_sec + (eltiempito.tv_usec / 1000000.0);
                        fprintf(allFP, "%lf: clicked %i\n", ctme - strtTime2, i);
                    }
                    //printf("click on %i\n", i);
                    direccion = i;
                }
            }
        }
    }
}

void Resize(int width, int height) {
    WIDTH  = width;
    HEIGHT = height;
    
    //printf("WIDTH = %i\n", WIDTH);
    
    /* Reset the viewport... */
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLfloat)width, 0.0, (GLfloat)height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void DrawWorld() {
    glPushMatrix();
    
    glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
    //glClearColor(0.1f, 0.4f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!gameOver) {
        if (!strncmp(partido,"prisoners",9)) {
            drawPD();
            drawAgents();
        }
        else {
            drawLEGOS();
        }
    }
    else {
        writeHighScores();
    }
    
    drawInformation();
    
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    
    //usleep(100000);
}

void writeHighScores() {
    int cx = WIDTH / 4;
    int cy = HEIGHT / 2;
    
    glColor3f(0.95f, 0.95f, 0.95f);
    FillBox(cx+10, cy, WIDTH / 2 - SQR_WDTH, HEIGHT - SQR_WDTH*2);
    glColor3f(0.0f, 0.0f, 0.5f);
    OutlineBox(cx+10, cy, WIDTH / 2 - SQR_WDTH-4, HEIGHT - SQR_WDTH*2-4);

    char buf[1024];
    strcpy(buf, "High Score List");
    DrawText24(cx - 60, HEIGHT - 70, buf, 0.0f, 0.0f, 0.5f);
    
    int i, px;
    cy = HEIGHT - 100;
    for (i = 0; i < high_numlist; i++) {
        if (i >= 24)
            break;
        
        sprintf(buf, "%i.", i+1);
        px = 70;
        if (i+1 < 10)
            px += 10;
        if (i == high_placement[yo])
            DrawText(px, cy, buf, 0.7f, 0.0f, 0.0f);
        else
            DrawText(px, cy, buf, 0.0f, 0.0f, 0.0f);
        
        strcpy(buf, high_namelist[i]);
        if (i == high_placement[yo])
            DrawText(110, cy, buf, 0.7f, 0.0f, 0.0f);
        else
            DrawText(110, cy, buf, 0.0f, 0.0f, 0.0f);
        
        sprintf(buf, "%.2lf", high_scorelist[i]);
        px = 330;
        if ((high_scorelist[i] < 10.0) && (high_scorelist[i] > 0.0))
            px += 10;
        if (i == high_placement[yo])
            DrawText(px, cy, buf, 0.7f, 0.0f, 0.0f);
        else
            DrawText(px, cy, buf, 0.0f, 0.0f, 0.0f);
        
        cy -= 22;
    }
    
    if (high_placement[yo] > 23) {
        sprintf(buf, "%i.", high_placement[yo]);
        px = 70;
        DrawText(px, cy, buf, 0.7f, 0.0f, 0.0f);
        
        strcpy(buf, high_namelist[high_placement[yo]]);
        DrawText(110, cy, buf, 0.7f, 0.0f, 0.0f);
        
        sprintf(buf, "%.2lf", high_scorelist[high_placement[yo]]);
        px = 330;
        if ((high_scorelist[high_placement[yo]] < 10.0) && (high_scorelist[high_placement[yo]] > 0.0))
            px += 10;
        if (i == high_placement[yo])
            DrawText(px, cy, buf, 0.7f, 0.0f, 0.0f);
        else
            DrawText(px, cy, buf, 0.0f, 0.0f, 0.0f);
    }
}

void drawPD() {
    int bx = 3 * SQR_WDTH / 2, by = 3 * SQR_WDTH / 2;
    int i, j, x, y;
    for (i = 0; i < ((MazeEngine *)ge)->wWidth; i++) {
        for (j = 0; j < ((MazeEngine *)ge)->wHeight; j++) {
            x = i;//((MazeEngine *)ge)->wWidth - i - 1;
            y = ((MazeEngine *)ge)->wHeight - j - 1;
            if (!strcmp((((MazeEngine *)ge)->currentWorld[x][y]),"w")) {
                if (((MazeEngine *)ge)->world[x][y][0] == 'g') {
                    glColor3f(0.6f, 0.6f, 0.6f);
                    FillBox(bx + i * SQR_WDTH, by + j * SQR_WDTH, SQR_WDTH, SQR_WDTH);
                    glColor3f(0.0f, 0.0f, 0.0f);
                    OutlineBox(bx + i * SQR_WDTH, by + j * SQR_WDTH, SQR_WDTH-2, SQR_WDTH-2);
                }
                else {
                    glColor3f(0.0f, 0.0f, 0.0f);
                    FillBox(bx + i * SQR_WDTH, by + j * SQR_WDTH, SQR_WDTH, SQR_WDTH);
                }
                //printf("%s ", ((MazeEngine *)ge)->world[i][j]);
            }
            OutlineBox(bx + i * SQR_WDTH, by + j * SQR_WDTH, SQR_WDTH, SQR_WDTH);
        }
    }
    
    char buf[1024];
    strcpy(buf, "G");
    
    glColor3f(0.9f, 0.75f, 0.0f);
    FillBox(bx+SQR_WDTH, by+SQR_WDTH, SQR_WDTH, SQR_WDTH);
    DrawText24(bx+SQR_WDTH/2+12, by+SQR_WDTH/2+10, buf, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    OutlineBox(bx+SQR_WDTH, by+SQR_WDTH, SQR_WDTH, SQR_WDTH);

    glColor3f(0.9f, 0.75f, 0.0f);
    FillBox(bx+7*SQR_WDTH, by+SQR_WDTH, SQR_WDTH, SQR_WDTH);
    DrawText24(bx+6*SQR_WDTH+SQR_WDTH/2+12, by+SQR_WDTH/2+10, buf, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    OutlineBox(bx+7*SQR_WDTH, by+SQR_WDTH, SQR_WDTH, SQR_WDTH);
}

void drawLEGOS() {
    // draw the board
    if ((((LEGOEngine *)ge)->whosTurn == yo)  && choiceActive) {
        glColor3f(0.0f, 0.6f, 0.0f);
        FillBox(WIDTH / 4, HEIGHT / 2 + SQR_WDTH, SQR_WDTH*9 + 8, SQR_WDTH*13 + 8);
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    FillBox(WIDTH / 4, HEIGHT / 2 + SQR_WDTH, SQR_WDTH*9, SQR_WDTH*13);
    
    // draw the "legos"
    int cx, cy;
    //float r, g, b;
    int i;
    char buf[1024];
    for (i = 0; i < 9; i++) {
        if (((LEGOEngine *)ge)->available[i])
            drawTheLego(((LEGOEngine *)ge)->legos[i], ((LEGOEngine *)ge)->legos[i]->cx, ((LEGOEngine *)ge)->legos[i]->cy);
    }
    
    if ((((LEGOEngine *)ge)->whosTurn == yo) && choiceActive) {
        strcpy(buf, "Your Turn");
        DrawText24(WIDTH/4 - 50, SQR_WDTH+25, buf, 0.0f, 0.6f, 0.0f);
    }
    
    glColor3f(0.0f, 0.0f, 0.0f);
    FillBox(WIDTH/4, SQR_WDTH * 6 - 20, 300, 4);
    FillBox(WIDTH/4, SQR_WDTH * 13 + 20, 300, 4);
    
    strcpy(buf, "Your blocks:");
    DrawText24(SQR_WDTH+28, SQR_WDTH*4+3, buf, 0.0f, 0.0f, 0.0f);
    OutlineBox(WIDTH/4+3, SQR_WDTH*4+9, 54, 54);
    OutlineBox(WIDTH/4+3 + 60, SQR_WDTH*4+9, 54, 54);
    OutlineBox(WIDTH/4+3 + 120, SQR_WDTH*4+9, 54, 54);
    for (i = 0; i < ((LEGOEngine *)ge)->numPieces[yo]; i++) {
        drawTheLego(((LEGOEngine *)ge)->legos[((LEGOEngine *)ge)->ownership[yo][i]], WIDTH/4+3 + i*60, SQR_WDTH*4+9);
    }

    strcpy(buf, "His blocks:");
    DrawText24(SQR_WDTH+28, SQR_WDTH*14+24, buf, 0.0f, 0.0f, 0.0f);
    OutlineBox(WIDTH/4+3, SQR_WDTH*14+30, 54, 54);
    OutlineBox(WIDTH/4+3 + 60, SQR_WDTH*14+30, 54, 54);
    OutlineBox(WIDTH/4+3 + 120, SQR_WDTH*14+30, 54, 54);
    for (i = 0; i < ((LEGOEngine *)ge)->numPieces[1-yo]; i++) {
        drawTheLego(((LEGOEngine *)ge)->legos[((LEGOEngine *)ge)->ownership[1-yo][i]], WIDTH/4+3 + i*60, SQR_WDTH*14+30);
    }
}

void drawTheLego(LEGO *l, int cx, int cy) {
    float r, g, b;
    
    if (l->col == RED) {
        r = 0.7f; g = 0.0f; b = 0.0f;
    }
    else if (l->col == BLUE) {
        r = 0.0f; g = 0.0f; b = 0.7f;
    }
    else if (l->col == GREEN) {
        r = 1.0f; g = 0.85f; b = 0.0f;
    }
    
    glColor3f(r, g, b);
    if (l->shape == SQUARE) {
        FillBox(cx, cy, 50, 50);
    }
    else if (l->shape == TRIANGLE) {
        drawTriangle(cx, cy, 50, 50);
        cy -= 5;
    }
    else if (l->shape == CIRCLE) {
        drawCircle(cx, cy, 25);
    }
    
    char buf[1024];
    sprintf(buf, "%i", l->value);
    if (l->col != GREEN) {
        if (l->value > 9)
            DrawText24(cx-12, cy - 9, buf, 1.0f, 1.0f, 1.0f);
        else
            DrawText24(cx-6, cy - 9, buf, 1.0f, 1.0f, 1.0f);
    }
    else {
        if (l->value > 9)
            DrawText24(cx-12, cy - 9, buf, 0.0f, 0.0f, 0.0f);
        else
            DrawText24(cx-6, cy - 9, buf, 0.0f, 0.0f, 0.0f);
    }
}

void drawAgents() {
    if (playedDireccion != NONE) {
        //printf("%i\n", playedDireccion);

        int bx = 3 * SQR_WDTH / 2, by = 3 * SQR_WDTH / 2;
        int i = ((MazeEngine *)ge)->Pos[yo][0];
        int j = ((MazeEngine *)ge)->Pos[yo][1];
        int x = i;//((MazeEngine *)ge)->wWidth - i - 1;
        int y = ((MazeEngine *)ge)->wHeight - j - 1;
        
        glColor3f(0.4f, 0.4f, 0.4f);
        playedArrow(bx + x * SQR_WDTH, by + y * SQR_WDTH, playedDireccion);
    }
    
    if ((((MazeEngine *)ge)->Pos[0][0] == ((MazeEngine *)ge)->Pos[1][0]) && (((MazeEngine *)ge)->Pos[0][1] == ((MazeEngine *)ge)->Pos[1][1])) {
        int bx = 3 * SQR_WDTH / 2, by = 3 * SQR_WDTH / 2;
        int i = ((MazeEngine *)ge)->Pos[0][0];
        int j = ((MazeEngine *)ge)->Pos[0][1];
        int x = i;//((MazeEngine *)ge)->wWidth - i - 1;
        int y = ((MazeEngine *)ge)->wHeight - j - 1;
        
        glColor3f(0.8f, 0.0f, 0.0f);
        drawCircle(bx + x * SQR_WDTH, by + y * SQR_WDTH, SQR_WDTH / 2 - 2);
        glColor3f(0.0f, 0.0f, 0.8f);
        drawHalfCircle(bx + x * SQR_WDTH, by + y * SQR_WDTH, SQR_WDTH / 2 - 2);
    }
    else {
        int bx = 3 * SQR_WDTH / 2, by = 3 * SQR_WDTH / 2;
        int i = ((MazeEngine *)ge)->Pos[0][0];
        int j = ((MazeEngine *)ge)->Pos[0][1];
        int x = i;//((MazeEngine *)ge)->wWidth - i - 1;
        int y = ((MazeEngine *)ge)->wHeight - j - 1;
        char buf[1024];
        strcpy(buf, "You");
        
        glColor3f(0.8f, 0.0f, 0.0f);
        drawCircle(bx + x * SQR_WDTH, by + y * SQR_WDTH, SQR_WDTH / 2 - 2);
        
        if (yo == 0) {
            DrawText12(bx + x * SQR_WDTH - 12, by + y * SQR_WDTH - 4, buf, 1.0f, 1.0f, 1.0f);
        }
        
        i = ((MazeEngine *)ge)->Pos[1][0];
        j = ((MazeEngine *)ge)->Pos[1][1];
        x = i;//((MazeEngine *)ge)->wWidth - i - 1;
        y = ((MazeEngine *)ge)->wHeight - j - 1;
        
        glColor3f(0.0f, 0.0f, 0.8f);
        drawCircle(bx + x * SQR_WDTH, by + y * SQR_WDTH, SQR_WDTH / 2 - 2);
        
        if (yo == 1) {
            DrawText12(bx + x * SQR_WDTH - 12, by + y * SQR_WDTH - 4, buf, 1.0f, 1.0f, 1.0f);
        }
    }
}

void playedArrow(int x, int y, int d) {
    if (d == UPP) {
        y += SQR_WDTH / 2 + 7;
        FillBox(x, y, 10, 10);

        glBegin(GL_POLYGON);
        glVertex2i(x, y + 12);
        glVertex2i(x + 12, y+1);
        glVertex2i(x - 12, y+1);
        glEnd();
    }
    else if (d == DOWNN) {
        y -= SQR_WDTH / 2 + 7;
        FillBox(x, y, 10, 10);
        
        glBegin(GL_POLYGON);
        glVertex2i(x, y - 12);
        glVertex2i(x + 12, y-1);
        glVertex2i(x - 12, y-1);
        glEnd();
    }
    else if (d == LEFTT) {
        x -= SQR_WDTH / 2 + 7;
        FillBox(x, y, 10, 10);
        
        glBegin(GL_POLYGON);
        glVertex2i(x - 12, y);
        glVertex2i(x - 1, y+12);
        glVertex2i(x - 1, y-12);
        glEnd();
    }
    else if (d == RIGHTT) {
        x += SQR_WDTH / 2 + 7;
        FillBox(x, y, 10, 10);
        
        glBegin(GL_POLYGON);
        glVertex2i(x + 12, y);
        glVertex2i(x + 1, y+12);
        glVertex2i(x + 1, y-12);
        glEnd();
    }
}

void drawInformation() {
    int cx, cy;

    cx = 3.0 * WIDTH / 4.0;
    cy = HEIGHT - HEIGHT / 7.5;
    
    glColor3f(0.8f, 0.8f, 0.8f);
    FillBox(cx, cy, WIDTH / 2 - 2 * SQR_WDTH, SQR_WDTH*2.9);
    glColor3f(0.0f, 0.6f, 0.0f);
    OutlineBox(cx, cy, WIDTH / 2 - 2 * SQR_WDTH, SQR_WDTH*2.9);
    
    //cx = WIDTH / 4.0;
    //cy = 11;
    // Current Round
    cx = WIDTH / 2.0 + 65;
    cy -= 3;
    
    char buf[1024];
    //if (currentRnd < 10) {
        sprintf(buf, "Current round #:");//         %i", currentRnd);
        DrawText(cx, cy+30, buf, 0.0f, 0.0f, 0.0f);
        sprintf(buf, "%i", currentRnd);
        DrawText(cx+140, cy+30, buf, 0.0f, 0.6f, 0.0f);
    //}
    //else {
    //    sprintf(buf, "Current round #:");//         %i", currentRnd);
    //    DrawText(cx, cy, buf, 0.0f, 0.0f, 0.0f);
    //    sprintf(buf, "%i", currentRnd);
    //    DrawText(cx, cy, buf, 0.0f, 0.6f, 0.0f);
    //}
    
    //cy = HEIGHT - SQR_WDTH+11;
    
    sprintf(buf, "Your average payout:");//      $ %.2lf", avePayoffs[yo]);
    DrawText(cx, cy-5, buf, 0.0f, 0.0f, 0.0f);
    if ((avePayoffs[yo] < 9.99999) && (avePayoffs[yo] >= 0))
        sprintf(buf, "0%.2lf", avePayoffs[yo]);
    else
        sprintf(buf, "%.2lf", avePayoffs[yo]);
    DrawText(cx + 191, cy-5, buf, 0.0f, 0.0f, 0.7f);

    sprintf(buf, "Money earned (AED):");//      $ %.2lf", avePayoffs[yo]);
    DrawText(cx, cy-40, buf, 0.0f, 0.0f, 0.0f);
    double moneys = totalPoints[yo] * filsPerPoint * 0.01;
    if ((moneys < 9.99999) && (moneys >= 0))
        sprintf(buf, "0%.2lf", moneys);
    else
        sprintf(buf, "%.2lf", moneys);
    DrawText(cx + 191, cy-40, buf, 0.7f, 0.0f, 0.0f);

    
    if (gameOver) {
        //sprintf(buf, "Game Over");
        //DrawText24(WIDTH/4 - 58, SQR_WDTH+10, buf, 0.0f, 0.6f, 0.0f);
    }
    else if (currentMove == 0) {
        if (!strncmp(partido, "prisoners",9)) {
            sprintf(buf, "New Round");
            DrawText24(WIDTH/4 - 60, SQR_WDTH+10, buf, 0.0f, 0.6f, 0.0f);
        }
    }

    cx = 3.0 * WIDTH / 4.0;
    cy = HEIGHT - HEIGHT / 2.0 - SQR_WDTH*1.7;
    
    glColor3f(0.8f, 0.8f, 0.8f);
    FillBox(cx, cy, WIDTH / 2 - 2 * SQR_WDTH, SQR_WDTH*12);
    glColor3f(0.0f, 0.6f, 0.0f);
    OutlineBox(cx, cy, WIDTH / 2 - 2 * SQR_WDTH, SQR_WDTH*12);

    //sprintf(buf, "Game Log");
    //DrawText24(cx - 48, cy+SQR_WDTH*5.5, buf, 0.0f, 0.0f, 0.0f);
    
    cx = WIDTH/2 + SQR_WDTH + 8;
    cy = SQR_WDTH + 5;
    int i, j;
    for (i = numMessages-1; i >= 0; i--) {
        for (j = messageHistory[i].numLines-1; j >= 0; j--) {
            if (messageHistory[i].origin < 0) {
                DrawText12(cx, cy, messageHistory[i].txts[j], messageHistory[i].r, messageHistory[i].g, messageHistory[i].b);
                cy += messageHistory[i].lineHeight;
            }
            else if (textOn) {
                if (messageHistory[i].origin == 0) {
                    if (j == 0) {
                        sprintf(buf, "RED:");
                        DrawText12(cx+15, cy, buf, messageHistory[i].r, messageHistory[i].g, messageHistory[i].b);
                    }
                    DrawText12(cx+47, cy, messageHistory[i].txts[j], messageHistory[i].r, messageHistory[i].g, messageHistory[i].b);
                }
                else {
                    if (j == 0) {
                        sprintf(buf, "BLUE:");
                        DrawText12(cx+15, cy, buf, messageHistory[i].r, messageHistory[i].g, messageHistory[i].b);
                    }
                    DrawText12(cx+55, cy, messageHistory[i].txts[j], messageHistory[i].r, messageHistory[i].g, messageHistory[i].b);
                }
                cy += messageHistory[i].lineHeight;
            }
            
            //if (cy > HEIGHT-2*SQR_WDTH)
            if (cy > (HEIGHT - HEIGHT / 7.5 - SQR_WDTH*2.3))
                break;
        }
        if (cy > (HEIGHT - HEIGHT / 7.5 - SQR_WDTH*2.3))
            break;
    }
}

void FillBox(int x, int y, int width, int height) {
    glBegin(GL_POLYGON);
    glVertex2i(x - (width / 2), y + (height / 2));
    glVertex2i(x + (width / 2), y + (height / 2));
    glVertex2i(x + (width / 2), y - (height / 2));
    glVertex2i(x - (width / 2), y - (height / 2));
    glEnd();
}

void drawTriangle(int x, int y, int width, int height) {
    glBegin(GL_POLYGON);
    glVertex2i(x, y + (height / 2));
    glVertex2i(x + (width / 2), y - (height / 2));
    glVertex2i(x - (width / 2), y - (height / 2));
    glEnd();
}

void OutlineBox(int x, int y, int width, int height) {
    glBegin(GL_LINES);
    glVertex2i(x - (width / 2), y + (height / 2));
    glVertex2i(x + (width / 2), y + (height / 2));
    
    glVertex2i(x + (width / 2), y + (height / 2));
    glVertex2i(x + (width / 2), y - (height / 2));
    
    glVertex2i(x + (width / 2), y - (height / 2));
    glVertex2i(x - (width / 2), y - (height / 2));
    
    glVertex2i(x - (width / 2), y - (height / 2));
    glVertex2i(x - (width / 2), y + (height / 2));
    glEnd();
}

void drawCircle(int x, int y, int r) {
    double ang, inc = 3.1415926 / 50;
    
    glBegin(GL_POLYGON);
    for (ang = 0; ang < ((2.0 * 3.1425926) + inc); ang += inc) {
        glVertex2f(x + cos(ang) * r, y + sin(ang) * r);
    }
    glEnd();
}

void drawHalfCircle(int x, int y, int r) {
    double ang, inc = 3.1415926 / 50;
    
    glBegin(GL_POLYGON);
    for (ang = 0; ang <= ((1.0 * 3.1425926) + inc); ang += inc) {
        glVertex2f(x + cos(ang) * r, y + sin(ang) * r);
    }
    glEnd();
}

void DrawText(GLint x, GLint y, char *s, GLfloat r, GLfloat g, GLfloat b) {
    int lines;
    char* p;
    
    glColor3f(r,g,b);
    glRasterPos2i(x, y);
    for(p = s, lines = 0; *p; p++) {
        if (*p == '\n') {
            lines++;
            glRasterPos2i(x, y-(lines*18));
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }
}

void DrawText12(GLint x, GLint y, char *s, GLfloat r, GLfloat g, GLfloat b) {
    int lines;
    char* p;
    
    glColor3f(r,g,b);
    glRasterPos2i(x, y);
    for(p = s, lines = 0; *p; p++) {
        if (*p == '\n') {
            lines++;
            glRasterPos2i(x, y-(lines*18));
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }
}

void DrawText24(GLint x, GLint y, char *s, GLfloat r, GLfloat g, GLfloat b) {
    int lines;
    char* p;
    
    glColor3f(r,g,b);
    glRasterPos2i(x, y);
    for(p = s, lines = 0; *p; p++) {
        if (*p == '\n') {
            lines++;
            glRasterPos2i(x, y-(lines*18));
        }
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    }
}

void addMessage(char msg[1024], int _origin, int _lineHeight) {
    if (nsamps > 1)
        return;
    
    messageHistory[numMessages].createEntry(msg, _origin, _lineHeight);
    numMessages++;
}




