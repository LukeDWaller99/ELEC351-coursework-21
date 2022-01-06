//AUTHOR: NOAH HARVEY

#include "buffer.h"

Semaphore spaceInBuffer(buffer_size);  //space in buffer
Semaphore samplesInBuffer(0);          //samples in buffer
Semaphore signalSample(0);             //signal to get new sample

liveData buffer[buffer_size];
liveData dataRecord; //for holding data in the buffer

sampler bufferSampler; 
samples sampledData;
SDBlockDevice mysd2(PB_5, PB_4, PB_3, PF_3);

/*
sampleData - output signal that buffer is ready for data
acquireData - get the data
writeBuffer - check for space, if available, write data
writeSD - write all data to sd card
*/

//constructor
bufferClass::bufferClass(){
    //THREAD TO CALL WRITE BUFFER WITH A FLAG SET BY SAMPLER?
}

//signal the sampling function (ISR)
void bufferClass::sampleFunc(){
    signalSample.release(); 
}

void bufferClass::emptyBuffer(){
    samplesInBuffer.release(); //reset the semaphore 
}

void bufferClass::writeBuffer(){
    bool spaceAvailable = spaceInBuffer.try_acquire_for(1ms);//check for space
        if(spaceAvailable == 0){ 
            printQueue.call(bufferFull);
            //fatal error
        }else{
            //PROTECT THE BUFFER
            if(bufferLock.trylock_for(1ms) == 0){
                printQueue.call(bufferLockTimeout);
                //fatal error
            } else{
                //PROTECT THE TIME
                if(timeLock.trylock_for(1ms) == 0){ //PROTECT THE DATA
                    printQueue.call(timeLockTimeout);
                }else{ //dataLock = 1

                    //************************************

                    //adding time here

                    //************************************

                    //copy environmental data
                    dataRecord.LDR = sampledData.LDR;
                    dataRecord.temp = sampledData.temp;
                    dataRecord.pressure = sampledData.pressure;

                    timeLock.unlock(); 
                }

                //update the buffer
                newIDX = (newIDX + 1); //increment buffer size
                buffer[newIDX] = dataRecord; //update the buffer
            }
        bufferLock.unlock();
    }
    //sample added signal
    samplesInBuffer.release();
} // writeBuffer function end


void bufferClass::acquireData(){
    bufferTick.attach(callback(this, &bufferClass::sampleFunc), 11s);
    while(1){
        signalSample.acquire();
        writeBuffer();
    }
}

void bufferClass::printBufferContents(){
    bool isDataInBuffer = samplesInBuffer.try_acquire_for(1ms);
    if(isDataInBuffer ==0 ){
        printf("no data\n");
    }
    else{
        bool unlockingBuffer = bufferLock.trylock_for(1ms);
        if(unlockingBuffer == 0){
            printf("could not unlock buffer\n");
        }
        else{
            int runPrint = 1;
            int printIDX = oldIDX;
            int printRecordsIDX = 0;
            while(runPrint == 1){
                if(printIDX == newIDX){
                    runPrint = 0;
                }
                else{
                    printIDX = (printIDX + 1) % buffer_size;
                    printRecord[printRecordsIDX] = buffer[printIDX];
                    printRecordsIDX++;
                }
            }
            bufferLock.unlock();
            for(int pRIDX = 0; pRIDX < printRecordsIDX; pRIDX++){
                printf(" printRecords \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", printRecord[pRIDX].temp, printRecord[pRIDX].pressure, printRecord[pRIDX].LDR);
            }
        }
    }
    samplesInBuffer.release();
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


//this function is not in the class, dont know how to return the data when it is?
/*
liveData printToWebpage(){
    
    bool isThereData = samplesInBuffer.try_acquire_for(1ms);
    if(isThereData == 0){
        printQueue.call(printf, "no data to put on webpage\n");
    }
    // not a fatal error but should probably output something?

    //liveData webpageData = 
    //return webpageData

    else{
        if(bufferLock.trylock_for(1ms) == 0){
            printQueue.call(printf, "buffer not locked to put data on webpage\n");
        }

        else{
            liveData webpageData = buffer[newIDX]; //latest set of environmental data
            //******************************
            //update to index through all values when i know how
            //******************************
            bufferLock.unlock();
            samplesInBuffer.release();

            return webpageData;
        }
    }
}
*/