import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
//import javafx.scene.control.ComboBox;


public class SpeechActs {

	private final IntegerProperty code;
    private final StringProperty text;
    private final StringProperty actionPlan1;
    private final StringProperty actionPlan2;



    public SpeechActs(int code, String text, String actionPlan1,String actionPlan2)
    {
    	this.code = new SimpleIntegerProperty(code);
    	this.text = new SimpleStringProperty(text);
    	this.actionPlan1 = new SimpleStringProperty(actionPlan1);
    	this.actionPlan2 = new SimpleStringProperty(actionPlan2);
    }


	public IntegerProperty codeProperty() {
        return code;
    }


	public StringProperty textProperty() {
		return text;
	}

	public StringProperty actionProperty1() {
		return actionPlan1;
	}
	public StringProperty actionProperty2() {
		return actionPlan2;
	}

	public int getCode(){
		return code.get();
	}

	public String getText(){
		return text.get();
	}

	public String getActionPlan1(){
		return actionPlan1.get();
	}
	public String getActionPlan2(){
		return actionPlan2.get();
	}

	public void setCode(int code){
		this.code.set(code);
	}

	public void setText(String text){
		this.text.set(text);
	}

	public void setAction1(String actionPlan1){
		this.actionPlan1.set(actionPlan1);
	}

	public void setAction2(String actionPlan2){
		this.actionPlan2.set(actionPlan2);
	}


}
