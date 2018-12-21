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

}