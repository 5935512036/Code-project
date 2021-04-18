#include "painlessMesh.h"
#include "DHT.h"
#define DHTPIN 2   // ขา Out ของ Sensor ต่อเข้าขา D4 ของ Esp8266
#define DHTTYPE DHT11     //DHT 11  ชนิดของ Sensor
DHT dht(DHTPIN, DHTTYPE);

int sensorPin = 17; 
int sensor = D0;  // Digital pin D7
int state = 0;
int value = 0;

int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );

size_t logServerId = 0;

// 2,000 ส่งข้อมูลให้ server node ทุกๆ 2 นาที 
Task myLoggingTask(2000, TASK_FOREVER, []() {
 
#if ARDUINOJSON_VERSION_MAJOR==6
        DynamicJsonDocument jsonBuffer(1024);
        JsonObject msg = jsonBuffer.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& msg = jsonBuffer.createObject();
#endif
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog ขา5 ที่ต่อกับ Soil Moisture Sensor Module v1
    state = digitalRead(sensor);
    ++value;
    msg["topic"] = "NODE2";
    msg["value"] = (String)state;
    msg["Humidity"] = (String)h;
    msg["Temperature"] = (String)t;
    msg["Soil"] = (String)val;
    String str;

    
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, str);
#else
    msg.printTo(str);
#endif
  // การส่งข้อความไปยังโหนดต่างๆมีด้วยกัน 2 วิธี 
    if (logServerId == 0) // If we don't know the logServer yet
        mesh.sendBroadcast(str);     //ส่งข้อความไปยังทุกโหนดที่กำลังเชื่อมต่อ
    else
        mesh.sendSingle(logServerId, str); //ส่งข้อความไปยังโหนดใดโหนดหนึ่งโดยเราจะต้องทำการระบุหมายเลข ChipID ของโหนดที่เราต้องการจะส่งข้อความไปให้ไว้ในส่วนของ dest

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
  pinMode(sensor, INPUT);
  dht.begin();
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  // Add the task to the your scheduler
  userScheduler.addTask(myLoggingTask);
  myLoggingTask.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

}

//แสดงข้อความที่ถูกส่งมาจากโหนดอื่นๆ
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());

  // Saving logServer
#if ARDUINOJSON_VERSION_MAJOR==6
  DynamicJsonDocument jsonBuffer(1024 + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
#endif
  if (root.containsKey("topic")) {
      if (String("logServer").equals(root["topic"].as<String>())) {
          // check for on: true or false
          logServerId = root["nodeId"];
          Serial.printf("logServer detected!!!\n");
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());
  }
}
