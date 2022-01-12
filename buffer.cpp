#include "buffer.h"

Semaphore spaceInBuffer(buffer_size); // space in buffer
Semaphore samplesInBuffer(0);         // samples in buffer
Semaphore signalSample(0);            // signal to get new sample

// sampler buffersampler;
samples sampledData;
//InterruptIn SDDetector(PF_4);
FILE *fp;
SDBlockDevice mysd(PB_5, PB_4, PB_3, PF_3);
//DigitalIn SDDetect(PF_4);
DigitalOut greenLED(PC_6);

// constructor
bufferClass::bufferClass(sampler *buffersampler, ErrorHandler *bufferEH,
                         CustomQueue *bufferPQ) {
  BF = buffersampler;
  BEH = bufferEH;
  PQ = bufferPQ;
  t.start();
  bufferClass::initSD();
  writeThread.start(callback(this, &bufferClass::writeBufferAuto));
  bufferWriteTick.attach(callback(this, &bufferClass::writeFlag), 12s);
  flushThread.start(callback(this, &bufferClass::whenToFlush));
  bufferFlushTick.attach(callback(this, &bufferClass::flushFlag), 10s);
}

// SDClass::SDClass(SDDetect){
//     SDClass(SDDetect)::initSD();
//     //SDDetector.rise(callback(this, &SDClass::initSD));
// }

void bufferClass::writeFlag() { bufferClass::writeThread.flags_set(1); }

void bufferClass::flushFlag() { bufferClass::flushThread.flags_set(1); }

void bufferClass::writeBufferAuto() {
  while (true) {
    ThisThread::flags_wait_any(1);
    if (spaceInBuffer.try_acquire_for(1ms) == 0) {
      PQ->custom.call(printf, "buffer full\n");
      BEH->setErrorFlag(BUFFER_FULL); // critical error
    } else {
      if (bufferLock.trylock_for(1ms) == 0) { // PROTECT THE BUFFER
        PQ->custom.call(printf, "buffer lock timeout\n");
        BEH->setErrorFlag(BUFFER_LOCK_TIMEOUT); // critical error
      } else {
        if (timeLock.trylock_for(1ms) == 0) { // PROTECT THE DATA
          PQ->custom.call(printf, "timeLockTimeout\n");
          BEH->setErrorFlag(TIMER_LOCK_TIMEOUT); // critical error
        } else {
          sampledData = BF->sampleData;
          timestamp = time(NULL);
          dataRecord.realTime = ctime(&timestamp);
          timeLock.unlock();
          // copy environmental data
          dataRecord.LDR = sampledData.LDR;
          dataRecord.temp = sampledData.temp;
          dataRecord.pressure = sampledData.pressure;
          dataInBuffer++; // increment no of data sets
        }
        newIDX = (newIDX + 1) % buffer_size; // increment buffer size
        buffer[newIDX] = dataRecord;         // update the buffer
      }
      bufferLock.unlock();
    }
    samplesInBuffer.release(); // sample added signal
  }
  ThisThread::flags_clear(1);
}

void bufferClass::bufferCount() {
  PQ->custom.call(
      printf, "Number of environmental data sets in the buffer: %i\n", newIDX);
}

void bufferClass::flashGreen(){
    greenLED = 0;
    ThisThread::sleep_for(100ms);
    greenLED = 1;
}

void bufferClass::whenToFlush() {
  while (true) {
    ThisThread::flags_wait_any(1);
    currentTime = duration_cast<seconds>(t.elapsed_time()).count();
    if ((currentTime > 60) &&
        (newIDX == oldIDX - (buffer_size * 0.1))) { // flush buffer at 90%
      PQ->custom.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      // bufferClass::printBufferContents(); //used to check contents
      flashGreen();
      bufferClass::flushBuffer();
    }
    if (currentTime == hourPassed) { // must flush at least once an hour
      greenLED = !greenLED;
      PQ->custom.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      // bufferClass::printBufferContents(); //used to check contents
      bufferClass::flushBuffer();
      // dataInBuffer = 0;
    }
    // dataInBuffer = 0; // reset the count as buffer has flushed
  }
  ThisThread::flags_clear(1);
}

