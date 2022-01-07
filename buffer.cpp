//AUTHOR: NOAH HARVEY

#include "buffer.h"

Semaphore spaceInBuffer(buffer_size);  //space in buffer
Semaphore samplesInBuffer(0);          //samples in buffer
Semaphore signalSample(0);             //signal to get new sample

liveData buffer[buffer_size];
liveData dataRecord; //for holding data in the buffer
liveData printRecord[buffer_size];

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


void bufferClass::writeBuffer2(){
    bool spaceAvailable = spaceInBuffer.try_acquire_for(20ms);//check for space
        // if(spaceAvailable == 0){ 
        //     printQueue.call(bufferFull);
        //     //fatal error
        // }else{
            //PROTECT THE BUFFER
            bufferLock.lock();

            // if(bufferLock.trylock_for(1ms) == 0){
            //     printQueue.call(bufferLockTimeout);
            //     //fatal error
            // } else{
            //     //PROTECT THE TIME
            //     if(timeLock.trylock_for(1ms) == 0){ //PROTECT THE DATA
            //         printQueue.call(timeLockTimeout);
            //     }else{ //dataLock = 1

                    //************************************

                    //adding time here

                    //************************************

                    //copy environmental data
                    dataRecord.LDR = sampledData.LDR;
                    dataRecord.temp = sampledData.temp;
                    dataRecord.pressure = sampledData.pressure;
                    bufferLock.unlock();
                    //timeLock.unlock(); 
               // }

                //update the buffer
                newIDX = (newIDX + 1) % buffer_size; //increment buffer size
                buffer[newIDX] = dataRecord; //update the buffer
            //}
       // bufferLock.unlock();
   // }
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
    if(isDataInBuffer == 0 ){ //there are no samples in the buffer
        printf("no data\n");
    }
    else{
        bool unlockingBuffer = bufferLock.trylock_for(1ms); //protect the buffer
        if(unlockingBuffer == 0){
            printf("could not unlock buffer\n");
        }
        else{
            //kept in header
            // int runPrint = 1;
            // int printIDX = oldIDX; //work from oldest to newest sets data
            // int printRecordsIDX = 0; //track no of data sets
            while(runPrint == 1){
                if(printIDX == newIDX){ //all data from buffer has been copied
                    runPrint = 0; //terminate
                    printQueue.call(printf, "all data printed\n");
                }
                else{
                    printIDX = (printIDX + 1) % buffer_size; //update print idx
                    printRecord[printRecordsIDX] = buffer[printIDX]; //copy the data
                    printRecordsIDX++; //increment print idx
                }
            }
            bufferLock.unlock();
            for(pRIDX = 0; pRIDX < printRecordsIDX; pRIDX++){ //iterate through data
                //printQueue.call(printf, "print all data now\n");
                printf(" printRecords \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", printRecord[pRIDX].temp, printRecord[pRIDX].pressure, printRecord[pRIDX].LDR);
            }
        }
    }
    samplesInBuffer.release(); 
}

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


// void bufferClass::printToWebpage(vector<int> & webpageData){
    
//     if(samplesInBuffer.try_acquire_for(1ms) == 0){
//         printQueue.call(printf, "no data to put on webpage\n");
//     }
//     else{
//         if(bufferLock.trylock_for(1ms) == 0){
//             printQueue.call(printf, "buffer not locked to put data on webpage\n");
//         }
//         else{
//             liveData webpageData = buffer[newIDX]; //latest set of environmental data
//             //******************************
//             //update to index through all once tested
//             //******************************
//             bufferLock.unlock();
//             samplesInBuffer.release();
//             //return webpageData;
//             //dont need to return anything with this approach
//         }
//     }
// }
