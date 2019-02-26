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
static char fahrenheitTemp[7];
static char humidityTemp[7];

// MQTT setup
#define mqtt_server  "192.168.1.109"
#define mqtt_name  "ESPnr1"

#define humidity_topic    "sensor1/humidity"
#define temperature_topic    "sensor1/temperature"

// Initializes the espClient.
// The espClient name should be unique
WiFiClient espClient;
PubSubClient client(espClient);

// Setup
void setup() {
  // Starting serial (for debugging)
  Serial.begin(9600);
  delay(10);

  // Starting DHT library
  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Waiting for the ESP IP...");
  delay(10000); // does it really need this much delay...? 

  Serial.println(WiFi.localIP());

  // Setting up MQTT
  client.setServer(mqtt_server, 1883);
}

// Loop
long lastMsg = 0;

void loop() {
  
  // Reconnecting if needed
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    Serial.println("New read");
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      strcpy(celsiusTemp,"Failed");
      strcpy(fahrenheitTemp, "Failed");
      strcpy(humidityTemp, "Failed");
    }
    else{
      // Computes temperature values in Celsius + Fahrenheit and Humidity
      float hic = dht.computeHeatIndex(t, h, false);
      dtostrf(hic, 6, 2, celsiusTemp);
      float hif = dht.computeHeatIndex(f, h);
      dtostrf(hif, 6, 2, fahrenheitTemp);
      dtostrf(h, 6, 2, humidityTemp);

      // Debug
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t Temperature: ");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(f);
      Serial.print(" *F\t Heat index: ");
      Serial.print(hic);
      Serial.print(" *C ");
      Serial.print(hif);
      Serial.println(" *F");

      //Publishing to the MQTT
      client.publish(humidity_topic, humidityTemp, true);
      client.publish(temperature_topic, celsiusTemp, true);
      Serial.println("MQTT Message Sent");
    }
  }
}   

// Reconneting to the MQTT broker 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_name)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

