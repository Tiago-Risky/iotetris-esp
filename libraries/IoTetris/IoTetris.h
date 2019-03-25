#pragma once
#ifndef IoTetris_h
#define IoTetris_h

#include "Arduino.h"
#include "Wire.h"

class IoTetris_I2C
{
    public:
        IoTetris_I2C();
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

/*
class IoTetris_PMSleep
{
    public:
        IoTetris_PMSleep();
};*/

#endif
