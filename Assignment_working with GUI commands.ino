#include <Wire.h>
#include <ZumoShield.h>
#include <NewPing.h> // used for the ultrasonic sensor
#include <ArduinoSTL.h> // the library which allows you to access the vectors
#include <vector> // for vectors for task 5 and 6

// Variables for wall detection.
#define QTR_THRESHOLD 500 // relfectance threshold for detecting darkness i.e. black lines
// Micro-seconds.
#define NUM_SENSORS 6 // Number of sensors on reflectance sensor array.
// Variables for object detection.
#define TRIGGER_PIN   6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 23 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define CORRIDOR1_ 1 // corridors which are going to be used to loop through an array to get the total time it takes for that corridor.
#define CORRIDOR2_ 2
#define CORRIDOR3_ 3
#define CORRIDOR4_ 4

// Instantiating objects.
ZumoBuzzer buzzer; // used for sound purposes.
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN); // allows you to access the reflectance sensor.
ZumoMotors motors; // allows you to access the motors and alter their speeds etc on the Zumo.
Pushbutton button(ZUMO_BUTTON);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sensor_values[NUM_SENSORS];// Initialise sensor array to hold 6 sensors.
bool stopForManual = false; // gives control to the manual.
bool wallStop = false; // indicates when a wall is hit.
bool objFound = false; // When an object is found.
char incomingByte = ' '; // Get manual commands.
bool recorded = false; // If zumo has record of movement.
bool changeSpeed0 = false; // toggled to true when slider speed is altered to 0 on speed scale.
bool changeSpeed1 = false; // toggled to true when slider speed is altered to first on speed scale.
bool changeSpeed2 = false; // toggled to true when slider speed is altered to second on speed scale.
bool changeSpeed3 = false; // toggled to true when slider speed is altered to third on speed scale.
bool changeSpeed4 = false; // toggled to true when slider speed is altered to fourth on speed scale.

int  corridorDelayCount = 45; // starts of at 45 miliseconds which is the first forward movement in wall detection. Then it is incremented until a wall is hit.
int  roomCounter = 0; // stores the direction of the rooms.
char leftDirection = 'y'; // store a left direction of the room which helps for the return journey.
char rightDirection = 'x'; // store a right direction of the room which helps for the return journey.

std::vector <int> corridors; // store the time delays for each corridor.
std::vector <int> rooms; // store the number of rooms.
std::vector <char> storeDirection; //store the direction of the rooms.

void setup()
{
  stopForManual = true; // gives full control to the G.U.I.
  wallStop = true; // gives full control to the G.U.I.
  Serial.begin(9600); // Start serial communication at 9600 bps.
};

void loop()
{

  zumoAutoDetect(); // Start wall detection once button is pressed on the G.U.I.
  if (stopForManual == true)
  {
    zumoManual(); // gives G.U.I control to the Zumo.
  }
}

void calibrateZumo()
{

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  delay(1000);

  //loop used for rotating the zumo in place to sweep the sensors over the line
  for (int i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);   //stop it from moving

  // Turn off LED and play buzzer to indicate we are through with calibration
  digitalWrite(13, LOW);
  buzzer.play(">g32>>c32");
}

