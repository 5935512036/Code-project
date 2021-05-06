/* 
 * ไลบรารี่ TridentTD_LineNotify version 2.1
 * ใช้สำหรับ ส่ง แจ้งเตือนไปยัง LINE สำหรับ ESP8266 และ ESP32
 * สามารถส่งได้ทั้ง ข้อความ , สติกเกอร์ และรูปภาพ(ด้วย url)
 * -----------------------------------------------------
 * ให้ save เป็น file ต่างหากก่อนถึงจะส่ง Line Notify ภาษาไทยได้
 */

#include <TridentTD_LineNotify.h>

#define SSID        "usman-19-"
#define PASSWORD    "pokmang19"
#define LINE_TOKEN  "HerQnLo9kojniXHIIilHAdjsQhs4nAtFvChclfRP576"

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());
  
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("เตือน!! \n มีผู้บุกรุกจุดที่1 : พบ \n Temperature: 32c \n Humidity: 30% \n Soil: 40%");
  LINE.notify("เตือน!! \n มีผู้บุกรุกจุดที่2 : ไม่พบ \n Temperature: 32c \n Humidity: 30% \n Soil: 40%");
  LINE.notify("เตือน!! \n มีผู้บุกรุกจุดที่3 : ไม่พบ \n Temperature: 31c \n Humidity: 37% \n Soil: 39%");
  LINE.notify("เตือน!! \n มีผู้บุกรุกจุดที่4 : ไม่พบ \n Temperature: 32c \n Humidity: 35% \n Soil: 48%");

}

void loop() {
  delay(1);
}
