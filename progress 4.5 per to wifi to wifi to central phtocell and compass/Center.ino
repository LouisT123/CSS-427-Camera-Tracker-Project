#include <Wire.h>
#include <SoftwareSerial.h>

bool photocellStatus;
float compassData;
const int Rx = 2;
const int Tx = 3;
//const int Rx = D2;
//const int Tx = D3;
SoftwareSerial s(Rx, Tx);
int counter = 0;
int trash;
char c;

bool status = false;
void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  s.begin(57600);
  Serial.begin(921600);
  while (!Serial)
    ;
  while (!s)
    ;
}

void receiveEvent() {
  while(Wire.available() > 0)
  {
    byte temp = Wire.read();
    Serial.write(temp);   
  }
   
}
void loop() {
  String data;
  while(s.available())
  {
    
    char temp = s.read();
    if(temp == '\n')
    {
      break;      
    }
    else
    {
      data += temp;            
    }
  }   
  if(data != "")
  {
    decode(data, photocellStatus, compassData);  
    Serial.println("pc");
    Serial.println(photocellStatus);
    Serial.println("cp");
    Serial.println(compassData);    
  }
  /*
  if (Serial.available()) {

    //s.write(Serial.read());
  }
  */
  //Wire.onReceive()

  //original ss code
  /*
  while(s.available())
  {
    Serial.write(s.read());
  }
  */
}

void requestEvent() {

  if (Serial.available()) {
    //Serial.read();
    //Serial.println("1");
    Wire.write(1);
    //Wire.write(Serial.read());
    Wire.write(Serial.read());
  }
  
  /*
  if(Serial.available())
  {
    Serial.println("sent");
    Wire.write(Serial.read());
  }
  */
}


void decode(String d, bool& b, float& f)
{
  char temp = d.charAt(0);
  d = d.substring(1);
  if(temp == '1')
  {
    b = true;
  }
  else
  {
    b = false;    
  }
  f = d.toFloat();
}
