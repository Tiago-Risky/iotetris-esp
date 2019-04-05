#include <Adafruit_NeoPixel.h>

#define PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2,PIN,NEO_GRB+NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
  strip.setPixelColor(0,20,230,20);//LED on the top size
  strip.setPixelColor(1,230,30,20);//LED on the back size
  strip.show();// Have to call it after every setPixel or setBrightness
}

void loop() {
  
}
