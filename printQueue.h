#include "mbed.h"

extern EventQueue printQueue;

//*******************
//buffer print queues
extern void bufferFull();
extern void bufferLockTimeout();
extern void timeLockTimeout();
extern void emptyFlush();
extern void bufferFlushTimeout();
extern void flushBuffer();

