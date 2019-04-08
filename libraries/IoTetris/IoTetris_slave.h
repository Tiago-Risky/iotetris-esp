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
        void setNeopixel(int pin, int brightness, int values[][3], int size);
        /*void onRequest(int howMany);*/
        void setUpdater(void (*f)());
        bool confirmSent();
    private:
        int _address;
        static void (*_onReceive)(size_t);
        static void (*_onRequest)(void);
        char * _sendString;
        void _update();
        static void (*_updater)();
        bool _hasUpdated;
        bool _hasSent;
};

#endif
