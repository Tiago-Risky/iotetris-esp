#include <Wire.h>
#include "DHT.h"
#include <avr/sleep.h>
#include <IoTetris_slave.h>

IoTetris_Slave tetris;
const int neopixelSize = 2;
int neopixelValues[neopixelSize][3] = {{20,20,200},{200,20,20}};

#define INTERRUPT_PIN 2

// DHT Sensor setup
#define DHTTYPE DHT11 // Sensor type
const int DHTPin = 7; // Sensor pin

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// String to be sent via i2c
static char finalString[20];

// Flag to go to sleep
boolean flag = false;

void setup()
{
  dht.begin();
  tetris.start(9, receiveEvent, requestEvent);
  tetris.setNeopixel(5,68,neopixelValues,neopixelSize);
  
  Serial.begin(9600);           // start serial for output
  pinMode(LED_BUILTIN, OUTPUT);
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	Serial.println("IoTetris: Start");
	delay(500);	//delay needed
}

void loop()
{
  readDHT11();
  delay(250);
  if(flag)
  {
    flag = false;
    delay(100);
    go_to_sleep();
  }
  
}

void readDHT11(){
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    strcpy(finalString,"DHT Sensor Failed");
  }
  else{
    // Computes Heat Index temperature (apparent temperature)
    float hic = dht.computeHeatIndex(t, h, false);
    String fstr = String(h) + "h"+ String(hic) + "ta"+String(t) + "tr";
    strcpy(finalString,fstr.c_str());
  }
}

void requestEvent(int howMany)
{
  Wire.write(finalString);
  flag = true;
}

void receiveEvent(int howMany)
{
  sleep_disable();
}

void go_to_sleep()
{
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
	Serial.println("IoTetris: Going to sleep");
	delay(100);
	sleep_cpu();
	Serial.println("IoTetris: Continuing");
	digitalWrite(LED_BUILTIN, HIGH);
}
