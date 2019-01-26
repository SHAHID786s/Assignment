/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.
 
 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
 // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void messagesFromZumo_change(GTextArea source, GEvent event) { //_CODE_:textarea1:653858:
  println("RECIEVING MESSAGES FROM ZUMO_BOT");
} //_CODE_:textarea1:653858:

public void roomMessagesFromZumo_change(GTextArea source, GEvent event) { //_CODE_:textarea1:653858:
  println("RECIEVING ROOM MESSAGES FROM ZUMO_BOT");
} 

public void messagesToZumo_change(GTextField source, GEvent event) { //_CODE_:textfield1:568846:
  println("SENDING MESSAGE TO ZUMO_BOT");
} //_CODE_:textfield1:568846:

public void Left_click(GButton source, GEvent event) { //_CODE_:button2:308221:
  port.write('L');
  println("Left command sent to ZUMO_BOT");
  messagesToZumo.setText("Left");
} //_CODE_:button2:308221:

public void Right_click(GButton source, GEvent event) { //_CODE_:button3:604378:
  port.write('R');
  println("Right command sent to ZUMO_BOT");
  messagesToZumo.setText("Right");
  messagesFromZumo.setText("");
} //_CODE_:button3:604378:

public void Forward_click(GButton source, GEvent event) { //_CODE_:button4:348760:
  port.write('F');
  println("Forward  command sent to ZUMO_BOT");
  messagesToZumo.setText("Forward");
  messagesFromZumo.setText("");
} //_CODE_:button4:348760:

public void Back_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('B');
  println("Back command sent to ZUMO_BOT");
  messagesToZumo.setText("Back");
  messagesFromZumo.setText("");
} //_CODE_:button5:984108:


public void Stop_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('S');
  println("Stop command sent to ZUMO_BOT");
  messagesToZumo.setText("Stop");
  messagesFromZumo.setText("");
} //_CODE_:button5:984108:


public void Automate_click(GButton source, GEvent event) { //_CODE_:button5:984108:

  port.write('A');
  println("Automate command sent to ZUMO_BOT");
  messagesToZumo.setText("Automating Mode");
  messagesFromZumo.setText(" ");
} 

public void Room_Left_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('Y');
  println("Room_Left command sent to ZUMO_BOT");
} 

public void Room_Right_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('Z');
  println("Room_Right command sent to ZUMO_BOT");
} 

public void Calibrate_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('C');
  println("Calibrate command sent to ZUMO_BOT");
} 

