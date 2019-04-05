/*
Thanks to Rui Santos from Random Nerd Tutorials

Guides I followed as reference:
https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
https://randomnerdtutorials.com/esp8266-and-node-red-with-mqtt/


*/

// Libs
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Network login (to be replaced with Wifi Manager?)
const char* ssid = "beep";
const char* password = "boop";

// DHT Sensor setup
#define DHTTYPE DHT11 // Sensor type
const int DHTPin = D5; // Sensor pin

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];

// MQTT setup
#define mqtt_server  "192.168.1.109"
#define mqtt_name  "ESPnr1"

#define humidity_topic    "sensor1/humidity"
#define temperature_topic    "sensor1/temperature"

// Sleep time between updates, in seconds
#define sleepTime 60

// Initializes the espClient.
// The espClient name should be unique
WiFiClient espClient;
PubSubClient client(espClient);

// Setup
void setup() {
  // Starting serial (for debugging)
  long startTime = millis();
  Serial.begin(9600);
  while(!Serial) { };
  Serial.println("");
  Serial.println("Starting");

  // Starting DHT library
  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("ESP IP:");
  Serial.println(WiFi.localIP());

  // Setting up MQTT
  client.setServer(mqtt_server, 1883);

  Serial.println("New read");
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

    // Debug
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C\t Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");

    //Publishing to the MQTT
    client.publish(humidity_topic, humidityTemp, true);
    client.publish(temperature_topic, celsiusTemp, true);
    Serial.println("MQTT Message Sent");
  }

  long now = millis();
  long tdif = 900 - (now - startTime);
  Serial.println("Going to sleep");
  ESP.deepSleep((sleepTime*1000000)+(tdif*1000));
}

// Loop
void loop() {

}
