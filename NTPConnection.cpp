// #include "NTPConnection.h"

// NTPConnection::NTPConnection() : BTN_C(PG_2) {

//   if (BTN_C == 1) {

//     NTPInterface = NetworkInterface::get_default_instance();

//     if (NTPInterface == nullptr) {
//       printf("No Network Interface found\n");
//       return;
//     }

//     // Connect to server
//     int connect = NTPInterface->connect();
//     if (connect != 0) {

//       printf("CONNECTION ERROR\n");
//       return;
//     }

//     printf("Connection success, MAC: %s\n", NTPInterface->get_mac_address());
//     printf("Getting time from the NTP server\n");

//     NTPClient NTP(NTPInterface);

//     NTP.set_server("time.google.com", 123);

//     timestamp = NTP.get_timestamp();

//     if (timestamp < 0) {
//       // cout << "Failed to get the current time, error: " << timestamp << endl;
//       NTPInterface->disconnect();
//       return;
//     }

//     set_time(timestamp);

//     printf("Time: %s\n", ctime(&timestamp));

//     NTPInterface->disconnect();
//   } else {
//   printf("NOTHING\n");
//   return;
//   }
// };

// time_t NTPConnection::getTime() { return time(NULL); }