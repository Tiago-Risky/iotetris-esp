#pragma once
#ifndef IoTetris_slave_h
#define IoTetris_slave_h

#include "Arduino.h"
#include "Wire.h"

class IoTetris_Slave
{
    public:
        IoTetris_Slave();
        void slaveStart(int address, void (*)(size_t), void (*)(void));
        void slaveOnRequest(int howMany);
        void slaveSetUpdater(void (*f)());
        bool slaveConfirmSent();
    private:
        int _address;
        static void (*_onReceive)(size_t);
        static void (*_onRequest)(void);
        void _wakeUpSlave(int slave);
        void _readSlave(int slave);
        char * _slaveString;
        void _slaveUpdate();
        static void (*_updater)();
        bool _slaveHasUpdated;
        bool _slaveHasSent;
};

#endif
