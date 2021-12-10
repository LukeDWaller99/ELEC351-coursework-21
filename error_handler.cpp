//AUTHOR - JACK PENDLEBURY
#include <error_handler.hpp>
// #define ERROR_THREAD_NAME eHandler


void error_handler::clear_all(){
    if (flagLock.trylock_for(10ms) == true){
        ThisThread::flags_clear(0xFF);
        flag_value=0;
        flagLock.unlock();
    }
    else {
        // Maybe try this? This will just reset the system if this error occours
        // I can see this being define as a critical error that may just require resetting the board?
        // From what I can see it isnt possible to set a flag for a thread that you are currently operating in
        NVIC_SystemReset();
        
    }
}

error_handler::error_handler(){
    Thread ERROR_THREAD_NAME;
    ERROR_THREAD_NAME.set_priority(osPriorityRealtime);
    ERROR_THREAD_NAME.start(error_thread);
}

 void error_handler::error_thread(){
    while(true){
    ThisThread::flags_wait_any(0xFF); //wait until
    flag_value = ThisThread::flags_get();
    ThisThread::flags_clear(1);
    }
}


void error_handler::severityHandler(){

    errorSeverity severity = error_handler::errorSeverity();

    switch(severity) 
    {
        case WARNING:
        yellowLED = 1;
        ThisThread::sleep_for(200ms); //test thist time
        yellowLED = 0;
        break;

        case CRITICAL:
        //alarm to sound for 30 seconds
        redLED = 1;
        ThisThread::sleep_for(30s);
        redLED = 0;
        ThisThread::sleep_for(30s);
        break;

        //does it need a
        // default:
        // break;

    }
}


