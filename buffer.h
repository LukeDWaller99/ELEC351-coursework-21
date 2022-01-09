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
#include "NTPConnection.h"
#include "HTTP_Server.h"

#define buffer_size 800
using namespace std;

class bufferClass{

    private: 
    //mutex locks
    Mutex bufferLock;
    Mutex timeLock;
    Ticker bufferTick;
    sampler dataSampler;
    HTTP_server webServer;
    time_t timestamp;

    public:
    void flushBuffer(FILE &fp);
    void flushBufferUpgrade();
    //void sampleFunc();
    void writeBuffer();
    //void writeBuffer2();
    //void acquireData();
    void printBufferContents();
    void emptyBuffer();
    //void printToWebpage(vector<int> & webpageData);
    void initSD();

    unsigned int newIDX = buffer_size - 1; 
    unsigned int oldIDX = buffer_size - 1;

    int runFlush = 1;
    int runPrint = 1;
    int printIDX = oldIDX; //work from oldest to newest sets data
    int printRecordsIDX = 0; //track no of data sets
    int pRIDX = 0;
    
    bool cardMount = 0;
    //semaphores
    // Semaphore samplesInBuffer;
    // Semaphore spaceInBuffer;
    // Semaphore signalSample;

    //constructor and destructor
    bufferClass();
    ~bufferClass();

};

//real time data
 struct liveData{
	float LDR;
	float temp;
	float pressure;

    time_t realTime;
};

extern liveData dataRecord;
extern liveData flushRecord;
extern liveData printRecord[buffer_size];

//liveData printToWebpage();
#endif