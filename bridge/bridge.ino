//************************************************************
// this is a simple example that uses the painlessMesh library to 
// connect to a node on another network. Please see the WIKI on gitlab
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
//************************************************************
#include "painlessMesh.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WiFi.h>
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "pokmang19"
#define AIO_KEY  "aio_FVrc43r7OnmnmuaBfsnMgitMkGCr"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

int  i  ;
boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}


// Node 1
Adafruit_MQTT_Publish node1_pir = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node1_pir");
Adafruit_MQTT_Publish node1_humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node1_humidity");
Adafruit_MQTT_Publish node1_temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node1_temperature");
Adafruit_MQTT_Publish node1_soilmoisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node1_soilmoisture");

// Node 2
Adafruit_MQTT_Publish node2_pir = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node2_pir");
Adafruit_MQTT_Publish node2_humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node2_humidity");
Adafruit_MQTT_Publish node2_temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node2_temperature");
Adafruit_MQTT_Publish node2_soilmoisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/node2_soilmoisture");

#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555

#define   STATION_SSID     "usman-19-"
#define   STATION_PASSWORD "pokmang19"
#define   STATION_PORT     5555
uint8_t   station_ip[4] =  {192,168,1,128}; // IP of the server

// prototypes
void receivedCallback( uint32_t from, String &msg );

painlessMesh  mesh;

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages


  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
  // Setup over the air update support
  mesh.initOTAReceive("bridge");

  mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT, station_ip);
  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  mesh.setContainsRoot(true);


  mesh.onReceive(&receivedCallback);
}

void loop() {
  mesh.update();
//    if (MQTT_connect()) {
//    if(feed_t.publish(t) && feed_h.publish(h) && feed_soil.publish(soil) && feed_ldr.publish(ldr) && feed_n_day.publish(n_day)) {
//      Serial.println("Sensors sent successfully.");
//    }
//      delay(3000)
//    }
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
    // จองพื้นที่ในการเก็บข้อมูล json
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if (error)
    return;
  // กำหนดตัวแปร เพื่อดงค่า ใน json จาก client  
   // กำหนดตัวแปร เพื่อดงค่า ใน json จาก client

  String topic = doc["topic"];
  int pir = doc["value"];
  int humidity = doc["Humidity"];
  int temperature = doc["Temperature"];
  int soilmoisture = doc["Soil"];
  
  if ( topic == "NODE1") {
    Serial.printf("from:%s\n", topic.c_str());
//    sendDataToAdafruit1(node1_pir,node1_humidity,node1_temperature,node1_soilmoisture);

  }
//  if ( topic == "NODE2") {
//    Serial.printf("from:%s\n", topic.c_str());
//    Serial.printf("PIR: %d\n",value);
//    Serial.printf("Humidity: %d\n",Humidity);
//    Serial.printf("Temperature: %d\n",Temperature);
//    Serial.printf("Soil: %d\n\n\n",Soil);
//    sendDataToAdafruit1(value,Humidity,Temperature,Soil)
//  }
}

//void sendDataToAdafruit1(Adafruit_MQTT_Publish feed_Pir, Adafruit_MQTT_Publish feed_Humidity, Adafruit_MQTT_Publish feed_Temperature, Adafruit_MQTT_Publish feed_Soil, 
//                        int pir, int humidity, int temperature, int soilmoisture) {
//  if (MQTT_connect()) {
//    if(feed_Pir.publish(pir) && feed_Humidity.publish(humidity) && feed_Temperature.publish(temperature) && feed_Soil.publish(soilmoisture) ) {
//      Serial.println("Sensors sent successfully.");
//    }
//    else {
//      Serial.println("Problem to send the data!");
//    }
//  }
//  else {
//    Serial.println("Problem connect to the site!");
//  }
//}

//void sendDataToAdafruit2(Adafruit_MQTT_Publish feed_t, Adafruit_MQTT_Publish feed_h, Adafruit_MQTT_Publish feed_soil, Adafruit_MQTT_Publish feed_ldr, 
//                        Adafruit_MQTT_Publish feed_n_day, int t, int h, int soil, int ldr, int n_day) {
//  if (MQTT_connect()) {
//    if(feed_t.publish(t) && feed_h.publish(h) && feed_soil.publish(soil) && feed_ldr.publish(ldr) && feed_n_day.publish(n_day)) {
//      Serial.println("Sensors sent successfully.");
//    }
//    else {
//      Serial.println("Problem to send the data!");
//    }
//  }
//  else {
//    Serial.println("Problem connect to the site!");
//  }
//}
