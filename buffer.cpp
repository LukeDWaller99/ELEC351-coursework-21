//AUTHOR - NOAH HARVEY

#include "mbed.h"
#include "buffer.h"

Semaphore spaceBuffer(buffer_size); //buffer space tracking
Semaphore samplesBuffer(0);         //sample no tracking
Semaphore signalSample(0);          //signal to get new sample

liveData buffer[buffer_size];
liveData dataRecord;
samples sampleData;

unsigned int newIDX = buffer_size - 1;
unsigned int oldIDX = buffer_size - 1;


/*
sampleData - output signal that buffer is ready for data
acquireData - get the data
writeBuffer - check for space, if available, write data
writeSD - write all data to sd card
*/

//constructor
bufferClass::bufferClass(){
//buffer size
    
}

//signal the sampling function
void bufferClass::sampleData(){
    signalSample.release();
}
//void sensorData(sampleData.temp, sampleData.pressure, sampleData.LDR);

void bufferClass::writeBuffer(){

//(sampleData.temp, sampleData.pressure, sampleData.LDR){
       //check for space
    bool spaceAvailable = spaceBuffer.try_acquire_for(1s);
        if(spaceAvailable == 0){
            printQueue.call(bufferFull);
            //severityHandler(CRITICAL);

        }else{
            
            if(bufferLock.trylock_for(1s) == 0){
                printQueue.call(bufferLockTimeout);
                //errorSeverity(CRITICAL);

            } else{
                

                if(dataLock.trylock_for(1s) == 0){
                    //the data has taken too long to access
                    //printQueue.call(bLockData);
                    //errorSeverity(CRITICAL);
                }else{   //dataLock = 1
                //copy all sensor data, for Jack to decide how
                //  dataRecord.LDR = sampleData.LDR;
                //  dataRecord.temp = sampleData.temp;
                //  dataRecord.pressure = sampleData.pressure;
                 //dataRecord.humidity = 
                dataLock.unlock(); //release time lock
                }
                
                //update the buffer
                newIDX = (newIDX + 1); //increment buffer size
                buffer[newIDX] = dataRecord;
            }
    
        bufferLock.unlock();
    }
    //sample added, release signal
    samplesBuffer.release();

} // writeBuffer function end


void bufferClass::acquireData(){
    //jacks data in here
    //buff.attach(&Sampler, T);
    while(1){
        signalSample.acquire();
        writeBuffer();
    }
}

//rename this, needed in new write SD function in sd.cpp
void bufferClass::flushBuffer(FILE &fp){

    //check for samples in the buffer
    bool checkBuffer = samplesBuffer.try_acquire_for(1s);
    if(checkBuffer == 0){
        printQueue.call(emptyFlush);
        //errorSeverity(CRITICAL);
        return;
    }else{
        samplesBuffer.release();

        //bool lockBufferTry = lockBuffer.trylock_for(1s);
        if(bufferLock.trylock_for(1s) == 0){
            printQueue.call(bufferFlushTimeout);
            //errorSeverity(WARNING);
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
                    //fprintf(&fp, "Time recorded = %d:%d:%d %d/%d/%d, \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r",flushRecord.hour,flushRecord.minute,flushRecord.second,flushRecord.day,flushRecord.month,flushRecord.year,flushRecord.temp,flushRecord.pressure,flushRecord.LDR);
                    
                    //just jacks sampled values
                    fprintf(&fp, " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
                    
                    spaceBuffer.release();//space in buffer signal
                }
            } //end while
            printQueue.call(flushedBuffer);
            //flash green led
            bufferLock.unlock();
        }
   }
} //end function writeSD

