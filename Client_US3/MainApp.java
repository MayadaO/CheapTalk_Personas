import java.io.*;
import java.net.*;
import java.util.Scanner;
import java.util.StringTokenizer;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.ComboBox;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;


public class MainApp extends Application {

	private Stage primaryStage;
	private BorderPane rootLayout;
	private static int round = 1;
	private static double avgPayoff;
	private static double money;
	String[] userInput = new String[4];
	static String hostIP;
	static String playerID;
	static String gameName;
	static String playerName;
	static String partnerName;
	static int me;
	int numActions;
	static int numRounds;
    public Socket s;
	public BufferedReader sin;
	public PrintWriter sout;
	static double Payoffs[][][] = new double[3][3][2];
    private ObservableList<SpeechActs> speechActsList = FXCollections.observableArrayList();
    private ObservableList<SpeechActs> selectedSAList = FXCollections.observableArrayList();
    private ObservableList<PlayerInfo> infoList = FXCollections.observableArrayList();
    private ComboBox<String> cmb1 = new ComboBox<String>();
    private ComboBox<String> cmb2_1 = new ComboBox<String>();
    private ComboBox<String> cmb2_2 = new ComboBox<String>();
    private ComboBox<String> cmb3 = new ComboBox<String>();
    static int numMSGs = 30;

    /**
     * Constructor
     */
    public MainApp() {

        // Add default list of Speech Acts
    	speechActsList.add(new SpeechActs(0, "Let's always play ","always",null));
    	speechActsList.add(new SpeechActs(1, "Let's alternate between ","alt1","alt2"));
    	speechActsList.add(new SpeechActs(2, "This round, let's play ","round",null));
    	speechActsList.add(new SpeechActs(3, "I accept your proposal.",null,null));
    }

	/**
     * Returns the selected action pair from the respective combobox
     * @return
     */

    public ComboBox<String> getCmb1() {
		return cmb1;
	}

    public ComboBox<String> getCmb2_1() {
		return cmb2_1;
	}

    public ComboBox<String> getCmb2_2() {
		return cmb2_2;
	}

    public ComboBox<String> getCmb3() {
		return cmb3;
	}

    public static double[][][] getPayoffs()
    {
    	return Payoffs;
    }

	public int getRound() {
		return round;
	}

	public int getNumRounds() {
		return numRounds;
	}

	public static int getNumMSGs()
	{
		return numMSGs;
	}

	public void updateNumMSGs()
	{
		numMSGs--;
	}

	public void setRound(int round) {
		MainApp.round = round;
	}

	public static double getAvgPayoff() {
		return avgPayoff;
	}

	public static void setAvgPayoff(double avgPayoff) {
		MainApp.avgPayoff = avgPayoff;
	}

	public static double getMoney() {
		return money;
	}

	public static void setMoney(double money) {
		MainApp.money = money;
	}

	public static String getPlayerName()
	{
		return playerName;
	}

	public static String getPlayerID()
	{
		return playerID;
	}

	public static int getMe()
	{
		return me;
	}
	
	public static String getPartnerName()
	{
		return partnerName;
	}

	public static String getGameName()
	{
		return gameName;
	}

	/**
     * Returns the data as an observable list of speech acts.
     * @return
     */
    public ObservableList<SpeechActs> getSA() {
        return speechActsList;
    }


    public ObservableList<SpeechActs> getMSG() {
        return selectedSAList;
    }

    public ObservableList<PlayerInfo> getInfoList() {
        return infoList;
    }
//************************************************************************************************************
	// Start the game by getting user input <game> <me> <numRounds> <hostIP>
	public void startGame()
	{
        System.out.println("Enter Your Nickname: ");
        @SuppressWarnings("resource")
		Scanner inputName = new Scanner(System.in);
        String nameStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");
        while (nameStr.trim().length() < 3)
        {
        	System.out.println("Please Re-Enter Your Nickname. It should contain at least 3 chars and must be alphanumeric.");
             nameStr = inputName.nextLine().replaceAll("[^\\w\\-_]", "");;
        }
		playerName = nameStr.trim();
		System.out.println("Hello " + playerName + ", we hope you enjoy the game ^-^ ");
	}

