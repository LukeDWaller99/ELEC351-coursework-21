#include "mbed.h"
#include "buffer.h"
#include "sampling.h"
#include <cstdio>

Semaphore spaceInBuffer(buffer_size);  //buffer space tracking
Semaphore samplesInBuffer(0);          //sample no tracking
Semaphore signalSample(0);             //signal to get new sample
SDBlockDevice mysd2(PB_5, PB_4, PB_3, PF_3);

// unsigned int newIDX = buffer_size - 1; 
// unsigned int oldIDX = buffer_size - 1;

liveData buffer[buffer_size];
samples sampledData;
sampler bufferSampler;
liveData dataRecord;

/*
sampleData - output signal that buffer is ready for data
acquireData - get the data
writeBuffer - check for space, if available, write data
writeSD - write all data to sd card
*/

//constructor
bufferClass::bufferClass(){
//buffer size
//thread to call buffer write every 11s?
   // bufferThread.start(callback(this, &))
//    bufferTick.attach(callback(this, &bufferClass::writeBuffer), 5s);

   //in the way the sampler was done:

   //bufferThread.start(callback(this, &))
}

//signal the sampling function
void bufferClass::sampleFunc(){
    signalSample.release(); 
}

void bufferClass::emptyBuffer(){
    samplesInBuffer.release(); //reset the semaphore 
}

void bufferClass::writeBuffer(){
    // bool spaceAvailable = spaceInBuffer.try_acquire_for(1ms);//check for space
    //     if(spaceAvailable == 0){
    //         printQueue.call(bufferFull);
    // //         //fatal error
    //     }else{
    // //         //printf("space available\n");
    //         if(bufferLock.trylock_for(1ms) == 0){
    //             printQueue.call(bufferLockTimeout);
    // //             //fatal error
    //         } else{
    //             if(dataLock.trylock_for(1ms) == 0){ //PROTECT THE DATA
    //             printQueue.call(timeLockTimeout);
    //             }else{ //dataLock = 1
                dataRecord.LDR = sampledData.LDR;
                dataRecord.temp = sampledData.temp;
                dataRecord.pressure = sampledData.pressure;

                //dataLock.unlock(); //release lock
                //}
                newIDX = (newIDX + 1); //increment buffer size
                buffer[newIDX] = dataRecord; //update the buffer
            //}
    //     bufferLock.unlock();
    // }
    //spaceInBuffer.release();
    //samplesInBuffer.release();
} // writeBuffer function end


void bufferClass::acquireData(){
    //bufferTick.attach(callback(this, &bufferClass::sampleFunc), 15s);
    while(1){
        signalSample.acquire();
        writeBuffer();
    }
}

void bufferClass::printBufferContents(){
    // bool isDataInBuffer = samplesInBuffer.try_acquire_for(1s);
    // if(isDataInBuffer ==0 ){
    //     printf("no data\n");
    // }
    // else{
    //     bool unlockingBuffer = bufferLock.trylock_for(1s);
    //     if(unlockingBuffer == 0){
    //         printf("could not unlock buffer\n");
    //     }
    //     else{
    //         int runPrint = 1;
    //         int printIDX = oldIDX;
    //         int printRecordsIDX = 0;
    //         while(runPrint == 1){
    //             if(printIDX == newIDX){
    //                 runPrint = 0;
    //             }
    //             else{
    //                 printIDX = (printIDX + 1) % buffer_size;
    //                 printRecord[printRecordsIDX] = buffer[printIDX];
    //                 printRecordsIDX++;
    //             }
    //         }

    //         bufferLock.unlock();
    //         for(int pRIDX = 0; pRIDX < printRecordsIDX; pRIDX++){
    //             printf(" printRecords \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", printRecord[pRIDX].temp, printRecord[pRIDX].pressure, printRecord[pRIDX].LDR);
    
    //         }
    //     }
    // }
    // samplesInBuffer.release();
}

//rename this, needed in new write SD function in sd.cpp
//void bufferClass::flushBuffer(){
void bufferClass::flushBuffer(FILE &fp){
    // //check for samples in the buffer
    // bool checkBuffer = samplesInBuffer.try_acquire_for(1s);
    // if(checkBuffer == 0){
    //     printQueue.call(emptyFlush);
    //     //errorSeverity(CRITICAL);
    //     return;
    // }else{
    //     samplesInBuffer.release();

        // if(bufferLock.trylock_for(1ms) == 0){
        //     printQueue.call(bufferFlushTimeout);
        //     //errorSeverity(WARNING);
        // }else{
            int run = 1;
            // FATFileSystem fs("sd", &mysd2);
            // FILE *fp = fopen("/sd/test.txt","w");

            while(run == 1){
                if(oldIDX == newIDX){
                    run = 0; //everything is out
                } else{
                    //greenLED =!greenLED;
                    samplesInBuffer.try_acquire_for(1ms);
                    oldIDX = (oldIDX + 1);
                    liveData flushRecord = buffer[oldIDX];
                    //fprintf(&fp, "Time recorded = );
                    //fprintf(&fp, " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
                    
                    fprintf(&fp, "printing things\n");
                    //spaceInBuffer.release();//space in buffer signal
                 }
             } //end while
            samplesInBuffer.release();
            printQueue.call(flushedBuffer);
            //flash green led
            //bufferLock.unlock();
            //printQueue.call(printf, "unlocked buffer after\n");
        //}
//    }
} //end function writeSD

