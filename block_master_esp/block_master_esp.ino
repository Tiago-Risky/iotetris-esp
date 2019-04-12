#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*List all the addresses for the blocks
These should be matching with the address
of each block
The second value is the i2c message size
*/
int addresses[][2] {{8,3}};

const int size = sizeof(addresses)/sizeof(addresses[0]);

int loopPeriod = 60000; // in ms

// Network login (to be replaced with Wifi Manager?)
const char* ssid = "beep";
const char* password = "boop";

// MQTT setup
#define mqtt_server  "beep"
#define mqtt_name  "ESPmaster"

#define data_topic    "esp1/data"

// Sleep time between updates, in seconds
#define sleepTime 60

// Initializes the espClient.
// The espClient name should be unique
WiFiClient espClient;
PubSubClient client(espClient);

String result[size][2];

void setup() {
  Wire.begin(SDA,SCL);

  Serial.begin(9600);
  while(!Serial) { };
  Serial.println("IoTetris - Master Start");

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
  client.connect(mqtt_name);

  /*
  Publishing to the MQTT (example)
    client.publish(humidity_topic, humidityTemp, true);
    client.publish(temperature_topic, celsiusTemp, true);
    Serial.println("MQTT Message Sent");
  */
}

void loop()
{
  masterRequestLoop();
  sendPayload();
  //This delay is to be changed for a sleep later on
  delay(loopPeriod);
}

void masterRequestLoop()
{
  for(int x=0; x<size; x++){
      wakeUpSlave(addresses[x][0]);
      delay(2000);
      result[x][0] = addresses[x][0];
      result[x][1] = readSlave(x);
  }
}

void sendPayload()
{
  client.connect(mqtt_name);
  String payload = "{";
  for(int x=0; x<size; x++){
    payload+="\"sensor"+result[x][0]+"\":\"";
    payload+=result[x][1]+"\"";
    if(x<size-1){
      payload+= ",";
    }
  }
  payload += "}";
  Serial.println(payload);
  delay(50);
  boolean mqttsuccess = false;
  for(int attempt=0;attempt<3;attempt++){
    mqttsuccess = pubmystuff(payload);
    if(mqttsuccess) break;
  }
  if(mqttsuccess){
    Serial.println("MQTT Message Sent");
  }else{
    Serial.println("MQTT Error");
  }
  
}

boolean pubmystuff(String payload){
  return client.publish(data_topic,payload.c_str(), true);
}

void wakeUpSlave(int slave)
{
  Serial.println("Waking up slave" + String(slave));
  Wire.beginTransmission(slave);
  Wire.write("beep");
  Wire.endTransmission();
}

String readSlave(int addressVal)
{
  int slave = addresses[addressVal][0];
  int msgSize = addresses[addressVal][1];
  /* Might want to change this from void
  so we can gather the data and process it in the loop
  maybe? 
  Also add the number of bytes to the args
  if needed*/
  String a = "";
  Serial.println("Reading from slave" + String(slave));
  Wire.requestFrom(slave, 20);
  int cont = 0;
  while(Wire.available() && cont<msgSize){
      char c = Wire.read();
      a = a+ String(c);
      Serial.print(c);
      cont++;
  }
  Serial.println();
  return a;
}
