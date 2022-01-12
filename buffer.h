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


#define buffer_size 20
#define SDDetect PF_4
using namespace std;

// class SDClass {
//     SDClass();

//     private:
//     InterruptIn SDDetector;

//     SDClass(PinName SDDetect) : SDDetector(SDDetect){
//         SDDetector.rise(callback(this, &SDClass::initSD));
//     }

//     public:

//     void initSD();
    
//     ~SDClass();
// };

class bufferClass {

private:
  typedef void (*funcPointer_t)(void);
  // mutex locks, tickers, timers, 
  Mutex bufferLock;
  Mutex timeLock;
  Ticker bufferWriteTick;
  Ticker bufferFlushTick;
  Timer t;

  //InterruptIn SDDetector(SDDetect);
  //CustomQueue bufferPrintQueue;
  sampler* BF;
  ErrorHandler* BEH;
  CustomQueue* PQ;

  void writeFlag();
  void flushFlag();
  Thread writeThread;
  Thread flushThread;

  void writeBufferAuto();
  unsigned int newIDX = buffer_size - 1;
  unsigned int oldIDX = buffer_size - 1;

  void whenToFlush();
  int dataInBuffer;
  int lastFlushTime = 0;
  float currentTime;
  float hourPassed = 59 * 60; // 59 minutes as we check every minute

//everything required to printing and flushing buffer contents
  int runFlush = 1;
  int runPrint = 1;
  int printIDX = oldIDX;   // work from oldest to newest sets data
  int printRecordsIDX = 0; // track no of data sets
  int pRIDX = 0;
  time_t timestamp;

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
  
  void bufferCount();

  void printBufferContents();

  void emptyBuffer();

  void initSD();
  bool SDMount = 0;

  void flashGreen();

  //destructor
  ~bufferClass();
};

// real time data
// struct liveData {
//   float LDR;
//   float temp;
//   float pressure;
//   time_t realTime;
// };


#endif
