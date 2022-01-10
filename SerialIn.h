#ifndef __SERIAL_IN__
#define __SERIAL_IN__

#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;

class SerialIn{
    private:
    Thread SerialInThread;
    static UnbufferedSerial serial_port(int USBTX, int USBRX);

    public:
    SerialIn(UnbufferedSerial* serial_port);
    void SerialListener();
    void Serial_IRQ();
    

};

#endif