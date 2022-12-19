#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fstream>
#include <strings.h>
#include <string>
#include <pthread.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

// some old constants
#define NUMACTIONS	5
#define STAY		0
#define SENTINAL	-999999
#define MAX_MOVES_PER_ROUND		50

#define NONE    -1
#define UPP     0
#define DOWNN   1
#define RIGHTT  2
#define LEFTT   3

#define CONNECT_PORT    3000
/*
#define LETS_COOP           0
#define TAKE_TURNS          1
#define SPECIFY             2
#define IACCEPT             3   // differentiate between recent proposals and past ones
#define UNACCEPTABLE_DTRUST 4
#define UNACCEPTABLE_NFAIR  5
#define CHANGE_O_HEART      6   // should 6 and 7 be mutually exclusive?
#define DOBETTER            7
#define CURSE_YOU           8   // 8 and 9 are the same
#define BETRAYED            9
#define YOULL_PAY           10
#define INYOURFACE          11
#define FORGIVENESS         12  // 12 and 13 are the same
#define FORGIVENESS2        13
#define EXCELLENT           14  // 14 and 15 are the same
#define EXCELLENT2          15
#define SWEET               16
#define GREETING            17
#define THREAT              18
*/
#define LETS_COOP           0
#define TAKE_TURNS          1
#define SPECIFY             2
#define IACCEPT             3   // differentiate between recent proposals and past ones
#define UNACCEPTABLE_DTRUST 4
#define UNACCEPTABLE_NFAIR  5
#define CHANGE_O_HEART      6   // should 6 and 7 be mutually exclusive?
#define DOBETTER            7
#define CURSES              8
#define YOULL_PAY           9
#define INYOURFACE          10
#define FORGIVENESS         11
#define EXCELLENT           12
#define THREAT              13
#define GREETING            14


#endif