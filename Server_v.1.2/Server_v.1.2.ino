#include "painlessMesh.h"
#include <SoftwareSerial.h>

#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555

Scheduler     userScheduler; 
painlessMesh  mesh;
SoftwareSerial mySerial(D1, D2); //Define hardware connections RX, TX

// Send my ID every 2 seconds to inform others
Task logServerTask(2000, TASK_FOREVER, []() {
  DynamicJsonDocument jsonBuffer(1024);
  JsonObject msg = jsonBuffer.to<JsonObject>();
  msg["topic"] = "I'm Server";
  msg["nodeId"] = mesh.getNodeId();

  String str;
  serializeJson(msg, str);
  mesh.sendBroadcast(str);

  // log to serial
  serializeJson(msg, Serial);
  Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(115200); //Initialize hardware serial with baudrate of 115200
  swSerial.begin(115200);    //Initialize software serial with baudrate of 115200
  
  mesh.setDebugMsgTypes( ERROR | CONNECTION | S_TIME );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);
  
  mesh.onNewConnection([](size_t nodeId) {
    Serial.printf("New Connection %u\n", nodeId);
  });

  mesh.onDroppedConnection([](size_t nodeId) {
    Serial.printf("Dropped Connection %u\n", nodeId);
  });

  // Add the task to the your scheduler
  userScheduler.addTask(logServerTask);
  logServerTask.enable();
}

void loop() {
  mesh.update();
}

// รับค่าตอบกลับจาก clieant
void receivedCallback( uint32_t from, String &msg ) {
  // จองพื้นที่ในการเก็บข้อมูล json
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if (error)
    return;
  // กำหนดตัวแปร เพื่อดงค่า ใน json จาก client
  String topic = doc["topic"];
  int value = doc["value"];
  int Humidity = doc["Humidity"];
  int Temperature = doc["Temperature"];
  int Soil = doc["Soil"];
  
  if ( topic == "NODE1") {
    Serial.printf("from:%s\n", topic.c_str());
    Serial.printf("PIR: %d\n",value);
    Serial.printf("Humidity: %d\n",Humidity);
    Serial.printf("Temperature: %d\n",Temperature);
    Serial.printf("Soil: %d\n\n\n",Soil);
 
  }
  if ( topic == "NODE2") {
    Serial.printf("from:%s\n", topic.c_str());
    Serial.printf("PIR: %d\n",value);
    Serial.printf("Humidity: %d\n",Humidity);
    Serial.printf("Temperature: %d\n",Temperature);
    Serial.printf("Soil: %d\n\n\n",Soil);
  }
   Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());
  
  mySerial.print(msg.c_str()); // Print received data in Mesh Network to Software Serial Port
  delay(2000);
}
