#include <Wire.h>
#include <ESP8266WiFi.h>
#include <IoTetris.h>

IoTetris_I2C tetris;

int addresses[] {9};

void setup() {
  tetris.masterStart(addresses,SDA,SCL);

  Serial.begin(9600);
  Serial.println("*** MASTER - START ***");
}

void loop()
{
  tetris.masterRequestLoop();
}
