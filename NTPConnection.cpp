#include "NTPConnection.h"


NTPConnection::NTPConnection(CustomQueue* printQueue, ErrorHandler* errorHandler) : BTN_C(PG_2) {

  printQueue = printQueue;
    errorHandler = errorHandler;

//   if (BTN_C == 1) {

      //setup a watch dog timer
    printQueue->custom.call(printf, "Starting NTP interface\n");
    NTPInterface = NetworkInterface::get_default_instance();

    if (NTPInterface == nullptr) {
    printQueue->custom.call(printf, "No Network Interface Found\n");
   wait_us(100000);
      errorHandler->setErrorFlag(NO_NETWORK_INTERFACE);
    }


    // Connect to server
    // int connect = NTPInterface->connect();
    // if (connect != 0) {

    // printQueue->custom.call(printf, "CONNECTION ERROR\n");
    //    wait_us(100000);
    //   errorHandler->setErrorFlag(CONNECTION_ERROR);
    // }

    printQueue->custom.call(printf, "Connection success, MAC: %s\n", NTPInterface->get_mac_address());
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
    // NVIC_SystemReset();
//   } else {
    //   printQueue->custom.call(printf, "NTP Aborted!\n");
    // delete[] &NTPInterface;
//   }
};

time_t NTPConnection::getTime() { return time(NULL); } // deprecated function
