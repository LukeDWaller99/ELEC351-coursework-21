#ifndef __SERIAL_IN__
#define __SERIAL_IN__

#include "CustomQueue.h"
#include "mbed.h"
#include "sampling.h"
#include "uop_msb.h"
#include "buffer.h"
using namespace uop_msb;

class SerialIn{
    private:
    Thread SerialWatcher;
    CustomQueue* printQueue;
    sampler* sample;
    bufferClass* buffer;

    public:
    SerialIn(CustomQueue* printQueue, sampler* sample, bufferClass* buffer);
    void SerialListener();
    void SerialInstructions();
};


#endif