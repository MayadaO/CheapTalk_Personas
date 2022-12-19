# CheapTalk_Personas
This is the code we wrote for our paper titled "How AI Wins Friends and Influences People"
If you use this code in any way, please cite our paper:

(M. Oudah, T. Rahwan, T. Crandall, and J. W. Crandall. “How AI Wins Friends and Influences People”. In proceedings of the 32nd AAAI Conference on Artificial Intelligence, Louisiana, 2018.)
-----------------------------------------------------------------------------------

To run a single game, you’ll need to run 3 programs.  The server and two clients (player 0 and player 1). 

1.  Compile the code in “Server.”  You can do this by going to the “Server” directory in a terminal, and typing “make”.  Next, you run the server by typing the following into a terminal:

./CheapTalk chicken2 50 0 cheaptalk

The first parameter (after the program name) specifies the game.  Here, I’ve specified the same “chicken2”.  You can change it to a different game if you want (look in the games directory for the games that are already created.  The second parameter specifies the number of rounds in the game (I’ve used 50 rounds in this example).  For now, you can just leave the other parameters as they are (no need to change them).

2.  After starting the server, you now need to start two clients.  In this case, we’ll make a human player the first player and a bot the second player.

A.  To start the human client, go to the directory “Client_US3” in a new terminal window or tab.  You may need to compile the java code on your computer.  To do so, type: javac *.java in the terminal.  Once you do that, you can run the client as follows:

java MainApp chicken2 0 50 localhost 0

As with the server, the parameter “chicken2” specifies the game.  You should change this parameter to match the game specified when you started the server.  The parameter after that one is the player number (0 or 1), and the parameter after that specifies the number of rounds (should match what you did when starting the server.  

Once you run the human client, the program will prompt you to enter a nickname — enter anything you want and press return.

Note, there’s a line of code that can be changed to make this GUI run better on a Mac.  I’ll have to find that and alert you of it.

B.  Next we need to start the machine client.  You do this by going to the directory named “Algs” in a new terminal window or tab.  Next, compile the code using the command “make”.  Now you can start the code by typing the following.

./Solver chicken2 50 S#-Trump 1 localhost 0

The first parameter after the program name specifies the game.  Make it match what you entered when you started the server.  The next parameter is the number of rounds in the game, which should also match what you entered when you started to the server.  The next parameter specifies the behavioral algorithm and signaling strategy (persona).  Here, I’ve specified the behavioral algorithm S# and the persona “Trump”.  You can change these to get different algorithms and personalities.  The other currently available behavioral strategy is EEE#.  After the dash “-“ comes the persona name.  Here I’ve specified Trump.  You can change this to anything in the “Personalities” folder.  Note the following mappings to the paper you may have read:

Dale=Carnegie
Trump=Biff
Silent=Thumper
Frank=Spock
