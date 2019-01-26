import g4p_controls.*; // for making G.U.I.
import processing.serial.*; // for accessing serial port and incoming bytes
import java.util.*; // java utilities
import java.awt.*;

int backgroundColour = 15; 
String characterCommand;
Serial port;
String input;
GCustomSlider slider; // for speed slider




public void setup() {
  size(520, 500); //dimentions of the G.U.I
  String portName = "COM13"; // using the port for the arduino board
  port = new Serial (this, portName, 9600); // Initialise the Serial constructor passing the port , baud value which is 9600
  createGUI(); // contains the buttons , textfields/areas and their events.
}



public void draw() {

  background  (255, 127, 80);
  fill(227, 300, 500); // back ground colour.
  while (port.available() > 0) // while we can read something from the port
  {
    input = port.readStringUntil('.'); // read byte until a '.' is at the end of the sentence.
    if (input != null) { // if incoming byte  is not null 
      if (input.startsWith("M")) // if the byte starts with 'M' char then display zumo hit a wall on the G.U.I, else display whatever action the zumo is doing
      {
        messagesFromZumo.setText("*ZUMO HIT A WALL*");
      } else
      {
        roomMessagesFromZumo.appendText(input);
      }
    }
  }
}
