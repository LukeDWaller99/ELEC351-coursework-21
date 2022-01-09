//AUTHOR - JACK PENDLEBURY
#include "CustomQueue.h"
#include <cstdio>

CustomQueue::CustomQueue(){
    QUEUE_THREAD.start(callback(&custom, &EventQueue::dispatch_forever));
    custom.call(printf, "Queue Initialised\n");
}
