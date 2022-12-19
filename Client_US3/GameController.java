import java.io.IOException;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;
import javafx.animation.KeyFrame;
import javafx.animation.PauseTransition;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.input.ClipboardContent;
import javafx.scene.input.DataFormat;
import javafx.scene.input.Dragboard;
import javafx.scene.input.TransferMode;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Control;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;
import javafx.scene.text.Text;
import javafx.util.Callback;
import javafx.util.Duration;


public class GameController {

	// Table of default messages/speech acts and its 3 columns
    @FXML
    private TableView<SpeechActs> defaultSATable;
    @FXML
    private TableColumn<SpeechActs, String> defaultTextColumn;
    @FXML
    private TableColumn<SpeechActs, String> actionColumn1;
    @FXML
    private TableColumn<SpeechActs, String> actionColumn2;

	// Table of selected speech acts
    @FXML
    private TableView<SpeechActs> selectedSATable;
    @FXML
    private TableColumn<SpeechActs, String> selectedSAColumn;

    // Chatting Table
    @FXML
    private TableView<PlayerInfo> chattingTable;
    @FXML
    private TableColumn<PlayerInfo, String> yourChatCol;
    @FXML
    private TableColumn<PlayerInfo, String> partnerChatCol;
    @FXML
    private TableColumn<PlayerInfo, String> jointActionCol;
    @FXML
    private TableColumn<PlayerInfo, Number> yourPayoffCol;

	// Labels for possible actions in the GUI for the row player
	@FXML
	private Label row00;
	@FXML
	private Label row01;
	@FXML
	private Label row10;
	@FXML
	private Label row11;
	// Labels for possible actions in the GUI for the col player
	@FXML
	private Label col00;
	@FXML
	private Label col01;
	@FXML
	private Label col10;
	@FXML
	private Label col11;
	@FXML
	private Label avgPayoff;
	@FXML
	private Label money;

	// reference to "send/don't send messages" button
    @FXML
    private Button sendMSG;

	// reference to the user's action buttons
    @FXML
    private Button ActionA;
    @FXML
    private Button ActionB;

	// anchorpanes which contain the 4 main parts of the GUI
	@FXML
	private AnchorPane selectAction;
	@FXML
	private AnchorPane selectSA;
	@FXML
	private AnchorPane sendSA;
	@FXML
	private AnchorPane chattingWindow;
	// defining actions pane
	@FXML
	private AnchorPane JA00;
	@FXML
	private AnchorPane JA01;
	@FXML
	private AnchorPane JA10;
	@FXML
	private AnchorPane JA11;

	@FXML
	private ImageView gameOver;
	@FXML
	private ImageView waitPartnerAction;
	@FXML
	private ImageView waitPartnerMSG;
	@FXML
	private ImageView playerPercentile;
	@FXML
	private Label playerPercentileLabel;
	@FXML
	private ImageView totalMoney;
	@FXML
	private Label totalMoneyLabel;

    // Reference to the main application.
    private MainApp mainApp;
    int roundIndex = 0;
    int currentRound = 1;
    long msgStartTime;
    long actStartTime;
    long msgEndTime;
    long actEndTime;
    double moneyEarned = 0;
    double pointValue;
    String perc = "";
	final PauseTransition waitPartnerCT = new PauseTransition(Duration.seconds(0.1));
	final PauseTransition waitJointAction = new PauseTransition(Duration.seconds(0.1));
	final PauseTransition sayPartnerCT = new PauseTransition(Duration.seconds(0.5));
    List<String> chatTokens = new ArrayList<String>();
    String chatString = "";
    private final DataFormat SERIALIZED_MIME_TYPE = new DataFormat("application/x-java-serialized-object");
    FileWriter writer;
    Process p;

	public GameController() throws Exception
	{
	    //writer to export data to txt file
	    writer = new FileWriter("Logs/" + MainApp.getPlayerID() + "_" + MainApp.getGameName() +"_"+ MainApp.getPlayerName() +"_"+ MainApp.getPartnerName() +".txt");
	    // columns header
		writer.append("Round \t GameName \t SubjectID \t PlayerName \t PartnerName \t PlayerTimeToSendSA \t PlayerTimeToAct \t PlayerSpeechAct \t PartnerSpeechAct \t PlayerAction \t PartnerAction \t PlayerPayoff \t PartnerPayoff \t MoneyEarned \t PlayerListOfSAs\n");

		if(MainApp.getGameName().toLowerCase().contains("prisoners"))
		{
			pointValue = (24.0 / (50.0 * 60.0)) / 4.0;
		}
		else if(MainApp.getGameName().toLowerCase().contains("chicken"))
		{
			pointValue = (24.0 / (50.0 * 84.0)) / 4.0;
		}
		else if(MainApp.getGameName().toLowerCase().contains("blocks"))
		{
			pointValue = (24.0 / (50.0 * 70.0)) / 4.0;
		}
		else if(MainApp.getGameName().toLowerCase().contains("endless"))
		{
			pointValue = (24.0 / (50.0 * 67.0)) / 4.0;
		}
	}


