#include <Wire.h>
#include <ESP8266WiFi.h>

/*List all the addresses for the blocks
These should be matching with the address
of each block
*/
int addresses[] {8, 9};


int loopPeriod = 10000; // in ms

void setup() {
  Wire.begin(SDA,SDL);

  Serial.begin(9600);
  Serial.println("IoTetris - Master Start");
}

void loop()
{
  masterRequestLoop();
}

void masterRequestLoop()
{
  int size = sizeof(addresses)/sizeof(addresses[0]);
    for(int x=0; x<size; x++){
        wakeUpSlave(addresses[x]);
        delay(1000);
        _readSlave(addresses[x]);
    }
    //This delay is to be changed for a sleep later on
    delay(loopPeriod);
}

void wakeUpSlave(int slave)
{
  Serial.println("Waking up slave" + String(slave));
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
  Serial.println("Reading from slave" + String(slave));
  Wire.requestFrom(slave, 20);
  while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
  }
  Serial.println();
}