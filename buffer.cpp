// AUTHOR: NOAH HARVEY

#include "buffer.h"
#include <Semaphore.h>
#include <ctime>

Semaphore spaceInBuffer(buffer_size); // space in buffer
Semaphore samplesInBuffer(0);         // samples in buffer
Semaphore signalSample(0);            // signal to get new sample

liveData buffer[buffer_size];
liveData dataRecord; // for holding data in the buffer
liveData printRecord[buffer_size];

Ticker bufferWriteTick;
Ticker bufferFlushTick;
Timer t;
samples sampledData;
float currentTime;
float hourPassed = 59 * 60; // 59 minutes as we check every minute

//FILE *fp;
SDBlockDevice mysd(PB_5, PB_4, PB_3, PF_3);
DigitalIn SDDetect(PF_4);
DigitalOut greenLED(PC_6);

// constructor
bufferClass::bufferClass() {
  t.start();
  writeThread.start(callback(this, &bufferClass::writeBufferAuto));
  bufferWriteTick.attach(callback(this, &bufferClass::writeFlag), 12s);
  flushThread.start(callback(this, &bufferClass::whenToFlush));
  bufferFlushTick.attach(callback(this, &bufferClass::flushFlag), 10s);
    bufferClass::initSD();
}

void bufferClass::flashGreen() {
  greenLED = 0;
  ThisThread::sleep_for(100ms);
  greenLED = 1;
}

void bufferClass::writeFlag() { bufferClass::writeThread.flags_set(1); }

void bufferClass::flushFlag() { bufferClass::flushThread.flags_set(1); }

void bufferClass::writeBufferAuto() {
  while (true) {
    ThisThread::flags_wait_any(1);
    if (spaceInBuffer.try_acquire_for(1ms) == 0) {
      printQueue.call(bufferFull);
      // fatal error
    } else {
      // PROTECT THE BUFFER
      if (bufferLock.trylock_for(1ms) == 0) {
        printQueue.call(bufferLockTimeout);
        // fatal error
      } else {
        // PROTECT THE TIME
        // if (timeLock.trylock_for(1ms) == 0) { // PROTECT THE DATA
        //   printQueue.call(timeLockTimeout);
        // } else {
          //************************************
          // adding time here
          //************************************
          // copy environmental data
          dataRecord.LDR = sampledData.LDR;
          dataRecord.temp = sampledData.temp;
          dataRecord.pressure = sampledData.pressure;
          //timeLock.unlock();
          dataInBuffer++;
        //}
        // update the buffer
        newIDX = (newIDX + 1) % buffer_size; // increment buffer size
        buffer[newIDX] = dataRecord;         // update the buffer
      }
      bufferLock.unlock();
    }
    samplesInBuffer.release(); // sample added signal
  }
  ThisThread::flags_clear(1);
}

void bufferClass::writeBuffer() {
  if (spaceInBuffer.try_acquire_for(1ms) == 0) {
    printQueue.call(bufferFull);
    // fatal error
  } else {
    // PROTECT THE BUFFER
    if (bufferLock.trylock_for(1ms) == 0) {
      printQueue.call(bufferLockTimeout);
      // fatal error
    } else {
      // copy environmental data
      dataRecord.LDR = sampledData.LDR;
      dataRecord.temp = sampledData.temp;
      dataRecord.pressure = sampledData.pressure;
      // dataInBuffer++;
    }
    newIDX = (newIDX + 1) % buffer_size; // increment buffer size
    buffer[newIDX] = dataRecord;         // update the buffer
    bufferLock.unlock();
  }
  samplesInBuffer.release(); // sample added
}

void bufferClass::bufferCount() {
  printQueue.call(printf,
                  "Number of environmental data sets in the buffer: %i\n",
                  dataInBuffer);
}

void bufferClass::whenToFlush() {
  // currentTime = t.read();
  while (true) {
    ThisThread::flags_wait_any(1);
    currentTime = duration_cast<seconds>(t.elapsed_time()).count();

    //if ((currentTime > 60) && (oldIDX - (buffer_size * 0.5))) {
    if((currentTime > 60) && (newIDX == (15))) {
      // flush buffer at 90%
      // currently printing to serial
      // flashGreen();
      //printQueue.call(printf, "capacity flush\n");
      //timeLock.lock();
      //bufferLock.lock();
      //__disable_irq();
      //printQueue.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      //bufferClass::printBufferContents();
      //bufferClass::flushBufferUpgrade();
      bufferClass::flushBuffer();
     //timeLock.unlock();
      //bufferLock.unlock(); 
      //__enable_irq();
      dataInBuffer = 0;
    }
    if (currentTime > hourPassed) {
      // flush no matter what
      //flash green led
      //timeLock.lock();
      //printQueue.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      //timeLock.unlock();
      bufferClass::printBufferContents();
      //printQueue.call(printf, "timing flush\n");
      dataInBuffer = 0;
    }
  }
  ThisThread::flags_clear(1);
}

