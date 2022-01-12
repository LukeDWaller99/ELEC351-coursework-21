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
#include "HTTP_Server.h"

/// Macro to control the HTTP Server usage
/// Set Macro to 0 if HTTP Server not being used, otherwise set to 1
/// Pre-processer directives will eiother try to connect the NTP server or use existing connection
#define HTTP_SERVER_USED 0

class NTPConnection{

    private:
    NetworkInterface* NTPInterface;
    CustomQueue* printQueue;
    ErrorHandler* errorHandler;


    public:
    time_t timestamp;
    /**
    Construct the Network Connecion obejct

    Connects to NTP server to get current date and time and stores this locally
    **/
    NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler);
    /**
        Get time the current local time
    **/
    time_t getTime();

};

#endif