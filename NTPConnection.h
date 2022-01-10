#ifndef __NTP_CLIENT__
#define __NTP_CLIENT__

#include "CustomQueue.h"
#include "DigitalIn.h"
#include "EventQueue.h"
#include "ErrorHandler.h"
#include "NTPClient.h"
#include <iostream>
#include "PinNames.h"
#include "EventQueue.h"
#include "ThisThread.h"
#include <cstdio>
#include <ctime>
#include "CustomQueue.h"

class NTPConnection{

    private:
    NetworkInterface* NTPInterface;
    DigitalIn BTN_C;
    CustomQueue* printQueue;
    ErrorHandler* errorHandler;

    public:
    time_t timestamp;
    /**
    Construct the Network Connecion obejct

    Connects to NTP server to get current date and time and stores this locally
    **/
    NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler);
    // /**
    // Get time the current local time
    // **/
    time_t getTime();

};

#endif