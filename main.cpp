#include "mbed.h"
#include "sampling.h"
#include "ErrorHandler.h"
#include "buffer.h"
#include "NTPConnection.h"
#include "CustomQueue.h"
#include "SerialIn.h"
#include "HTTP_Server.h"

CustomQueue printQueue;
ErrorHandler EH(&printQueue);
sampler SampleModule(&EH);
bufferClass mainBuffer(&SampleModule, &EH, &printQueue);
HTTP_server HTTP(&printQueue, &EH, &SampleModule);
NTPConnection NTP(&printQueue, &EH);
SerialIn serialInput(&printQueue, &SampleModule, &mainBuffer);

int main() {
    while(true){sleep();}
}