#include "Arduino.h"
#include "Wire.h"
#include "iottetris.h"

IotTetris_I2C::IotTetris_I2C()
{

}

IotTetris_I2C::slaveStart(int address, void (*onReceive)(size_t), void (*onRequest)(void))
{
    Wire.begin(address);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

IotTetris_I2C::masterStart(int addresses[], int sda, int scl)
{
    Wire.begin(sda,scl);
    _addresses[] = addresses;
    _loopPeriod = 10000; //in ms
}

IotTetris_I2C::masterRequestLoop()
{
    int size = sizeof(_addresses)/sizeof(_addresses[0]);
    for(int x=0; x<size; x++){
        wakeUpSlave(_addresses[x]);
        delay(1000);
        readSlave(_addresses[x]);
    }
}

IotTetris_I2C::_wakeUpSlave(int slave)
{
    Wire.beginTransmission(slave);
    Wire.write("beep");
    Wire.endTransmission();
}

IotTetris_I2C::_readSlave(int slave)
{
    /* Might want to change this from void
    so we can gather the data and process it in the loop
    maybe? 
    Also add the number of bytes to the args
    if needed*/
    Wire.requestFrom(slave, 20);
    while(Wire.available()){
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
    delay(10000);
}