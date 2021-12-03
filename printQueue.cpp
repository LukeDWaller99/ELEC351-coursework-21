#include "printQueue.h"

EventQueue printQueue;

//**************************
//BUFFER PRINT QUEUES
//**************************
void bufferFull(){
    printf("CRITICAL ERROR - the buffer is full. you cannot write to the buffer. \n");
}

void bufferLockTimeout(){
    printf("CRITICAL ERROR - mutex lock timeout, cannot acquire lock. \n");

}

void timeLockTimeout(){
    printf("CRITICAL ERROR - time lock timeout. \n");
}

void emptyFlush(){
    printf("NOT SO CRITICAL ERROR - worth mentioning that the buffer is empty \n");
}

void bufferFlushTimeout(){
    printf("CRITIAL ERROR - buffer flush lock timeout, cannot acquire lock. \n");
}

void flushedBuffer(){
    printf("get flushed! data from buffer loaded onto sd card. \n"); 
}

//**************************
//SD CARD PRINT QUEUES
//**************************

void mountError(){
    printf("sd card - not mounted \n"); 
}

void mountedSD(){
    printf("sd card is mounted \n"); 
}
void noSDFile(){
    printf("sd card - cannot create new file \n"); 
}

void unmountedFlush(){
    printf("sd card - unmounted while flushing. \n"); 
}

void unmountedSD(){
    printf("sd card - you have unmounted the SD card \n"); 
}