#include <Wire.h>
#include <ESP8266WiFi.h>

void setup() {
  Wire.begin(SDA,SCL);

  Serial.begin(115200);
  Serial.println("*** MASTER - START ***");
}

void loop() {

  Wire.requestFrom(9, 20);
  while(Wire.available()){
    char c = Wire.read();
    Serial.print(c);
    }
  Serial.println();
  delay(500);
}
