//AUTHOR - JACK PENDLEBURY
#include "CustomQueue.h"
#include <cstdio>

CustomQueue::CustomQueue(){
    QUEUE_THREAD.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call(printf, "Queue Initialised\n");
}
