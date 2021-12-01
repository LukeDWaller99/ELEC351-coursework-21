#include "mbed.h"
#include "sd.h"
#include "uop_msb.h"
#include "FATFileSystem.h"

bool SDState = 0;

void initSD(){
    if (SDState == 0){ //the card is not mounted
        //criticalError
    }else{
        //the sd card is mounted
        //greenLED = 1;
        //printQueue

    }

}

void writeSD(){

    FATFileSystem fs("sd, &sd");

    FILE* fp = fopen("/sd/test.csv", "a");

    if (fp == NULL){
        //criticalError
    }

    if(SDState == 0){
        //sd card is unmounted
        //criticalError
    }else{
        //flush(*fp);
        //TFlush = T1;

    }
    fclose(fp);
}