    @FXML
    private void initialize()
    {
    	selectSA.setDisable(false);
    	sendSA.setDisable(false);
    	ActionA.setDisable(true);
  	    ActionB.setDisable(true);
    	changeColors();
    	//*****************************************************
        //initialize the payoff matrix of the game
    	//*****************************************************
    	row00.setText(Integer.toString((int)MainApp.getPayoffs()[0][0][0]));
    	row01.setText(Integer.toString((int)MainApp.getPayoffs()[0][1][0]));
    	row10.setText(Integer.toString((int)MainApp.getPayoffs()[1][0][0]));
    	row11.setText(Integer.toString((int)MainApp.getPayoffs()[1][1][0]));
		col00.setText(Integer.toString((int)MainApp.getPayoffs()[0][0][1]));
		col01.setText(Integer.toString((int)MainApp.getPayoffs()[0][1][1]));
		col10.setText(Integer.toString((int)MainApp.getPayoffs()[1][0][1]));
		col11.setText(Integer.toString((int)MainApp.getPayoffs()[1][1][1]));

		//*****************************************************
		//define functions to be processed with some delay
		//*****************************************************
		waitPartnerCT.setOnFinished( ( ActionEvent event ) -> {
			sendMyChat();
			displayPartnerCheapTalk();
		});
		sayPartnerCT.setOnFinished( ( ActionEvent event ) -> {
			sayPartnerCheapTalk();
		});
		waitJointAction.setOnFinished( ( ActionEvent event ) -> {
			readResult();
		});


        //*****************************************************
        //define the columns of the selecting speach acts table
        //*****************************************************

		defaultTextColumn.setCellValueFactory(cellData -> cellData.getValue().textProperty());
		defaultTextColumn.setCellFactory(new Callback<TableColumn<SpeechActs, String>, TableCell<SpeechActs, String>>() {


	        @Override
	        public TableCell<SpeechActs, String> call(
	            TableColumn<SpeechActs, String> param) {
	            TableCell<SpeechActs, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(defaultTextColumn.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            return cell ;
	        }

	    });

		actionColumn1.setCellValueFactory(cellData -> cellData.getValue().actionProperty1());
		actionColumn1.setCellFactory(column -> {
		    return new TableCell<SpeechActs, String>() {
		        @Override
		        protected void updateItem(String item, boolean empty) {
		            super.updateItem(item, empty);
		            if (item == null)
		            {setGraphic(null);}
		            else if (item == "always")
		            {
		            	ComboBox<String> cb1 = new ComboBox<String>();
		            	cb1.setPromptText("...");
		            	cb1.getItems().addAll("A-X","A-Y","B-X","B-Y");
		            	setGraphic(cb1);
		                cb1.valueProperty().addListener(new ChangeListener<String>() {
		                    @Override public void changed(ObservableValue<? extends String> ov, String t, String t1) {
		                        mainApp.getCmb1().setValue(t1);
		                        defaultSATable.getSelectionModel().select(defaultSATable.getItems().get(0));
		                    }
		                });

		            }

		            else if (item == "alt1")
		            {
		            	ComboBox<String> cb1 = new ComboBox<String>();
		            	cb1.setPromptText("...");
		            	cb1.getItems().addAll("A-X","A-Y","B-X","B-Y");
		            	setGraphic(cb1);
		                cb1.valueProperty().addListener(new ChangeListener<String>() {
		                    @Override public void changed(ObservableValue<? extends String> ov, String t, String t1) {
		                        mainApp.getCmb2_1().setValue(t1);
		                        defaultSATable.getSelectionModel().select(defaultSATable.getItems().get(1));
		                    }
		                });

		            }

		            else if (item == "round")
		            {
		            	ComboBox<String> cb1 = new ComboBox<String>();
		            	cb1.setPromptText("...");
		            	cb1.getItems().addAll("A-X","A-Y","B-X","B-Y");
		            	setGraphic(cb1);
		                cb1.valueProperty().addListener(new ChangeListener<String>() {
		                    @Override public void changed(ObservableValue<? extends String> ov, String t, String t1) {
		                        mainApp.getCmb3().setValue(t1);
		                        defaultSATable.getSelectionModel().select(defaultSATable.getItems().get(2));
		                    }
		                });

		            }

		            }};});

		actionColumn2.setCellValueFactory(cellData -> cellData.getValue().actionProperty2());
		actionColumn2.setCellFactory(column -> {
	    return new TableCell<SpeechActs, String>() {
	        @Override
	        protected void updateItem(String item, boolean empty) {
	            super.updateItem(item, empty);
	            if (item == null)
	            {setGraphic(null);}
	            else
	            {
	            	ComboBox<String> cb2 = new ComboBox<String>();
	            	cb2.setPromptText("...");
	            	cb2.getItems().addAll("A-X","A-Y","B-X","B-Y");
	            	setGraphic(cb2);
	                cb2.valueProperty().addListener(new ChangeListener<String>() {
	                    @Override public void changed(ObservableValue<? extends String> ov, String t, String t1) {
	                        mainApp.getCmb2_2().setValue(t1);
	                        defaultSATable.getSelectionModel().select(defaultSATable.getItems().get(1));
	                    }
	                });

	            }

	            }};});

        //*********************************************************
        //define the columns of the Selected Messages window table*
        //*********************************************************


        selectedSAColumn.setCellValueFactory(cellData -> cellData.getValue().textProperty());
        selectedSAColumn.setCellFactory(new Callback<TableColumn<SpeechActs, String>, TableCell<SpeechActs, String>>() {
	        @Override
	        public TableCell<SpeechActs, String> call(
	            TableColumn<SpeechActs, String> param) {
	            TableCell<SpeechActs, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(selectedSAColumn.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            return cell ;
	        }

	    });


		// drag/drop functionality for the "selected messages" window


        selectedSATable.setRowFactory(tv -> {
            TableRow<SpeechActs> row = new TableRow<>();

            row.setOnDragDetected(event -> {
                if (! row.isEmpty()) {
                    Integer index = row.getIndex();
                    Dragboard db = row.startDragAndDrop(TransferMode.MOVE);
                    db.setDragView(row.snapshot(null, null));
                    ClipboardContent cc = new ClipboardContent();
                    cc.put(SERIALIZED_MIME_TYPE, index);
                    db.setContent(cc);
                    event.consume();
                }
            });

            row.setOnDragOver(event -> {
                Dragboard db = event.getDragboard();
                if (db.hasContent(SERIALIZED_MIME_TYPE)) {
                    if (row.getIndex() != ((Integer)db.getContent(SERIALIZED_MIME_TYPE)).intValue()) {
                        event.acceptTransferModes(TransferMode.COPY_OR_MOVE);
                        event.consume();
                    }
                }
            });

            row.setOnDragDropped(event -> {
                Dragboard db = event.getDragboard();
                if (db.hasContent(SERIALIZED_MIME_TYPE)) {
                    int draggedIndex = (Integer) db.getContent(SERIALIZED_MIME_TYPE);
                    SpeechActs draggedPerson = selectedSATable.getItems().remove(draggedIndex);

                    int dropIndex ;

                    if (row.isEmpty()) {
                        dropIndex = selectedSATable.getItems().size() ;
                    } else {
                        dropIndex = row.getIndex();
                    }

                    selectedSATable.getItems().add(dropIndex, draggedPerson);

                    event.setDropCompleted(true);
                    selectedSATable.getSelectionModel().select(dropIndex);
                    event.consume();
                }
            });

            return row ;
        });

		// Selection through double clicking in the "list of messages" window

        defaultSATable.setRowFactory(tv -> {
            TableRow<SpeechActs> row = new TableRow<>();

            row.setOnMouseClicked(event -> {
            	if (mainApp.getMSG().size() < 10) //can only select 10 msgs per round
            	{
                    if (event.getClickCount() == 2 && (! row.isEmpty()))
                    {
                    	SpeechActs SA = row.getItem();
                        System.out.println(SA.getText());

                        if (SA != null) {
                        	SpeechActs selectedMSG = new SpeechActs(SA.getCode(),SA.getText(),SA.getActionPlan1(),SA.getActionPlan2());
                        	if(selectedMSG.getCode() == 0)
                        	{
                        		selectedMSG.setAction1(mainApp.getCmb1().getValue());
                        		if(selectedMSG.getActionPlan1() != null)
                        		{
                        			selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + ". ");
                        			mainApp.getMSG().add(selectedMSG);
                        			selectedSATable.setItems(mainApp.getMSG());
                        		}
                                else {
                                    // No action selected.
                                    Alert alert = new Alert(AlertType.WARNING);
                                    alert.initOwner(mainApp.getPrimaryStage());
                                    alert.setTitle("No Action Value Selection");
                                    alert.setHeaderText("No Action Selected");
                                    alert.setContentText("Please select an action to send.");

                                    alert.showAndWait();
                                }

                        	}
                        	else if(selectedMSG.getCode() == 1)
                        	{
                        		selectedMSG.setAction1(mainApp.getCmb2_1().getValue());
                        		selectedMSG.setAction2(mainApp.getCmb2_2().getValue());
                        		if((selectedMSG.getActionPlan1() != null) && (selectedMSG.getActionPlan2() != null))
                        		{
                        			selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + " and " + selectedMSG.getActionPlan2() + ". ");
                                	mainApp.getMSG().add(selectedMSG);
                                	selectedSATable.setItems(mainApp.getMSG());
                        		}
                                else {
                                    // No action selected.
                                    Alert alert = new Alert(AlertType.WARNING);
                                    alert.initOwner(mainApp.getPrimaryStage());
                                    alert.setTitle("No Action Value Selection");
                                    alert.setHeaderText("No Action Selected");
                                    alert.setContentText("Please select an action to send.");

                                    alert.showAndWait();
                                }


                        	}
                        	else if(selectedMSG.getCode() == 2)
                        	{
                        		selectedMSG.setAction1(mainApp.getCmb3().getValue());
                        		if(selectedMSG.getActionPlan1() != null)
                        		{
                            		selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + ". ");
                            		mainApp.getMSG().add(selectedMSG);
                                	selectedSATable.setItems(mainApp.getMSG());
                        		}
                                else {
                                    // No action selected.
                                    Alert alert = new Alert(AlertType.WARNING);
                                    alert.initOwner(mainApp.getPrimaryStage());
                                    alert.setTitle("No Action Value Selection");
                                    alert.setHeaderText("No Action Selected");
                                    alert.setContentText("Please select an action to send.");

                                    alert.showAndWait();
                                }

                        	}
                        	else
                        	{
                        		if ((selectedMSG.getText().trim().endsWith(".") == false)&&(selectedMSG.getText().trim().endsWith("?") == false)&&(selectedMSG.getText().trim().endsWith("!") == false))
                        		{
                        			selectedMSG.setText(selectedMSG.getText() + ". ");
                        		}
                        		else
                        		{
                        			selectedMSG.setText(selectedMSG.getText() + " ");
                        		}
                            	mainApp.getMSG().add(selectedMSG);

                                //*********************************************************
                                //scroll down to the recently added item
                                //*********************************************************
                        		selectedSATable.scrollTo(selectedSATable.getItems().size()-1);
                        	}}
                        else {
                            // Nothing selected.
                            Alert alert = new Alert(AlertType.WARNING);
                            alert.initOwner(mainApp.getPrimaryStage());
                            alert.setTitle("No Selection");
                            alert.setHeaderText("No Message Selected");
                            alert.setContentText("Please select a message to send.");

                            alert.showAndWait();
                        }

                        // change the text of the "send/don't send msgs" button in the "selected messages" window.
                        sendButtonText();
                	}
            	}
                else
                {
                    // Nothing selected.
                    Alert alert = new Alert(AlertType.WARNING);
                    alert.initOwner(mainApp.getPrimaryStage());
                    alert.setTitle("Maximum Number of Selected Messages");
                    alert.setHeaderText("Reached Maximum Number of Selected Messages");
                    alert.setContentText("You can't send more than 10 messages! Remove some if you want to add more.");
                    alert.showAndWait();
                }

            });

            return row ;
        });


