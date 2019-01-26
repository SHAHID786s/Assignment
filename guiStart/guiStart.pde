import g4p_controls.*;
import processing.serial.*;
import java.util.*;
import java.awt.*;

int backgroundColour = 15; 
String characterCommand;
Serial port;
String input;
GCustomSlider slider;




public void setup() {
  size(520, 500);
  String portName = "COM13";
  port = new Serial (this, portName, 9600);
  createGUI();
}



public void draw() {

  background  (255, 127, 80);
  fill(227, 300, 500);
  while (port.available() > 0) 
  {
    input = port.readStringUntil('.');
    if (input != null) {
      if (input.startsWith("M"))
      {
        messagesFromZumo.setText("*ZUMO HIT A WALL*");
      } else
      {
        roomMessagesFromZumo.appendText(input);
      }
    }
  }
}
