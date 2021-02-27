#include "DHT.h"

#define DHTPIN D4   // ขา Out ของ Sensor ต่อเข้าขา D4 ของ Esp8266

//ชนิดของ Sensor
#define DHTTYPE DHT11     //DHT 11


DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
}

int value = 0;

void loop() {
  delay(5000);
  ++value;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" \n");



}
