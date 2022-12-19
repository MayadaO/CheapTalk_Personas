import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;



public class CDialogController {

	@FXML
	private TextField cTextField;
	@FXML
	private Label msgsLeft;

    private Stage dialogStage;
    private SpeechActs speechAct;
    private boolean okClicked = false;


    /**
     * Initializes the controller class. This method is automatically called
     * after the fxml file has been loaded.
     */
    @FXML
    private void initialize() {
    	msgsLeft.setText(Integer.toString(MainApp.getNumMSGs()));
    	Platform.runLater(new Runnable() {
    	    @Override
    	    public void run() {
    	    	cTextField.requestFocus();
    	    }
    	});

    }

    /**
     * Sets the stage of this dialog.
     *
     * @param dialogStage
     */
    public void setDialogStage(Stage dialogStage) {
        this.dialogStage = dialogStage;
    }

    /**
     * Sets the speechAct to be edited in the dialog.
     *
     * @param speechAct
     */
    public void setSpeechAct(SpeechActs speechAct) {
    	this.speechAct = speechAct;
    }

    /**
     * Returns true if the user clicked OK, false otherwise.
     *
     * @return
     */
    public boolean isOkClicked() {
        return okClicked;
    }

    /**
     * Called when the user clicks ok.
     */
    @FXML
    private void handleOk() {
        if (isInputValid()) {
        	while (cTextField.getText().endsWith(" "))
        	{
        		cTextField.setText(cTextField.getText().substring(0, cTextField.getText().length()-1));
        	}
        	speechAct.setText(cTextField.getText());
        	okClicked = true;
        	dialogStage.close();

        }
    }

    /**
     * Called when the user clicks cancel.
     */
    @FXML
    private void handleCancel() {
        dialogStage.close();
    }

    /**
     * Validates the user input in the text field.
     *
     * @return true if the input is valid
     */
    private boolean isInputValid() {
        String errorMessage = "";

        if (cTextField.getText() == null || cTextField.getText().length() == 0 || cTextField.getText().trim().length() < 1) {
            errorMessage += "Blank or Invaild Message!\n";
        }

        if (errorMessage.length() == 0) {
            return true;
        } else {
            // Show the error message.
            Alert alert = new Alert(AlertType.ERROR);
            alert.initOwner(dialogStage);
            alert.setTitle("Invalid Message");
            alert.setHeaderText("Please correct the invalid text message");
            alert.setContentText(errorMessage);

            alert.showAndWait();

            return false;
        }
    }

}
