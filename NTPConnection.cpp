#include "NTPConnection.h"


NTPConnection::NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler) {

    printQueue->custom.call(printf, "Starting NTP interface\n");
    NTPInterface = NetworkInterface::get_default_instance();

    if (!NTPInterface) {
        printQueue->custom.call(printf, "No Network Interface Found\n");
        wait_us(100000);
        errorHandler->setErrorFlag(NO_NETWORK_INTERFACE);
    }

    // Connect to server
    // use an external macro or something to check if there is going to be an ethernet connection 
    // if true skip the connection, if not try to connect. 
    #if HTTP_SERVER_USED == 0
        int connect = NTPInterface->connect();

        // if network cannot connect, raise a connection error
        if (connect != 0) {
        printQueue->custom.call(printf, "CONNECTION ERROR\n");
        wait_us(100000);
        errorHandler->setErrorFlag(CONNECTION_ERROR);
        }
    #else
        printQueue->custom.call(printf, "Ethernet Server already running\n");
    #endif

    printQueue->custom.call(printf, "Getting time from the NTP server\n");

    // Crreate an NPTClient using the connected interface
    NTPClient NTP(NTPInterface);

    // set up NTP server 
    NTP.set_server("time.google.com", 123);

    // get the current time from the time server
    timestamp = NTP.get_timestamp();
    if (timestamp < 0) {
      printQueue->custom.call(printf, "Failed to get the current time, error: %s. \n Exception Raised\n", ctime(&timestamp));
      NTPInterface->disconnect();
      errorHandler->setErrorFlag(TIME_ERROR);
    }

    // set the current time on the board by the value given by the NTP server
    set_time(timestamp);

    // output the time and date to check that the current date and time are correct
    printQueue->custom.call(printf, "Time: %s\n", ctime(&timestamp));

    // disconnect the NTP interface as it will not be needed as long as the board is powered
    NTPInterface->disconnect();
};

// deprecated function - can use time(NULL) instead
time_t NTPConnection::getTime() { return time(NULL); } 
 