public void Scan_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('G');
  println("Scan command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void ReachedEnd_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('E');
  println("Reached an End command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void AutomatePrevious_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('I');
  println("Automated previous command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void Return_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('p');
  println("Return command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void corridorTurnRight_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('x');
  println("corridorRight command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void corridorTurnLeft_click(GButton source, GEvent event) { //_CODE_:button5:984108:
  port.write('y');
  println("corridorLeft command sent to ZUMO_BOT");
} //_CODE_:button5:984108:

public void messagesToZumo(GTextField source, GEvent event) { //_CODE_:showCommands:358095:
} //_CODE_:showCommands:358095:

// for when the slider is being used by the G.U.I
public void handleSliderEvents(GValueControl s, GEvent event) { 
  //if (s == slider)  // The slider being configured?
  //  println(slider.getValueS() + "    " + event);    // gives scale value
  if (Integer.parseInt(slider.getValueS()) == 50)
  {
    port.write('1');
    System.out.println(" slider value is 50");
  } else if (Integer.parseInt(slider.getValueS()) == 100)
  {
    port.write('2');
    System.out.println(" slider value is 100");
  } else if (Integer.parseInt(slider.getValueS()) == 150)
  {
    port.write('3');
    System.out.println(" slider value is 150");
  } else if (Integer.parseInt(slider.getValueS()) == 200)
  {
    port.write('4');
    System.out.println(" slider value is 200");
  } else if (Integer.parseInt(slider.getValueS()) == 0)
  {
    port.write('0');
    System.out.println(" slider value is 0");
  }
}

// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI() {
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Sketch Window");
  messagesFromZumo =     new GTextArea(this, 215, 15, 120, 200, G4P.SCROLLBARS_BOTH | G4P.SCROLLBARS_AUTOHIDE);
  roomMessagesFromZumo = new GTextArea(this, 340, 15, 120, 200, G4P.SCROLLBARS_BOTH | G4P.SCROLLBARS_AUTOHIDE); //X,Y,L,W

  roomMessagesFromZumo.setText(" Room Messages from Zumo");
  roomMessagesFromZumo.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  roomMessagesFromZumo.setOpaque(true);
  roomMessagesFromZumo.addEventHandler(this, "roomMessagesFromZumo_change");


  messagesFromZumo.setText("Messages from Zumo");
  messagesFromZumo.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  messagesFromZumo.setOpaque(true);
  messagesFromZumo.addEventHandler(this, "messagesFromZumo_change");

  messagesToZumo = new GTextField(this, 40, 285, 160, 60, G4P.SCROLLBARS_NONE);
  messagesToZumo.setText("\t \t \t \t \t \t Messages To Zumo");
  messagesToZumo.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  messagesToZumo.setOpaque(true);
  messagesToZumo.addEventHandler(this, "messagesToZumo_change");

  Left = new GButton(this, 11, 169, 80, 30);
  Left.setText("Left");
  Left.setTextBold();
  Left.addEventHandler(this, "Left_click");
  Left.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Right = new GButton(this, 127, 169, 80, 30);
  Right.setText("Right");
  Right.setTextBold();
  Right.addEventHandler(this, "Right_click");
  Right.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Forward = new GButton(this, 71, 121, 80, 30);
  Forward.setText("Forward");
  Forward.setTextBold();
  Forward.addEventHandler(this, "Forward_click");
  Forward.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Automate = new GButton(this, 285, 255, 100, 30);
  Automate.setText("Automate");
  Automate.setTextBold();
  Automate.addEventHandler(this, "Automate_click");
  Automate.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Stop = new GButton(this, 285, 290, 100, 30);
  Stop.setText("Stop");
  Stop.setTextBold();
  Stop.addEventHandler(this, "Stop_click");
  Stop.setLocalColorScheme(GCScheme.PURPLE_SCHEME);


  scan = new GButton(this, 285, 220, 100, 30);
  scan.setText("Scan");
  scan.setTextBold();
  scan.addEventHandler(this, "Scan_click");  
  scan.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Back = new GButton(this, 71, 210, 80, 30);
  Back.setText("Back");
  Back.setTextBold();
  Back.addEventHandler(this, "Back_click");
  Back.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Room_Left = new GButton(this, 180, 220, 100, 30);
  Room_Left.setText("Room Left");
  Room_Left.setTextBold();
  Room_Left.addEventHandler(this, "Room_Left_click");
  Room_Left.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Room_Right = new GButton(this, 390, 220, 100, 30);
  Room_Right.setText("Room Right");
  Room_Right.setTextBold();
  Room_Right.addEventHandler(this, "Room_Right_click"); 
  Room_Right.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  reachedEnd = new GButton(this, 35, 70, 150, 30);
  reachedEnd.setText("Reached an End");
  reachedEnd.setTextBold();
  reachedEnd.addEventHandler(this, "ReachedEnd_click");
  reachedEnd.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  automatePrevious = new GButton(this, 35, 30, 150, 30);
  automatePrevious.setText("Automate Previous");
  automatePrevious.setTextBold();
  automatePrevious.addEventHandler(this, "AutomatePrevious_click");
  automatePrevious.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  calibrate = new GButton(this, 285, 325, 100, 30);
  calibrate.setText("Calibrate");
  calibrate.setTextBold();
  calibrate.addEventHandler(this, "Calibrate_click");
  calibrate.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  Return = new GButton(this, 390, 255, 100, 30);
  Return.setText("RETURN HOME");
  Return.setTextBold();
  Return.addEventHandler(this, "Return_click");
  Return.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  corridorTurnRight = new GButton(this, 390, 290, 100, 30);
  corridorTurnRight.setText("CORRIDOR TURN RIGHT");
  corridorTurnRight.setTextBold();
  corridorTurnRight.addEventHandler(this, "corridorTurnRight_click");
  corridorTurnRight.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  corridorTurnLeft = new GButton(this, 390, 325, 100, 30);
  corridorTurnLeft.setText("CORRIDOR TURN LEFT");
  corridorTurnLeft.setTextBold();
  corridorTurnLeft.addEventHandler(this, "corridorTurnLeft_click");
  corridorTurnLeft.setLocalColorScheme(GCScheme.PURPLE_SCHEME);

  slider = new GCustomSlider(this, 60, 360, 400, 100, "purple18px"); //position /color /cursor
  slider.setShowValue(true);
  slider.setShowLimits(true);
  slider.setLimits(0, 0, 200); // range of the scale on slider
  slider.setNbrTicks(5); // number of indicators on scale
  slider.setShowTicks(true);
  slider.setStickToTicks(true); // auto stick cursor to a indicator on scale
  slider.setEasing(10.0); // the time it takes for cursor to move to indicator
  slider.setNumberFormat(G4P.INTEGER, 0); //visibility


  sliderlbl = new GTextField(this, 205, 420, 100, 20, G4P.SCROLLBARS_NONE);
  sliderlbl.setText("Alter Zumo Speed");
  sliderlbl.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  sliderlbl.setOpaque(false);
}

// Variable declarations 
// autogenerated do not edit
GTextArea messagesFromZumo; 
GTextArea roomMessagesFromZumo; 
GTextField messagesToZumo;
GTextField sliderlbl;
GButton Left; 
GButton Right; 
GButton Forward; 
GButton Back; 
GButton Stop; 
GButton Automate;
GButton scan;
GButton Room_Left;
GButton Room_Right;
GButton reachedEnd;
GButton automatePrevious;
GButton calibrate;
GButton Return;
GButton corridorTurnRight;
GButton corridorTurnLeft;
