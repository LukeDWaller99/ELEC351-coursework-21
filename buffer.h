#ifndef __FIFO_BUFFER__
#define __FIFO_BUFFER__

#include "CustomQueue.h"
#include <FATFileSystem.h>
#include <Mutex.h>
#include <SDBlockDevice.h>
#include <mbed.h>
#include "sampling.h"
#include <uop_msb.h>
#include <Semaphore.h>
#include <ctime>
#include <InterruptIn.h>


#define buffer_size 100
#define SDDetect PF_4
using namespace std;

class bufferClass {

private:
  typedef void (*funcPointer_t)(void);
  // mutex locks, tickers, timers, 
  Mutex bufferLock;
  Mutex timeLock;
  Ticker bufferWriteTick;
  Ticker bufferFlushTick;
  Timer t;
    time_t timestamp;
  InterruptIn SDDetector;
  sampler* bSamp;
  ErrorHandler* BEH;
  CustomQueue* PQ;

  void writeFlag();
  Thread writeThread;

  void flushFlag();
  Thread flushThread;

  void writeBufferAuto();
  unsigned int newIDX = buffer_size - 1;
  unsigned int oldIDX = buffer_size - 1;

  void whenToFlush();
  float flushTiming;    //current time of timer used to track last fush
  float hourPassed = 59*60; // 59 minutes as we check every minute


  
  

public:
  bufferClass(sampler* buffersampler, ErrorHandler* bufferEH, CustomQueue* bufferPQ);

struct liveData {
  float LDR;
  float temp;
  float pressure;
  char* realTime;
};

samples sampleData; //sampled values, every 10s
liveData buffer[buffer_size];
liveData dataRecord; // for holding data in the buffer
liveData printRecord[buffer_size];
  
  void fetchLatestRecord();

  void flushBuffer();
  int runFlush = 1;

  void bufferCount();

  void printBufferContents();
    int runPrint = 1;


  void initSD();
  bool SDMount;

  void flashGreen(); //for showing that the sd card is being flushed

  //destructor
  ~bufferClass();
};


#endif
