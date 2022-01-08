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
#include <sd2.h>
// #include "SDBlockDevice.h"
// #include "FATFileSystem.h"

using namespace uop_msb;
using namespace std;

extern samples sampleData;

sampler samplerer;
bufferClass mybuffer;
SDCard mySDCard;

//Threads
Thread print;

/*
SDRun updated - can test now with a thread for timing?
writeSD updated - need to copy last time flush occured and use that to track, uses
its own thread to check capacity of buffer throughout
*/

// void sd_check(){
//     while(true){
//         if(mySDCard.cardMount == 1){
//             mySDCard.initSD();
//             //test write?
//         } else if (mySDCard.cardMount == 0){
//             printQueue.call(printf, "sd check, failed\n");
//         }
//     }
// }

int main() {
    print.start(callback(&printQueue, &EventQueue::dispatch_forever));
    //bufferTick.start(callback(&mybuffer::acquireData), 15s);
    
    //mybuffer.emptyBuffer();
    //mySDCard.initSD();
    
    int i = 0;
    while(i < 5){
    mySDCard.initSD();
    i++;
    printf("%i\n", i);       
    printf(" raw \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", sampledData.temp, sampledData.pressure, sampledData.LDR);
    wait_us(100000);
    mybuffer.writeBuffer();
    printf(" stored \tTemperature = %2.1f, \tPressure = %3.1f, \tLDR = %1.2f;\n\r", dataRecord.temp, dataRecord.pressure, dataRecord.LDR);
    wait_us(100000);
    }
    
    mybuffer.printBufferContents();
    mySDCard.initSD();
    mybuffer.flushBufferUpgrade();
    

}
