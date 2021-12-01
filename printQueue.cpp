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

void flushBuffer(){
    printf("get flushed! data from buffer loaded onto sd card. \n"); 
}
