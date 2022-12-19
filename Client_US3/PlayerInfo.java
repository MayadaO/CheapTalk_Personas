import javafx.beans.property.DoubleProperty;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;



public class PlayerInfo {

	private final IntegerProperty Round;
	private final StringProperty myAction;
	private final StringProperty partnerAction;
	private final StringProperty jointAction;
    private final StringProperty myChat;
    private final StringProperty partnerChat;
    private final DoubleProperty myPayoff;
    private final DoubleProperty partnerPayoff;



    public PlayerInfo(int Round, String myChat, String partnerChat, String myAction, String partnerAction, String jointAction, double myPayoff, double partnerPayoff)
    {
    	this.Round = new SimpleIntegerProperty(Round);
    	this.jointAction = new SimpleStringProperty(jointAction);
    	this.myAction = new SimpleStringProperty(myAction);
    	this.partnerAction = new SimpleStringProperty(partnerAction);
    	this.myChat = new SimpleStringProperty(myChat);
    	this.partnerChat = new SimpleStringProperty(partnerChat);
    	this.myPayoff = new SimpleDoubleProperty(myPayoff);
    	this.partnerPayoff = new SimpleDoubleProperty(partnerPayoff);
    }


	public IntegerProperty RoundProperty() {
        return Round;
    }

	public StringProperty jointActionProperty(){
		return jointAction;
	}

	public StringProperty myActionProperty() {
        return myAction;
    }

	public StringProperty partnerActionProperty() {
        return partnerAction;
    }

	public DoubleProperty myPayoffProperty() {
        return myPayoff;
    }

	public DoubleProperty partnerPayoffProperty() {
        return partnerPayoff;
    }

	public StringProperty myChatProperty() {
		return myChat;
	}

	public StringProperty partnerChatProperty() {
		return partnerChat;
	}


	public int getRound(){
		return Round.get();
	}

	public String getJointAction(){
		return jointAction.get();
	}

	public String getMyAction() {
        return myAction.get();
    }

	public String getPartnerAction() {
        return partnerAction.get();
    }

	public double getMyPayoff() {
        return myPayoff.get();
    }

	public double getPartnerPayoff() {
        return partnerPayoff.get();
    }

	public String getMyChat(){
		return myChat.get();
	}

	public String getPartnerChat(){
		return partnerChat.get();
	}

	public void setRound(int round){
		this.Round.set(round);
	}

	public void setJointAction(String jointAction){
		this.jointAction.set(jointAction);
	}

	public void setMyAction(String myAction){
		this.myAction.set(myAction);
	}

	public void setPartnerAction(String partnerAction){
		this.partnerAction.set(partnerAction);
	}

	public void setMyPayoff(double myPayoff){
		this.myPayoff.set(myPayoff);
	}

	public void setPartnerPayoff(double partnerPayoff){
		this.partnerPayoff.set(partnerPayoff);
	}

	public void setMyChat(String myChat){
		this.myChat.set(myChat);
	}

	public void setPartnerChat(String partnerChat){
		this.partnerChat.set(partnerChat);
	}


}
