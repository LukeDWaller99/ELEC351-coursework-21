//AUTHOR - JACK PENDLEBURY
#include "Callback.h"
#include "SevenSegmentDisplay.h"
#include "ThisThread.h"
#include <ErrorHandler.h>
#include <cstdio>

int flag_value = 0;
int currentErrorSeverity = 0;

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


ErrorHandler::ErrorHandler(EventQueue* outputQueue):override_button(USER_BUTTON){
    // ERROR_THREAD_NAME.set_priority(osPriorityRealtime);
    queue = outputQueue;
    ERROR_THREAD_NAME.start(callback(this, &ErrorHandler::error_thread));
    ERROR_SERVICER.start(callback(&errorQueue, &EventQueue::dispatch_forever));
    override_button.rise(callback(this, &ErrorHandler::alarm_override));
    // errorDisplay.test();
}


void ErrorHandler::error_thread(void){
    while(true){
    ThisThread::flags_wait_any(0x7fffffff, false); // sleeps thread until any flag is raised
    int errorNumber = ThisThread::flags_get();
    errorDisplay = errorNumber;
    errorQueue->call(ErrorPointer,errorNumber);

}
}
void ErrorHandler::ErrorServicer(int errorNumber){
    switch(currentErrorSeverity) 
    {
        case WARNING:
        queue->call(printf, "WARNING Error Code - %d\n", (errorNumber & 255));
        yellowLED = 1;
        break;

        case CRITICAL:
        queue->call(printf, "CRITICAL Error Code - %d\n", (errorNumber & 255));
        // turn on red led
        redLED = 1;
        alarm_status = 1;
        #if BUZZER_ENABLE == 1
            buzz.playTone(&note);
            ThisThread::sleep_for(30s);
            buzz.rest();
        #else
            queue->call(printf, "BUZZER ON FOR 30\n");
            ThisThread::sleep_for(30s);
            queue->call(printf,"BUZZER OFF\n");
        #endif
        alarm_status = 0;
        // buzzer for 30 seconds
        // system reset
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong  
        break;

        case FATAL:                     //FATAL ERROR - Immediate hardware reset
        queue->call(printf, "FATAL Error Code - %d\n", (errorNumber & 255));
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong 
        break;

        case BUFF_FULL:
        queue->call(printf, "BUFFER FULL\n");
        redLED = 1;
        // turn on red LED
        break;

        case ENV_ERR:
        //printf("ENV Error %d\n",(errorNumber & 255));
        alarm_status = 1;
        queue->call(printf, "ENV Error %d\n",(errorNumber & 255));
        #if BUZZER_ENABLE == 1
            buzz.playTone(&note);
            ThisThread::sleep_for(3s);
            buzz.rest();
        #else
            queue->call(printf, "BUZZER ON FOR 3\n");
            ThisThread::sleep_for(3s);
            queue->call(printf,"BUZZER OFF\n");
        #endif
        alarm_status = 0;
        // sound buzzer
        break; 

        case CLEAR:
        printf("ALL CLEAR - CODE %d\n",(errorNumber & 255));
        errorDisplay.clear();
        yellowLED = 0;
        redLED = 0;
        #if BUZZER_ENABLE == 1
        buzz.rest();
        #endif
    }
    clear_all();
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

        case CLEAR:
        currentErrorSeverity = CLEAR;
        ERROR_THREAD_NAME.flags_set(errorVal);
    }

}

void ErrorHandler::alarm_override(){
    #if BUZZER_ENABLE == 0
    if (alarm_status == 1){
    queue->call(printf,"ALARM DISABLED - OVERRIDE\n");
    }
    #else
    if (alarm_status == 1){
    buzz.rest();
    }
    #endif
    alarm_status = 0; //disable alarm
}

int ErrorHandler::compare_severity(errorSeverity new_ES){
    //first convert to nice numbers
    int currentErrorInt, newErrorInt,output;
    switch (currentError) {
    case WARNING:
    currentErrorInt = 0;
    break;
    case CRITICAL:
    currentErrorInt = 2;
    break;
    case FATAL:
    currentErrorInt = 3;
    break;
    case ENV_ERR:
    currentErrorInt = 1;
    break;
    case CLEAR:
    currentErrorInt = 0;
    break;
    case BUFF_FULL:
    currentErrorInt = 0;
    break;
    default:
    currentErrorInt = 0;
    }
    switch (new_ES) {
    case WARNING:
    newErrorInt = 0;
    break;
    case CRITICAL:
    newErrorInt = 2;
    break;
    case FATAL:
    newErrorInt = 3;
    break;
    case ENV_ERR:
    newErrorInt = 1;
    break;
    case CLEAR:
    newErrorInt = 0;
    break;
    case BUFF_FULL:
    newErrorInt = 0;
    break;
    default:
    newErrorInt = 0;
    }
    //perform comparison
    if (newErrorInt > currentErrorInt){
        output = 1;
    } else{
        output = 0;
    }
    return output;
}