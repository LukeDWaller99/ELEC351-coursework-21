//AUTHOR - NOAH HARVEY

#ifndef _FIFOBUFFER_
#define _FIFOBUFFER_

#include "mbed.h"
#include "Mutex.h"
#include "printQueue.h"
#include "uop_msb.h"
#include "FATFileSystem.h"
#include "sampling.h"
#include "SDBlockDevice.h"
#include <cstdio>

#define buffer_size 800
using namespace std;
//pc2 for full error, red led

class bufferClass{

    private: 
    //mutex locks
    Mutex bufferLock;
    Mutex timeLock;
    Ticker bufferTick;
    sampler dataSampler;

    public:
    void flushBuffer(FILE &fp);
    void sampleFunc();
    void writeBuffer();
    void writeBuffer2();
    void acquireData();
    void printBufferContents();
    void emptyBuffer();
    //void printToWebpage(vector<int> & webpageData);

    unsigned int newIDX = buffer_size - 1; 
    unsigned int oldIDX = buffer_size - 1;
    int runPrint = 1;
    int printIDX = oldIDX; //work from oldest to newest sets data
    int printRecordsIDX = 0; //track no of data sets
    int pRIDX = 0;
    
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
extern liveData printRecord[buffer_size];


liveData printToWebpage();
#endif