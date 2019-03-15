#include <Wire.h>
#include "DHT.h"
#include <avr/sleep.h>

#define INTERRUPT_PIN 2

// DHT Sensor setup
#define DHTTYPE DHT11 // Sensor type
const int DHTPin = 7; // Sensor pin

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];

boolean flag = false;

void setup()
{
  dht.begin();
  Wire.begin(9);                // join i2c bus with address #9
  Wire.onRequest(requestEvent);
  
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
  //readDHT11();
  if(flag)
  {
    flag = false;
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
    //Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp,"Failed");
    strcpy(humidityTemp, "Failed");
  }
  else{
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    float hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    dtostrf(h, 6, 2, humidityTemp);
  }
}


void requestEvent(int howMany){
  //delay(500);
  Wire.write("hello");
  flag = true;
}

void go_to_sleep()
{
	sleep_enable();
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wakeUp_function, LOW);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
	Serial.println("IoTetris: Going to sleep");
	delay(100);
	sleep_cpu();
	Serial.println("IoTetris: Continuing");
	digitalWrite(LED_BUILTIN, HIGH);
}
void wakeUp_function()
{
	Serial.println("IoTetris: Interrupt");
	sleep_disable();
	detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));
}
