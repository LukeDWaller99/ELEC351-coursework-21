#include "NTPConnection.h"
#include <cstdio>


NTPConnection::NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler) {

    printQueue->custom.call(printf, "Starting NTP interface\n");
    NetworkInterface *NTPInterface = NetworkInterface::get_default_instance();

    if (!NTPInterface) {
        printQueue->custom.call(printf, "No Network Interface Found\n");
        wait_us(100000);
        errorHandler->setErrorFlag(NO_NETWORK_INTERFACE);
    }

    // Connect to server
    // use an external macro or something to check if there is going to be an ethernet connection then test this, if true skip the connection, if not connect. 
    
    #if HTTP_SERVER_USED == 0
        int connect = NTPInterface->connect();
        if (connect != 0) {

        printQueue->custom.call(printf, "CONNECTION ERROR\n");
        wait_us(100000);
        errorHandler->setErrorFlag(CONNECTION_ERROR);
        }
    #else
        printQueue->custom.call(printf, "Ethernet Server already running\n");
    #endif

    printQueue->custom.call(printf, "Getting time from the NTP server\n");

    NTPClient NTP(NTPInterface);

    NTP.set_server("time.google.com", 123);

    timestamp = NTP.get_timestamp();

    if (timestamp < 0) {
      printQueue->custom.call(printf, "Failed to get the current time, error: %s. \n Exception Raised\n", ctime(&timestamp));
      NTPInterface->disconnect();
      wait_us(100000);
      errorHandler->setErrorFlag(TIME_ERROR);
    }

    set_time(timestamp);

    printQueue->custom.call(printf, "Time: %s\n", ctime(&timestamp));

    NTPInterface->disconnect();
    wait_us(100000);
};

// deprecated function - can use time(NULL) instead
time_t NTPConnection::getTime() { return time(NULL); } 
 