
#include <Wire.h>
#include <ZumoShield.h>
#include <NewPing.h>
#include <ArduinoSTL.h>
#include <vector>

#define QTR_THRESHOLD 850 // microseconds
#define NUM_SENSORS 6
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 15 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define CORRIDOR1_ 1
#define CORRIDOR2_ 2
#define CORRIDOR3_ 3
#define CORRIDOR4_ 4
// Instantiating objects.
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int lastError = 0;
//Initialise sensor array to hold 6 sensors;
unsigned int sensor_values[NUM_SENSORS];
bool stop1 = false;
bool stop2 = false;
bool foundWall = false;
int  corridorDelayCount = 45;
int  roomCounter = 0;
char leftDirection = 'y';
char rightDirection = 'x';


//get manual commands
char incomingByte = ' ';

std::vector <int> corridors;
std::vector <int> rooms;
std::vector <char> storeDirection;

void setup()
{


  Serial.begin(9600); // Start serial communication at 9600 bps
 // calibrateZumo();
  stop1 =true;
  stop2 = true;
};

void loop()
{
   reflectanceSensors.read(sensor_values); // reads raw values from sensor
  //stop1 = false;
  zumoAutoDetect();
  if (stop1 == true)
  {
    zumoManual();
  }

}

void calibrateZumo()
{
 
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
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
  motors.setSpeeds(0, 0);
}

void zumoManual()
{

  motors.setSpeeds(0, 0);
  delay(200);
  digitalWrite(13, HIGH);

  while (Serial.available()) {
    // do MANUAL CONTROL ONCE THE ZUMO_BOT HITS THE WALL
    incomingByte = Serial.read(); //read and store value from gui
    if (incomingByte == 'L')
    {
      motors.setSpeeds(-200, 600);
      delay(45);
    }
    else if (incomingByte == 'R')
    {
      motors.setSpeeds(600, -200);
      delay(45);
    }
    else if (incomingByte == 'F')
    {
      motors.setSpeeds(200, 200);
      delay(45);
    }
    else if (incomingByte == 'B')
    {
      motors.setSpeeds(-200, -200);
      delay(30);
    }
    else if (incomingByte == 'C')
    {
      calibrateZumo();
    }
      else if (incomingByte == 'S')
      {
        //corridors.push_back(corridorDelayCount);// save the delays
        motors.setSpeeds(0, 0);
        zumoManual();
      } //Y & Z FOR ROOM DETECTION ON THEIR RESPECTIVE SIDES VIA G.U.I.
      else if (incomingByte == 'Y')
      {
        // Room LEFT.
        roomCounter = roomCounter + 1;
        rooms.push_back(roomCounter);
        Serial.print(" Room to the LEFT.");
        Serial.print(rooms[rooms.size() - 1]);
        Serial.print(".");
      }
      else if (incomingByte == 'Z')
      {
        // Room RIGHT.
        roomCounter = roomCounter + 1;
        rooms.push_back(roomCounter);
        Serial.print(" Room to the RIGHT.");
        Serial.print(rooms[rooms.size() - 1]);
        Serial.print(".");
      }
      else if (incomingByte == 'G')
      {
        movingScan();
      }
      else if (incomingByte == 'A')
      {
        stopForManual = false;
        while (stopForManual == false)
        {
          zumoAutoDetect(); // check if more than 1 sensory array detects low reflectancy
          //and then stops the motors to identify wall detection.
        }
      }
      else if (incomingByte == 'E')
      {
        Serial.print("Reached and END.");
        motors.setSpeeds(0, 0);
      }
      else if (incomingByte == 'I')
      {
        Serial.print("Automating previous step.");
        for (int i = corridors[corridors.size() - 1]; i > 0; i = i - 45)        {
          sidesDetect();
        }
      }
      else if (incomingByte == 'y')
      {
        //corridor left
        Serial.print("SAVING CORRIDOR LEFT TURN.");
        storeDirection.push_back(leftDirection); // negate for return journey.
      }
      else if (incomingByte == 'x')
      {
        //corridor right
        Serial.print("SAVING CORRIDOR RIGHT TURN.");
        storeDirection.push_back(rightDirection);
      }
      else if (incomingByte == 'p')
      {
        returnJourney();// return journey
      }

  }

}

