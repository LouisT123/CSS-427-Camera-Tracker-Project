//MAC Address:  38:2B:78:03:83:8B
//D20 D21
//i2c D1,D2
const int RXpin = D3;
const int TXpin = D4;

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h>
SoftwareSerial s(RXpin,TXpin);
uint8_t broadcastAddress[] = {0x50,0x02,0x91,0xDC,0xE7,0x3E};
//I2C
#include <Wire.h>

bool photocellStatus = false;
float compassData;

typedef struct SensorData{
  bool b;
  float f;
};

typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

typedef struct test{
  String str;
}test;
String sss = "test";
// Create a struct_message called myData
struct_message myData;
test receive;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

void setup() {
  //I2C attempt
  Wire.begin(1);
  Wire.onReceive(cameraReader);
  //Wire.onRequest(cameraRequest);
  

  
  Serial.begin(115200);
  s.begin(9600);
  pinMode(RXpin, INPUT);
  pinMode(TXpin,OUTPUT);
  while(!Serial);
  while(!s);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_register_recv_cb(OnDataRecv);

}
 
void loop() {
  delay(1000);
  if ((millis() - lastTime) > timerDelay) {
    //strcpy(myData.a, "THIS IS A CHAR");

    //esp_now_send(broadcastAddress, (uint8_t *) &sss, sizeof(sss));

    lastTime = millis();
  }


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
    SensorData temp;
    temp.b = photocellStatus;
    temp.f = compassData;
    esp_now_send(broadcastAddress, (uint8_t *) &temp, sizeof(temp));    
    Serial.println("pc");
    Serial.println(photocellStatus);
    Serial.println("cp");
    Serial.println(compassData); 
  }

}

//I2C attempt camera
void cameraReader(int j)
{
  while (Wire.available())
  {
     Serial.println("recieving from camera, hopefully");
  }
 
}
/*void cameraRequest()
{
  Wire.write(1);
} */

void OnDataRecvCamera(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&receive, incomingData, sizeof(receive));
  Serial.println(receive.str);
  delay(100);
  s.print(receive.str + '\n');
}
//end of I2C test 

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    //Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&receive, incomingData, sizeof(receive));
  Serial.println(receive.str);
  delay(100);
  s.print(receive.str + '\n');
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
