/**
    @file Main.cpp
    Main file for Group D ELEC351 Coursework - Jack Pendlebury, Noah Harvey, Luke Waller
**/

#include "mbed.h"
#include "sampling.h"
#include "ErrorHandler.h"
#include "buffer.h"
#include "NTPConnection.h"
#include "CustomQueue.h"
#include "SerialIn.h"
#include "HTTP_Server.h"

///Set to 1 to enable demo mode, this will execute code to induce several errors, before resetting the board with a simulated fatal error.
int demo_mode = 0; 

CustomQueue printQueue;
ErrorHandler EH(&printQueue);
sampler SampleModule(&EH);
bufferClass mainBuffer(&SampleModule, &EH, &printQueue);
HTTP_server HTTP(&printQueue, &EH, &SampleModule);
NTPConnection NTP(&printQueue, &EH);
SerialIn serialInput(&printQueue, &SampleModule, &mainBuffer);

int main() {
    if (demo_mode == 0){    //demo disabled, standard execution
    while(true){sleep();}
    }
    else{
        ThisThread::sleep_for(30s); //sleep for 30s
        EH.setErrorFlag(EMPTY_FLUSH); //simulate a warning
        ThisThread::sleep_for(15s);
        EH.setErrorFlag(L_UPPER); //simulate an environmental error
        ThisThread::sleep_for(15s);
        EH.setErrorFlag(BUFFER_FULL);//simulate a critical error
        //THE BOARD WILL NOW RESET
    }
}
