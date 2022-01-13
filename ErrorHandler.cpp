//AUTHOR - JACK PENDLEBURY
#include <ErrorHandler.h>

//Private methods
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


void ErrorHandler::alarm_override(){
    #if BUZZER_ENABLE == 0
    if (alarm_status == 1){
    queue->custom.call(printf,"ALARM DISABLED - OVERRIDE\n");
    }
    #else
    if (alarm_status == 1){
    buzz.rest();
    }
    #endif
    alarm_status = 0; //disable alarm
}

//Public Methods


ErrorHandler::ErrorHandler(CustomQueue* outputQueue):override_button(USER_BUTTON){
    
    queue = outputQueue;
    ERROR_THREAD_NAME.start(callback(this, &ErrorHandler::error_thread));
    override_button.rise(callback(this, &ErrorHandler::alarm_override));
    ERROR_THREAD_NAME.set_priority(osPriorityRealtime7);
    queue->custom.call(printf, "Error Handler Initialised\n");
}


void ErrorHandler::error_thread(void){
    while(true){
    ThisThread::flags_wait_any(0x7fffffff, false); // sleeps thread until any flag is raised

    int errorNumber = ThisThread::flags_get();

    errorDisplay = errorNumber;

    switch(currentErrorSeverity) 
    {
        case WARNING:
        queue->custom.call(printf, "WARNING Error Code - %x\n", (errorNumber & 255));
        yellowLED = 1;
        break;

        case CRITICAL:
        queue->custom.call(printf, "CRITICAL Error Code - %x\n", (errorNumber & 255));
        // turn on red led
        redLED = 1;
        alarm_status = 1;
        #if BUZZER_ENABLE == 1
            buzz.playTone(&note);
            ThisThread::sleep_for(30s);
            buzz.rest();
        #else
            queue->custom.call(printf, "BUZZER ON FOR 30\n");
            ThisThread::sleep_for(30s);
            queue->custom.call(printf,"BUZZER OFF\n");
        #endif
        alarm_status = 0;
        // buzzer for 30 seconds
        // system reset
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong  
        break;

        case FATAL:                     //FATAL ERROR - Immediate hardware reset
        queue->custom.call(printf, "FATAL Error Code - %x\n", (errorNumber & 255));
        ThisThread::sleep_for(1s);
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong 
        break;

        case BUFF_FULL:
        queue->custom.call(printf, "BUFFER FULL\n");
        redLED = 1;
        // turn on red LED
        break;

        case ENV_ERR:
        //printf("ENV Error %d\n",(errorNumber & 255));
        queue->custom.call(printf, "ENV Error %x\n",(errorNumber & 255));
        #if BUZZER_ENABLE == 1
            buzz.playTone(&note);
            ThisThread::sleep_for(3s);
            buzz.rest();
        #else
            queue->custom.call(printf, "BUZZER ON FOR 3\n");
            ThisThread::sleep_for(3s);
            queue->custom.call(printf,"BUZZER OFF\n");
        #endif
        alarm_status = 0;
        // sound buzzer
        break; 

        case NET_ERROR:
        queue->custom.call(printf, "NETWORK Error %d\n",(errorNumber & 255));
        queue->custom.call(printf, "FATAL Error Code - %d\n", (errorNumber & 255));
        ThisThread::sleep_for(1s);
        NVIC_SystemReset(); //reset the system - this should only be called if something goes VERY wrong 
        break;


        case CLEAR:
        printf("ALL CLEAR - CODE %x\n",(errorNumber & 255));
        errorDisplay.clear();
        yellowLED = 0;
        redLED = 0;
        #if BUZZER_ENABLE == 1
        buzz.rest();
        #endif
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

        case NET_ERROR:
        currentErrorSeverity = NET_ERROR;
        ERROR_THREAD_NAME.flags_set(errorVal);
        break;

        case CLEAR:
        currentErrorSeverity = CLEAR;
        ERROR_THREAD_NAME.flags_set(errorVal);
    }

}

