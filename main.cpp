#include "mbed.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
//#include "NTPClient.h"
//#include "azure_c_shared_utility/xlogging.h"
#include <cstring>
#include <string.h>
#include <sampling.h>
#include <buffer.h>
#include <sd.h>

using namespace uop_msb;
using namespace std;
sampler SampleModule;
sampler samplerer;
//samples sampleData;
//extern samples sampledData1;
//extern liveData dataRecord1;
bufferClass mybuffer;
//liveData flushRecord;
//liveData dataRecord;
SDCard SDCardClass;

//threads
Thread samplingThread(osPriorityRealtime);
Thread SDThread;
Thread print;

//check sd mounted
//FIND PIN FOR SD CARD
    void sd_check(){
        while(true){
            if(SDCardClass.SDState == 1){
                SDCardClass.unmountSD();
            }
            else if (SDCardClass.SDState == 0){
                SDCardClass.initSD();
                SDCardClass.writeSD();
            }
        ThisThread::sleep_for(20s);
        }
    }

int main() {
    print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    SDThread.start(&SDCardClass::SDRun);
    samplingThread.start(samplerer.sample);

    //test buffer code
    /*
    while(1){
    printf(" raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData1.temp, sampledData1.pressure, sampledData1.LDR);
    mybuffer.writeBuffer();
    //printf(" out \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
    printf(" stored \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", dataRecord1.temp, dataRecord1.pressure, dataRecord1.LDR);
    wait_us(1000);
    }
    */


    

}
