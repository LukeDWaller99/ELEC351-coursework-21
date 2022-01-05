#ifndef __SDCARD__
#define __SDCARD__

#include "mbed.h"
#include "FATFileSystem.h"
#include "buffer.h"
#include "SDBlockDevice.h"
#include "uop_msb.h"

/*
CS PE3 //DigitalOut
SPI1 SCK PB_3
SPI1 MOSI PB_5
SPI1 MISO PB_4
DETECT PF_4
*/
//green led PC_6 for when mounted


class SDCard{
    private:
    Thread SDThread;

    public:
    void initSD();
    void testWriteSD();
    void writeSD();
    void unmountSD();
    void SDRun();
    void readSD();

    bufferClass buffer;
    bool SDState = 0;

    SDCard();
    ~SDCard();
};


#endif
