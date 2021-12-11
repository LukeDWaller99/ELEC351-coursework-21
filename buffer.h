//AUTHOR - NOAH HARVEY

#ifndef _FIFOBUFFER_
#define _FIFOBUFFER_

#include "mbed.h"
#include "printQueue.h"
#include "FATFileSystem.h"

#define buffer_size 800
//eventually this will be in an overrding contructor
//where you can redfine the buffer size
using namespace std;


class bufferClass{

    private: 
    //maybe a red led

    public:
    void flushBuffer(FILE &fp);
    void sampleData();
    void writeBuffer();
    void acquireData();


};

//external semaphores
extern Semaphore samplesBuffer;
extern Semaphore spaceBuffer;
extern Semaphore signalSample;

//mutex
extern Mutex lockBuffer;
extern Mutex Time;

extern Ticker buff;

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
	double LDR;
	double temp;
	double pressure;
};

extern liveData dataRecord;



#endif