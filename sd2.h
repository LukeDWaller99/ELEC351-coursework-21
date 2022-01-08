#ifndef __SDCARD2__
#define __SDCARD2__

#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include "buffer.h"

class SDCard{
    private:
    bufferClass SDBuffer;
    //Thread SDThread;

    public:
    void initSD();
    void flashGreen();
    //void writeSD();
    // void unmountSD();
    // void SDRun();

    
    bool cardMount = 0;

    SDCard();
    ~SDCard();
};


#endif