#pragma once
#ifndef IotTetris_h
#define IotTetris_h

#include "Arduino.h"
#include "Wire.h"

class IotTetris_I2C
{
    public:
        IotTetris_I2C();
        void slaveStart(int address, void (*)(size_t), void (*)(void));
        void masterStart(int addresses[], int sda, int scl);
        void masterRequestLoop();
    private:
        int _address;
        int _addresses[];
        static void (*_onReceive)(size_t);
        static void (*_onRequest)(void);
        int _sda;
        int _scl;
        void _wakeUpSlave(int slave);
        void _readSlave(int slave);
        int _loopPeriod;
};

class IotTetris_PMSleep
{
    public:
        IotTetris_PMSleep();
};

#endif
