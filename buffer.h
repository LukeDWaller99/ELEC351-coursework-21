#ifndef _FIFOBUFFER_
#define _FIFOBUFFER_

#include "mbed.h"
#include "printQueue.h"

#define buffer_size 800

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