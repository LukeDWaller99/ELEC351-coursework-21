#include "HTTP_Server.h"
#include "mbed.h"
#include "sampling.h"
#include "ErrorHandler.h"
#include "buffer.h"
#include "NTPConnection.h"
#include "CustomQueue.h"

CustomQueue printQueue;
ErrorHandler EH(&printQueue);
sampler SampleModule(&EH);
bufferClass mainBuffer(&SampleModule, &EH, &printQueue);
HTTP_server HTTP(&printQueue, &EH, &SampleModule);
NTPConnection NTP(&printQueue, &EH);

int main() { 
    while(true){sleep();}
}