void zumoManual()
{
  zumoStop(200); // stop the Zumo so its not moving for 200 miliseconds.
  digitalWrite(13, HIGH); // turn L.E.D. pin on
  if (wallStop == true )
  {
    while (Serial.available()) {
      //  MANUAL CONTROL ONCE THE ZUMO_BOT HITS THE WALL.
      incomingByte = Serial.read(); // Read char commands from G.U.I.

      if (incomingByte == 'L')
      {
        motors.setSpeeds(-200, 280); // if G.U.I left button is clicked the zumo turns left with a delay of 45 miliseconds.
        delay(45);
      }
      else if (incomingByte == 'R')
      {
        motors.setSpeeds(280, -200); // if G.U.I right button is clicked the zumo turns right with a delay of 45 miliseconds.
        delay(45);
      }
      else if (incomingByte == 'F')
      {
        motors.setSpeeds(200, 200); // if G.U.I forward button is clicked the zumo goes forward with a delay of 45 miliseconds.
        delay(45);
      }
      else if (incomingByte == 'B')
      {
        motors.setSpeeds(-200, -200); // if G.U.I back button is clicked the zumo goes back with a delay of 30 miliseconds.
        delay(30);
      }
      else if (incomingByte == 'C')
      {
        calibrateZumo(); // if G.U.I calibrate button is clicked the zumo turns left and right and calibrates the reflectance sensor array.
      }

      else if (incomingByte == 'S')
      {
        motors.setSpeeds(0, 0);// if G.U.I stop button is clicked the zumo stops.
        zumoManual(); // give manual control once more.
      } //Y & Z FOR ROOM DETECTION ON THEIR RESPECTIVE SIDES VIA G.U.I.
      else if (incomingByte == 'Y')
      {
        // Room LEFT.
        roomCounter = roomCounter + 1; // increment the counter for counting rooms.
        rooms.push_back(roomCounter); // store the room number in an array.
        Serial.print(" Room to the LEFT."); // show the response into a G.U.I
        Serial.print(rooms[rooms.size() - 1]); // print the room number on the G.U.I
        Serial.print("."); // terminate G.U.I response.
      }
      else if (incomingByte == 'Z')
      {
        // Room RIGHT.
        roomCounter = roomCounter + 1; // increment the counter for counting rooms.
        rooms.push_back(roomCounter); // store the room number in an array.
        Serial.print(" Room to the RIGHT."); // show the response into a G.U.I
        Serial.print(rooms[rooms.size() - 1]); // print the room number on the G.U.I
        Serial.print("."); // terminate G.U.I response.
      }
      else if (incomingByte == 'G')
      {
        movingScan(); // allows the Zumo to move left and right and scan for an object
      }
      else if (incomingByte == 'A')
      {
        stopForManual = false;
        while (stopForManual == false)
        {
          zumoAutoDetect(); // User hits the automate button and  check if more than 1 sensory array detects low reflectancy
          //and then stops the motors to identify wall detection.
        }
      }
      else if (incomingByte == 'E')
      {
        Serial.print("Reached and END."); // indicates on the G.U.I that an end has reached and stops the Zumo.
        motors.setSpeeds(0, 0);
      }
      else if (incomingByte == 'I')
      {
        Serial.print("Automating previous step.");
        for (int i = corridors[corridors.size() - 1]; i > 0; i = i - 45)        {
          sidesDetect(); // Zumo hits the automate previous button and for the total delay for the last corridor keep going forward and if a wall is hit side detect.
        }
      }
      else if (incomingByte == 'y')
      {
        //corridor left
        Serial.print("SAVING CORRIDOR LEFT TURN."); // display on the G.U.I the direction of the corridor.
        storeDirection.push_back(leftDirection); // store the direction of the corridor into an array as a string and negate for return journey.
      }
      else if (incomingByte == 'x')
      {
        //corridor right
        Serial.print("SAVING CORRIDOR RIGHT TURN.");// display on the G.U.I the direction of the corridor.
        storeDirection.push_back(rightDirection);// store the direction of the corridor into an array as a string and negate for return journey.
      }
      else if (incomingByte == 'p')
      {
        returnJourney();// return journey
      } else if (incomingByte == '1')
      { // first part of the speed scale on the G.U.I
        changeSpeed0 = false; // toggle speeds off
        changeSpeed2 = false; // toggle speeds off
        changeSpeed3 = false; // toggle speeds off
        changeSpeed4 = false; // toggle speeds off

        changeSpeed1 = true; // toggle speed 1 which is specified int he zumoForward function.
      }
      else if (incomingByte == '2')
      {
        // second part of the speed scale on the G.U.I
        changeSpeed0 = false;
        changeSpeed1 = false; // toggle speeds off
        changeSpeed3 = false; // toggle speeds off
        changeSpeed4 = false; // toggle speeds off

        changeSpeed2 = true; // toggle speed 2 which is specified int he zumoForward function.
      }
      else if (incomingByte == '3')
      {

        // third part of the speed scale on the G.U.I
        changeSpeed0 = false;
        changeSpeed1 = false; // toggle speeds off
        changeSpeed2 = false; // toggle speeds off
        changeSpeed4 = false; // toggle speeds off

        changeSpeed3 = true; // toggle speed 3 which is specified int he zumoForward function.
      }
      else if (incomingByte == '4')
      {
        // fourth part of the speed scale on the G.U.I
        changeSpeed0 = false; // toggle speeds off
        changeSpeed1 = false; // toggle speeds off
        changeSpeed2 = false; // toggle speeds off
        changeSpeed3 = false; // toggle speeds off

        changeSpeed4 = true; // toggle speed 4 which is specified int he zumoForward function.
      }
    }
  }
}

