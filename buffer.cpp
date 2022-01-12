#include "buffer.h"

Semaphore spaceInBuffer(buffer_size); // space in buffer
Semaphore samplesInBuffer(0);         // samples in buffer
Semaphore signalSample(0);            // signal to get new sample

//sampler buffersampler;
samples sampledData;

FILE *fp;
SDBlockDevice mysd(PB_5, PB_4, PB_3, PF_3);
DigitalIn SDDetect(PF_4);
DigitalOut greenLED(PC_6);

// constructor
bufferClass::bufferClass(sampler* buffersampler, ErrorHandler* bufferEH, CustomQueue* bufferPQ) {
    BF = buffersampler;
    BEH = bufferEH;
    PQ = bufferPQ;
  t.start();
  writeThread.start(callback(this, &bufferClass::writeBufferAuto));
  bufferWriteTick.attach(callback(this, &bufferClass::writeFlag), 12s);
  flushThread.start(callback(this, &bufferClass::whenToFlush));
  bufferFlushTick.attach(callback(this, &bufferClass::flushFlag), 10s);
}



void bufferClass::writeFlag() { bufferClass::writeThread.flags_set(1); }

void bufferClass::flushFlag() { bufferClass::flushThread.flags_set(1); }

void bufferClass::writeBufferAuto() {
  while (true) {
    ThisThread::flags_wait_any(1);
    if (spaceInBuffer.try_acquire_for(1ms) == 0) {
      PQ->custom.call(printf, "buffer full\n");
      BEH -> setErrorFlag(BUFFER_FULL); //critical error
    } else {
      if (bufferLock.trylock_for(1ms) == 0) {// PROTECT THE BUFFER
        PQ->custom.call(printf, "buffer lock timeout\n");
        BEH -> setErrorFlag(BUFFER_LOCK_TIMEOUT); //critical error
      } else {
        if (timeLock.trylock_for(1ms) == 0) { // PROTECT THE DATA
          PQ->custom.call(printf, "timeLockTimeout\n");
        BEH -> setErrorFlag(TIMER_LOCK_TIMEOUT); //critical error
        } else {
      sampledData =  BF->sampleData;
          dataRecord.realTime = ctime(&timestamp);
          timeLock.unlock();
          // copy environmental data
          dataRecord.LDR = sampledData.LDR;
          dataRecord.temp = sampledData.temp;
          dataRecord.pressure = sampledData.pressure;
          dataInBuffer++; //increment no of data sets
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
//   PQ->custom.call(printf,
//                   "Number of environmental data sets in the buffer: %i\n",
//                   dataInBuffer);
                  PQ->custom.call(printf,
                  "Number of environmental data sets in the buffer: %i\n",
                  newIDX);
}

void bufferClass::whenToFlush() {
  while (true) {
    ThisThread::flags_wait_any(1);
    currentTime = duration_cast<seconds>(t.elapsed_time()).count();

    //if ((currentTime > 60) && (oldIDX - (buffer_size * 0.1))) {
    if((currentTime > 60) && (newIDX == oldIDX - (buffer_size * 0.1))) {// flush buffer at 90%
      //printQueue.call(printf, "capacity flush\n");
      PQ->custom.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      bufferClass::printBufferContents();
      dataInBuffer = 0;
    }
    // //in the case of not catching 90%, for whatever reason
    // if((currentTime > 60) && (newIDX == (oldIDX - buffer_size))) {// flush buffer at 90%
    //   //printQueue.call(printf, "capacity flush\n");
    //   bufferPrintQueue.custom.call(printf, "Time recorded = %s\n", ctime(&timestamp));
    //   bufferClass::printBufferContents();
    //   dataInBuffer = 0;
    // }
    if (currentTime == hourPassed){ //must flush at least once an hour
      //flash green led
      PQ->custom.call(printf, "Time recorded = %s\n", ctime(&timestamp));
      bufferClass::printBufferContents();
      dataInBuffer = 0;
      //printQueue.call(printf, "timing flush\n");
    }
    dataInBuffer = 0; //reset the count as buffer has flushed
  }
  ThisThread::flags_clear(1);
}

void bufferClass::printBufferContents() {
  while (true) {
    ThisThread::flags_wait_any(1);

    if (samplesInBuffer.try_acquire_for(1ms) ==
        0) { // check for samples in the buffer
        BEH -> setErrorFlag(EMPTY_FLUSH); //critical error
     PQ->custom.call(printf, "no data\n");
    } else {
      samplesInBuffer.release();
      if (bufferLock.trylock_for(1ms) == 0) { // try to acquire buffer data
        BEH -> setErrorFlag(BUFFER_LOCK_TIMEOUT); //critical error
        //bufferPrintQueue.custom.call(printf, "could not unlock buffer\n");
      } else {
        while (runPrint == 1) {   // remain in loop, iterating through data
          if (oldIDX == newIDX) { // all data from buffer has been copied
            runPrint = 0;         // terminate
          } else {
            samplesInBuffer.try_acquire_for(1ms);
            oldIDX = (oldIDX + 1) % buffer_size;
            liveData flushRecord = buffer[oldIDX];
            PQ->custom.call(printf, "Time recorded = %s\n\t", flushRecord.realTime);
            PQ->custom.call(
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



// //my way
// void bufferClass::flushBuffer(){
//     if(samplesInBuffer.try_acquire_for(1ms) == 0){
//         printQueue.call(emptyFlush);
//         //errorSeverity(CRITICAL);
//         return;
//     }else{
//         samplesInBuffer.release();
//         if(bufferLock.trylock_for(1ms) == 0){
//             printQueue.call(bufferFlushTimeout);
//             //errorSeverity(WARNING);
//         }else{
//             FATFileSystem fs("sd", &mysd);
//             FILE *fp = fopen("/sd/environmental_data.txt","w");

//             while(runFlush == 1){
//                 if(oldIDX == newIDX){
//                     runFlush = 0; //everything is out
//                 } else{
//                     //greenLED =!greenLED;
//                     samplesInBuffer.try_acquire_for(1ms);
//                     oldIDX = (oldIDX + 1);
//                     liveData flushRecord = buffer[oldIDX];
//                     //fprintf(&fp, "Time recorded = );
//                     fprintf(fp, " \tTemperature = %2.1f, \tPressure = %3.1f,
//                     \tLDR = %1.2f;\n\r", flushRecord.temp,
//                     flushRecord.pressure, flushRecord.LDR);

//                     printf("printing things\n");
//                     //spaceInBuffer.release();//space in buffer signal
//                  }
//              } //end while
//             samplesInBuffer.release();
//             printQueue.call(flushedBuffer);
//             //flash green led
//             bufferLock.unlock();
//             //printQueue.call(printf, "unlocked buffer after\n");
//         }
//     }
// } //end function writeSD

void bufferClass::printToWebpage(vector<int> & webpageData){
    if(samplesInBuffer.try_acquire_for(1ms) == 0){
        //printQueue.call(printf, "no data to put on webpage\n");
        //new warning error? empty flush to webpage?
    } else{
        if(bufferLock.trylock_for(1ms) == 0){
            BEH -> setErrorFlag(BUFFER_LOCK_TIMEOUT); //critical error

            // printQueue.call(printf, "buffer not locked to put data on
            // webpage\n");
        } else{
            liveData webpageData = buffer[newIDX]; //latest set of
            //environmental data 
            bufferLock.unlock();
            samplesInBuffer.release();
        }
    }
}

void bufferClass::initSD() {
  // if(mysd.init() != 0){
  if (SDDetect == 1) {
    PQ->custom.call(printf, "sd card not mounted\n");
    greenLED = 0;
    cardMount = 0;
  } else {
    cardMount = 1;
    greenLED = 1;
    PQ->custom.call(printf, "sd card is moutned\n");
  }
}

// nicks way
// int bufferClass::flushBufferUpgrade() {
//   printf("Initialise and write to a file\n");
//   int err1;
//   err1 = mysd.init();
//   if (0 != err1) {
//     printf("Init failed %d\n", err1);
//     return -1;
//   }

//   FATFileSystem fs("sd", &mysd);
//   FILE *fp = fopen("/sd/test.txt", "w");

//   if (fp == NULL) {
//     error("Could not open file for write\n");
//     mysd.deinit();
//     return -1;
//   } else {
//     //***********************
//     while (runFlush == 1) {
//       if (oldIDX == newIDX) {
//         runFlush = 0; // everything is out
//         break;
//         // return;
//       } else {
//         // greenLED =!greenLED;
//         samplesInBuffer.try_acquire_for(1ms);
//         oldIDX = (oldIDX + 1);
//         liveData flushRecord = buffer[oldIDX];
//         //changed this, no longer require timelock as its all saved time
//         fprintf(fp, "Time recorded = %s\n", ctime(&timestamp));
    
//         fprintf(
//             fp,
//             " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",
//             flushRecord.temp, flushRecord.pressure, flushRecord.LDR);

//         // printf("printing things\n");
//         // spaceInBuffer.release();//space in buffer signal
//       }
//     } // end while
//     samplesInBuffer.release();
//     bufferPrintQueue.custom.call(printf, "flushed buffer\n");

//     //***********************
//     fclose(fp);
//     printf("SD Write done...\n");
//     mysd.deinit();
//     return 0;
//   }
// }

