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
    CustomQueue* pQ;
    sampler* serialSampler;
    bufferClass* serialBuff;
    Thread SerialWatcher;
    

    public:
    SerialIn(CustomQueue* printQueue, sampler* serialSamples, bufferClass* serialBuffer);
    void SerialListener();
    void SerialInstructions();
};


#endif