void zumoAutoDetect()
{
  // Stop automation if the STOP button is pressed via the G.U.I.
  while (Serial.available()) {
    if (incomingByte == "S") // if the stop button is hit in the G.U.I  stop the zumo motors and get auto of the zumo automation loop.
      motors.setSpeeds(0, 0);
    stopForManual = true;
    zumoManual();
  }

  if (stopForManual == false || wallStop == false  )
  {
    reflectanceSensors.readLine(sensor_values);
    zumoForward(45); // keep going forward unless side or wall is detected the if/else  statements will be invoked.

    corridorDelayCount = corridorDelayCount + 45; // start saving time it takes to hit wall.

    if (sensor_values[1] >= QTR_THRESHOLD || sensor_values[2] >= QTR_THRESHOLD || sensor_values[3] >= QTR_THRESHOLD || sensor_values[4] >= QTR_THRESHOLD || sensor_values[0] >= QTR_THRESHOLD && sensor_values[5] >= QTR_THRESHOLD || sensor_values[1] >= QTR_THRESHOLD && sensor_values[4] >= QTR_THRESHOLD   ) // Detect wall through observing low reflectance via
      //reflectance sensors and if 1 or more detects this threshold then stop.
    {
      corridors.push_back(corridorDelayCount); // save the delays
      zumoBack(100); // move zumo back for 100 miliseconds.
      zumoStop(300); // stop zumo for 300 miliseconds.
      Serial.print(corridorDelayCount); // SEND MANUAL MODE TO GUI WHEN WALL DETECTED
      Serial.print("."); //stop sending message on the G.U.I
      stopForManual = true; // get out of the wall automation loop.
      wallStop = true; // ONLY HAPPENS IN A SPECIAL CASE WHEN WALL IS DETECTED!

      Serial.print("M."); // SEND MANUAL MODE TO GUI WHEN WALL DETECTED
      zumoManual(); // now enter the manual mode once a wall is hit.
      corridorDelayCount = 0; // reset the counter per wall delay.
    }

    else if (sensor_values[0] >= QTR_THRESHOLD) // if the left most reflectance array picks ups a reflectance above the reflectance threshold.
    {
      zumoBack(100); // move away from the side of corridors for the left side.
      zumoRight(96); // move the zumo to the right for 96 miliseconds.
      zumoStop(10); // zumo stops for 10 miliseconds.
      corridorDelayCount = (corridorDelayCount - 155); // back and stop delay subtracted from the total count of the forward delay up until wall is hit.
    }

    else if (sensor_values[5] >= QTR_THRESHOLD )// if the right most reflectance array picks ups a reflectance above the reflectance threshold.
    {
      zumoBack(100); // move away from the side of corridors for the right side
      zumoLeft(96); // move the zumo to the left for 96 miliseconds.
      zumoStop(10); // zumo stops for 10 miliseconds.
      corridorDelayCount = (corridorDelayCount - 155); // back and stop delay subtracted from the total count of the forward delay up until wall is hit.
    }
  }
}

