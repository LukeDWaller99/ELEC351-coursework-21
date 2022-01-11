#ifndef __FIFO_BUFFER__
#define __FIFO_BUFFER__

#include "FATFileSystem.h"
#include "Mutex.h"
#include "SDBlockDevice.h"
#include "mbed.h"
#include "sampling.h"
#include "uop_msb.h"
#include <ctime>
#include <Semaphore.h>
#include "CustomQueue.h"

#define buffer_size 80
using namespace std;

class bufferClass {

private:
    typedef void(*funcPointer_t)(void);
  // mutex locks, tickers, timers, semaphores, threads
  Mutex bufferLock;
  Mutex timeLock;
  Ticker bufferWriteTick;
  Ticker bufferFlushTick;
  Timer t;
    sampler* BF;
    ErrorHandler* BEH;
  //
  // HTTP_server webServer;
  time_t timestamp;
  Thread writeThread;
  Thread flushThread;

  unsigned int newIDX = buffer_size - 1;
  unsigned int oldIDX = buffer_size - 1;
  int runFlush = 1;
  int runPrint = 1;
  int printIDX = oldIDX;   // work from oldest to newest sets data
  int printRecordsIDX = 0; // track no of data sets
  int pRIDX = 0;

public:
bufferClass(sampler* buffersampler);
bufferClass(ErrorHandler* bufferEH);
  // void flushBuffer();
  int flushBufferUpgrade();
  void whenToFlush();
  void writeBufferAuto();
  void writeFlag();
  void flushFlag();
  void writeBuffer();
  void bufferCount();
  void printBufferContents();
  void emptyBuffer();
  void printToWebpage(vector<int> & webpageData);
  void initSD();
  void flushTimer();

  int dataInBuffer;
  int lastFlushTime = 0;
  float currentTime;
  float hourPassed = 59 * 60; // 59 minutes as we check every minute
  bool cardMount = 0;

  // constructor and destructor
  bufferClass();
  ~bufferClass();
};

// real time data
struct liveData {
  float LDR;
  float temp;
  float pressure;

  char realTime;
};

extern liveData dataRecord;
extern liveData flushRecord;
extern liveData printRecord[buffer_size];
// liveData printToWebpage();

#endif