void bufferClass::printBufferContents() {
  while (true) {
    ThisThread::flags_wait_any(1);

    if (samplesInBuffer.try_acquire_for(1ms) ==
        0) { // check for samples in the buffer
      printQueue.call(printf, "no data\n");
    } else {

      // maybe need this?
      samplesInBuffer.release();

      // there is data in the buffer
      if (bufferLock.trylock_for(1ms) == 0) { // try to acquire buffer
        printQueue.call(printf, "could not unlock buffer\n");
      } else {

        while (runPrint == 1) {   // remain in loop, iterating through data
          if (oldIDX == newIDX) { // all data from buffer has been copied
            runPrint = 0;         // terminate
            // printQueue.call(printf, "all data printed\n");
          } else {
            samplesInBuffer.try_acquire_for(1ms);
            oldIDX = (oldIDX + 1) % buffer_size;

            liveData flushRecord = buffer[oldIDX];
            printQueue.call(
                printf,
                "\tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",
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

// void bufferClass::printBufferContents() {
//   while (true) {
//     ThisThread::flags_wait_any(1);

//     if (samplesInBuffer.try_acquire_for(1ms) ==
//         0) { // check for samples in the buffer
//       printQueue.call(printf, "no data\n");
//     } else {

//       // there is data in the buffer
//       if (bufferLock.trylock_for(1ms) == 0) { // try to acquire buffer
//         printQueue.call(printf, "could not unlock buffer\n");
//       } else {

//         while (runPrint == 1) {     // remain in loop, iterating through data
//           if (printIDX == newIDX) { // all data from buffer has been copied
//             runPrint = 0;           // terminate
//             // printQueue.call(printf, "all data printed\n");
//           } else {
//             printIDX = (printIDX + 1) % buffer_size;         // update print
//             idx printRecord[printRecordsIDX] = buffer[printIDX]; // copy the
//             data printRecordsIDX++; // increment print idx
//           }
//         }

//         bufferLock.unlock();
//       }

//       printQueue.call(printf, "Time: %s\n", ctime(&timestamp));
//       for (pRIDX = 0; pRIDX < printRecordsIDX;
//            pRIDX++) { // iterate through data
//         printf(" printRecords \tTemperature = %2.1f, \tPressure = %3.1f, "
//                "\tLDR = %1.2f;\n\r",
//                printRecord[pRIDX].temp, printRecord[pRIDX].pressure,
//                printRecord[pRIDX].LDR);
//       }

//       t.reset(); // reset flush timer
//       samplesInBuffer.release();
//     }
//   }
//   ThisThread::flags_clear(1);
// }

//my way - works as of 12/01
void bufferClass::flushBuffer(){
    if(samplesInBuffer.try_acquire_for(1ms) == 0){
        printQueue.call(emptyFlush);
        //errorSeverity(CRITICAL);
        return;
    }else{
        samplesInBuffer.release();
        if(bufferLock.trylock_for(1ms) == 0){
            printQueue.call(bufferFlushTimeout);
            //errorSeverity(WARNING);
        }else{
            FATFileSystem fs("sd", &mysd);
            FILE *fp = fopen("/sd/environmental_data.txt", "a");
            while(runFlush == 1){
                if(oldIDX == newIDX){
                    runFlush = 0; //everything is out
                } else{
                    //greenLED =!greenLED;
                    samplesInBuffer.try_acquire_for(1ms);
                    oldIDX = (oldIDX + 1) % buffer_size;
                    liveData flushRecord = buffer[oldIDX];
                    // fprintf(&fp, "Time recorded = );
                    fprintf(fp, " \tTemperature = %2.1f, \tPressure = %3.1f,\tLDR = % 1.2f;\n\r ", flushRecord.temp,
                                    flushRecord.pressure,
                                    flushRecord.LDR);

                    //printf("printing things\n");
                    spaceInBuffer.release();//space in buffer signal
                 }
             } //end while
            //samplesInBuffer.release();
            printQueue.call(flushedBuffer);
            //flash green led
            bufferLock.unlock();
            //printQueue.call(printf, "unlocked buffer after\n");
        }
    }
} //end function writeSD

// void bufferClass::printToWebpage(vector<int> & webpageData){
//     if(samplesInBuffer.try_acquire_for(1ms) == 0){
//         printQueue.call(printf, "no data to put on webpage\n");
//     } else{
//         if(bufferLock.trylock_for(1ms) == 0){
//             printQueue.call(printf, "buffer not locked to put data on
//             webpage\n");
//         } else{
//             liveData webpageData = buffer[newIDX]; //latest set of
//             environmental data bufferLock.unlock();
//             samplesInBuffer.release();
//         }
//     }
// }

void bufferClass::initSD() {
  // if(mysd.init() != 0){
  if (SDDetect == 1) {
    printQueue.call(mountError);
    greenLED = 0;
    cardMount = 0;
  } else {
    cardMount = 1;
    greenLED = 1;
    printQueue.call(mountedSD);
  }
}

// nicks way
int bufferClass::flushBufferUpgrade() {
  printf("Initialise and write to a file\n");
  int err1;
  err1 = mysd.init();
  if (0 != err1) {
    printf("Init failed %d\n", err1);
    return -1;
  }

  FATFileSystem fs("sd", &mysd);
  FILE *fp = fopen("/sd/testwrite.txt", "w");

  if (fp == NULL) {
    error("Could not open file for write\n");
    mysd.deinit();
    return -1;
  } else {
    //***********************
    while (runFlush == 1) {
      if (oldIDX == newIDX) {
        runFlush = 0; // everything is out
        //break;
        // return;
      } else {
        // greenLED =!greenLED;
        samplesInBuffer.try_acquire_for(1ms);
        oldIDX = (oldIDX + 1);
        liveData flushRecord = buffer[oldIDX];
        timeLock.lock();
        fprintf(fp, "Time recorded = %s\n", ctime(&timestamp));
        timeLock.unlock();
        fprintf(
            fp,
            " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",
            flushRecord.temp, flushRecord.pressure, flushRecord.LDR);

        // printf("printing things\n");
        // spaceInBuffer.release();//space in buffer signal
      }
    } // end while
    samplesInBuffer.release();
    printQueue.call(flushedBuffer);

    //***********************
    fclose(fp);
    printf("SD Write done...\n");
    mysd.deinit();
    return 0;
  }
}

