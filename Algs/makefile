PROGRAM = Solver
cpp_FILES = main.cpp MarkovGame.cpp State.cpp GameEngine.cpp MatrixEngine.cpp MazeEngine.cpp GridWarEngine.cpp LEGOEngine.cpp PhysicsRule.cpp minimaxLog.cpp Qset.cpp mega.cpp Solution.cpp Expert.cpp Player.cpp Xprt.cpp jefe++.cpp expertLearner.cpp a.cpp Exp3.cpp eee.cpp gameWriter.cpp Preventer.cpp rndmPlyr.cpp Node.cpp CFR.cpp MySocket.cpp client_socket.cpp CommAgent.cpp Bouncer.cpp Br.cpp Leader.cpp Follower.cpp RL.cpp stageGame.cpp ctModel.cpp personality.cpp
c_FILES =
CC = g++

CFLAGS = -O -g
LIBs = 

OBJECTS = $(c_FILES:.c=.o) $(cpp_FILES:.cpp=.o)

.SUFFIXES: .o .cpp .c

.cpp.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES) -Wno-deprecated

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) $< -c $(INCLUDES)

$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $(INCLUDES) $(OBJECTS) $(LIBS)

clean:
	rm *.o

rebuild: clean
	make
