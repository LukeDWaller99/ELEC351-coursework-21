#include "NTPConnection.h"
#include "CustomQueue.h"


NTPConnection::NTPConnection(CustomQueue* printQueue) : BTN_C(PG_2) {

  printQueue = printQueue;
  if (BTN_C == 1) {

    printQueue->custom.call(printf, "Starting NTP interface\n");
    NTPInterface = NetworkInterface::get_default_instance();

    if (NTPInterface == nullptr) {
    printQueue->custom.call(printf, "No Network Interface Found\n");
   wait_us(100000);
      NVIC_SystemReset();
    }

    // Connect to server
    int connect = NTPInterface->connect();
    if (connect != 0) {

    printQueue->custom.call(printf, "CONNECTION ERROR\n");
       wait_us(100000);
      NVIC_SystemReset();
    }

    printQueue->custom.call(printf, "Connection success, MAC: %s\n", NTPInterface->get_mac_address());
    printQueue->custom.call(printf, "Getting time from the NTP server\n");

    NTPClient NTP(NTPInterface);

    NTP.set_server("time.google.com", 123);

    timestamp = NTP.get_timestamp();

    if (timestamp < 0) {
      printQueue->custom.call(printf, "Failed to get the current time, error: %s. \n Exception Raised\n", ctime(&timestamp));
      NTPInterface->disconnect();
   wait_us(100000);
      NVIC_SystemReset();
    }

    set_time(timestamp);

    printQueue->custom.call(printf, "Time: %s\n", ctime(&timestamp));

    NTPInterface->disconnect();
    wait_us(100000);
    NVIC_SystemReset();
  } else {
    delete[] &NTPInterface;
  }
};

time_t NTPConnection::getTime() { return time(NULL); } // deprecated function