void bufferClass::printBufferContents() {
  while (true) {
    ThisThread::flags_wait_any(1);
    if (samplesInBuffer.try_acquire_for(1ms) ==
        0) {                          // check for samples in the buffer
      BEH->setErrorFlag(EMPTY_FLUSH); // critical error
      PQ->custom.call(printf, "no data\n");
    } else {
      samplesInBuffer.release();
      if (bufferLock.trylock_for(1ms) == 0) {   // try to acquire buffer data
        BEH->setErrorFlag(BUFFER_LOCK_TIMEOUT); // critical error
        // bufferPrintQueue.custom.call(printf, "could not unlock buffer\n");
      } else {
        while (runPrint == 1) {   // remain in loop, iterating through data
          if (oldIDX == newIDX) { // all data from buffer has been copied
            runPrint = 0;         // terminate
          } else {
            samplesInBuffer.try_acquire_for(1ms);
            oldIDX = (oldIDX + 1) % buffer_size;
            liveData flushRecord = buffer[oldIDX];
            PQ->custom.call(printf, "Time recorded = %s\n\t",
                            flushRecord.realTime);
            PQ->custom.call(
                printf,
                "Temperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",
                flushRecord.temp, flushRecord.pressure, flushRecord.LDR);

            spaceInBuffer.release(); // another space free
            // samplesInBuffer.release();
          }
        }
        bufferLock.unlock();
        t.reset(); // reset flush timer
      }
    }
  }
  ThisThread::flags_clear(1);
}

void bufferClass::flushBuffer() {
  if (SDMount == 0) {
    BEH->setErrorFlag(MOUNT_ERROR);
  } else {
    if (samplesInBuffer.try_acquire_for(1ms) == 0) {
      PQ->custom.call(printf, "empty buffer\n");
      BEH->setErrorFlag(EMPTY_FLUSH); // warning, no data to flush
      return;
    } else {
      samplesInBuffer.release();
      if (bufferLock.trylock_for(1ms) == 0) {
        PQ->custom.call(printf, "buffer lock timeout\n");
        BEH->setErrorFlag(BUFFER_LOCK_TIMEOUT); // critical error
      } else {
        FATFileSystem fs("sd", &mysd);
        FILE *fp = fopen("/sd/environmental_data.txt", "a");

        if (fp == NULL) {
          BEH->setErrorFlag(NO_SD_FILE);
        }
        while (runFlush == 1) {
          if (oldIDX == newIDX) {
            runFlush = 0; // everything is out
          } else {
            samplesInBuffer.try_acquire_for(1ms);
            oldIDX = (oldIDX + 1) % buffer_size;
            liveData flushRecord = buffer[oldIDX];
            fprintf(fp, "Time recorded = %s\n\t", flushRecord.realTime);
            fprintf(fp,
                    " \tTemperature = %2.1f, \tPressure = %3.1f,\tLDR = % "
                    "1.2f;\n\r ",
                    flushRecord.temp, flushRecord.pressure, flushRecord.LDR);

            spaceInBuffer.release(); // space in buffer signal
          }
        } // end while
        PQ->custom.call(printf, "buffer has been flushed\n");
        bufferLock.unlock();
      }
    }
  }
}

void bufferClass::fetchLatestRecord() {
  if (samplesInBuffer.try_acquire_for(1ms) ==
      0) {                          // check for samples in the buffer
    BEH->setErrorFlag(EMPTY_FLUSH); // critical error
    PQ->custom.call(printf, "no data\n");
  } else {
    samplesInBuffer.release();
    if (bufferLock.trylock_for(1ms) == 0) {   // try to acquire buffer data
      BEH->setErrorFlag(BUFFER_LOCK_TIMEOUT); // critical error
      // bufferPrintQueue.custom.call(printf, "could not unlock buffer\n");
    } else {
      // get latest set of data
      samplesInBuffer.try_acquire_for(1ms);
      oldIDX = (oldIDX + 1) % buffer_size;
      liveData flushRecord = buffer[oldIDX];
      PQ->custom.call(printf, "Time recorded = %s\n\t", flushRecord.realTime);
      PQ->custom.call(
          printf, "Temperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",
          flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
      //spaceInBuffer.release(); // another space free
      samplesInBuffer.release();
    }
  }
  bufferLock.unlock();
  //t.reset(); // reset flush timer
}

void bufferClass::initSD() {
  if (SDDetect == 1) {
    PQ->custom.call(printf, "sd card not mounted\n");
    BEH->setErrorFlag(MOUNT_ERROR);
    greenLED = 0;
    SDMount = 0;
  } else {
    SDMount = 1;
    greenLED = 1;
    // PQ->custom.call(printf, "sd card is mounnted\n");
  }
}
