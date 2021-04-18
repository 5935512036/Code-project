#include "painlessMesh.h"
#include "ArduinoJson.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFiClientSecure.h>

// ตั้งค่า key ของ Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "pokmang19"
#define AIO_KEY  "aio_FVrc43r7OnmnmuaBfsnMgitMkGCr"
WiFiClient client;

// ตั้งค่าชื่อ wifi
#define WIFI_SSID "usman-19-"
#define WIFI_PASS "pokmang19"

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

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

bool current = false;

#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
// Prototype
void receivedCallback( uint32_t from, String &msg );


// Send my ID every 2 seconds to inform others
Task logServerTask(2000, TASK_FOREVER, []() {
#if ARDUINOJSON_VERSION_MAJOR==6
  DynamicJsonDocument jsonBuffer(1024);
  JsonObject msg = jsonBuffer.to<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& msg = jsonBuffer.createObject();
#endif
  msg["topic"] = "I'm Server";
  msg["nodeId"] = mesh.getNodeId();

  String str;
#if ARDUINOJSON_VERSION_MAJOR==6
  serializeJson(msg, str);
#else
  msg.printTo(str);
#endif
  mesh.sendBroadcast(str);

  // log to serial
#if ARDUINOJSON_VERSION_MAJOR==6
  serializeJson(msg, Serial);
#else
  msg.printTo(Serial);
#endif
  Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to Adafruit IO");
  io.connect(); // เริ่มเชื่อมต่อ Adafruit IO
  while (io.status() < AIO_CONNECTED) { //เช็คว่าเชื่อมต่อสำเร็จ
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText()); //แสดงข้อความสถานะการเชื่อมต่อกับ Adafruit IO
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE | DEBUG ); // all types on
  //mesh.setDebugMsgTypes( ERROR | CONNECTION | SYNC | S_TIME );  // set before init() so that you can see startup messages
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
  io.run(); // เริ่มทำงานร่วมกับ Adafruit IO
  // it will run the user scheduler as well
  mesh.update();
}

// รับค่าตอบกลับจาก clieant
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());

  // จองพื้นที่ในการเก็บข้อมูล json
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if (error)
    return;
  // กำหนดตัวแปร เพื่อดงค่า ใน json จาก client   
  int value = doc["value"];
  String topic = doc["topic"];
  if ( value == 1) {
    Serial.printf("from:%s\n", topic.c_str());
    digitalnode1->save(value); // ส่งค่าไปที่ feed ชื่อ vr
  }
  if ( value == 2) {
    Serial.printf("from:%s\n", topic.c_str());
    digitalnode2->save(value); // ส่งค่าไปที่ feed ชื่อ vr

  }

  delay(2000);
}

boolean MQTT_connect() {  
  int8_t ret; 
  if (mqtt.connected()) {    
    return true; 
  }  
  uint8_t retries = 3;  
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect(); delay(2000);
    retries--;
    if (retries == 0) {
      return false; 
    }
  }
  return true;
}

void sendDataToAdafruit(Adafruit_MQTT_Publish feed_t, Adafruit_MQTT_Publish feed_h, Adafruit_MQTT_Publish feed_soil, Adafruit_MQTT_Publish feed_ldr, 
                        Adafruit_MQTT_Publish feed_n_day, int t, int h, int soil, int ldr, int n_day) {
  if (MQTT_connect()) {
    if(feed_t.publish(t) && feed_h.publish(h) && feed_soil.publish(soil) && feed_ldr.publish(ldr) && feed_n_day.publish(n_day)) {
      Serial.println("Sensors sent successfully.");
    }
    else {
      Serial.println("Problem to send the data!");
    }
  }
  else {
    Serial.println("Problem connect to the site!");
  }
}
