#ifndef __SDCARD__
#define __SDCARD__

#include "mbed.h"
#include "FATFileSystem.h"


extern void initSD();
extern void writeSD();
extern void unmountSD();

#endif