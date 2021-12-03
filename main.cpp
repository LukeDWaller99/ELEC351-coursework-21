/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_wait_api.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include "NTPClient.h"
#include "azure_c_shared_utility/xlogging.h"
#include <cstring>
#include <string.h>
#include <LEDMatrix.h>
using namespace uop_msb;

LEDMatrix matrix;

double samples[8];

// extern void azureDemo();
// extern NetworkInterface *_defaultSystemNetwork;

// bool connect()
// {
//     LogInfo("Connecting to the network");

//     _defaultSystemNetwork = NetworkInterface::get_default_instance();
//     if (_defaultSystemNetwork == nullptr) {
//         LogError("No network interface found");
//         return false;
//     }

//     int ret = _defaultSystemNetwork->connect();
//     if (ret != 0) {
//         LogError("Connection error: %d", ret);
//         return false;
//     }
//     LogInfo("Connection success, MAC: %s", _defaultSystemNetwork->get_mac_address());
//     return true;
// }

// bool setTime()
// {
//     LogInfo("Getting time from the NTP server");

//     NTPClient ntp(_defaultSystemNetwork);
//     ntp.set_server("time.google.com", 123);
//     time_t timestamp = ntp.get_timestamp();
//     if (timestamp < 0) {
//         LogError("Failed to get the current time, error: %ud", timestamp);
//         return false;
//     }
//     LogInfo("Time: %s", ctime(&timestamp));
//     set_time(timestamp);
//     return true;
// }

int main() {
    while (true) {

        samples[0] = 0xffff;
        samples[1] = 0x0000;
        samples[2] = 0xff00;
        samples[3] = 0xf0f0;
        samples[4] = 0x0f0f;
        samples[5] = 0x000f;
        samples[6] = 0xfff0;
        samples[7] = 0x0ff0;

    // if (!connect()) return -1;

    // if (!setTime()) return -1;

    // // The two lines below will demonstrate the features on the MSB. See uop_msb.cpp for examples of how to use different aspects of the MSB
    // // UOP_MSB_TEST board;  //Only uncomment for testing - DO NOT USE OTHERWISE
    // // board.test();        //Only uncomment for testing - DO NOT USE OTHERWISE

    // // Write fake data to Azure IoT Center. Don't forget to edit azure_cloud_credentials.h
    // printf("You will need your own connection string in azure_cloud_credentials.h\n");
    // LogInfo("Starting the Demo");
    // azureDemo();
    // LogInfo("The demo has ended");

    // return 0;

    // matrix.clear();

    matrix.test();

    }
}
