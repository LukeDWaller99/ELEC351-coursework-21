#include "mbed.h"
#include "sd.h"
#include "buffer.h"


bool SDState = 0;
FILE *fp; //file pointer

// //function to flash green LED before and after the SD card is being written to, LED alternates state during write process
// void flashGreen() {		
// 	//always leaves green on
// 	greenLED = 0;
// 	ThisThread::sleep_for(500);		//put this thread to sleep for 500ms
// 	greenLED = 1;
// }

void SDCard::initSD(){
    if (SDState == 0){ //the card is not mounted
        //criticalError - not fatal while initialising
        printQueue.call(mountError);
        //print to LCD?
    }else{
        //the sd card is mounted
        //greenLED = 1;
        printQueue.call(mountedSD);
        //print to lcd saying mounted?
    }
} //end initSD function

void SDCard::writeSD(){

    FATFileSystem fs("sd, &sd"); //create filing system
    FILE *fp = fopen("/sd/test.csv", "a"); //open so you can write

    if (fp == NULL){ //cannot open file - it does not exist
        //criticalError
        printQueue.call(noSDFile);
    }

    if(SDState == 0){
        //sd card is unmounted
        //criticalError
        printQueue.call(unmountedFlush);
    }else{
        bufferClass::flushBuffer(*fp); //pass flushBuffer function file pointer
        //bufferClass::flushBuffer(FILE &fp); //flush buffer data

    }
    fclose(fp); //close file
} //end writeSD function

void SDCard::unmountSD(){
    SDCard::writeSD();
    SDState = 0;
    printQueue.call(unmountedSD);
    //display on LCD?
} //end unmountSD function

//thread for SD writing
void SDCard::SDThread(){
    initSD();
    while(1){
        if(SDState == 1){
            //if buffer is almost full at 1 minute, flush

            //if buffer hasn't filled in an hour, still flush

        }
    }
} //end SDThread function