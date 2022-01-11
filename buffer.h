#ifndef __FIFO_BUFFER__
#define __FIFO_BUFFER__

#include "FATFileSystem.h"
#include "Mutex.h"
#include "SDBlockDevice.h"
#include "mbed.h"
#include "printQueue.h"
#include "sampling.h"
#include "uop_msb.h"

#define buffer_size 20
using namespace std;

class bufferClass {

private:
  // mutex locks
  Mutex bufferLock;
  Mutex timeLock;
  Ticker bufferTick;
  sampler dataSampler;
  // HTTP_server webServer;
  time_t timestamp;
  Thread writeThread;
  Thread flushThread;

public:
  // void flushBuffer();
  int flushBufferUpgrade();
  void whenToFlush();
  void flashGreen();
  void writeBufferAuto();
  void writeFlag();
  void flushFlag();
  // void sampleFunc();
  void writeBuffer();
  void bufferCount();
  // void acquireData();
  void printBufferContents();
  void emptyBuffer();
  // void printToWebpage(vector<int> & webpageData);
  void initSD();
  void flushTimer();
  int dataInBuffer;

  unsigned int newIDX = buffer_size - 1;
  unsigned int oldIDX = buffer_size - 1;

  int runFlush = 1;
  int runPrint = 1;
  int printIDX = oldIDX;   // work from oldest to newest sets data
  int printRecordsIDX = 0; // track no of data sets
  int pRIDX = 0;
  int lastFlushTime = 0;

  bool cardMount = 0;
  // semaphores
  // Semaphore samplesInBuffer;
  // Semaphore spaceInBuffer;
  // Semaphore signalSample;

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

extern liveData dataRecord;
extern liveData flushRecord;
extern liveData printRecord[buffer_size];

// liveData printToWebpage();

#endif
