#ifndef __FIFO_BUFFER__
#define __FIFO_BUFFER__

#include "mbed.h"
#include "Mutex.h"
#include "printQueue.h"
#include "uop_msb.h"
#include "FATFileSystem.h"
#include "sampling.h"
#include "SDBlockDevice.h"
//#include <cstdio>
//#include "NTPConnection.h"
//#include "HTTP_Server.h"

#define buffer_size 20
using namespace std;

class bufferClass{

    private: 
    //mutex locks
    Mutex bufferLock;
    Mutex timeLock;
    Ticker bufferTick;
    sampler dataSampler;
    //HTTP_server webServer;
    time_t timestamp;

    public:
    //void flushBuffer();
    int flushBufferUpgrade();
    void whenToFlush();
    void flashGreen();
    //void sampleFunc();
    void writeBuffer();
    void bufferCount();
    //void acquireData();
    void printBufferContents();
    void emptyBuffer();
    //void printToWebpage(vector<int> & webpageData);
    void initSD();
    void flushTimer();


    unsigned int newIDX = buffer_size - 1; 
    unsigned int oldIDX = buffer_size - 1;

    int runFlush = 1;
    int runPrint = 1;
    int printIDX = oldIDX; //work from oldest to newest sets data
    int printRecordsIDX = 0; //track no of data sets
    int pRIDX = 0;
    int lastFlushTime = 0;
    
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

