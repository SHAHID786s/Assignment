
#include <Wire.h>
#include <ZumoShield.h>
#include <NewPing.h>
#define QTR_THRESHOLD 850 // microseconds
#define NUM_SENSORS 6
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 15 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

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


//get manual commands
char incomingByte = ' ';

void setup()
{


  Serial.begin(9600); // Start serial communication at 9600 bps
  calibrateZumo();
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

  // Initialize the reflectance sensors module
  reflectanceSensors.init();
  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

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

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  button.waitForButton();
  buzzer.play(">g32>>c32");
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
        GUISTOP = true;
        Serial.write("S");
        motors.setSpeeds(0,0);
        zumoManual();
      }//Y & Z FOR ROOM DETECTION ON THEIR RESPECTIVE SIDES VIA G.U.I.
      else if (incomingByte == 'Y')
      {
        // Room LEFT.
        Serial.print("Room to the LEFT.");
      }
    
     else if (incomingByte == 'I')
      {
        Serial.print("Automating previous step.");
        //corridors.push_back(corridorDelayCount= corridorDelayCount + 45);
        for (int i = corridors[corridors.size() - 1]; i > 0; i = i - 45)        {
          sidesDetect();
        }
      }
      else if (incomingByte == 'Z')
      {
        // Room RIGHT.
        Serial.print("Room to the RIGHT.");
      }
      else if (incomingByte == 'G')
      {
         movingScan();
      }
      else if (incomingByte == 'A')

      {
        Serial.write("A");
        //reflectanceSensors.read(sensor_values); // reads raw values from sensor
        stop1 = false;
        while (stop1 == false)
        {
          zumoAutoDetect(); // code that uses to check if more than 1 sensory array detects low reflectancy and then stops the motors
        }



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


