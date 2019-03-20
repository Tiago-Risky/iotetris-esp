#include <Wire.h>
#include <ESP8266WiFi.h>

void setup() {
  Wire.begin(SDA,SCL);

  Serial.begin(9600);
  Serial.println("*** MASTER - START ***");
}

void loop()
{
  wakeUpSlave(9);
  delay(1000);
  readSlave(9);
}

void wakeUpSlave(int slave)
{
  Wire.beginTransmission(slave);
  Wire.write("beep");
  Wire.endTransmission();
}

void readSlave(int slave)
{
  /* Might want to change this from void
  so we can gather the data and process it in the loop
  maybe? 
  
  Also add the number of bytes to the args
  if needed*/
  Wire.requestFrom(slave, 20);
  while(Wire.available()){
    char c = Wire.read();
    Serial.print(c);
    }
  Serial.println();
  delay(10000);
}
