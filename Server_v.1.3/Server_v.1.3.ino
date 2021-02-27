#include "painlessMesh.h"
#include <SoftwareSerial.h>
#include "painlessMesh.h"

#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555

SoftwareSerial swSerial(D1, D2, false, 128); //Define hardware connections RX, TX

Scheduler     userScheduler; 
painlessMesh  mesh;

// Send my ID every 10 seconds to inform others
Task logServerTask(10000, TASK_FOREVER, []() {
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

  // Add the task to the mesh scheduler
  mesh.scheduler.addTask(logServerTask);
  logServerTask.enable();
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());
  
  swSerial.print(msg.c_str()); // Print received data in Mesh Network to Software Serial Port
  
}
