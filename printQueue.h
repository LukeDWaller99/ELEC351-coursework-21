#ifndef __print_Queue__
#define __print_Queue__


#include "mbed.h"

extern EventQueue printQueue;

//*******************
//buffer print queues
//*******************
extern void bufferFull();
extern void bufferLockTimeout();
extern void timeLockTimeout();
extern void emptyFlush();
extern void bufferFlushTimeout();
extern void flushedBuffer();

//*******************
//sd card print queues
//*******************

extern void mountError();
extern void mountedSD();
extern void noSDFile();
extern void unmountedFlush();
extern void unmountedSD();



#endif

