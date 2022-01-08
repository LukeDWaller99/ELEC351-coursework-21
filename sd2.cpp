#include "sd2.h"

// FILE *fp; 
// SDBlockDevice mysd(PB_5, PB_4, PB_3, PE_3);
// DigitalIn SDDetect(PF_4);
// DigitalOut greenLED(PC_6);

SDCard::SDCard(){
    //SDThread.start(callback(this, &SDCard::SDRun));
}

// //this works in the meantime but needs to be interrupt driven
// void SDCard::initSD(){
//     //if(mysd.init() != 0){
//     if(SDDetect == 1){
//         printQueue.call(mountError);
//         greenLED = 0;
//         cardMount = 0;
//     } else{
//         cardMount = 1;
//         greenLED = 1;
//         printQueue.call(mountedSD);
//     }
// }

// void SDCard::flashGreen(){
//     greenLED = 0;
//     ThisThread::sleep_for(50ms);
//     greenLED = 1;
// }

// void SDCard::writeSD(){
//     FATFileSystem fs("sd, &mysd");
//     FILE* fp = fopen("/sd/environmental_data", "w");

//     if(fp == NULL){ //cannot create file
//         printQueue.call(noSDFile);
//         //error
//     }

//     if(SDDetect == 1){ //no sd card mounted
//         printQueue.call(printf, "sd card is not mounted\n");
//     } else{
//         SDBuffer.flushBuffer(*fp);

//         //*******************

//         // while(runFlush == 1){
//         //         if(oldIDX == newIDX){
//         //             runFlush = 0; //everything is out
//         //         } else{
//         //             //greenLED =!greenLED;
//         //             samplesInBuffer.try_acquire_for(1ms);
//         //             oldIDX = (oldIDX + 1);
//         //             liveData flushRecord = buffer[oldIDX];
//         //             //fprintf(&fp, "Time recorded = );
//         //             fprintf(&fp, " \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
                    
//         //             printf("printing things\n");
//         //             //spaceInBuffer.release();//space in buffer signal
//         //          }
//         //      } //end while
//         //     samplesInBuffer.release();
//         //     printQueue.call(flushedBuffer);

//         //*******************
//     }
//     fclose(fp); //close file    
// }
