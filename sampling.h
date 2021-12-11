#ifndef H_SAMPLING
#define H_SAMPLING
#include <mbed.h>
#include <Mutex.h>
#include <uop_msb.h>
#include <buffer.h>

class sampler {
    private:
    typedef void(*funcPointer_t)(void);
    Mutex sampleLock;
    Ticker sampleTick;
    Thread sampleThread;
    bufferClass sampleBuffer;
    
    
    AnalogIn LDR;

     void sample();

    public:
    sampler();
    ~sampler();

};

#endif