void zumoAutoDetect()
{

  // Stop automation if the STOP button is pressed via the G.U.I.
  while (Serial.available()) {
    if (incomingByte == "S")
      motors.setSpeeds(0, 0);
    stop1 = true;
    zumoManual();
  }

  if (stop1 == false || stop2 == false  )
  {
    reflectanceSensors.readLine(sensor_values);
    zumoForward(0); // keep going forward

    if (sensor_values[1] >= QTR_THRESHOLD || sensor_values[2] >= QTR_THRESHOLD || sensor_values[3] >= QTR_THRESHOLD || sensor_values[4] >= QTR_THRESHOLD) // Detect wall through observing low reflectance via
      //reflectance sensors and if 1 or more detects this threshold then stop.
    {
      zumoBack(300);
      zumoStop(300);
      
      stop1 = true;
      stop2 = true; // ONLY HAPPENS IN A SPECIAL CASE WHEN WALL IS DETECTED!
      
      Serial.print("M."); // SEND MANUAL MODE TO GUI WHEN WALL DETECTED
      
      //corridors.push_back(corridorDelayCount);// save the time
      
      zumoManual();
    }

    else if (sensor_values[0] >= QTR_THRESHOLD)
    {
      zumoBack(300);
      zumoRight(400);
      zumoStop(0);
    }

    else if (sensor_values[5] >= QTR_THRESHOLD )
    {
      zumoBack(300);
      zumoLeft(400);
      zumoStop(0);
    }

  }
}
void scanForObjects()
{
  float dist = sonar.ping_cm(); //delay(100);
  if ( dist > 2) // Ultrasonic sensor picks up an object when ping value is greater than 0.
  {
    objFound = true;
  }

}



void zumoLeft(int setDelay)
{
  motors.setSpeeds(-50, 50); // Zumo turn LEFT.
  delay(setDelay);
  //corridorDelayCount = setDelay;
}

void zumoRight(int setDelay)
{
  motors.setSpeeds(50, -50); // Zumo turn RIGHT.
  delay(setDelay);
}

void zumoForward(int setDelay)
{
  motors.setSpeeds(200, 200); // Keep moving Zumo in a straight line.
  delay(setDelay);
  motors.setSpeeds(0, 0);
}

void zumoBack(int setDelay)
{
  motors.setSpeeds(-200, -200); //move back
  delay(setDelay);
}

void zumoStop(int setDelay)
{
  motors.setSpeeds(0, 0);
  delay(setDelay);
}
void sidesDetect()
{
  reflectanceSensors.readLine(sensor_values);
  zumoForward(45); // keep going forward
  if (sensor_values[0] >= QTR_THRESHOLD)
  {
    zumoBack(100);
    zumoRight(400);
    zumoStop(10);
  }
  if (sensor_values[5] >= QTR_THRESHOLD )
  {
    zumoBack(100);
    zumoLeft(400);
    zumoStop(10);
  }
}

void movingScan()
{
  int count1 = 0;
  int count2 = 0;
  objFound = false;

  while (count1 != 12)
  {
    count1++;
    scan();
    motors.setSpeeds(130, -130); //right
    delay(60);
    scan();

  }
  while (count2 != 22)
  {
    count2++;
    scan();
    motors.setSpeeds(-130, 130); //left
    delay(60);
    scan();
  }
  if (objFound == true)
  {
    Serial.print("OBJECT FOUND.");
  }
  else
  {
    Serial.print("NO OBJECT FOUND.");
  }
}
void scan()
{
  float dist = sonar.ping_cm(); //delay(100);
  if ( dist > 2) // Ultrasonic sensor picks up an object when ping value is greater than 0.
  {
    objFound = true;
  }
}

void getCorridorDirection(int pos)
{
  if (storeDirection[storeDirection.size() - pos] == 'y') // If indicated corridor right, turn. Then do oppoisite for the left side
  {

    Serial.print(storeDirection[storeDirection.size() - pos]); //print out directions for testing on return journey in gui
    Serial.print('.');
    zumoRight(695);
  }
  else
  {
    Serial.print(storeDirection[storeDirection.size() - pos]);
    Serial.print('.');
    zumoLeft(695);
  }
}

void turnAroundFromCorridor()
{
  zumoRight(1870); // turn around. For the return journey in task 6.
}

void travelCorridorlengthDuration(int corridorNO)
{
  for (int i = corridors[corridors.size() - corridorNO]; i > 0; i = i - 45) //last corridor
  {
    sidesDetect(); // travel whilst detecting blacklines according to the duration of the last most corridor.
  }
}

void returnJourney()
{
  digitalWrite(13, HIGH); // light beacon
  Serial.print("RETURNING HOME."); // send return message to G.U.I
  travelCorridorlengthDuration(CORRIDOR1_);

  travelCorridorlengthDuration(CORRIDOR2_);
  turnAroundFromCorridor(); // turn around to go back home.
  travelCorridorlengthDuration(CORRIDOR2_);// for that delay and for that corridor go down in the delay (45 milisecs).

  getCorridorDirection(CORRIDOR1_); // get last corridor direction.

  travelCorridorlengthDuration(CORRIDOR3_); // get second to last corridor delay.
  getCorridorDirection(CORRIDOR3_); // get the turn for the second to last corridor

  travelCorridorlengthDuration(CORRIDOR4_); // get the final delay for the final corridor for which the Zumo can travel back down to automatically.
  digitalWrite(13, LOW); // light beacon off
}

