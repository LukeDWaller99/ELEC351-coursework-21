#include "mbed.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
//#include "NTPClient.h"
//#include "azure_c_shared_utility/xlogging.h"
#include <cstdio>
#include <cstring>
#include <string.h>
#include <sampling.h>
#include <buffer.h>
#include <sd.h>
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

using namespace uop_msb;
using namespace std;
sampler SampleModule;
sampler samplerer;
extern samples sampleData;
//extern samples sampledData1;
//extern liveData dataRecord1;
bufferClass mybuffer;
liveData flushRecord;
//liveData dataRecord;
SDCard SDCardClass;

//threads
//Thread SDThread;
Thread print;
Thread buffer_run;

//check sd mounted
//FIND PIN FOR SD CARD

    void sd_check(){
        while(true){
            if(SDCardClass.SDState == 1){
               
                //SDCardClass.unmountSD();
            }
            else if (SDCardClass.SDState == 0){
                SDCardClass.initSD();
                //SDCardClass.testWriteSD();
            }
        ThisThread::sleep_for(20s);
        }
    }

int main() {
    print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    mybuffer.emptyBuffer();



    //sd_check();
    //SDCardClass.initSD();
    //SDCardClass.testWriteSD();
    //SDCardClass.readSD();


    //test buffer code
    int i = 0;
    while(i < 200){
         SDCardClass.initSD();
    
    printf(" raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData.temp, sampledData.pressure, sampledData.LDR);
    wait_us(100);
    //mybuffer.writeBuffer();
    //wait_us(100000);
    // //mybuffer.acquireData();
    // //wait_us(10000000);
    // printf(" out \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
    printf(" stored \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", dataRecord.temp, dataRecord.pressure, dataRecord.LDR);
    //wait_us(1000);
    i++;
    }
    //SDCardClass.testWriteSD();
    //wait_us(1000);
    //sd_check();
    //SDCardClass.readSD();

    //mybuffer.flushBuffer(FILE &fp);
}
