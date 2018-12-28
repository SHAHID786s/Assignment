import g4p_controls.*;
import processing.serial.*;

int backgroundColour = 15; 
String characterCommand;
Serial port;

public void setup() {
  size(500, 360);
  //String portName = Serial.list()[0];
  String portName = "COM13";
  port = new Serial (this,portName,9600);
  createGUI();
}

public void draw() {

  background(200, 200, 200);
  fill(227, 230, 255);
  formatTxt();
  while (port.available() > 0) 
  {
    input = port.readStringUntil('.');
    if (input != null) {
      if (input.startsWith("M"))
      {
         messagesFromZumo.setText("*** MANUAL MODE IS ON ZUMO HIT A WALL ***");
        
      } else
      {
        roomMessagesFromZumo.appendText(input);
      }
    }
  }
}
