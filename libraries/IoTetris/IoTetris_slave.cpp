#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_NeoPixel.h"
#include "IoTetris_slave.h"

IoTetris_Slave::IoTetris_Slave()
{
}

// address for i2c, onReceive function, onRequest function
void IoTetris_Slave::start(int address, void (*onReceive)(size_t), void (*onRequest)(void))
{
    Wire.begin(address);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

void IoTetris_Slave::changeNeopixel(int pin, int brightness, int values[][3], int size)
{
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(size,pin,NEO_GRB+NEO_KHZ800);
    strip.begin();
    strip.show();
    strip.setBrightness(brightness);
    for(int x=0; x<size; x++){
        strip.setPixelColor(x,values[x][0],values[x][1],values[x][2]);
    }
    strip.show();// Have to call it after every setPixel or setBrightness
}