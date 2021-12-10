#ifndef H_SAMPLING
#define H_SAMPLING
#include <mbed.h>
#include <Mutex.h>
#include <uop_msb.h>

class sampler {
    private:
    Mutex sampleLock;
    Ticker sampleTick;
    Thread sampleThread;
    
    AnalogIn LDR;

    void sample();
    void sample_thread_starter();

    public:
    sampler();
    ~sampler();

};

#endif