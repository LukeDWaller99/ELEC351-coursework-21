//AUTHOR - JACK PENDLEBURY
#include "CustomQueue.h"
#include <cstdio>
#include <string>

CustomQueue::CustomQueue(){
    QUEUE_THREAD.start(callback(&custom, &EventQueue::dispatch_forever));
    custom.call(printf, "Queue Initialised\n");
}

void CustomQueue::operator=(string output){
    string lineEnding = "\n";
    string outputer = output + lineEnding;
    custom.call(printf, "%s", outputer);
};
