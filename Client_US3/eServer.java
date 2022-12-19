import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.LineNumberReader;

public class eServer {
	static String[][] userInput = new String[4][4]; //GameName, Alg, Port, hostIP
	static String hostIP;
	static int iters = 53;
	static FileWriter writer;


	public static void main(String[] args) throws Exception {
		hostIP = args[0];//10.102.15.44 //"192.168.56.101";
		iters = 50;//Integer.parseInt(args[1]);
		System.out.println("Reading Exp. Info and Writing Scripts ...");
		writeScripts();
		System.out.println("Done!");

	}

	@SuppressWarnings("resource")
	public static void writeScripts() throws Exception
	{
		String sessionInfo = "ExpInfo.csv";
		BufferedReader br = new BufferedReader(new FileReader(sessionInfo));
		String[] header = br.readLine().split(","); // the names of the four games [prisoners,chicken,blocks,endless]
		String strLine;
		String[] row;

		LineNumberReader numLines = new LineNumberReader(new FileReader(sessionInfo));
		numLines.skip(Long.MAX_VALUE);
		numLines.close();

        for (int playerID = 0; (playerID < numLines.getLineNumber()-1); playerID ++)
		{
        	writer = new FileWriter("Scripts/"+ (playerID + 1) + "_script.sh");
        	writer.append("#!/bin/bash \n");
    		if((strLine = br.readLine()) != null)
    		{
    			row = strLine.split(",");
    			for(int j = 1; j < row.length;j++)
    			{
    				//<game> <me>  <hostIP>: header[j], String[] algPort = row[j].split(" "); algPort[1]
    				String[] algPort = row[j].split(" ");
    				String game = header[j].trim().toLowerCase(); //gameName
    				String alg = algPort[0].trim(); //alg
    				String port = algPort[1].trim(); //portNumber

    				writer.append("cd ..\n");
    				writer.append("cd Server \n");
    				writer.append("./CheapTalk " + game + " " + iters + " " + port + " cheaptalk & \n");
    				writer.append("cd ..\n");
    				writer.append("cd Algs \n");
    				writer.append("./Solver " + game + " " + iters + " " + alg + " 1 " + hostIP + " " + port + " \n");
    				if(j < 4)
    				{
        				writer.append("echo Press enter to proceed to Game " + (j + 1) + " \n");
        				writer.append("read next \n");
    				}
    			}
    		}

            writer.flush();
            writer.close();
        }
	}


}
