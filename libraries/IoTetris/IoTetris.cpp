#include "Arduino.h"
#include "Wire.h"
#include "IoTetris.h"

IoTetris_I2C::IoTetris_I2C()
{

}

// address for i2c, onReceive function, onRequest function
IoTetris_I2C::slaveStart(int address, void (*onReceive)(size_t), void (*onRequest)(void))
{
    Wire.begin(address);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

// array with addresses for all the blocks, SDA port, SCL port
IoTetris_I2C::masterStart(int addresses[], int sda, int scl)
{
    Wire.begin(sda,scl);
    _addresses[] = addresses;
    _loopPeriod = 10000; //in ms
}

IoTetris_I2C::masterRequestLoop()
{
    int size = sizeof(_addresses)/sizeof(_addresses[0]);
    for(int x=0; x<size; x++){
        _wakeUpSlave(_addresses[x]);
        delay(1000);
        _readSlave(_addresses[x]);
    }
    //This delay is to be changed for a sleep later on
    delay(_loopPeriod);
}

IoTetris_I2C::_wakeUpSlave(int slave)
{
    Serial.println("Waking up slave" + String(slave));
    Wire.beginTransmission(slave);
    Wire.write("beep");
    Wire.endTransmission();
}

IoTetris_I2C::_readSlave(int slave)
{
    /* Might want to change this from void
    so we can gather the data and process it in the loop
    maybe? 
    Also add the number of bytes to the args
    if needed*/
    Serial.println("Reading from slave" + String(slave));
    Wire.requestFrom(slave, 20);
    while(Wire.available()){
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
}