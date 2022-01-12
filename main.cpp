#include "HTTP_Server.h"
#include "mbed.h"
#include "mbed_wait_api.h"
#include "rtos/ThisThread.h"
#include <cstdio>
#include <sampling.h>
#include "ErrorHandler.h"
#include "buffer.h"
#include "NTPConnection.h"
#include "CustomQueue.h"
#include "SerialIn.h"

CustomQueue printQueue;
ErrorHandler EH(&printQueue);
sampler SampleModule(&EH);
bufferClass mainBuffer(&SampleModule, &EH, &printQueue);
//HTTP_server HTTP(&printQueue, &EH, &SampleModule);
//NTPConnection NTP(&printQueue, &EH);
SerialIn Serial(&printQueue, &SampleModule, &mainBuffer);

int main() {
    while(true){sleep();}
}