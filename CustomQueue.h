#ifndef __CUSTOM_QUEUE__
#define __CUSTOM_QUEUE__

#include "mbed.h"
#include "uop_msb.h"
#include "string.h"

class CustomQueue{
    private:

    public: 
    CustomQueue();
    EventQueue queue;
    Thread QUEUE_THREAD;
    void call(string str);
};

#endif