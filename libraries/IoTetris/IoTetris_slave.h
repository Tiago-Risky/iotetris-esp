#pragma once
#ifndef IoTetris_slave_h
#define IoTetris_slave_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_NeoPixel.h"

class IoTetris_Slave
{
    public:
        IoTetris_Slave();
        void start(int address, void (*)(size_t), void (*)(void));
        void changeNeopixel(int pin, int brightness, int values[][3], int size);
};

#endif