        //*****************************************************
        //define the columns of the chatting window table
        //*****************************************************

    	yourPayoffCol.setCellValueFactory(cellData -> cellData.getValue().myPayoffProperty());
    	yourPayoffCol.setStyle( "-fx-alignment: CENTER;");
    	yourPayoffCol.setCellFactory(column -> {
            return new TableCell<PlayerInfo, Number>() {
                @Override
                protected void updateItem(Number item, boolean empty) {
                    super.updateItem(item, empty);
                    if (item != null)
                    {
                        if(String.valueOf(item) == "NaN")
                        {
                        		setText(null);
                        }
                        else
                        {
                        	setText(String.valueOf(item));
                        }
                    }
                    else
                    {
                    	setText(null);
                    }


                }};});


    	jointActionCol.setCellValueFactory(cellData -> cellData.getValue().jointActionProperty());
    	jointActionCol.setStyle( "-fx-alignment: CENTER;");
    	jointActionCol.setCellFactory(column -> {
            return new TableCell<PlayerInfo, String>() {
                @Override
                protected void updateItem(String item, boolean empty) {
                    super.updateItem(item, empty);
                    if (item != null)
                    {
                        if(item.equals("00"))
                        {setText("A-X");}
                        else if(item.equals("01"))
                        {setText("A-Y");}
                        else if(item.equals("10"))
                        {setText("B-X");}
                        else if(item.equals("11"))
                        {setText("B-Y");}
                    }
                    else
                    {
                    	setText(null);
                    }
                    }};});



