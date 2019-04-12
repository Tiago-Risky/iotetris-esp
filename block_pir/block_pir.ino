#include <Wire.h>
#include <avr/sleep.h>
#include <IoTetris_slave.h>

boolean debug = false;

IoTetris_Slave tetris;
const int neopixelSize = 2;
const int neopixelValues[neopixelSize][3] = {{200,20,20},{20,200,20}};
const int neopixelPin = 4;

#define INTERRUPT_PIN 2

// String to be sent via i2c
static String fstring = "";

// Flag to go to sleep
boolean flagMaster = false;
boolean flagPIR = false;

// People counter
static int counter = 0;

void setup()
{
  tetris.start(8, receiveEvent, requestEvent);
  tetris.changeNeopixel(neopixelPin,68,neopixelValues,neopixelSize);
  
  if(debug) Serial.begin(9600);           // start serial for output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  if(debug) Serial.println("IoTetris: Start");
  delay(500); //delay needed

  go_to_sleep();
}

void loop()
{
  fstring = "p";
  if(counter<10) fstring = fstring + "0";
  fstring = fstring + String(counter);
  delay(250);
  if(flagMaster)
  {
    const int ncolours[neopixelSize][3] = {{20,20,190},{20,20,190}};
    blink(counter, ncolours);
    counter = 0;
    flagMaster = false;
    delay(100);
    if(debug) Serial.println("Sent to master");
    go_to_sleep();
  }
  if(flagPIR)
  {
    blink();
    counter++;
    flagPIR=false;
    delay(100);
    if(debug) Serial.println("Person");
    if(debug) Serial.println(counter);
    go_to_sleep();
  } 
}

void requestEvent(int howMany)
{
  Wire.write(fstring.c_str());
  flagMaster = true;
}

void receiveEvent(int howMany)
{
  sleep_disable();
}

void go_to_sleep()
{
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN),wakeUp, RISING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  if(debug) Serial.println("IoTetris: Going to sleep");
  delay(100);
  sleep_cpu();
  if(debug) Serial.println("IoTetris: Continuing");
  digitalWrite(LED_BUILTIN, HIGH);
}

void wakeUp()
{
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));
  flagPIR=true;
}

void blink()
{
  tetris.changeNeopixel(neopixelPin,0,neopixelValues,neopixelSize);
  delay(500);
  tetris.changeNeopixel(neopixelPin,68,neopixelValues,neopixelSize);
  delay(500);
}

void blink(int times)
{
  for(int x=0;x<times;x++){
  tetris.changeNeopixel(neopixelPin,0,neopixelValues,neopixelSize);
  delay(500);
  tetris.changeNeopixel(neopixelPin,68,neopixelValues,neopixelSize);
  delay(500);
  }
}

void blink(int times, int colours[][3])
{
  for(int x=0;x<times;x++){
  tetris.changeNeopixel(neopixelPin,0,colours,neopixelSize);
  delay(500);
  tetris.changeNeopixel(neopixelPin,68,colours,neopixelSize);
  delay(500);
  }
  tetris.changeNeopixel(neopixelPin,68,neopixelValues,neopixelSize);
}
