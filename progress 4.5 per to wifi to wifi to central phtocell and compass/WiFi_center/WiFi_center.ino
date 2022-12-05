//MAC Address:  50:02:91:DC:E7:3E
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = {0x38,0x2B,0x78,0x03,0x83,0x8B};
#include <SoftwareSerial.h>
const int Rx = D3;
const int Tx = D4;
SoftwareSerial s (Rx, Tx);
typedef struct SensorData{
  bool b;
  float f;
}SensorData;

SensorData receivedData;
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  String temp = encode(receivedData.b, receivedData.f);
  //s.println(temp);
  //Serial.println(recvstr);
  s.print(temp);
  Serial.println(receivedData.b);
  Serial.println(receivedData.f);
}
 
void setup() {
  Serial.begin(115200);
  s.begin(57600);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  //sendmsg.str = "test";
  //esp_now_send(broadcastAddress,(uint8_t*) &sendmsg, sizeof(sendmsg));
  //delay(1000);
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