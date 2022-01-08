/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "NTPClient.h"
#include "SevenSegmentDisplay.h"
#include "mbed.h"
#include "mbed_wait_api.h"
#include "rtos/ThisThread.h"
#include "uop_msb.h"
#include <ErrorHandler.h>
#include <LEDMatrix.h>
#include <cstring>
#include <string.h>
using namespace uop_msb;
EventQueue *queue = new EventQueue();
ErrorHandler EH(queue);
LEDMatrix matrix;
Thread t;

int main() {
    int samples[8];
  matrix.clear();
  for (int i = 0; i <= 7; i++){
      samples[i] = 1;
  }
  matrix.update(samples);
  while (true) {
    

    // if (!connect()) return -1;

    // if (!setTime()) return -1;

    // // The two lines below will demonstrate the features on the MSB. See
    // uop_msb.cpp for examples of how to use different aspects of the MSB
    // // UOP_MSB_TEST board;  //Only uncomment for testing - DO NOT USE
    // OTHERWISE
    // // board.test();        //Only uncomment for testing - DO NOT USE
    // OTHERWISE

    // // Write fae data to Azure IoT Center. Don't forget to edit
    // azure_cloud_credentials.h printf("You will need your own connection
    // string in azure_cloud_credentials.h\n"); LogInfo("Starting the Demo");
    // azureDemo();
    // LogInfo("The demo has ended");

    // return 0;

    // matrix.clear();

    // Err_thread.start(&EH.error_thread);
    // display.test();
    // matrix.test();
    // EH.alarmtest();
  }
}