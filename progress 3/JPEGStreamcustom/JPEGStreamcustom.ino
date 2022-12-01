/*
JPEGStream.ino : capture images and stream them to host as JPEG files
*/

#include <Wire.h>
//#include <BreezyArduCAM.h>
//#include <SPI.h>

//compass
#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>

//stepper motor
#include <Stepper.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
const int stepsPerRevolution = 1024;  // change this to fit the number of steps per revolution
// for your motor

//uncomment for camera
//static const int CS = 7;
//Serial_ArduCAM_FrameGrabber fg;
/* Choose your camera */
//ArduCAM_Mini_2MP myCam(CS, &fg);


//compass setup
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

//photocell setup
const int photocell =  A0;
int brightness  = 0;

 //set up motor
 // initialize the stepper library on pins 8 through 11:
 Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
 int startLocation = 1;
 int CW = 0;
 int CCW = 0;


void setup(void) 
{
    // ArduCAM Mini uses both I^2C and SPI buses
    //Wire.begin();
    //SPI.begin();

    // Fastest baud rate (change to 115200 for Due)
    Serial.begin(921600);
    //Serial.begin(115200);

    // Start the camera in JPEG mode with a specific image size
    //myCam.beginJpeg320x240();

    /* Initialise the compass sensor */
    if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
    }
    //set up photocell
    pinMode(photocell, INPUT);

   //set start position
   
    //set up motor speed in rpm (75)
    myStepper.setSpeed(20);
}
void compass()
{
   //compass functions  sent to serial terminal for user
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  delay(500);
}

void motorCW()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    CW = (stepsPerRevolution);
    myStepper.step(CW);
    delay(500);
    Serial.println("at location A");

}

void motorCCW()
{
  // step one revolution  in the other direction:
    Serial.println("counterclockwise");
    CCW = (-stepsPerRevolution);
    myStepper.step(CCW);
    delay(500);
    Serial.println("at location B");
}

void loop() 
{
    //camera capture
    //myCam.capture();

    brightness = analogRead(photocell);

    if (brightness > 150)
    {
      Serial.println("bright enough, on");
      compass();
    }
    else
    {
      Serial.println("too dark, off");
    }

    motorCW();
    motorCCW();
    
 
  
}
