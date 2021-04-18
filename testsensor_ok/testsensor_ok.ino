int sensor = D0;  // Digital pin D7
int state = 0;
void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(115200);
  
}
void loop() {
   state = digitalRead(sensor);
  if (state == HIGH) {
    Serial.println(" detected!");
  }
  else {
    Serial.println("NO no.");  
  }
  delay(500);
}

//int sensor = D0;  // Digital pin D7
//void setup() {
//  pinMode(sensor, INPUT_PULLUP);
//  Serial.begin(115200);
//  attachInterrupt(digitalPinToInterrupt(sensor),interrupt,RISING);
//}
//void loop() {
//  Serial.println("NO");
//}
//void interrupt(){
//  Serial.println("detected");
//}
