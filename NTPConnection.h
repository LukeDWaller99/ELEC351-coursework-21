#ifndef __NTP_CLIENT__
#define __NTP_CLIENT__

#include "DigitalIn.h"
#include "NTPClient.h"
#include <iostream>
#include "PinNames.h"

class NTPConnection{

    private:
    NetworkInterface* NTPInterface;
    DigitalIn BTN_C;

    public:
    time_t timestamp;
    /**
    Construct the Network Connecion obejct
    Connects to NTP server to get current date and time and stores this locally
    **/
    NTPConnection();
    // /**
    // Get time the current local time
    // **/
    time_t getTime();

};

#endif