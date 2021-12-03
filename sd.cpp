#include "mbed.h"
#include "sd.h"
#include "uop_msb.h"
#include "FATFileSystem.h"

bool SDState = 0;

void initSD(){
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

void writeSD(){

    FATFileSystem fs("sd, &sd"); //create filing system
    FILE* fp = fopen("/sd/test.csv", "a"); //open, begin write

    if (fp == NULL){
        //criticalError
        printQueue.call(noSDFile);
    }

    if(SDState == 0){
        //sd card is unmounted
        //criticalError
        printQueue.call(unmountedFlush);
    }else{
        flush(*fp); //flush buffer data

    }
    fclose(fp); //close file
} //end writeSD function

void unmountSD(){
    writeSD();
    SDState = 0;
    printQueue.call(unmountedSD);
    //display on LCD?
} //end unmountSD function

//thread for SD writing
void SDThread(){
    initSD();
    while(1){
        if(SDState == 1){
            //if buffer is almost full at 1 minute, flush

            //if buffer hasn't filled in an hour, still flush

        }
    }
} //end SDThread function