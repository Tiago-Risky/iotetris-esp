#include <Wire.h>
#include <avr/sleep.h>
#include <IoTetris_slave.h>

IoTetris_Slave tetris;
const int neopixelSize = 2;
const int neopixelValues[neopixelSize][3] = {{200,20,20},{20,200,20}};

#define INTERRUPT_PIN 2

// String to be sent via i2c
static char finalString[20];

// Flag to go to sleep
boolean flagMaster = false;
boolean flagPIR = false;

// People counter
int counter = 0;

void setup()
{
  tetris.start(8, receiveEvent, requestEvent);
  tetris.setNeopixel(4,68,neopixelValues,neopixelSize);
  
  Serial.begin(9600);           // start serial for output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("IoTetris: Start");
  delay(500); //delay needed

  go_to_sleep();
}

void loop()
{
  delay(250);
  if(flagMaster)
  {
    counter = 0;
    flagMaster = false;
    delay(100);
    go_to_sleep();
  }
  if(flagPIR)
  {
    counter++;
    flagPIR=false;
    delay(100);
    go_to_sleep();
  } 
}

void requestEvent(int howMany)
{
  strcpy(finalString,String(counter).c_str());
  Wire.write(finalString);
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
  Serial.println("IoTetris: Going to sleep");
  delay(100);
  sleep_cpu();
  Serial.println("IoTetris: Continuing");
  digitalWrite(LED_BUILTIN, HIGH);
}

void wakeUp()
{
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));
  flagPIR=true;
}
