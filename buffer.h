//AUTHOR - NOAH HARVEY

#ifndef _FIFOBUFFER_
#define _FIFOBUFFER_

#include "mbed.h"
#include "Mutex.h"
#include "printQueue.h"
//#include "uop_msb.h"
#include "FATFileSystem.h"
#include "sampling.h"


#define buffer_size 800
using namespace std;
//pc2 for full error, red led

class bufferClass{

    private: 
    //mutex locks
    Mutex bufferLock;
    Mutex dataLock;
    Ticker bufferTick;
    sampler dataSampler;

    public:
    void flushBuffer(FILE &fp);
    void sampleFunc();
    void writeBuffer();
    void acquireData();
    void printBufferContents();
    void emptyBuffer();
    
    //semaphores
    Semaphore samplesInBuffer;
    Semaphore spaceInBuffer;
    Semaphore signalSample;

    //constructor and destructor
    bufferClass();
    ~bufferClass();

};

//real time data
 struct liveData{
    //luke is doing the time stamp
    //data
	float LDR;
	float temp;
	float pressure;
    float humidity;
};

extern liveData dataRecord;
extern liveData flushRecord;
//extern liveData printRecord{buffer_size};

#endif