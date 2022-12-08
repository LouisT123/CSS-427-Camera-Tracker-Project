#include <SoftwareSerial.h>
#include <Wire.h>
#include <BreezyArduCAM.h>
#include <SPI.h>

//compass
#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>

//stepper motor
#include <Stepper.h>
const int stepsPerRevolution = 390;  // change this to fit the number of steps per revolution
// for your motor

//compass setup   
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

//photocell setup
const int photocell =  A0;
int brightness  = 0;

//set up motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10,9, 11);
int startLocation = 1;
int CW = 0;
int CCW = 0;


static const int CS = 5;
//communication test
SoftwareSerial s(6,7);
Serial_ArduCAM_FrameGrabber fg;
/* Choose your camera */
ArduCAM_Mini_2MP myCam(CS, &fg);

void setup() {
  Wire.setClock(400000);
  Wire.begin();
  //TWBR = 12;
  SPI.begin();
  //s.begin(9600);
  Serial.begin(921600);
  s.begin(9600);
  myCam.beginJpeg160x120();

  //set up photocell sensor
  pinMode(photocell, INPUT);

  //set up motor speed
  myStepper.setSpeed(20);

  //User on demand data
  Serial.println("Press 1 for on demand compass data, 2 for on demand photocell brightness data");
  while(!mag.begin());
}

float compass()
{
   //compass functions  sent to serial terminal for user
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y, event.magnetic.z) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }
  Serial.print("Compass Heading: ");
  Serial.println(heading);
  //delay(500);
  return(heading);
}

void motorCW()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    CW = (stepsPerRevolution);
    myStepper.step(CW);
    delay(200);
    Serial.println("at location A");

}

void motorCCW()
{
  // step one revolution  in the other direction:
    Serial.println("counterclockwise");
    CCW = (-stepsPerRevolution);
    myStepper.step(CCW);
    delay(200);
    Serial.println("at location B");
}

bool getPhotoCell(){
  
  brightness = analogRead(photocell);


  //if bright enough, run camera
  if (brightness > 150)
  {
    Serial.println("bright enough, camera on");
    return true;
    //myCam.capture(); doesnt work in loop here
  }
  else
  {
    Serial.println("too dark, camera paused");
    return false;    
  }
}

void loop() {

  //if (s.available() > 0)
  //{
   // int userIn = Serial.parseInt();
  /*
    //read compass data
    if (userIn == 1)
    {
      Serial.print("Compass data requested, heading is: ");
      compass();
    }
    //read brightness
      
    if (userIn == 2)
    {
      Serial.print("Photocell data requested, brightness is: ");
      Serial.println(brightness);
    }
    */
//myCam.capture();
 brightness = analogRead(photocell);
 //pause camera capturing if its too dark
 if (brightness > 150)
 {
  
  myCam.capture();
  
 }
s.print(encode(getPhotoCell(), compass()));

//I2C testing 
Wire.beginTransmission(1);
Wire.write("test");
Wire.endTransmission();

//getPhotoCell();
//compass();

    //motorCW();
    //halfway
//s.print(encode(getPhotoCell(), compass()));
    //delay(100);
    //rotate to 
    //motorCW();
//s.print(encode(getPhotoCell(), compass()));
    
    //motorCCW();
//s.print(encode(getPhotoCell(), compass()));
    //delay(100);
    //motorCCW();
//s.print(encode(getPhotoCell(), compass()));
    
  //}
  //s.print(encode(getPhotoCell(), compass()));

}

String encode(bool b, float i)
{
  String return_value = "";
  if(b == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }
  String temp = String(i,3);
  return_value += temp;
  return_value += '\n';
  Serial.println(return_value);
  return return_value;

}
