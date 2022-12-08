

//install library from https://www.arduino.cc/reference/en/libraries/liquidcrystal/
// include the library code:
#include <LiquidCrystal.h>

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
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

   //set up motor speed
  myStepper.setSpeed(20);

}

void loop() {
  
// replace all helloWorldTest with serial read once data communication is done
  helloWorldTest();
  motorCW();
  
  delay(100);
  helloWorldTest();

  motorCW();
  delay(100);
  helloWorldTest();

  motorCCW();
  delay(100);
  helloWorldTest();

  motorCCW();
  delay(100);
  helloWorldTest();
}

void serialRead()
{
   // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}

void helloWorldTest()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
   lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
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
