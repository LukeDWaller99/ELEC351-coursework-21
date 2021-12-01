#include "mbed.h"
#include "buffer.h"
#include "sd.h"


Semaphore spaceBuffer(buffer_size);
Semaphore samplesBuffer(0); //
Semaphore signalSample(0); //signal to get new sample

liveData buffer[buffer_size];
unsigned int newIDX = buffer_size - 1;
unsigned int oldIDX = buffer_size - 1;

Mutex lockBuffer;

Ticker buff;
/*
sampleData - output signal that buffer is ready for data
acquireData - 
writeBuffer - check for space, if available, write data
writeSD - write all data to sd card
*/

//signal the sampling function
void sampleData(){
    signalSample.release();
}

void writeBuffer(){
    //check for space
    bool spaceAvailable = spaceBuffer.try_acquire_for(1s);
        if(spaceAvailable == 0){
            printQueue.call(bufferFull);
            //criticalError

        }else{
            bool lockBufferTry = lockBuffer.trylock_for(1s);
            if(lockBufferTry == 0){
                printQueue.call(bufferLockTimeout);
                //criticalError

            } else{
                bool timeLock = Time.trylock_for(1s);
                if(timeLock == 0){
                    printQueue.call(timeLockTimeout);
                    //critical error
                }else{
                //Time.lock();//maybe?
                //copy time and date
                
                // dataRecord.day = T1.day;
                // dataRecord.month = T1.month;
                // dataRecord.year = T1.year;
                //release time lock

                Time.unlock();
                }
                //copy all sensor data
                // dataRecord.LDR = 
                // dataRecord.temp = 
                // dataRecord.pressure = 
        
                //update the buffer
                newIDX = (newIDX + 1); //increment buffer size
                buffer[newIDX] = dataRecord;
            }
    
        lockBuffer.unlock();
    }
    //sample added, release signal
    samplesBuffer.release();

} // writeBuffer function end


void acquireData(){
    //jacks data in here
    while(1){
        signalSample.acquire();
        writeBuffer();
    }
}


void writeSD(FILE &fp){

    //check for samples in the buffer
    bool checkBuffer = samplesBuffer.try_acquire_for(1s);
    if(checkBuffer == 0){
        printQueue.call(emptyFlush);
        //criticalError- not so fatal?
        return;
    }else{
        samplesBuffer.release();

        bool lockBufferTry = lockBuffer.trylock_for(1s);
        if(lockBufferTry == 0){
            printQueue.call(bufferFlushTimeout);
            //criticalError - not so fatal?
        }else{
            int run = 1;
            while(run == 1){
                if(oldIDX == newIDX){
                    run = 0; //everything is out
                } else{
                    //greenLED =!greenLED;
                    samplesBuffer.try_acquire_for(1s);
                    oldIDX = (oldIDX + 1);
                    liveData flushRecord = buffer[oldIDX];
                    spaceBuffer.release();//space in buffer signal
                }
            } //end while
            printQueue.call(flushBuffer);
            lockBuffer.unlock();
        }
   }
} //end function writeSD

