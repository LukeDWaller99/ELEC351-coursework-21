#include "NTPConnection.h"


NTPConnection::NTPConnection(EventQueue *printQueue) : BTN_C(PG_2) {

  printQueue = printQueue;
  if (BTN_C == 1) {

    printQueue->call(printf, "Starting NTP interface\n");
    NTPInterface = NetworkInterface::get_default_instance();

    if (NTPInterface == nullptr) {
    printQueue->call(printf, "No Network Interface Found\n");
   wait_us(100000);
      NVIC_SystemReset();
    }

    // Connect to server
    int connect = NTPInterface->connect();
    if (connect != 0) {

    printQueue->call(printf, "CONNECTION ERROR\n");
       wait_us(100000);
      NVIC_SystemReset();
    }

    printQueue->call(printf, "Connection success, MAC: %s\n", NTPInterface->get_mac_address());
    printQueue->call(printf, "Getting time from the NTP server\n");

    NTPClient NTP(NTPInterface);

    NTP.set_server("time.google.com", 123);

    timestamp = NTP.get_timestamp();

    if (timestamp < 0) {
      printQueue->call(printf, "Failed to get the current time, error: %s. \n Exception Raised\n", ctime(&timestamp));
      NTPInterface->disconnect();
   wait_us(100000);
      NVIC_SystemReset();
    }

    set_time(timestamp);

    printQueue->call(printf, "Time: %s\n", ctime(&timestamp));

    NTPInterface->disconnect();
    wait_us(100000);
    NVIC_SystemReset();
  } else {
    delete[] &NTPInterface;
  }
};

time_t NTPConnection::getTime() { return time(NULL); } // deprecated function
