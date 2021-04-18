#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const char* ssid = "usman-19-";  //wifi ssid
const char* password = "pokmang19"; //wifi passwd


SoftwareSerial swSerial(D1, D2, false, 256); //Define hardware connections RX, TX


void setup() {
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(115200);   //Initialize hardware serial with baudrate of 115200
  swSerial.begin(115200);    //Initialize software serial with baudrate of 115200
  

}
void loop() {
   while (swSerial.available() > 0)
  {
    swSerial.print(msg.c_str()); 
  }

  delay(1000);
}
