
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.LineNumberReader;
import java.util.Arrays;
import java.util.Scanner;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;

public class eClient extends Application {
	static String[][] userInput = new String[4][3]; //Game, Port, HostIP
	static String subjectID;
	static String hostIP;
	static int iters;


	@Override
	public void start(Stage primaryStage) {

	}

	@SuppressWarnings("resource")
	public static void main(String[] args) throws Exception {
		hostIP = args[0];
		iters = 50;//args[1];

		System.out.println("Subject ID:");
		Scanner keyboard = new Scanner(System.in);
		String inputStr = keyboard.nextLine();
		String[] splitStrings = inputStr.trim().split(" ");
        while (splitStrings.length < 1)
        {
        	inputStr = keyboard.nextLine();
        	splitStrings = inputStr.trim().split(" ");
        }
        subjectID = splitStrings[0];
		readExpInfo();

		for(int i = 0; i < 4; i++)
		{
			if(i == 0)
			{
				Process p = Runtime.getRuntime().exec("cmd /c start /wait runGame.bat" + " " + userInput[i][0] + " " + userInput[i][1] + " " + iters  + " " + userInput[i][2]  + " " + subjectID);
				p.waitFor();
			}

			if(i > 0 && i < 4)
			{
				System.out.println("Finished Game " + (i));
				System.out.println("Please complete the post-game survey and do not move to the next game until you are told so.");
				System.out.println("Ready? Enter code to move on to the next game: ");
				Scanner inputName = new Scanner(System.in);
		        String readyStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");
		        if (i == 1)
		        {
			        while (readyStr.trim().toLowerCase().equals("game2go") == false)
			        {
						System.out.println("Please enter the correct code to move on to the next game: ");
			        	readyStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");
			        }
		        }
		        if (i == 2)
		        {
			        while (readyStr.trim().toLowerCase().equals("gogame3") == false)
			        {
						System.out.println("Please enter the correct code to move on to the next game: ");
			        	readyStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");
			        }
		        }
		        if (i == 3)
		        {
			        while (readyStr.trim().toLowerCase().equals("game4run") == false)
			        {
						System.out.println("Please enter the correct code to move on to the next game: ");
			        	readyStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");
			        }
		        }

				Process p = Runtime.getRuntime().exec("cmd /c start /wait runGame.bat" + " " + userInput[i][0] + " " + userInput[i][1] + " " + iters + " " + userInput[i][2]  + " " + subjectID);
				p.waitFor();

			}

		}

		Platform.exit();

	}



	@SuppressWarnings("resource")
	public static void readExpInfo() throws Exception
	{
		String sessionInfo = "ExpInfo.csv";
		BufferedReader br = new BufferedReader(new FileReader(sessionInfo));
		String[] header = br.readLine().split(",");
		String strLine;
		String[] row;

		LineNumberReader numLines = new LineNumberReader(new FileReader(sessionInfo));
		numLines.skip(Long.MAX_VALUE);
		numLines.close();

		boolean notFound = true; //check if subjectID is found

		while(notFound)
		{
    		if((strLine = br.readLine()) != null)
    		{
    			row = strLine.split(",");
    			if (row[0].equals(subjectID))
    			{
    				for(int j = 1; j < row.length;j++)
    				{
    					//<game> <me>  <hostIP>: header[j], String[] algPort = row[j].split(" "); algPort[1]
    					String[] algPort = row[j].split(" ");
    					userInput[j-1][0] = header[j].trim(); //gameName
    					userInput[j-1][1] = algPort[1].trim(); //portNumber
    					userInput[j-1][2] = hostIP; //hostIP
    				}
					System.out.println("Starting the games ...");
    				notFound = false;
    			}
    		}
		}
	}




}
