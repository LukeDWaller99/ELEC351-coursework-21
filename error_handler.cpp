#include <error_handler.hpp>
#include <uop_msb.h>

void error_handler::clear_all(){
    if (flagLock.trylock_for(10ms) == true){
        ThisThread::flags_clear(0xFF);
        flag_value=0;
        flagLock.unlock();
    }
    else {
        //ERROR_THREAD_NAME.flags_set(0x99);
    }
}

 void error_handler::error_thread(){
    while(true){
    ThisThread::flags_wait_any(0xFF); //wait until
    flag_value = ThisThread::flags_get();
    ThisThread::flags_clear(1);
    }
}

error_handler::error_handler(){
    Thread ERROR_THREAD_NAME;
    ERROR_THREAD_NAME.set_priority(osPriorityRealtime);
    ERROR_THREAD_NAME.start(error_thread);
}