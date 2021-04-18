#include "painlessMesh.h"
#include <WiFi.h> 
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// กำหนด IP หรือ Domain ของ Server MQTT
const char* mqtt_server = "143.198.201.74"; 
long lastMsg = 0;
int value = 0;

WiFiClient espClient;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// เชื่อมต่อ mqtt
PubSubClient client(mqtt_server, 1883, callback, espClient);

// กำหนดเครื่อข่าย Mesh
#define   MESH_PREFIX     "iotrabber"
#define   MESH_PASSWORD   "iotrabber"
#define   MESH_PORT       5555


painlessMesh  mesh;



void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );              // set ค่าก่อนเริ่มต้นแสดงสถานะ
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 ); // กำหนด 6 ช่องอุปกรณ์
  mesh.initOTAReceive("bridge");                                      // เริ่มต้นส่งค่า  OTAReceive  ส่งค่าผ่านอากาศ
  mesh.setRoot(true);                                                 // Set ให้เป็น Node root  หลัก
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);                                  // เปืดการรับข้อมูล
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("bridge: Received !! \n");
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if (error)
    return;
  // กำหนดตัวแปร เพื่อดงค่า ใน json จาก client
  String topic = doc["topic"];
  int PIR = doc["value"];
  int Humidity = doc["Humidity"];
  int Temperature = doc["Temperature"];
  int soil = doc["Soil"];
  int Soil = map(soil, 0, 1024, 100, 0);
  
  if ( topic == "NODE1") {
        WiFi.disconnect();
        delay(3000);
        Serial.println("START");
        WiFi.begin("usman-19-","pokmang19");
        while ((!(WiFi.status() == WL_CONNECTED))){
          delay(300);
          Serial.print("..");
        }
        // เเสดงการเชื่อมต่อ
          Serial.println("Connected");
          Serial.println("Your IP is");
          Serial.println((WiFi.localIP()));
        // เชื่อมต่อกับ mqtt  
        client.connect("Node1", "Muslim2021Dev", "Muslim2021Dev");
        client.setCallback(callback);
        client.subscribe("command");
        // แสดงค่าที่ได้รับ
        Serial.printf("from:%s\n", topic.c_str());
        Serial.printf("PIR: %d\n", PIR);
        Serial.printf("Humidity: %d\n", Humidity);
        Serial.printf("Temperature: %d\n", Temperature);
        Serial.printf("Soil: %d\n", Soil);
        // กำหนดค่าให้เป็น string
        String textPIR = String(PIR);
        String textHumidity = String(Humidity);
        String textTemperature = String(Temperature);
        String textSoil = String(Soil);
        // แปลงค่า เป็น toCharArray
        char textPIRChar[50];
        char textHumidityChar[50];
        char textTemperatureChar[50];
        char textSoilChar[50];
        textPIR.toCharArray(textPIRChar, 50);
        textHumidity.toCharArray(textHumidityChar, 50);
        textTemperature.toCharArray(textTemperatureChar, 50);
        textSoil.toCharArray(textSoilChar, 50);
        
        Serial.print("Publish message: START");
        Serial.print("...");
        // ส่งค่าไปยัง dashbord
        if(PIR == 0 ){
              client.publish("node1/PIR", "No Detect");
          }
        else{
              client.publish("node1/PIR", "Detect");
        }
        client.publish("node1/Humidity", textHumidityChar);
        client.publish("node1/Temperature", textTemperatureChar);
        client.publish("node1/Soil", textSoilChar);

        // ค่าสมมุติ node 3,4
        client.publish("node3/PIR", "No Detect");
        client.publish("node3/Humidity", textHumidityChar);
        client.publish("node3/Temperature", textTemperatureChar);
        client.publish("node3/Soil", textSoilChar);
        client.publish("node4/PIR", "No Detect");
        client.publish("node4/Humidity", textHumidityChar);
        client.publish("node4/Temperature", textTemperatureChar);
        client.publish("node4/Soil", textSoilChar);
        
        Serial.print("\nPublish message: FINISH");
  }


  
  if ( topic == "NODE2") {
        WiFi.disconnect();
        delay(3000);
        Serial.println("START");
        WiFi.begin("usman-19-","pokmang19");
        while ((!(WiFi.status() == WL_CONNECTED))){
          delay(300);
          Serial.print("..");
        }
        // เเสดงการเชื่อมต่อ
          Serial.println("Connected");
          Serial.println("Your IP is");
          Serial.println((WiFi.localIP()));
        // เชื่อมต่อกับ mqtt  
        client.connect("Node1", "Muslim2021Dev", "Muslim2021Dev");
        client.setCallback(callback);
        client.subscribe("command");
        // แสดงค่าที่ได้รับ
        Serial.printf("from:%s\n", topic.c_str());
        Serial.printf("PIR: %d\n", PIR);
        Serial.printf("Humidity: %d\n", Humidity);
        Serial.printf("Temperature: %d\n", Temperature);
        Serial.printf("Soil: %d\n", Soil);
        // กำหนดค่าให้เป็น string
        String textPIR = String(PIR);
        String textHumidity = String(Humidity);
        String textTemperature = String(Temperature);
        String textSoil = String(Soil);
        // แปลงค่า เป็น toCharArray
        char textPIRChar[50];
        char textHumidityChar[50];
        char textTemperatureChar[50];
        char textSoilChar[50];
        textPIR.toCharArray(textPIRChar, 50);
        textHumidity.toCharArray(textHumidityChar, 50);
        textTemperature.toCharArray(textTemperatureChar, 50);
        textSoil.toCharArray(textSoilChar, 50);
        
        Serial.print("Publish message: START");
        Serial.print("...");
        // ส่งค่าไปยัง dashbord
        if(PIR == 0 ){
              client.publish("node2/PIR", "No Detect");
          }
        else{
              client.publish("node2/PIR", "Detect");
        }
        client.publish("node2/Humidity", textHumidityChar);
        client.publish("node2/Temperature", textTemperatureChar);
        client.publish("node2/Soil", textSoilChar);

                // ค่าสมมุติ node 3,4
        client.publish("node3/PIR", "No Detect");
        client.publish("node3/Humidity", textHumidityChar);
        client.publish("node3/Temperature", textTemperatureChar);
        client.publish("node3/Soil", textSoilChar);
        client.publish("node4/PIR", "No Detect");
        client.publish("node4/Humidity", textHumidityChar);
        client.publish("node4/Temperature", textTemperatureChar);
        client.publish("node4/Soil", textSoilChar);
        Serial.print("\nPublish message: FINISH");

  }
  if ( topic == "NODE3") {
        WiFi.disconnect();
        delay(3000);
        Serial.println("START");
        WiFi.begin("usman-19-","pokmang19");
        while ((!(WiFi.status() == WL_CONNECTED))){
          delay(300);
          Serial.print("..");
        }
        // เเสดงการเชื่อมต่อ
          Serial.println("Connected");
          Serial.println("Your IP is");
          Serial.println((WiFi.localIP()));
        // เชื่อมต่อกับ mqtt  
        client.connect("Node1", "Muslim2021Dev", "Muslim2021Dev");
        client.setCallback(callback);
        client.subscribe("command");
        // แสดงค่าที่ได้รับ
        Serial.printf("from:%s\n", topic.c_str());
        Serial.printf("PIR: %d\n", PIR);
        Serial.printf("Humidity: %d\n", Humidity);
        Serial.printf("Temperature: %d\n", Temperature);
        Serial.printf("Soil: %d\n", Soil);
        // กำหนดค่าให้เป็น string
        String textPIR = String(PIR);
        String textHumidity = String(Humidity);
        String textTemperature = String(Temperature);
        String textSoil = String(Soil);
        // แปลงค่า เป็น toCharArray
        char textPIRChar[50];
        char textHumidityChar[50];
        char textTemperatureChar[50];
        char textSoilChar[50];
        textPIR.toCharArray(textPIRChar, 50);
        textHumidity.toCharArray(textHumidityChar, 50);
        textTemperature.toCharArray(textTemperatureChar, 50);
        textSoil.toCharArray(textSoilChar, 50);
        
        Serial.print("Publish message: START");
        Serial.print("...");
        // ส่งค่าไปยัง dashbord
        if(PIR == 0 ){
              client.publish("node3/PIR", "No Detect");
          }
        else{
              client.publish("node3/PIR", "Detect");
        }
        client.publish("node3/Humidity", textHumidityChar);
        client.publish("node3/Temperature", textTemperatureChar);
        client.publish("node3/Soil", textSoilChar);
        Serial.print("\nPublish message: FINISH");
  }
  if ( topic == "NODE4") {
        WiFi.disconnect();
        delay(3000);
        Serial.println("START");
        WiFi.begin("usman-19-","pokmang19");
        while ((!(WiFi.status() == WL_CONNECTED))){
          delay(300);
          Serial.print("..");
        }
        // เเสดงการเชื่อมต่อ
          Serial.println("Connected");
          Serial.println("Your IP is");
          Serial.println((WiFi.localIP()));
        // เชื่อมต่อกับ mqtt  
        client.connect("Node1", "Muslim2021Dev", "Muslim2021Dev");
        client.setCallback(callback);
        client.subscribe("command");
        // แสดงค่าที่ได้รับ
        Serial.printf("from:%s\n", topic.c_str());
        Serial.printf("PIR: %d\n", PIR);
        Serial.printf("Humidity: %d\n", Humidity);
        Serial.printf("Temperature: %d\n", Temperature);
        Serial.printf("Soil: %d\n", Soil);
        // กำหนดค่าให้เป็น string
        String textPIR = String(PIR);
        String textHumidity = String(Humidity);
        String textTemperature = String(Temperature);
        String textSoil = String(Soil);
        // แปลงค่า เป็น toCharArray
        char textPIRChar[50];
        char textHumidityChar[50];
        char textTemperatureChar[50];
        char textSoilChar[50];
        textPIR.toCharArray(textPIRChar, 50);
        textHumidity.toCharArray(textHumidityChar, 50);
        textTemperature.toCharArray(textTemperatureChar, 50);
        textSoil.toCharArray(textSoilChar, 50);
        
        Serial.print("Publish message: START");
        Serial.print("...");
        // ส่งค่าไปยัง dashbord
                if(PIR == 0 ){
              client.publish("node4/PIR", "No Detect");
          }
        else{
              client.publish("node4/PIR", "Detect");
        }
        client.publish("node4/Humidity", textHumidityChar);
        client.publish("node4/Temperature", textTemperatureChar);
        client.publish("node4/Soil", textSoilChar);
        Serial.print("\nPublish message: FINISH");
  }
  delay(5000);
}