void movingScan()
{
  //creates a counter that imitates a left and right sweep . Scans the room for objects and then sets the boolean to true displaying a message in G.U.I.
  int count1 = 0; // count1 is counter the allows the zumo to make a right turn and slowly scan at the same time.
  int count2 = 0;// count2 is counter the allows the zumo to make a left turn and slowly scan at the same time.
  objFound = false; // toggle to true when and object is found in the scan

  while (count1 != 12)
  {
    count1++; //increment counter until it reaches 12
    scan(); // perform a scan
    motors.setSpeeds(130, -130); // turn right
    delay(60); // slowly turn for 60 milliseconds
    scan(); // perform a scan

  }
  while (count2 != 22)
  {
    count2++; //increment counter until it reaches 22 a little more because the zumo has to travel the right hand side distance to go to the left.
    scan(); // perform a scan
    motors.setSpeeds(-130, 130); //left
    delay(60);// slowly turn for 60 milliseconds
    scan();// perform a scan
  }
  if (objFound == true) // if an object is detected in the scan() function
  {
    Serial.print("OBJECT FOUND."); //print object found in G.U.I  if boolean is true.
  }
  else
  {
    Serial.print("NO OBJECT FOUND."); //print object not found in G.U.I if boolean is true.
  }
}

void scan()
{ // Uses the ping function of the Newping library which detects objects.
  float dist = sonar.ping_cm(); //delay(100);
  if ( dist > 2) // Ultrasonic sensor picks up an object when ping value is greater than 0.
  {
    objFound = true; // toggles boolean to true when object is found a displays the respective message.
  }
}

void zumoLeft(int setDelay)
{
  // Delays are saved for turns to make it easier for the later automation tasks. So the Zumo effectively "remembers" where it has travelled.
  motors.setSpeeds(-150, 120); // Zumo turn LEFT.
  delay(setDelay); // Determines how long the turn is for.
}

void zumoRight(int setDelay)
{
  motors.setSpeeds(120, -150); // Zumo turn RIGHT.
  delay(setDelay);// Determines how long the turn is for.
}

void zumoForward(int setDelay)
{

  if (changeSpeed1 == true) // if the G.U.I sends a char that the first notch of the scale is selected then this varaible is set to true and the default speed is over riden.
  {
    motors.setSpeeds(50, 50); // turn both motors forward very slowly
    delay(setDelay);// Determines how long the turn is for.
    motors.setSpeeds(0, 0); // stop motors from moving in automation after the command has been given
  }
  else if (changeSpeed2 == true)// if the G.U.I sends a char that the first notch of the scale is selected then this varaible is set to true and the default speed is over riden.
  {
    motors.setSpeeds(100, 100);
    delay(setDelay); // Determines how long the turn is for.
    motors.setSpeeds(0, 0);
  } else if (changeSpeed3 == true)// if the G.U.I sends a char that the first notch of the scale is selected then this varaible is set to true and the default speed is over riden.
  {
    motors.setSpeeds(150, 150);
    delay(setDelay);// Determines how long the turn is for.
    motors.setSpeeds(0, 0);// stop motors from moving in automation after the command has been given
  }
  else if (changeSpeed4 == true)// if the G.U.I sends a char that the first notch of the scale is selected then this varaible is set to true and the default speed is over riden.
  {
    motors.setSpeeds(180, 180); //scalle speed set to maximum and motors turn very fast.
    delay(setDelay);// Determines how long the turn is for.
    motors.setSpeeds(0, 0);// stop motors from moving in automation after the command has been given
  }
  else  if (changeSpeed0 == true)// if the G.U.I sends a char that the first notch of the scale is selected then this varaible is set to true and the default speed is over riden.
  {
    motors.setSpeeds(0, 0); // stop both motors if the scale speed is changes to 0.
    delay(setDelay);// Determines how long the turn is for.
    motors.setSpeeds(0, 0);// stop motors from moving in automation after the command has been given
  }
  else // if slider speed is not given then zumo moves forward with default values below.
  {
    motors.setSpeeds(100, 100); // Keep moving Zumo in a straight line. at the set speed if slider value has not been utilised
    delay(setDelay);// Determines how long the turn is for.
    motors.setSpeeds(0, 0);// stop motors from moving in automation after the command has been given
  }
}

