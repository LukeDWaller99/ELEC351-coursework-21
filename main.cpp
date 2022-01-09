/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_wait_api.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include <cstring>
#include <string.h>
#include <LEDMatrix.h>
#include "SevenSegmentDisplay.h"
#include <ErrorHandler.h>
#include "NTPConnection.h"
#include "HTTP_Server.h"
using namespace uop_msb;
EventQueue* queue = new EventQueue();
ErrorHandler EH(queue);
NTPConnection NTP;
// SevenSegmentDisplay display;
HTTP_server HTPP;
// LEDMatrix matrix;
// Thread t;


time_t timestamp;


int main() {
    // wait_us(5000000);
    // timestamp = NTP.getTime();
    // printf("%s\n", ctime(&timestamp));
    // matrix.clear();
    // display.clear();
    // display.test();
        // samples[0] = 0xffff;
        // samples[1] = 0x0000;
        // samples[2] = 0xff00;
        // samples[3] = 0xf0f0;
        // samples[4] = 0x0f0f;
        // samples[5] = 0x000f;
        // samples[6] = 0xfff0;
        // samples[7] = 0x0ff0;

    // if (!connect()) return -1;

    // if (!setTime()) return -1;

    // // The two lines below will demonstrate the features on the MSB. See uop_msb.cpp for examples of how to use different aspects of the MSB
    // // UOP_MSB_TEST board;  //Only uncomment for testing - DO NOT USE OTHERWISE
    // // board.test();        //Only uncomment for testing - DO NOT USE OTHERWISE

    // // Write fae data to Azure IoT Center. Don't forget to edit azure_cloud_credentials.h
    // printf("You will need your own connection string in azure_cloud_credentials.h\n");
    // LogInfo("Starting the Demo");
    // azureDemo();
    // LogInfo("The demo has ended");

    // return 0;

    // matrix.clear();

    // Err_thread.start(&EH.error_thread);
    // display.test();
    // matrix.test();
    // t.start(callback(queue, &EventQueue::dispatch_forever));
    // matrix.clear();
    // wait_us(100000);
    // EH.setErrorFlag(T_UPPER);
    // wait_us(5000000);
    // EH.setErrorFlag(ALL_CLEAR);
    // wait_us(100000);
    // EH.setErrorFlag(EMPTY_FLUSH);
    // wait_us(5000000);
    // EH.setErrorFlag(ALL_CLEAR);
    // wait_us(5000000);
    // EH.setErrorFlag(BUFFER_FULL);
    //      while(true)
    //  {
        
    //  }
    //EH.alarmtest();

}