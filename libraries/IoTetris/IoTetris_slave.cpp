#include "Arduino.h"
#include "Wire.h"
#include "IoTetris_slave.h"

IoTetris_Slave::IoTetris_Slave()
{
    _slaveHasSent = false;
    _slaveHasUpdated = false;
}

// address for i2c, onReceive function, onRequest function
void IoTetris_Slave::slaveStart(int address, void (*onReceive)(size_t), void (*onRequest)(void))
{
    Wire.begin(address);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

void IoTetris_Slave::slaveOnRequest(int howMany)
{
    if (_slaveHasUpdated)
    {
        Wire.write(_slaveString);
        _slaveHasSent = true;
    }
    else
    {
        _slaveUpdate();
    }
}

void IoTetris_Slave::slaveSetUpdater(void (*f)())
{
    _updater = f;
}

void IoTetris_Slave::_slaveUpdate()
{
    _updater();
    _slaveHasUpdated = true;
}

bool IoTetris_Slave::slaveConfirmSent()
{
    if (_slaveHasSent)
    {
        _slaveHasSent = false;
        _slaveHasUpdated = false;
        return true;
    }
    else{
        return false;
    }
}
