/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_wait_api.h"
#include "uop_msb.h"
#include "rtos/ThisThread.h"
#include "NTPClient.h"
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
    while(true)
    {
        
    }
}
