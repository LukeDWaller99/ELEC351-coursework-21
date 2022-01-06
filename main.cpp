#include "mbed.h"
#include "stm32_hal_legacy.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
//#include "NTPClient.h"
//#include "azure_c_shared_utility/xlogging.h"
#include <cstdio>
#include <cstring>
#include <string.h>
#include <sampling.h>
#include <buffer.h>
#include <printQueue.h>
#include <sd.h>
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

using namespace uop_msb;
using namespace std;
sampler samplerer;
extern samples sampleData;
//extern samples sampledData1;
//extern liveData dataRecord1;
bufferClass mybuffer;
//liveData flushRecord;

//liveData dataRecord;
SDCard mySDCard;

//threads
//Thread SDThread;
Thread print;
//Thread buffer_run;

//*************
/* going to get SDState to store if its mounted or not to use ISR in main
green flash added
constructor updated
SDRun updated - can test now with a thread for timing?
writeSD updated - need to copy last time flush occured and use that to track, uses
its own thread to check capacity of buffer throughout
*/

//*************

//UOP_MSB_TEST board;

//************

// void SDCardISR(){
//     SDCardThread.flags_set(SDDetect);
// }

    // void sd_check(){
    //     while(true){
    //         if(SDCardClass.SDState == 1){
               
    //             //SDCardClass.unmountSD();
    //         }
    //         else if (SDCardClass.SDState == 0){
    //             SDCardClass.initSD();
    //             //SDCardClass.testWriteSD();
    //         }
    //     ThisThread::sleep_for(20s);
    //     }
    // }

int main() {
    print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    mybuffer.emptyBuffer();
    
    int i = 0;
    while(i < 20){
             
    printf(" raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData.temp, sampledData.pressure, sampledData.LDR);
    //wait_us(900);
    mybuffer.writeBuffer();
    wait_us(100);
    //mySDCard.writeSD();
    //mySDCard.testWriteSD();
    // //mybuffer.acquireData();
    // //wait_us(100);
    // printf(" out \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", flushRecord.temp, flushRecord.pressure, flushRecord.LDR);
    printf(" stored \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", dataRecord.temp, dataRecord.pressure, dataRecord.LDR);
    wait_us(100);
    i++;
    printf("%i\n", i);
    //SDCardClass.readSD();
        
    }
    mybuffer.printBufferContents();
    //these functions work
    //SDCardClass.writeSD();

    // wait_us(100);
    // mySDCard.mywrite_sdcard();
    // wait_us(100);
    // mySDCard.testWriteSD();
    
    //mySDCard.myread_sdcard();
    //mySDCard.readSD();
    // wait_us(100000);

    // mySDCard.writeDataSD();
    // wait_us(100000);
    // //mySDCard.readSD();
    // wait_us(100000);
    //board.test();
}
