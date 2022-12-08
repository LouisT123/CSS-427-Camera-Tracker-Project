//install library from https://www.arduino.cc/reference/en/libraries/liquidcrystal/
// include the library code:
#include <LiquidCrystal.h>

//pins for buttons
const char pinA = A1;
const char pinB = A0;

char a_byte = 0;// defining a character data for a variable
String a_str = "";// creating a string having space

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//stepper motor
#include <Stepper.h>
const int stepsPerRevolution = 390;  // change this to fit the number of steps per revolution
// for your motor

//set up motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 7, 9,8, 10);
int startLocation = 1;
int CW = 0;
int CCW = 0;

void setup() {
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //reset lcd:
  lcd.clear();
   //set up motor speed
  myStepper.setSpeed(20);

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

}

void loop() {
  checkButton();
  //motorCW();

  checkButton();
  //motorCW();

  checkButton();
  //motorCCW();

  checkButton();
  //motorCCW();
}

void checkButton()
{
    bool state = digitalRead(pinB); 
    bool stateA = digitalRead(pinA); 
    //if button state is pressed
    if (state == false)
    {
      Serial.println("button B send compass:"); //print to serial (can prob delete later, just for testing)
      //cannot trigger multiple times if holding button
      while(digitalRead(pinB)== false)
      {
        //do nothing
      }
      lcd.print("compass: XXX.X"); //purely to test lcd, later on replace mesage with incoming data variable
      delay(1000);
      lcd.clear();
    }

    if (stateA == false)
    {
      Serial.println("button A send photocell:");
      while(digitalRead(pinA)== false)
      {
        //do nothing
      }
      lcd.print("photocell: XXX"); //purely to test lcd, later on replace mesage with incoming data variable
      delay(1000);
      lcd.clear();
    }
}


void motorCW()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    CW = (stepsPerRevolution);
    myStepper.step(CW);
    delay(1000);
    Serial.println("at location A");

}

void motorCCW()
{
  // step one revolution  in the other direction:
    Serial.println("counterclockwise");
    CCW = (-stepsPerRevolution);
    myStepper.step(CCW);
    delay(1000);
    Serial.println("at location B");
}
