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
//create a class for buffer
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
    //time
	int hour;
	int minute;
	int second;
    //date
    int day;
	int month;
	int year;
    //data
	float LDR;
	float temp;
	float pressure;
    float humidity;
};

extern liveData dataRecord;
extern liveData flushRecord;

#endif