void zumoBack(int setDelay)
{
  motors.setSpeeds(-100, -100); //move back
  delay(setDelay);// Determines how long the turn is for.
}

void zumoStop(int setDelay)
{
  motors.setSpeeds(0, 0); //stop
  delay(setDelay);// Determines how long the turn is for.
}

void sidesDetect() // side wall detection to stop zumo going into black ines for task 5 and 6 automation sequence
{
  reflectanceSensors.readLine(sensor_values);// read sensor values
  zumoForward(45); // keep going forward
  if (sensor_values[0] >= QTR_THRESHOLD) // if left most sensor detects black line i.e. low reflectance  = high qtr threshold
  {
    zumoRight(96); //zumo turns right ofr 96 miliseconds.
  }
  if (sensor_values[5] >= QTR_THRESHOLD )// if right most sensor detects black line i.e. low reflectance  = high qtr threshold
  {
    zumoLeft(96);//zumo turns right ofr 96 miliseconds.
  }
}

void getCorridorDirection(int pos)
{
  if (storeDirection[storeDirection.size() - pos] == 'y') // If indicated corridor left turn, do oppoisite turn when coming back
  {

    Serial.print(storeDirection[storeDirection.size() - pos]); //print out directions for testing on return journey in gui
    Serial.print('.'); // terminates the G.U.I promting message.
    zumoRight(695); // do oppossite turn when this function is called
  }
  else
  {
    Serial.print(storeDirection[storeDirection.size() - pos]);//print out directions for testing on return journey in gui
    Serial.print('.'); // terminates the G.U.I promting message.
    zumoLeft(695);// do oppossite turn when this function is called
  }
}

void turnAroundFromCorridor()
{
  zumoRight(1870); // turn around. For the return journey in task 6.
}

void travelCorridorlengthDuration(int corridorNO)
{
  for (int i = corridors[corridors.size() - corridorNO]; i > 0; i = i - 45) // for the the total delays for the corridor number passed in decrement in 45 until that corridor delay count (which is stored in the vector)hits zero
  {
    sidesDetect(); // travel whilst detecting blacklines according to the duration of the last most corridor.
  }
}

void returnJourney()
{
  digitalWrite(13, HIGH); // light beacon
  Serial.print("RETURNING HOME."); // send return message to G.U.I
  travelCorridorlengthDuration(CORRIDOR1_); // pass in the last corridor and move forward whilst keeping in the black lines

  travelCorridorlengthDuration(CORRIDOR2_); // pass in the second to last corridor and move forward whilst keeping in the black lines
  turnAroundFromCorridor(); // turn around to go back home.
  travelCorridorlengthDuration(CORRIDOR2_);// for that delay and for that corridor go down in the delay (45 milisecs).

  getCorridorDirection(CORRIDOR1_); // get last corridor direction.

  travelCorridorlengthDuration(CORRIDOR3_); // // pass in the last corridor and move forward whilst keeping in the black lines
  getCorridorDirection(CORRIDOR3_); // get the turn for the third to last corridor

  travelCorridorlengthDuration(CORRIDOR4_); // get the final delay for the final corridor for which the Zumo can travel back down to automatically.
  digitalWrite(13, LOW); // light beacon off
}
