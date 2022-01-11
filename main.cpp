#include "mbed.h"
#include "mbed_wait_api.h"
#include "rtos/ThisThread.h"
#include <sampling.h>
#include "ErrorHandler.h"
#include "buffer.h"

using namespace uop_msb;
using namespace std;

CustomQueue printQueue;
samples sampledData;
ErrorHandler EH(&printQueue);
//ErrorHandler* EH_P = &EH;
sampler SampleModule(&EH);
bufferClass mainBuffer(&EH);
bufferClass mainBufferSampler(&SampleModule);

//threads
Thread samplingThread(osPriorityRealtime);
//Thread SDThread;

Thread print;


int main() {
    //print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    // SampleModule.displayLimits();
    // wait_us(1000000);
    while(true){
    //     int sensor = SampleModule.get_current_sensor();
    sampledData = SampleModule.sampleData;
    // int i;
    // for(i=0;i<8;i++){
    //     sampledData = SampleModule.internal_buffer[i];
    //     printQueue.custom.call(printf,"%d raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", i, sampledData.temp, sampledData.pressure, sampledData.LDR);
    // }
    // printQueue.custom.call(printf,"Sensorval %d\n",sensor);
    //printQueue.call(printf," raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData.temp, sampledData.pressure, sampledData.LDR);
    mainBuffer.bufferCount();
    wait_us(10000000);
    }
}
// #include "NTPClient.h"
// #include <cstring>
// #include <string.h>
// #include <LEDMatrix.h>
// #include "SevenSegmentDisplay.h"
// #include <ErrorHandler.h>
// #include "CustomQueue.h"
// using namespace uop_msb;
// //EventQueue* queue = new EventQueue();
// CustomQueue queue;

// ErrorHandler EH(&queue);
// LEDMatrix matrix;
// Thread t;


// int main() {
//     while(true)
//     {
//     wait_us(100000);
//     EH.setErrorFlag(T_UPPER);
//     wait_us(5000000);
//     EH.setErrorFlag(ALL_CLEAR);
//     wait_us(100000);
//     EH.setErrorFlag(EMPTY_FLUSH);
//     wait_us(5000000);
//     EH.setErrorFlag(ALL_CLEAR);
//     wait_us(5000000);
//     EH.setErrorFlag(BUFFER_FULL);
//     }
// }
