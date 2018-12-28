/*
   Demo line-following code for the Pololu Zumo Robot

   This code will follow a black line on a white background, using a
   PID-based algorithm.  It works decently on courses with smooth, 6"
   radius curves and has been tested with Zumos using 30:1 HP and
   75:1 HP motors.  Modifications might be required for it to work
   well on different courses or with different motors.

   https://www.pololu.com/catalog/product/2506
   https://www.pololu.com
   https://forum.pololu.com
*/

#include <Wire.h>
#include <ZumoShield.h>
#define QTR_THRESHOLD 850 // microseconds
#define NUM_SENSORS 6
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

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

 while (Serial.available()) {
  if(incomingByte == "S")
  
  motors.setSpeeds(0,0);
  stop1 =true;
  zumoManual();
 
  }


  if (stop1 == false || stop2 == false  )
  {

    reflectanceSensors.readLine(sensor_values);
    motors.setSpeeds(200, 200);
    Serial.write("S");

    if (sensor_values[1] >= QTR_THRESHOLD || sensor_values[2] >= QTR_THRESHOLD || sensor_values[3] >= QTR_THRESHOLD || sensor_values[4] >= QTR_THRESHOLD) // detect wall and stop
    {


      motors.setSpeeds(-200, -200);
      delay(300);
      motors.setSpeeds(0, 0);
      delay(300);        
      stop1 = true;
      stop2 = true; // ONLY HAPPENS IN A SPECIAL CASE WHEN WALL IS DETECTED!
      zumoManual();
    }

    else if (sensor_values[0] >= QTR_THRESHOLD)
    {

      motors.setSpeeds(-200, -200);
      delay(300);
      motors.setSpeeds(50, -50); // go right
      delay(400);
      motors.setSpeeds(0, 0); // stop
      //stop1 = true;
    }

    else if (sensor_values[5] >= QTR_THRESHOLD )
    {
      motors.setSpeeds(-200, -200);
      delay(300);
      motors.setSpeeds(-50, 50); // go right
      delay(400);
      motors.setSpeeds(0, 0);
      // stop1 = true;
    }

  }
}

