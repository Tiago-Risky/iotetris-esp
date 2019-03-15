#include <Wire.h>
#include "DHT.h"

// DHT Sensor setup
#define DHTTYPE DHT11 // Sensor type
const int DHTPin = 7; // Sensor pin

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];

void setup()
{
  dht.begin();
  Wire.begin(9);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  
  Serial.begin(9600);           // start serial for output
  Serial.println("/// Slave - Start ///"); 
}

void loop()
{
  delay(100);
}

void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read();      // receive byte as a character
    Serial.print(c);           // print the character
  }
  char x = Wire.read();         // receive byte as an integer
  Serial.println(x);           // print the integer
}

void requestEvent(int howMany){
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp,"Failed");
    strcpy(humidityTemp, "Failed");
  }
  else{
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    float hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    dtostrf(h, 6, 2, humidityTemp);

    Wire.write(humidityTemp);
  }
}