    	yourChatCol.setCellValueFactory(cellData -> cellData.getValue().myChatProperty());
    	yourChatCol.setCellFactory(new Callback<TableColumn<PlayerInfo, String>, TableCell<PlayerInfo, String>>() {

	        @Override
	        public TableCell<PlayerInfo, String> call(
	            TableColumn<PlayerInfo, String> param) {
	            TableCell<PlayerInfo, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(yourChatCol.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            if (MainApp.getMe() == 0)
	            {
	            	text.setStyle("-fx-fill: midnightblue");
	            }
	            else if (MainApp.getMe() == 1)
	            {
	            	text.setStyle("-fx-fill: #b2490c");
	            }
	            return cell ;
	        }
	    });

    	partnerChatCol.setCellValueFactory(cellData -> cellData.getValue().partnerChatProperty());
    	partnerChatCol.setCellFactory(new Callback<TableColumn<PlayerInfo, String>, TableCell<PlayerInfo, String>>() {

	        @Override
	        public TableCell<PlayerInfo, String> call(
	            TableColumn<PlayerInfo, String> param) {
	            TableCell<PlayerInfo, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(partnerChatCol.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            if (MainApp.getMe() == 0)
	            {
	            	text.setStyle("-fx-fill: #b2490c");
	            }
	            else if (MainApp.getMe() == 1)
	            {
	            	text.setStyle("-fx-fill: midnightblue");
	            }

	            return cell ;
	        }

	    });

    	msgStartTime = System.currentTimeMillis();

    }


    private void changeColors() {
    	if(selectSA.isDisabled())
    	{
    		selectSA.setStyle("-fx-background-color: f4f4f4");
    	}
    	else
    	{
    		selectSA.setStyle("-fx-background-color: peachpuff");
    	}
    	if(sendSA.isDisabled())
    	{
    		sendSA.setStyle("-fx-background-color: f4f4f4");
    	}
    	else
    	{
    		sendSA.setStyle("-fx-background-color: peachpuff");
    	}
    	if(ActionA.isDisabled() && ActionB.isDisabled())
    	{
    		selectAction.setStyle("-fx-background-color: f4f4f4");
    	}
    	else
    	{
    		selectAction.setStyle("-fx-background-color: peachpuff");
    	}
	}


	/**
     * Is called by the main application to give a reference back to itself.
     *
     * @param mainApp
     */
    public void setMainApp(MainApp mainApp) {
        this.mainApp = mainApp;
        // Add speachacts list to the speachact table
        defaultSATable.setItems(mainApp.getSA());
    	// Add selected msg texts to selected msgs table
    	selectedSATable.setItems(mainApp.getMSG());
        // Add chat information to chatting table
    	chattingTable.setItems(mainApp.getInfoList());

    }


//****************************************************************************************************
//-------------------------------------------------------------- Select Cheap Talk [Select SA Window]*
//****************************************************************************************************
    /**
     * Called when the user clicks the "select message" button under the default msg table.
     */
    @FXML
    private void handleSelectSA() {
    	//can only select 10 msgs per round
    	if(mainApp.getMSG().size() < 10)
    	{
            SpeechActs SA = defaultSATable.getSelectionModel().getSelectedItem();
            if (SA != null) {
            	SpeechActs selectedMSG = new SpeechActs(SA.getCode(),SA.getText(),SA.getActionPlan1(),SA.getActionPlan2());
            	if(selectedMSG.getCode() == 0)
            	{
            		selectedMSG.setAction1(mainApp.getCmb1().getValue());
            		if(selectedMSG.getActionPlan1() != null)
            		{
            			selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + ". ");
            			mainApp.getMSG().add(selectedMSG);
            			selectedSATable.setItems(mainApp.getMSG());
            		}
                    else {
                        // No action selected.
                        Alert alert = new Alert(AlertType.WARNING);
                        alert.initOwner(mainApp.getPrimaryStage());
                        alert.setTitle("No Action Value Selection");
                        alert.setHeaderText("No Action Selected");
                        alert.setContentText("Please select an action to send.");

                        alert.showAndWait();
                    }

            	}
            	else if(selectedMSG.getCode() == 1)
            	{
            		selectedMSG.setAction1(mainApp.getCmb2_1().getValue());
            		selectedMSG.setAction2(mainApp.getCmb2_2().getValue());
            		if((selectedMSG.getActionPlan1() != null) && (selectedMSG.getActionPlan2() != null))
            		{
            			selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + " and " + selectedMSG.getActionPlan2() + ". ");
                    	mainApp.getMSG().add(selectedMSG);
                    	selectedSATable.setItems(mainApp.getMSG());
            		}
                    else {
                        // No action selected.
                        Alert alert = new Alert(AlertType.WARNING);
                        alert.initOwner(mainApp.getPrimaryStage());
                        alert.setTitle("No Action Value Selection");
                        alert.setHeaderText("No Action Selected");
                        alert.setContentText("Please select an action to send.");

                        alert.showAndWait();
                    }


            	}
            	else if(selectedMSG.getCode() == 2)
            	{
            		selectedMSG.setAction1(mainApp.getCmb3().getValue());
            		if(selectedMSG.getActionPlan1() != null)
            		{
                		selectedMSG.setText(selectedMSG.getText() + selectedMSG.getActionPlan1() + ". ");
                		mainApp.getMSG().add(selectedMSG);
                    	selectedSATable.setItems(mainApp.getMSG());
            		}
                    else {
                        // No action selected.
                        Alert alert = new Alert(AlertType.WARNING);
                        alert.initOwner(mainApp.getPrimaryStage());
                        alert.setTitle("No Action Value Selection");
                        alert.setHeaderText("No Action Selected");
                        alert.setContentText("Please select an action to send.");

                        alert.showAndWait();
                    }

            	}
            	else
            	{
            		if ((selectedMSG.getText().trim().endsWith(".") == false)&&(selectedMSG.getText().trim().endsWith("?") == false)&&(selectedMSG.getText().trim().endsWith("!") == false))
            		{
            			selectedMSG.setText(selectedMSG.getText() + ". ");
            		}
            		else
            		{
            			selectedMSG.setText(selectedMSG.getText() + " ");
            		}
                	mainApp.getMSG().add(selectedMSG);

                    //*********************************************************
                    //scroll down to the recently added item
                    //*********************************************************
            		selectedSATable.scrollTo(selectedSATable.getItems().size()-1);
            	}}
            else {
                // Nothing selected.
                Alert alert = new Alert(AlertType.WARNING);
                alert.initOwner(mainApp.getPrimaryStage());
                alert.setTitle("No Selection");
                alert.setHeaderText("No Message Selected");
                alert.setContentText("Please select a message to send.");

                alert.showAndWait();
            }

            // change the text of the "send/don't send msgs" button in the "selected messages" window.
            sendButtonText();
    	}
        else {
            // Nothing selected.
            Alert alert = new Alert(AlertType.WARNING);
            alert.initOwner(mainApp.getPrimaryStage());
            alert.setTitle("Maximum Number of Selected Messages");
            alert.setHeaderText("Reached Maximum Number of Selected Messages");
            alert.setContentText("You can't send more than 10 messages! Remove some if you want to add more.");
            alert.showAndWait();
        }


    }

    /**
     * Called when the user clicks the "create a new msg" button. Opens a dialog to type in the text of the new msg
     */
    @FXML
    private void handleNewSA() {
        int index = defaultSATable.getItems().size();
        if (index < 34)
        {
            SpeechActs newSA = new SpeechActs((index),"",null,null);

            boolean okClicked = mainApp.showSAEditDialog(newSA);
            if (okClicked)
            {
            	mainApp.getSA().add(newSA);
            	mainApp.updateNumMSGs();
                //*********************************************************
                // scroll down to the recently added item and select it
                //*********************************************************
		    	//defaultSATable.getSelectionModel().clearSelection();
		    	defaultSATable.getSelectionModel().select(defaultSATable.getItems().size()-1);
		    	defaultSATable.scrollTo(defaultSATable.getItems().size()-1);
            }
        }

        else
        {
            // You can't add more messages
            Alert alert = new Alert(AlertType.WARNING);
            alert.initOwner(mainApp.getPrimaryStage());
            alert.setTitle("Your Set of Messages is Full");
            alert.setHeaderText("You Are Not Allowed to Add More Messages");
            alert.setContentText("Your set of messages is full, you cannot add more messages.");

            alert.showAndWait();
        }
    }


  //*********************************************************************************************************
  //---------------------------------------- Send Cheap Talk [Triggered when clicking send/dont send button]*
  //*********************************************************************************************************
    @FXML
    public void sendMessages() {
    	msgEndTime = System.currentTimeMillis() - msgStartTime;
    	ActionA.setStyle("-fx-underline : false");
    	ActionB.setStyle("-fx-underline : false");
    	//System.out.println("Current Round: " + mainApp.getRound());
    	mainApp.getInfoList().add(new PlayerInfo(mainApp.getRound(), null, null, null, null, null, Double.NaN, Double.NaN));
    	mainApp.getInfoList().get(roundIndex).setRound(mainApp.getRound());
    	waitPartnerMSG.setVisible(true);
    	waitPartnerCT.play();
    	/********************************************
    	 * Send my cheap talk ***********************
    	 *******************************************/
    	//sendMyChat();

    	/********************************************
    	 * Read Partner's cheap talk and adjust GUI**
    	 *******************************************/
    	//displayPartnerCheapTalk();

    	/********************************************
    	 * Say Partner's cheap talk *****************
    	 *******************************************/
    	sayPartnerCT.play();


    }

    
    private void sendMyChat()
    {
		if (sendMSG.getText() == "Send Messages")
    	{
    		//to fill the column of "my chat"
    		String chatList = "";
    		String speechActs = "";

    		for (int n = 0; n < mainApp.getMSG().size(); n++)
    		{
    			if((mainApp.getMSG().get(n).getCode() > 2))
    			{
    				speechActs = speechActs + (mainApp.getMSG().get(n).getCode() + "@" + mainApp.getMSG().get(n).getText() + ";");
    			}
    			else if(mainApp.getMSG().get(n).getCode() == 1)
    			{
    				speechActs = speechActs + (mainApp.getMSG().get(n).getCode() +" "+ mainApp.getMSG().get(n).getActionPlan1().replaceAll("\\-", "") +" "+ mainApp.getMSG().get(n).getActionPlan2().replaceAll("\\-", "")+"@" + mainApp.getMSG().get(n).getText() + ";");
    			}
    			else if((mainApp.getMSG().get(n).getCode() == 0) || (mainApp.getMSG().get(n).getCode() == 2))
    			{
    				speechActs = speechActs + (mainApp.getMSG().get(n).getCode() +" "+ mainApp.getMSG().get(n).getActionPlan1().replaceAll("\\-", "") + "@" + mainApp.getMSG().get(n).getText() + ";");
    			}

    			chatList = chatList + mainApp.getMSG().get(n).getText();

    			if (n == (mainApp.getMSG().size()-1))
    			{
    				speechActs = speechActs + "$ " + msgEndTime;
    			}
    		}
    		mainApp.getInfoList().get(roundIndex).setMyChat(chatList);
    		mainApp.sout.println(speechActs);

    	}
    	else
    	{
    		mainApp.getInfoList().get(roundIndex).setMyChat("No Messages Sent");
    		mainApp.sout.println(";$ " + msgEndTime);
    	}
    	
    }


	public void displayPartnerCheapTalk()
	{
        String cheapoTalko;
        try {
            cheapoTalko = mainApp.sin.readLine();
            while ((cheapoTalko == null) || ("".equals(cheapoTalko)))
            {
            	cheapoTalko = mainApp.sin.readLine();
            }
            
            StringTokenizer st = new StringTokenizer(cheapoTalko, ";");
            String theString = "", thatOne;
            while (st.hasMoreElements()) {
                String thisOne = st.nextToken();
                //System.out.println("thisone: " + thisOne);
                if (thisOne.charAt(0) != '$') {
                    StringTokenizer st2 = new StringTokenizer(thisOne, "@");
                    st2.nextToken();
                    thatOne = st2.nextToken() + " ";
                    chatTokens.add(thatOne);
                    //System.out.print(thatOne);
                    theString = theString + thatOne;
                    //System.out.println("the msg: " + theString);
                    System.out.println("Partner's msg: " + theString);
                    //System.out.println("My Turn: " + MainApp.getMe());

                }
            }
            
            //System.out.println("\n");
            if (theString.length() > 1) {
            	mainApp.getInfoList().get(roundIndex).setPartnerChat(theString);
            	chatString = theString;

            }
            
            else if (theString.length()<1)
            {
            	chatString = "";
            	mainApp.getInfoList().get(roundIndex).setPartnerChat("No Messages Sent");
            }

        }
        catch (IOException e) {
            System.err.println("Caught IOException: " + e.getMessage());
        }

    	yourChatCol.setCellFactory(new Callback<TableColumn<PlayerInfo, String>, TableCell<PlayerInfo, String>>() {

	        @Override
	        public TableCell<PlayerInfo, String> call(
	            TableColumn<PlayerInfo, String> param) {
	            TableCell<PlayerInfo, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(yourChatCol.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            if (MainApp.getMe() == 0)
	            {
	            	text.setStyle("-fx-fill: midnightblue ");
	            }
	            else if (MainApp.getMe() == 1)
	            {
	            	text.setStyle("-fx-fill: #b2490c ");
	            }
	            
	            return cell ;
	        }
	    });

    	partnerChatCol.setCellFactory(new Callback<TableColumn<PlayerInfo, String>, TableCell<PlayerInfo, String>>() {

	        @Override
	        public TableCell<PlayerInfo, String> call(
	            TableColumn<PlayerInfo, String> param) {
	            TableCell<PlayerInfo, String> cell = new TableCell<>();
	            Text text = new Text();
	            cell.setGraphic(text);
	            cell.setPrefHeight(Control.USE_COMPUTED_SIZE);
	            text.wrappingWidthProperty().bind(partnerChatCol.widthProperty());
	            text.textProperty().bind(cell.itemProperty());
	            if (MainApp.getMe() == 0)
	            {
	            	text.setStyle("-fx-fill: #b2490c");
	            }
	            
	            else if (MainApp.getMe() == 1)
	            {
	            	text.setStyle("-fx-fill: midnightblue");
	            }
	            
	            return cell ;
	        }

	    });

        //*********************************************************
        // scroll down to the recently added item and select it
        //*********************************************************
    	chattingTable.getSelectionModel().clearSelection();
		chattingTable.getSelectionModel().select(chattingTable.getItems().size()-1);
		chattingTable.scrollTo(chattingTable.getItems().size()-1);

    	//clear the selected messages list
    	mainApp.getMSG().clear();

    	//change the "send/don't send button text"
    	sendButtonText();
    	sendSA.setDisable(true);
    	selectSA.setDisable(true);

    	JA00.setStyle("-fx-background-color: black,white; -fx-background-insets: 0, 1 1 1 1;");
    	JA01.setStyle("-fx-background-color: black,white; -fx-background-insets: 0, 1 1 1 0;");
    	JA10.setStyle("-fx-background-color: black,white; -fx-background-insets: 0, 0 1 1 1;");
    	JA11.setStyle("-fx-background-color: black,white; -fx-background-insets: 0, 0 1 1 0;");

    }

	public void sayPartnerCheapTalk()
	{
		waitPartnerMSG.setVisible(false);
		waitPartnerCT.stop();
		try {
			//Windows OS
			//p = Runtime.getRuntime().exec("cscript say.vbs " + "\"" + chatString + "\"");
			//MAC OS
			p = Runtime.getRuntime().exec("say " + "\"" + chatString + "\"");
			p.waitFor();
			//enable choosing actions and change the focus color
	    	ActionA.setDisable(false);
	    	ActionB.setDisable(false);
	    	changeColors();
	    	actStartTime = System.currentTimeMillis();
		} catch (IOException | InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		sayPartnerCT.stop();
	}

	private void sendButtonText() {
        // change the text of the "send/don't send msgs" button in the "selected messages" window.
        if (mainApp.getMSG().size() != 0)
        {
        	sendMSG.setText("Send Messages");
        }
        else
        {
        	sendMSG.setText("Don't Send Messages");
        }
}

    /**
     * Called when the user clicks on the "remove message" button under selected messages window.
     */
    @FXML
    private void handleDeleteMSG() {

        int selectedIndex = selectedSATable.getSelectionModel().getSelectedIndex();
        if (selectedIndex >= 0) {
        	mainApp.getMSG().remove(selectedIndex);
            // change the text of the "send/don't send msgs" button in the "selected messages" window.
            if (mainApp.getMSG().size() != 0)
            {
            	sendMSG.setText("Send Messages");
            }
            else
            {
            	sendMSG.setText("Don't Send Messages");
            }
        } else {
            // Nothing selected.
            Alert alert = new Alert(AlertType.WARNING);
            alert.initOwner(mainApp.getPrimaryStage());
            alert.setTitle("No Selection");
            alert.setHeaderText("No Message Selected");
            alert.setContentText("Please select a message from the table of selected messages.");
            alert.showAndWait();
        }
    }


    //***********************************************************************************************
    //-------------------------------------------------------------- Choose and Send an Action      *
    //***********************************************************************************************
      /**
       * Called when the user clicks an action button.
       */
      @FXML
  	public void chooseAction(ActionEvent event) {

    	  waitPartnerAction.setVisible(true);

          if (ActionA.isArmed())
          {
              mainApp.getInfoList().get(roundIndex).setMyAction("0");
              actEndTime = System.currentTimeMillis() - actStartTime;
              ActionA.setStyle("-fx-underline : true");
          	  ActionA.setDisable(true);
        	  ActionB.setDisable(true);
          }

          if (ActionB.isArmed())
          {
        	  mainApp.getInfoList().get(roundIndex).setMyAction("1");
        	  actEndTime = System.currentTimeMillis() - actStartTime;
        	  ActionB.setStyle("-fx-underline : true");
          	  ActionA.setDisable(true);
        	  ActionB.setDisable(true);
          }

          int act = Integer.parseInt(mainApp.getInfoList().get(roundIndex).getMyAction());
          mainApp.sout.println(act + "$ " + actEndTime);

/*============================================================================================
           * Get the partner's action and the result of this round
=============================================================================================*/
          waitJointAction.play();
          //readResult();
      }

      public void readResult()
      {

          // Read result
          try {
              String result = mainApp.sin.readLine();
              while ((result == null) || ("".equals(result)))
              {
            	  result = mainApp.sin.readLine();
              }
              StringTokenizer st = new StringTokenizer(result);
              //assigning actions
              mainApp.getInfoList().get(roundIndex).setMyAction(st.nextToken());
              mainApp.getInfoList().get(roundIndex).setPartnerAction(st.nextToken());
              int myAct = Integer.parseInt(mainApp.getInfoList().get(roundIndex).getMyAction());
              int partnerAct = Integer.parseInt(mainApp.getInfoList().get(roundIndex).getPartnerAction());
              //assigning payoffs
              if (MainApp.getMe() == 0)
              {
                  mainApp.getInfoList().get(roundIndex).setMyPayoff(MainApp.getPayoffs()[myAct][partnerAct][0]);
                  mainApp.getInfoList().get(roundIndex).setPartnerPayoff(MainApp.getPayoffs()[myAct][partnerAct][1]);
              }
              
              else if (MainApp.getMe() == 1)
              {
                  mainApp.getInfoList().get(roundIndex).setMyPayoff(MainApp.getPayoffs()[myAct][partnerAct][1]);
                  mainApp.getInfoList().get(roundIndex).setPartnerPayoff(MainApp.getPayoffs()[myAct][partnerAct][0]);
              }


              //System.out.format("Result (%d, %d): %.2f, %.2f%n", myAct, partnerAct, MainApp.getPayoffs()[myAct][partnerAct][0], MainApp.getPayoffs()[myAct][partnerAct][1]);


              if((myAct == 0) && (partnerAct == 0))
              {
            	  mainApp.getInfoList().get(roundIndex).setJointAction("00");
            	  JA00.setStyle("-fx-background-color: black,lightSalmon; -fx-background-insets: 0, 1 1 1 1;");
              }
              else if((myAct == 0) && (partnerAct == 1))
              {
            	  mainApp.getInfoList().get(roundIndex).setJointAction("01");
            	  JA01.setStyle("-fx-background-color: black,lightSalmon; -fx-background-insets: 0, 1 1 1 0;");
              }
              else if((myAct == 1) && (partnerAct == 0))
              {
            	  mainApp.getInfoList().get(roundIndex).setJointAction("10");
            	  JA10.setStyle("-fx-background-color: black,lightSalmon; -fx-background-insets: 0, 0 1 1 1;");
              }
              else if((myAct == 1) && (partnerAct == 1))
              {
            	  mainApp.getInfoList().get(roundIndex).setJointAction("11");
            	  JA11.setStyle("-fx-background-color: black,lightSalmon; -fx-background-insets: 0, 0 1 1 0;");
              }
              waitPartnerAction.setVisible(false);
          }
          catch (IOException e) {
              System.err.println("Caught IOException: " + e.getMessage());
          }
          /*=================================================================================
           * Append data to the csv file
           =================================================================================*/
		  try {
			  writer.append(Integer.toString(mainApp.getInfoList().get(roundIndex).getRound()));
			  writer.append("\t");
			  writer.append(MainApp.getGameName());
			  writer.append("\t");
			  writer.append(MainApp.getPlayerID());
			  writer.append("\t");
			  writer.append(MainApp.getPlayerName());
			  writer.append("\t");
			  writer.append(MainApp.getPartnerName());
			  writer.append("\t");
			  writer.append(Long.toString(msgEndTime));
			  writer.append("\t");
			  writer.append(Long.toString(actEndTime));
			  writer.append("\t");
			  writer.append(mainApp.getInfoList().get(roundIndex).getMyChat());
			  writer.append("\t");
			  writer.append(mainApp.getInfoList().get(roundIndex).getPartnerChat());
			  writer.append("\t");
			  writer.append(mainApp.getInfoList().get(roundIndex).getMyAction());
			  writer.append("\t");
			  writer.append(mainApp.getInfoList().get(roundIndex).getPartnerAction());
			  writer.append("\t");
			  writer.append(Double.toString(mainApp.getInfoList().get(roundIndex).getMyPayoff()));
			  writer.append("\t");
			  writer.append(Double.toString(mainApp.getInfoList().get(roundIndex).getPartnerPayoff()));
			  writer.append("\t");
			  writer.append(Double.toString(moneyEarned));
			  writer.append("\t");
			  for (int n = 0; n < mainApp.getSA().size(); n++)
			  {
				  writer.append("[" + mainApp.getSA().get(n).getCode() + "] " + mainApp.getSA().get(n).getText() + ";");

			  }
			  writer.append("\n");
			  writer.flush();
		} catch (IOException e1) {
			e1.printStackTrace();
		}

		  /*
		   * Print the average payoff and update the money
		   */
		  double myAvgPayoff = 0.0;
		  for(int n = 0; n < mainApp.getInfoList().size();n++)
		  {
			  myAvgPayoff = myAvgPayoff + mainApp.getInfoList().get(n).getMyPayoff();
		  }
		  avgPayoff.setText(String.format("%.2f",(myAvgPayoff/currentRound)));

		  moneyEarned = moneyEarned + (pointValue * mainApp.getInfoList().get(roundIndex).getMyPayoff());
		  money.setText(String.format("%.2f", moneyEarned));



/*============================================================================================
           * Update the round number or close the game
=============================================================================================*/
          roundIndex++;
          currentRound++;
          mainApp.setRound(currentRound);

          if (mainApp.getRound() <= mainApp.getNumRounds())
          {
        	  selectSA.setDisable(false);
        	  sendSA.setDisable(false);
          }
          else
          {
        	  gameOver.setVisible(true);
        	  playerPercentile.setVisible(true);
        	  totalMoney.setVisible(true);
        	  totalMoneyLabel.setText(String.format("%.2f", moneyEarned)+" USD");
        	  totalMoneyLabel.setVisible(true);
        	  selectSA.setDisable(true);
        	  sendSA.setDisable(true);
        	  chattingWindow.setDisable(true);

    		  try {
    			  perc = mainApp.sin.readLine();
    			  //System.out.println("Perc: " + perc);
				  playerPercentileLabel.setText(perc);
				  playerPercentileLabel.setVisible(true);
				  writer.close();
				  }
    		  catch (IOException e1)
				{e1.printStackTrace();}

        	  System.out.println("Game Over!");
          }
          changeColors();
          chatTokens.clear();
          msgStartTime = System.currentTimeMillis();
          waitJointAction.stop();
      }


      public void closeGame()
      {
    	  p.destroy();
    	  Timeline timeline = new Timeline();
    	  timeline.getKeyFrames().add(new KeyFrame(Duration.seconds(3),
    			    new EventHandler<ActionEvent>() {
    			        @Override
    			        public void handle(ActionEvent event) {
    			        	Platform.exit();
    			        	//mainApp.getPrimaryStage().close();
    			        }
    			    }));
    	  timeline.play();
      }


}
