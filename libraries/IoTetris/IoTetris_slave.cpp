#include "Arduino.h"
#include "Wire.h"
#include "IoTetris_slave.h"

IoTetris_Slave::IoTetris_Slave()
{
    _hasSent = false;
    _hasUpdated = false;
}

// address for i2c, onReceive function, onRequest function
void IoTetris_Slave::start(int address, void (*onReceive)(size_t), void (*onRequest)(void))
{
    Wire.begin(address);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

void IoTetris_Slave::onRequest(int howMany)
{
    if (_hasUpdated)
    {
        Wire.write(_sendString);
        _hasSent = true;
    }
    else
    {
        _slaveUpdate();
    }
}

void IoTetris_Slave::setUpdater(void (*f)())
{
    _updater = f;
}

void IoTetris_Slave::_update()
{
    _updater();
    _hasUpdated = true;
}

bool IoTetris_Slave::confirmSent()
{
    if (_hasSent)
    {
        _hasSent = false;
        _hasUpdated = false;
        return true;
    }
    else{
        return false;
    }
}
