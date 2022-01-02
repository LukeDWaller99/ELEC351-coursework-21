#include "mbed.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include <sampling.h>

using namespace uop_msb;
using namespace std;
sampler SampleModule;
EventQueue printQueue;
samples sampledData;

//threads
Thread samplingThread(osPriorityRealtime);
//Thread SDThread;
Thread print;


int main() {
    print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    SampleModule.displayLimits();
    wait_us(1000000);
    while(true){
    //sampledData = SampleModule.sampleData;
    int i;
    for(i=0;i<8;i++){
        sampledData = SampleModule.internal_buffer[i];
        printQueue.call(printf,"%d raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", i, sampledData.temp, sampledData.pressure, sampledData.LDR);
    }
    //printQueue.call(printf," raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData.temp, sampledData.pressure, sampledData.LDR);
    wait_us(1000000);
    }

}
