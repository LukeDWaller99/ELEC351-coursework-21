//AUTHOR: NOAH HARVEY

#include "buffer.h"
#include <ctime>


Semaphore spaceInBuffer(buffer_size);  //space in buffer
Semaphore samplesInBuffer(0);          //samples in buffer
Semaphore signalSample(0);             //signal to get new sample

liveData buffer[buffer_size];
liveData dataRecord; //for holding data in the buffer
liveData printRecord[buffer_size];

//sampler bufferSampler; 
samples sampledData;

FILE *fp; 
SDBlockDevice mysd(PB_5, PB_4, PB_3, PF_3);
DigitalIn SDDetect(PF_4);
DigitalOut greenLED(PC_6);

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

// //signal the sampling function (ISR)
// void bufferClass::sampleFunc(){
//     signalSample.release(); 
// }

// void bufferClass::emptyBuffer(){
//     samplesInBuffer.try_acquire();
//     samplesInBuffer.release(); //reset the semaphore 
// }

void bufferClass::writeBuffer(){
        if(spaceInBuffer.try_acquire_for(1ms) == 0){ 
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
                } else{ 
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
                newIDX = (newIDX + 1) % buffer_size; //increment buffer size
                buffer[newIDX] = dataRecord; //update the buffer
            }
        bufferLock.unlock();
    }
    samplesInBuffer.release(); //sample added signal
}

// void bufferClass::acquireData(){
//     //bufferTick.attach(callback(this, &bufferClass::sampleFunc), 11s);
//     while(1){
//         signalSample.acquire();
//         writeBuffer();
//     }
// }

void bufferClass::printBufferContents(){
    if(samplesInBuffer.try_acquire_for(1ms) == 0){
        printf("no data\n"); 
    } else{
        if(bufferLock.trylock_for(1ms) == 0){
            printf("could not unlock buffer\n");
        } else{
            while(runPrint == 1){
                if(printIDX == newIDX){ //all data from buffer has been copied
                    runPrint = 0; //terminate
                    //printQueue.call(printf, "all data printed\n");
                } else{
                    printIDX = (printIDX + 1) % buffer_size; //update print idx
                    printRecord[printRecordsIDX] = buffer[printIDX]; //copy the data
                    printRecordsIDX++; //increment print idx
                }
            }
            bufferLock.unlock();
            for(pRIDX = 0; pRIDX < printRecordsIDX; pRIDX++){ //iterate through data
                //printQueue.call(printf, "print all data now\n");
                //timestamp = time(NULL);
                printf("Time: %s\n", ctime(&timestamp));
                printf(" printRecords \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", printRecord[pRIDX].temp, printRecord[pRIDX].pressure, printRecord[pRIDX].LDR);
            }
        }
    }
    samplesInBuffer.release(); 
}

// void bufferClass::flushBuffer(FILE &fp){
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
//             // FATFileSystem fs("sd", &mysd2);
//             // FILE *fp = fopen("/sd/environmental_data.txt","w");

//             while(runFlush == 1){
//                 if(oldIDX == newIDX){
//                     runFlush = 0; //everything is out
//                 } else{
//                     //greenLED =!greenLED;
//                     samplesInBuffer.try_acquire_for(1ms);
//                     oldIDX = (oldIDX + 1);
//                     liveData flushRecord = buffer[oldIDX];
//                     //fprintf(&fp, "Time recorded = );
//                     fprintf(&fp, " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
                    
//                     printf("printing things\n");
//                     //spaceInBuffer.release();//space in buffer signal
//                  }
//              } //end while
//             samplesInBuffer.release();
//             printQueue.call(flushedBuffer);
//             //flash green led
//             //bufferLock.unlock();
//             //printQueue.call(printf, "unlocked buffer after\n");
//         }
//     }
// } //end function writeSD


// void bufferClass::printToWebpage(vector<int> & webpageData){
//     if(samplesInBuffer.try_acquire_for(1ms) == 0){
//         printQueue.call(printf, "no data to put on webpage\n");
//     } else{
//         if(bufferLock.trylock_for(1ms) == 0){
//             printQueue.call(printf, "buffer not locked to put data on webpage\n");
//         } else{
//             liveData webpageData = buffer[newIDX]; //latest set of environmental data
//             bufferLock.unlock();
//             samplesInBuffer.release();
//         }
//     }
// }

void bufferClass::initSD(){
    //if(mysd.init() != 0){
    if(SDDetect == 1){
        printQueue.call(mountError);
        greenLED = 0;
        cardMount = 0;
    } else{
        cardMount = 1;
        greenLED = 1;
        printQueue.call(mountedSD);
    }
}

void bufferClass::flushBufferUpgrade(){
    printf("Initialise and write to a file\n");
    int err1;
    err1=mysd.init();
    if ( 0 != err1) {
        printf("Init failed %d\n",err1);
        //return;
    }
    
    FATFileSystem fs("sd", &mysd);
    FILE *fp = fopen("/sd/test.txt","w");

    if(fp == NULL) {
        error("Could not open file for write\n");
        mysd.deinit();
        //return;
    } else{
        //***********************
        while(runFlush == 1){
                if(oldIDX == newIDX){
                    runFlush = 0; //everything is out
                    break;
                    //return;
                } else{
                    //greenLED =!greenLED;
                    samplesInBuffer.try_acquire_for(1ms);
                    oldIDX = (oldIDX + 1);
                    liveData flushRecord = buffer[oldIDX];
                    fprintf(fp, "Time recorded = %s\n", ctime(&timestamp));
                    fprintf(fp, " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
                    
                    //printf("printing things\n");
                    //spaceInBuffer.release();//space in buffer signal
                }
             } //end while
            samplesInBuffer.release();
            printQueue.call(flushedBuffer);

        //***********************
        fclose(fp);
        printf("SD Write done...\n");
        mysd.deinit();
    }
    
}


// void bufferClass::write_sdtest()
// {
//     printf("Initialise and write to a file\n");
//     int err;
//     // call the SDBlockDevice instance initialisation method.

//     err=mysd.init();
//     if ( 0 != err) {
//         printf("Init failed %d\n",err);
//         return;
//     }
    
//     FATFileSystem fs("sd", &mysd);
//     FILE *fp = fopen("/sd/test.txt","w");
//     if(fp == NULL) {
//         error("Could not open file for write\n");
//         mysd.deinit();
//         //return -1;
//     } else {
//         //Put some text in the file...
//         fprintf(fp, "Martin Says Hi.. there is nothing in here!\n");
//         //Tidy up here
//         fclose(fp);
//         printf("SD Write done...\n");
//         mysd.deinit();
//         //return 0;
//     }
    
// }