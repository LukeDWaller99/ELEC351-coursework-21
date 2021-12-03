#ifndef __SDCARD__
#define __SDCARD__

#include "mbed.h"
#include "FATFileSystem.h"

/*
CS PE3 //DigitalOut
SPI1 SCK PB_3
SPI1 MOSI PB_5
SPI1 MISO PB_4
DETECT PF_4
*/




extern void initSD();
extern void writeSD();
extern void unmountSD();

#endif