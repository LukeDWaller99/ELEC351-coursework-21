#ifndef __NTP_CLIENT__
#define __NTP_CLIENT__

#include "NTPClient.h"
#include <iostream>

class NTPConnection{

    private:
    NetworkInterface* NTPInterface;

    public:
    time_t timestamp;
    /**
    Construct the Network Connecion obejct

    Connects to NTP server to get current date and time and stores this locally
    **/
    NTPConnection();
    ~NTPConnection();
    // /**
    // Get time the current local time
    // **/
    time_t getTime();

};

#endif