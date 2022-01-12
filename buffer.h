#ifndef __FIFO_BUFFER__
#define __FIFO_BUFFER__

#include "CustomQueue.h"
#include "FATFileSystem.h"
#include "Mutex.h"
#include "SDBlockDevice.h"
#include "mbed.h"
#include "sampling.h"
#include "uop_msb.h"
#include <Semaphore.h>
#include <ctime>


#define buffer_size 800
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

  CustomQueue bufferPrintQueue;
  sampler *BF;
  ErrorHandler *BEH;

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
  bufferClass(sampler *buffersampler);
  bufferClass(ErrorHandler *bufferEH);

  // old
  // void flushBuffer();
  // void writeBuffer();
  
  void bufferCount();

  void printBufferContents();

  void emptyBuffer();

  void printToWebpage(vector<int> &webpageData);
  //potentially just take samples from sampler?

  void initSD();
  bool cardMount = 0;

  int flushBufferUpgrade();
  // constructor and destructor
  bufferClass();
  ~bufferClass();
};

// real time data
struct liveData {
  float LDR;
  float temp;
  float pressure;
  time_t realTime;
};


#endif
