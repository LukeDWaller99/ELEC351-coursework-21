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
#include "SevenSegmentDisplay.h"
#include <ErrorHandler.h>
using namespace uop_msb;
EventQueue* queue = new EventQueue();
ErrorHandler EH(queue);
LEDMatrix matrix;
Thread t;


int main() {
    // Err_thread.start(&EH.error_thread);
    // display.test();
    // matrix.test();
    t.start(callback(queue, &EventQueue::dispatch_forever));
    matrix.clear();
    wait_us(100000);
    EH.setErrorFlag(T_UPPER);
    wait_us(5000000);
    EH.setErrorFlag(ALL_CLEAR);
    wait_us(100000);
    EH.setErrorFlag(EMPTY_FLUSH);
    wait_us(5000000);
    EH.setErrorFlag(ALL_CLEAR);
    wait_us(5000000);
    EH.setErrorFlag(BUFFER_FULL);
         while(true)
     {
        
     }
    //EH.alarmtest();
}
