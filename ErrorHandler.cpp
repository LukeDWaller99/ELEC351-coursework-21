//AUTHOR - JACK PENDLEBURY
#include "Callback.h"
#include "SevenSegmentDisplay.h"
#include "ThisThread.h"
#include <ErrorHandler.h>
#include <cstdio>

int flag_value = 0;
int currentErrorSeverity = 0;

Thread ERROR_THREAD_NAME;

void ErrorHandler::clear_all(){
    if (flagLock.trylock_for(10ms) == true){
        ThisThread::flags_clear(0x7fffffff);
        flag_value = 0;
        flagLock.unlock();
    }
    else 
    {
        NVIC_SystemReset(); // resets system - this is called if the error handler cannot clear flags 
    }
}

ErrorHandler::ErrorHandler(){
    // ERROR_THREAD_NAME.set_priority(osPriorityRealtime);
    ERROR_THREAD_NAME.start(callback(this, &ErrorHandler::error_thread));
    // errorDisplay.test();
}

void ErrorHandler::error_thread(void){
    while(true){
    ThisThread::flags_wait_any(0x7fffffff, false); // sleeps thread until any flag is raised

    int errorNumber = ThisThread::flags_get();

    errorDisplay = errorNumber;

    switch(currentErrorSeverity) 
    {
        case WARNING:
        printf("Warning Error\n");
        // yellow LED
        break;

        case CRITICAL:
        printf("Critical Error\n");
        // turn on red led
        // buzzer for 30 seconds
        // system reset 
        break;

        case FATAL:
        printf("Fatal Error\n");
        ThisThread::sleep_for(1000ms);
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong 
        break;

        case BUFF_FULL:
        printf("Buffer Full Error\n");
        // turn on red LED
        break;

        case ENV_ERR:
        printf("ENV Error\n");
        // sound buzzer
        break; 
    }
    clear_all();
    }
}


void ErrorHandler::setErrorFlag(int errorCode){

    int severityVal = errorCode >> 8; 

    int errorVal = errorCode & 255;

    errorSeverity severity = ErrorHandler::errorSeverity(severityVal);

    switch(severity) 
    {
        case WARNING:

        currentErrorSeverity = WARNING;
        ERROR_THREAD_NAME.flags_set(errorVal);
        break;

        case CRITICAL:
        currentErrorSeverity = CRITICAL;
        ERROR_THREAD_NAME.flags_set(errorVal);
        break;

        case FATAL:
        currentErrorSeverity = FATAL;
        ERROR_THREAD_NAME.flags_set(99); // fatal hardware error
        break;

        case BUFF_FULL:
        currentErrorSeverity = BUFF_FULL;
        ERROR_THREAD_NAME.flags_set(errorVal);
        break;

        case ENV_ERR:
        currentErrorSeverity = ENV_ERR;
        ERROR_THREAD_NAME.flags_set(errorVal);
        break;
    }

}