    public void initClient(String host) {
        int portNumber = 3000+me;
        boolean scanning = true;

        while(scanning)
        {
        	//System.out.println("Connecting to " + host + " on port "+portNumber);
        	System.out.println("Connecting to server");
            try
            {
            	s = new Socket(host, portNumber);
                scanning=false;
                System.out.println("Connected to the server, waiting for partner's connection");
            }
            catch(IOException e)
            {
                System.out.println("Connection failed, waiting and trying again ...");
                try
                {
                    Thread.sleep(3000);//3 seconds
                }
                catch(InterruptedException ie){
                    ie.printStackTrace();
                }
            }
        }

        try {
			sout = new PrintWriter(s.getOutputStream(), true);
			sin = new BufferedReader(new InputStreamReader(s.getInputStream()));
	        sout.println(playerName);
	        String goMsg = sin.readLine();
	        partnerName = goMsg;
	        System.out.println("Partner connected! Starting the game!");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

    }

    public void readGame(String _game) {
        int i, j;
        String pair;
        String gameFile = "games/" + _game + ".txt";
        if ((gameFile.toLowerCase().contains("shapleys".toLowerCase())) || (gameFile.toLowerCase().contains("blocks".toLowerCase())))
        {numActions = 3;}
        else
        {numActions = 2;}

        try {
            @SuppressWarnings("resource")
			BufferedReader br = new BufferedReader(new FileReader(gameFile));
            for (i = 0; i < numActions; i++) {
                for (j = 0; j < numActions; j++) {
                    pair = br.readLine();
                    StringTokenizer st = new StringTokenizer(pair);
                    Payoffs[j][i][0] = Double.parseDouble(st.nextToken());
                    Payoffs[j][i][1] = Double.parseDouble(st.nextToken());
                }
            }
            //System.out.println(Arrays.deepToString(Payoffs));
        } catch (IOException e) {
            System.err.println("Caught IOException: " + e.getMessage());
        }

    }

	@Override
	public void start(Stage primaryStage) {
		startGame();
		initClient(hostIP);
		readGame(gameName);
        this.primaryStage = primaryStage;
        this.primaryStage.initStyle(StageStyle.UNDECORATED);
        this.primaryStage.setTitle("User Study X");
        //maximize stage
        primaryStage.setMaximized(true);

        initRootLayout();
        showGameLayout();
	}

    /**
     * Initializes the root layout.
     */
    public void initRootLayout() {
        try {
            // Load root layout from fxml file.
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();

            // Show the scene containing the root layout.
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Shows the game layout inside the root layout.
     */
    public void showGameLayout() {
    	if (numActions == 2)
    	{
            try {
                // Load the game layout.
                FXMLLoader loader = new FXMLLoader();
                if (me == 0) //player 1
                {
                	loader.setLocation(MainApp.class.getResource("GameLayout.fxml"));
                }
                else if (me == 1) //player 2
                {
                	loader.setLocation(MainApp.class.getResource("GameLayoutC.fxml"));
                }
                
                AnchorPane gameLayout = (AnchorPane) loader.load();

                // Set the game layout into the center of root layout.
                rootLayout.setCenter(gameLayout);

                // Give the controller access to the main app.
            	GameController controller = loader.getController();
            	controller.setMainApp(this);
                }
            catch (IOException e) {
                e.printStackTrace();
                }
    	}
    	else if (numActions == 3)
    	{
            try {
                // Load the game layout.
                FXMLLoader loader = new FXMLLoader();
                if (me == 0) //player 1
                {
                	loader.setLocation(MainApp.class.getResource("GameLayout2.fxml"));
                }
                else if (me == 1) //player 2
                {
                	loader.setLocation(MainApp.class.getResource("GameLayout2C.fxml"));
                }

                AnchorPane gameLayout = (AnchorPane) loader.load();

                // Set the game layout into the center of root layout.
                rootLayout.setCenter(gameLayout);

                // Give the controller access to the main app.
            	GameController2 controller = loader.getController();
            	controller.setMainApp(this);
                }
            catch (IOException e) {
                e.printStackTrace();
                }
    	}
    }


    /**
     * Opens a dialog to add a new speechAct. If the user
     * clicks OK, a new speechAct is saved into the speechAct list and true
     * is returned.
     *
     * @param speechAct the speechAct object to be customized
     * @return true if the user clicked OK, false otherwise.
     */
    public boolean showSAEditDialog(SpeechActs speechAct) {
        try {
            // Load the fxml file and create a new stage for the popup dialog.
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("CustomizingDialog.fxml"));
            AnchorPane page = (AnchorPane) loader.load();

            // Create the dialog Stage.
            Stage dialogStage = new Stage();
            dialogStage.setTitle("Add a New Message");
            dialogStage.initModality(Modality.WINDOW_MODAL);
            dialogStage.initOwner(primaryStage);
            Scene scene = new Scene(page);
            dialogStage.setScene(scene);

            // Set the speechAct into the controller.
            CDialogController controller = loader.getController();
            controller.setDialogStage(dialogStage);
            controller.setSpeechAct(speechAct);

            // Show the dialog and wait until the user closes it
            dialogStage.showAndWait();

            return controller.isOkClicked();
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * Returns the main stage.
     * @return
     */
    public Stage getPrimaryStage() {
        return primaryStage;
    }


	public static void main(String[] args) {
		gameName = args[0];
		me = Integer.parseInt(args[1]);
		numRounds = Integer.parseInt(args[2]);
		hostIP = args[3];
		playerID = args[4];
		String game = "";
        if ((gameName.toLowerCase().contains("prisoners".toLowerCase())))
        {game = "1";}
        else if ((gameName.toLowerCase().contains("chicken".toLowerCase())))
        {game = "2";}
        else if ((gameName.toLowerCase().contains("blocks".toLowerCase())))
        {game = "3";}
        else if ((gameName.toLowerCase().contains("endless".toLowerCase())))
        {game = "4";}
		System.out.println("Game " + game +" :");
		launch(args);
	}
}
